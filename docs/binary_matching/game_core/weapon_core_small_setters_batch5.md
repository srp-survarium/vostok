# weapon_core batch5: setters + animation-callback forwarders

Unit (all in `sources/vostok/game_core/sources/weapon_core.cpp`, class `weapon_core`):
- `set_fire_bullet_transform(float4x4 const&)`
- `set_next_fire_queue_type()`
- `set_animation_callback(pcstr, pcvoid, boost::function<...> const&)`
- `set_animation_callback(reserved_channel_ids_enum, pcvoid, boost::function<...> const&)`
- `remove_animation_callback(pcstr, pcvoid)`
- `remove_animation_callback(reserved_channel_ids_enum, pcvoid)`

All in the `public:` section of weapon_core.h -> public access.

## Commands
- `pdb_rich_query --index binaries/rich/target/index.jsonl --list | grep weapon_core::...`
  gives RVAs: set_next_fire_queue_type 0x592bb0, set_fire_bullet_transform 0x592c00,
  remove_animation_callback(enum,..) 0x592b40, remove_animation_callback(pcstr,..) 0x592b70,
  set_animation_callback(enum,..) 0x594240, set_animation_callback(pcstr,..) 0x594290.
- `pdb_fetch --target-index binaries/rich/target/index.jsonl --rva <rva> --view target`

## Asm -> source mapping

### set_fire_bullet_transform (0x592c00)
`mov byte ptr [eax+489h],1` -> `m_ready_for_fire = true;` (0x489).
`rep movsd` 0x10 dwords from [ebp+8] into edi=this+118h -> `m_fire_bullet_transform = fire_bullet_transform;` (0x118).

### set_next_fire_queue_type (0x592bb0)
0x47E = m_fire_queue_type, 0x485 = m_weapon_fire_queue_types_count.
`if ( m_fire_queue_type == m_weapon_fire_queue_types_count - 1 ) m_fire_queue_type = 0; else ++m_fire_queue_type;`
Carcass 693 had +0x002 (braced if-true block) - matches the if/else shape.

### remove_animation_callback (0x592b40 enum / 0x592b70 pcstr)
Forwards via m_user (offset 0x44c) virtual call. vtable+0x54 = unsubscribe_animation_player(enum,pcvoid),
vtable+0x58 = unsubscribe_animation_player(pcstr,pcvoid).
`m_user->unsubscribe_animation_player( channel_id, callback_uid );`

### set_animation_callback (0x594240 enum / 0x594290 pcstr)
Forwards via m_user virtual call. vtable+0x4C = subscribe(enum,...5 args), +0x50 = subscribe(pcstr,...6 args).
Constructs a temp managed_resource_ptr(NULL) at [ebp-4] (push 0 + resource_ptr ctor), dtored at end.
enum: `m_user->subscribe_animation_player( channel_id, animation_callback, callback_uid, managed_resource_ptr(NULL), this );`
pcstr: extra `push 0FFh` (u8 arg) ->
`m_user->subscribe_animation_player( channel_id, animation_callback, callback_uid, managed_resource_ptr(NULL), 0xff, this );`

## Anchors
Added to `use_game_core_weapon_core_small_setters()` in temp_include_all.cpp.

## Results (report.json top-level fuzzy_match_percent)
- set_fire_bullet_transform              -> 100.00% DONE
- set_next_fire_queue_type               -> 100.00% DONE
- remove_animation_callback(enum,..)     -> 100.00% DONE
- remove_animation_callback(pcstr,..)    -> 100.00% DONE
- set_animation_callback(enum,..)        ->  80.52% PARTIAL
- set_animation_callback(pcstr,..)       ->  81.17% PARTIAL

### vtable-slot fix (made the subscribe call hit the right overload slot)
MSVC assigns vtable slots to same-name overloaded virtuals in REVERSE declaration
order. base_player.h had subscribe(enum) declared BEFORE subscribe(pcstr), which put
enum at the HIGHER slot (+0x50) - target wants enum at +0x4C. Swapped the two
subscribe_animation_player declarations (enum now declared LAST) -> base call now
emits +0x4C (enum) / +0x50 (pcstr), matching target. (Confirmed by the matched
unsubscribe pair: enum declared 2nd -> +0x54 lower, pcstr declared 1st -> +0x58.)
Only weapon_core + weapon_user_animations_selector use these; the swap regressed
nothing (report-changes regressed: 0).

### set_animation_callback residual (PARTIAL, ~80-81%, the wall)
After the vtable fix, args/call/dtor/vtable-slot are ALL byte-identical. The sole
remaining diff is temporary scheduling: the target constructs the
`managed_resource_ptr( NULL )` temp FIRST (push 0; call ctor) and only then pushes
`this`, recomputing `&temp` via `lea ecx,[ebp-4]`; our base pushes `this` first, then
constructs the temp inline and pushes the ctor's eax return. `managed_resource_ptr()`
(default ctor) is wrong: it is `inline {}` (no push 0 / no call), so the `( NULL )`
pointer-ctor form is required to reproduce the ctor bytes.

claude@review (PR #208 audit): the matcher's "not source-steerable" framing is too
strong - temp-materialization ORDER is the kind of diff MATCHING.md expects solved
from source. UNTRIED re-match opportunity: bind the rvalue to a NAMED local declared
BEFORE the call, so the temp ctor is forced to run ahead of the argument pushes:

    resources::managed_resource_ptr tmp( NULL );
    m_user->subscribe_animation_player( channel_id, animation_callback, callback_uid, tmp, this );        // enum
    m_user->subscribe_animation_player( channel_id, animation_callback, callback_uid, tmp, 0xff, this );   // pcstr

The named local's dtor still fires at end-of-scope (function end), matching the target's
dtor placement at 0x40/0x43. Worth a rebuild before banking PARTIAL. (Reviewer did NOT
rebuild; flagged for a faster machine.) Verified vs report.json: enum 80.51724%,
pcstr 81.166664% - both percentages accurate.

base_player.h vtable swap - reviewer verification: the swap exchanges only the two
ADJACENT same-name `subscribe_animation_player` overloads (slots +0x4C/+0x50); no other
virtual's slot shifts. base_player is a pure-virtual interface and NO concrete subclass
in the matched source overrides subscribe (only weapon_core + weapon_user_animations_selector
CALL through it). report-changes.json: regressed []. Target vtable confirmed: enum overload
dispatches via `[edx+4Ch]` (verified in target asm @0x594240) -> enum genuinely at +0x4C.
Swap is safe.

regressions: none.
