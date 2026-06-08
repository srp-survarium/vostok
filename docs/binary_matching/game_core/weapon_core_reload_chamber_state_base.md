# weapon_core_reload_state_base + weapon_core_chamber_a_round_state_base (6 fns, one unit)

Two sibling fsm-state-base classes of identical shape, derived from
`weapon_core_animation_end_aware_state` (which itself derives `weapon_core_base_state`).
Matched together in ONE rebuild (103s, watchdog not engaged).

## Branch
- Stack tip / PR base: `match/game_core-character_dispersion_calculator` (HEAD 96009512).
- New branch: `match/game_core-weapon_core-reload-chamber-state-base`.

## Result
| fn | % | tag |
|---|---|---|
| reload::weapon_core_reload_state_base(weapon_core&, float) | 100 | DONE |
| reload::initialize() | 99.83 | DONE (round_is_chambered out-lined; sole residual = eax-vs-ecx this-load arg-passing) |
| reload::on_animation_end_impl(bool&) | 100 | DONE |
| chamber::weapon_core_chamber_a_round_state_base(weapon_core&, float) | 100 | DONE |
| chamber::initialize() | 100 | DONE |
| chamber::on_animation_end_impl(bool&) | 100 | DONE |

(serialize/deserialize on both remain BLOCKED - udp_match_packet/packet_reader
never-compiled cluster, untouched.)

## Commands
```
pdb_rich_query --index binaries/rich/target/index.jsonl --list --function weapon_core_reload_state_base
pdb_rich_query --index binaries/rich/target/index.jsonl --rva 0x7666b0   # reload ctor
pdb_rich_query --index binaries/rich/target/index.jsonl --rva 0x766720   # reload initialize
pdb_rich_query --index binaries/rich/target/index.jsonl --rva 0x766790   # reload on_animation_end_impl
pdb_rich_query --index binaries/rich/target/index.jsonl --rva 0x751cc0   # chamber ctor
pdb_rich_query --index binaries/rich/target/index.jsonl --rva 0x751d30   # chamber initialize
pdb_rich_query --index binaries/rich/target/index.jsonl --rva 0x751c40   # chamber on_animation_end_impl
python3 scripts/rebuild.py
```

## Target asm -> source

### ctor (both, identical shape)
```
push 1; push [ebp+8]; call weapon_core_base_state::weapon_core_base_state   ; : weapon_core_animation_end_aware_state(weapon, true) inlined to base_state(weapon,true)
mov [this],   ??_7animation_end_aware_state...   ; vtable stores (compiler-emitted)
mov [this+18],...
add ecx,138h; call resource_ptr<base_scene,...>  ; m_animation_to_wait_for member ctor (@0x138)
mov [this],   ??_7<derived>...                   ; derived vtables
mov [this+18],...
movss xmm0,[ebp+0Ch]; movss [this+140h],xmm0      ; m_animation_timescale = animation_time_scale  (@0x140)  <-- FIRST
mov [this+130h], 0FFFFFFFDh                        ; m_body_part_mask_for_user = body_part_whole_body_but_hands (-3, @0x130)  <-- SECOND
```
SOURCE body (order matters - 0x140 store before 0x130):
```cpp
m_animation_timescale     = animation_time_scale;
m_body_part_mask_for_user = animation::body_part_whole_body_but_hands;
```
(The existing reload stub was missing the timescale store; chamber had neither.)

### reload::initialize (92%, the only non-100)
```
call animation_end_aware_state::initialize
call weapon_core_base_state::deserializing ; test al; jne end   -> if (!deserializing()
mov edx,[this+128h]; mov al,[edx+48Fh]; mov [ebp-1],al; test; je end   -> && chamber_a_round_on_reload()  (inlined, @0x48F)
TARGET: mov eax,[this+128h]; call weapon_core::round_is_chambered     -> && round_is_chambered()  (OUT-OF-LINE call)
test; je end
m_weapon.unload_chambered_round();
```
SOURCE:
```cpp
weapon_core_animation_end_aware_state::initialize();
if ( !deserializing() && m_weapon.chamber_a_round_on_reload() && m_weapon.round_is_chambered() )
    m_weapon.unload_chambered_round();
```
RESIDUAL (FIXED by reviewer) = inline-vs-call of the trivial accessor `round_is_chambered()`:
- target keeps it standalone (`pdb_rich_query target ...round_is_chambered` -> 0x09b360) and emits `call`.
- our /GL was inlining it at this site -> `mov cl,[eax+48Eh]` (reads m_is_round_chambered @0x48E).
- `chamber_a_round_on_reload()` is inlined on BOTH sides (no standalone in either), so it matches.
FIX (NOT unsteerable - same class the #153 matcher already solved): out-lined `round_is_chambered`
(decl in weapon_core.h, body in weapon_core.cpp, mirroring ammo_in_magazine). It has exactly ONE
matched call site (this file; the other "round_is_chambered" tokens are a struct field in
weapon_ammo_info.h, unrelated), so out-lining is regression-free. Rebuilt: 92% -> 99.83%, no
real regressions (only symmetric ICF fold-rep churn). The remaining 0.17% is the call's `this`
load `mov eax,[edx+128h]` (target) vs `mov ecx,[edx+128h]` (base) - round_is_chambered @0x09b360
takes `this` in eax (link-time custom calling convention), the permitted call-boundary arg-passing
class (identical to #153's ammo_in_magazine). Now DONE, not PARTIAL.

### reload::on_animation_end_impl / chamber::on_animation_end_impl (100%)
```cpp
m_weapon.instant_reload();           // chamber: m_weapon.instant_chamber_a_round();
animation_player_tick_result = true; // mov byte [arg], 1
```

### chamber::initialize (100%)
```cpp
weapon_core_animation_end_aware_state::initialize();   // base call only - no deserializing/unload branch
```

## Source changes
- `weapon_core.h`: filled inline getter `chamber_a_round_on_reload() { return m_chamber_a_round_on_reload; }` (@0x48F).
- Both headers: ctor public->protected (IAE), initialize stays virtual but moved protected (MAE),
  serialize/deserialize/on_animation_end_impl moved private (EAE/EBE); added
  `friend void ::vostok::use_game_core_weapon_core_<x>_state_base();` + the namespace fwd decl.
- `temp_include_all.cpp`: includes + two `use_game_core_weapon_core_{reload,chamber_a_round}_state_base`
  anchors (concrete derived stub overriding the still-pure `weapon_and_hands_expression`, qualified
  calls to initialize/on_animation_end_impl, escape &state/&tick_result) + dispatcher calls.

## Access mangling (read from binaries/rich/target/index.jsonl)
- `??0...@@IAE` ctor = protected
- `?initialize@...@@MAE` = protected virtual
- `?serialize@...@@EBE` = private virtual const, `?deserialize@/on_animation_end_impl@...@@EAE` = private virtual

## Rebuild
- ONE rebuild, 103s, watchdog NOT engaged.
- report-changes: 10 regressed (all ICF fold-rep churn - boost storage ctors, float3/size_policy
  ctors, scaleform vector-deleting-dtor, interlocked_decrement, sun_cascade copy ctor, network::world
  vcall thunks; none touch this unit), 33 improved (incl. all 6 of this unit's scored fns + empty_stub).

## Review #2 (reviewer, batchA - LOGIC CHANGE on reload::initialize)
The 92% PARTIAL residual was banked as "unsteerable trivial-accessor inline-vs-call". That is the
check-4 mistake: the #153 matcher had ALREADY solved this exact class (ammo_in_magazine) by
out-lining the accessor. round_is_chambered has ONE matched call site, so out-lining it (decl
weapon_core.h, body weapon_core.cpp) is regression-free. Rebuilt: reload::initialize 92% -> 99.83%
DONE, no real regressions. The sole 0.17% is the eax-vs-ecx this-load (permitted arg-passing). The
table/STATE/PROGRESS below are updated to 99.83% DONE.

## Review #1 (no logic change - superseded for reload::initialize by Review #2)
Audited against report.json + both rich disassemblies:
- All 6 percents agree across .cpp STATE / this .md / PROGRESS / report.json:
  reload {100, [92 -> now 99.83], 100}, chamber {100, 100, 100}.
- No base/target swap. reload::initialize @0x43: TARGET `call ...round_is_chambered`,
  BASE `mov cl,[+48Eh]` (verified both views) - the comment is the right way round.
- The "identical-shape, why chamber=100" question is a false premise: the two
  `initialize` bodies DIFFER. chamber::initialize (target rva 0x751d30) is a bare
  `weapon_core_animation_end_aware_state::initialize();` - no accessor calls, nothing to
  inline-vs-call. Only reload::initialize (0x766720) has the `!deserializing() &&
  chamber_a_round_on_reload() && round_is_chambered()` guard. So the 92% residual is real
  and not cross-contaminated from the sibling.
- The 92% was a trivial-accessor inline-vs-call (round_is_chambered standalone in target
  @0x09b360, inlined in base; chamber_a_round_on_reload absent in BOTH so it matches).
  CORRECTION (Review #2): this was NOT unsteerable - out-lining round_is_chambered took it to
  99.83% DONE. The "permitted unsteerable class" conclusion here was wrong.
- ctor stores (`movss [+140h]` before `mov [+130h],0FFFFFFFDh`), the filled
  chamber_a_round_on_reload getter, and the access specifiers (IAE/MAE->protected,
  EAE/EBE->private per the target mangled names) all verified.
- Lean policy ok: 5 DONE keep bare STATE lines (carcass+signature deleted, no logs);
  the PARTIAL keeps its `// FUNCTION BODY` carcass verbatim with right-side annotations.
