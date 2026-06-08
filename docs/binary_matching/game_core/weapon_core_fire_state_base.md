# weapon_core_fire_state_base (class unit, 6 functions)

One class unit, one rebuild (698s, ninja watchdog not engaged). Branch
`match/game_core-weapon_core_fire_state_base` off
`match/game_core-weapon_core-reload-chamber-state-base` (HEAD 6ef80f5d).

## Targets (rva, all in vostok/game_core/sources/weapon_core_fire_state_base.cpp)
- ctor                 0x58ec20  ??0...@@IAE@AAVweapon_core@1@M@Z   (protected)
- initialize           0x58ec90  @@MAEXXZ                          (protected virtual)
- execute              0x58ec00  @@MAEXXZ                          (protected virtual)
- finalize             0x58ebb0  @@MAEXXZ                          (protected virtual)
- on_animation_end_impl 0x58eb90 @@MAEXAA_N@Z                      (protected virtual)
- on_shot_event        0x58ed80  @@MAE?AW4callback_return_type_enum (protected virtual)

## Commands
```
pdb_rich_query --index binaries/rich/target/index.jsonl --function "weapon_core_fire_state_base" --list
pdb_fetch --target-index binaries/rich/target/index.jsonl --rva <rva> --view target   # for each of the 6
grep -oE '\?[a-z_]+@weapon_core_fire_state_base@survarium@@[A-Z]{3}[^"]*' binaries/rich/target/index.jsonl | sort -u   # access chars (all M=protected virtual; ctor I)
python3 scripts/rebuild.py    # 698s
pdb_fetch ... --rva <rva> --view diff --objdiff-base-dir binaries/objdiff/base --objdiff-target-dir binaries/objdiff/target
```

## Source decode (read straight from target asm before the build)
- **ctor**: `: weapon_core_animation_end_aware_state(weapon, true)` (push 1 then `call
  weapon_core_base_state::weapon_core_base_state` - the inline intermediate ctor is inlined).
  Body: `m_animation_timescale = animation_timescale;` (movss [+140h]),
  `m_playback_type = animation::mixing::play_cyclically;` (mov [+144h],0). NO store to
  m_body_part_mask_for_user (@0x130) - differs from reload/chamber ctors which DO overwrite it.
  The resource_ptr ctor @+0x138 and both vtable-store pairs are compiler-emitted.
- **execute**: `weapon_core_base_state::execute();` then `m_animation_has_been_ended = false;`
  (mov byte[+135h],0). Qualified call to base_state (anim_end_aware does not override execute).
- **finalize**: `weapon_core_animation_end_aware_state::finalize();`,
  `m_weapon.remove_animation_callback("shoot", this);` (pcstr overload; push this=callback_uid,
  push "shoot"=channel; ecx=m_weapon @+0x128), a compiled-out ASSERT, `*m_is_firing_ptr = false;`
  (m_is_firing_ptr @+0x12C).
- **on_animation_end_impl**: just `animation_player_tick_result = true;` (mov byte[arg],1).
- **initialize**: base::initialize(); the boost::bind/set_animation_callback("shoot", this,
  bind(&on_shot_event,this,_1)) statement (same shape as anim_end_aware::initialize, 100%);
  `m_playback_type = animation::mixing::playback_enum(m_weapon.get_bullets_in_queue() <= 1);`
  (reads u16 @+0x47C = m_bullets_in_queue; xor edx; cmp ecx,1; setle dl -> enum store @+0x144);
  a compiled-out ASSERT; `*m_is_firing_ptr = true;`.
- **on_shot_event**: `params.interrupt_animation_player_tick = true;` (byte @+0x16); two leading
  compiled-out ASSERTs; `if (!m_weapon.get_bullets_in_queue()) { LOG_ERROR(...); return
  callback_return_type_call_me_again; }`; a compiled-out ASSERT;
  `m_weapon.instant_fire(params.callback_time_in_ms);` (u32 @+0x0C); a trailing compiled-out
  ASSERT; `return callback_return_type_call_me_again;`. Both returns are `xor eax,eax` (enum 0).
  The LOG block is the `__LOG`/`LOG_ERROR` macro expansion (g_log_filter_tree NULL-or-
  has_passed_filters ?: append), format=&g_log_format, data=g_log_flags, level 2=error,
  message string = "!m_weapon.get_bullets_in_queue()", __LINE__=82 (push 52h).

## Header/source changes
- weapon_core_fire_state_base.h: ctor public->protected; the 5 virtuals public->protected (all M).
  Added `friend void ::vostok::use_game_core_weapon_core_fire_state_base();` + fwd decl.
- weapon_core.h: filled `get_bullets_in_queue() { return m_bullets_in_queue; }` (@0x47C).
- temp_include_all.cpp: include + concrete_fire_state anchor (derived stub overriding the still-pure
  weapon_and_hands_expression with VOSTOK_UNREACHABLE_CODE), calls all 5 methods qualified + escapes
  &state/&anim/&params/&tick_result; dispatcher call added.

## Results (report.json, after one rebuild)
| fn | % | tag |
|---|---|---|
| ctor | 100 | DONE |
| initialize | 99.71 | DONE (4-byte /Od frame-size slot packing only) |
| execute | 99.09 | DONE (out-lined empty base_state::execute -> target `call`; sole residual = edx-vs-eax this-load reg-alloc) |
| finalize | 100 | DONE |
| on_animation_end_impl | 100 | DONE |
| on_shot_event | 95.26 | PARTIAL (logging __LINE__/__FILE__/__FUNCSIG__ + boost::function ctor reorder) |

## Residuals (diff details)
- **execute (80.91% -> 99.09%, STEERED, structure-verifier-v2 batchA)**: the TARGET emitted
  `call weapon_core_base_state::execute` then the member store; OUR BASE elided the call (empty
  body inlined at this call site under /GL). FIX: out-lined `weapon_core_base_state::execute` -
  moved its empty body from the `weapon_core_base_state.h` inline `{ /* 0x97f80 */ }` to a real
  out-of-line definition in `weapon_core_base_state.cpp` (decl kept in the header, `UAE` public
  virtual = the target's access). This is the same device class as round_is_chambered /
  ammo_in_magazine. After the rebuild the site emits `mov ecx,[ebp-4]; call <execute>` exactly
  like the target. Sole residual (the 0.91%): the following `m_animation_has_been_ended = false`
  store loads `this` as `mov edx,[ebp-4]` (target) vs `mov eax,[ebp-4]` (base) - a single
  call-boundary register-allocation choice (permitted arg-passing class). base_state::execute is
  the ONLY qualified call site in game_core (other states override execute), and the target keeps
  a standalone `UAE` execute @0x97f80 so out-lining matches the original. Regression-free (only
  symmetric ICF fold-rep churn; the standalone execute symbol itself folds into empty_stub's
  class). assembly_patterns.md entry "empty base virtual called via qualified".
- **initialize (99.71%)**: every instruction identical; base `sub esp,5Ch` vs target `58h` (4 bytes),
  cascading the [ebp-N] slot numbers. /Od stack-slot packing, not logic/structure.
- **on_shot_event (95.26%)**: residual is entirely inside the LOG_ERROR expansion - __LINE__ immediate
  (base push 4Bh=75 vs target 52h=82; our file is shorter, could pad to line 82 but the string relocs
  cap it regardless), __FILE__/__FUNCSIG__ string content, and a one-instruction boost::function
  log_callback temp-ctor reorder. Same "Logging issues" class as
  animation_analysis_result_cook::translate_query (87.15%).

## Regressions
None. report-changes: 21 regressed / 21 improved, all symmetric ICF fold-representative churn
(empty_stub, dtors, thunks, resource_ptr/buffer_string/interlocked/float3/btHashMap trivials).
None touch this unit's source (verified by grep for weapon_core_fire/base_state::execute/
get_bullets_in_queue in the regressed list - empty).
