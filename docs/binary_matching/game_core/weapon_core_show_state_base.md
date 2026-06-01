# game_core::weapon_core_show_state_base::{ctor,initialize,finalize,on_animation_end_impl}

Group of four members of `survarium::weapon_core_show_state_base`
(derives from `weapon_core_animation_end_aware_state` -> `weapon_core_base_state`
-> `ai::fsm_state` + `resources::unmanaged_resource`).
Mirror of `weapon_core_aimed_state_base` (PR #124) / `weapon_core_idle_state_base`
(PR #123), but the DIRECT base is `weapon_core_animation_end_aware_state` (not
`weapon_core_base_state`), and there is a 4th member `on_animation_end_impl`.

Stacked on `match/game_core-weapon_core_aimed_state_base`.

Target rvas (from `pdb_rich_query --index binaries/rich/target/index.jsonl
--function weapon_core_show_state_base --list`):
- on_animation_end_impl  0x6ed320
- finalize               0x6ed350
- ctor                   0x6ed380
- initialize             0x6ed3f0
(The carcass 0x6fd... addresses are the stale BASE-build rvas.)

## Access chars from the target COFF (read up front)
`strings binaries/objdiff/target/vostok/game_core/sources/weapon_core_show_state_base.cpp.obj`:
- ctor:                  `??0...@@IAE@AAVweapon_core@1@AA_N@Z`  -> I = protected, (weapon_core&, bool&)
- initialize:            `?initialize@...@@MAEXXZ`              -> M = PROTECTED virtual
- finalize:              `?finalize@...@@MAEXXZ`                -> M = PROTECTED virtual
- on_animation_end_impl: `?on_animation_end_impl@...@@EAEXAA_N@Z` -> E = PRIVATE virtual, (bool&)

## Target asm

### ctor (0x6ed380)
- push 1; push weapon; call weapon_core_base_state::weapon_core_base_state  (the
  inline weapon_core_animation_end_aware_state(weapon, TRUE) ctor forwards to base)
- vtable stores for weapon_core_animation_end_aware_state (fsm_state@0, unmanaged_resource@0x18)
- add ecx,138h; call resource_ptr<...>  (m_animation_to_wait_for @ 0x138, the aware-state member)
- vtable stores for weapon_core_show_state_base
- mov [edx+140h],eax                    (m_is_shown ref store @ 0x140)
- mov dword [ecx+130h], 0FFFFFFFDh       (m_body_part_mask_for_user = -3 = body_part_whole_body_but_hands)
  -> the base ctor set it to body_part_whole_body (-1); show OVERRIDES to -3 in its body (line 22).
SOURCE: init list weapon_core_animation_end_aware_state(weapon,true), m_is_shown(is_shown);
        body: m_body_part_mask_for_user = animation::body_part_whole_body_but_hands;

### initialize (0x6ed3f0)
- call weapon_core_animation_end_aware_state::initialize         (line 27)
- mov byte[ebp-1],0; lea eax,[ebp-1]; call <empty finalize_impl> (line 28) -- see MYSTERY below
- (line 29 inlined, no instrs)
- mov ecx,[ecx+128h]; call weapon_core::instant_toggle_start     (line 30; NON-virtual direct call)

### finalize (0x6ed350)
- call weapon_core_animation_end_aware_state::finalize           (line 35)
- (line 36 inlined, no instrs)
- mov ecx,[eax+128h]; call weapon_core::instant_toggle_end       (line 37; NON-virtual direct call)

### on_animation_end_impl (0x6ed320)
- mov byte[ebp-1],0; lea eax,[ebp-1]; call <empty finalize_impl> (line 42) -- MYSTERY (same as init line 28)
- mov edx,[ecx+140h]; mov byte[edx],1                            (line 43; *m_is_shown = true)
- mov eax,[ebp+8]; mov byte[eax],1                               (line 44; animation_player_tick_result = true)

## MYSTERY: `mov byte[ebp-1],0; lea eax,[ebp-1]; call <empty>`
Appears in initialize (line 28) and on_animation_end_impl (line 42), NOT in finalize.
The call target (0x03f210) is a single `ret` (empty COMDAT-folded fn the delinker
misnames `fixed_size_allocator<task_type,mutex_tasks_unaware>::finalize_impl`).
Documented "stripped/inlined call" pattern (assembly_patterns.md): a local bool=false
whose address is passed to an empty/inlined helper. NOT obviously reproducible from
this function's source. Plan: build the confident bodies first, read the diff to see
the exact residual, then decide reproduce-vs-PARTIAL.

## Commands run (verbatim)
- git checkout match/game_core-weapon_core_aimed_state_base
- git checkout -b match/game_core-weapon_core_show_state_base
- pdb_rich_query --index binaries/rich/target/index.jsonl --function weapon_core_show_state_base --list
- pdb_fetch --target-index binaries/rich/target/index.jsonl --function weapon_core_show_state_base --rva 0x6ed380 --view target  (ctor)
- pdb_fetch ... --rva 0x6ed3f0 --view target  (initialize)
- pdb_fetch ... --rva 0x6ed350 --view target  (finalize)
- pdb_fetch ... --rva 0x6ed320 --view target  (on_animation_end_impl)
- pdb_fetch ... --function finalize_impl --rva 0x03f210 --view target  (single ret)
- strings binaries/objdiff/target/.../weapon_core_show_state_base.cpp.obj | grep show_state_base
- python3 scripts/rebuild.py   # NO module arg

## Iterations
1. INPUT: header - ctor protected:, initialize/finalize protected: virtual,
   on_animation_end_impl private: virtual, forward-decl + friend of
   use_game_core_weapon_core_show_state_base. .cpp bodies:
   - ctor: init list `weapon_core_animation_end_aware_state(weapon,true), m_is_shown(is_shown)`
     + body `m_body_part_mask_for_user = animation::body_part_whole_body_but_hands;`
   - initialize: `weapon_core_animation_end_aware_state::initialize(); m_weapon.instant_toggle_start();`
   - finalize: `weapon_core_animation_end_aware_state::finalize(); m_weapon.instant_toggle_end();`
   - on_animation_end_impl: `m_is_shown = true; animation_player_tick_result = true;`
   anchor: concrete_show_state stub (overrides pure weapon_and_hands_expression),
   construct(weapon, is_shown), qualified non-virtual initialize()/finalize()/
   on_animation_end_impl(tick_result), escape &state/&is_shown/&tick_result.
   (The MYSTERY byte+call statement on initialize line 28 / on_animation_end_impl
   line 42 deliberately left out of the first pass to read its exact residual.)
   BUILD (rebuild.py, no module arg): code 27.08% -> 27.09% (+0.01).
   62 regressed / 63 improved - the regressed set is ALL unrelated baseline-artifact
   churn (scalar-deleting-dtors, btHashMap, particle, scaleform, ai-navigation) from the
   rotating prior-branch baseline; NONE of my four functions or any sibling state regressed.
   SCORES: ctor 100%, finalize 100%, initialize 75.13%, on_animation_end_impl 69.93%.
   DIFF: base initialize == target EXCEPT the missing byte+call (line 28); base
   on_animation_end_impl == target EXCEPT the leading byte+call (line 42). Both
   functions are byte-exact for every other statement.

## Outcome
STATE: ctor 100% DONE, finalize 100% DONE, initialize 75.13% PARTIAL,
on_animation_end_impl 69.93% PARTIAL.

The PARTIAL pair's ONLY residual is the line-28/line-42 statement
`mov byte[ebp-1],0; lea eax,[ebp-1]; call <empty>`:
- The call target is a single `ret` (empty), delinker-misnamed via COMDAT folding.
- The arg `&local` is passed in EAX with no push and no stack cleanup - matches no
  writable C++ calling convention.
- No nameable symbol for the callee exists in EITHER rich index (target or base).
- set_is_firing_ptr / set_is_firing are inlined everywhere (no standalone symbol in
  either index), so the call is NOT one of them.
This is the documented stripped/folded out-of-line call artifact (assembly_patterns.md
`mov byte[ebp-1],0; lea eax,[ebp-1]; call empty_stub`), here mid-body with a source
line rather than a prologue. Not reproducible from this function's source -> PARTIAL.
Regressions caused: none (baseline-artifact churn only).

Key learnings (added to assembly_patterns.md):
- ctor override of an inherited member default: base ctor set m_body_part_mask_for_user
  = body_part_whole_body (-1); the derived show ctor's body OVERWRITES it to
  body_part_whole_body_but_hands (-3) -> the trailing `mov [this+130h],0FFFFFFFDh`. The
  member is protected on weapon_core_base_state, accessible from the derived body.
- instant_toggle_start/end are NON-virtual on weapon_core -> direct `call` (contrast
  aimed's virtual instant_aim_start/end via vtable). Read the .h `virtual` keyword.
- The stripped/folded empty `call` can appear MID-body with a real source line (here
  line 28/42), not only as a prologue artifact - same unreproducible class.
