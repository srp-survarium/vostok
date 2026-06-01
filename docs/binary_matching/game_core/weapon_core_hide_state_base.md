# weapon_core_hide_state_base (group: ctor, initialize, finalize, on_animation_end_impl)

MIRROR of `weapon_core_show_state_base` (PR #125). Same class shape
(`weapon_core&`, `bool&` ctor + initialize/finalize/on_animation_end_impl), same
multiple-inheritance state hierarchy, same LTCG-folded empty out-of-line call
artifacts. One difference in `on_animation_end_impl` (sets `m_is_shown = false`
where show sets it `= true`).

## Branch
Stacked on `match/game_core-weapon_core_show_state_base` (PR #125, the stack tip).
```
git checkout match/game_core-weapon_core_show_state_base
git checkout -b match/game_core-weapon_core_hide_state_base
```

## Target lookups
```
pdb_rich_query --index binaries/rich/target/index.jsonl --function weapon_core_hide_state_base --list
```
RVAs (target):
- ctor                  0x6ed280
- initialize            0x6ed2f0
- finalize              0x6ed250
- on_animation_end_impl 0x6ed220
- is_ready_for_transition 0x087fd0 (inline in header: `mov al,[ecx+135h]; ret` = has_animation_ended())

```
pdb_fetch --target-index binaries/rich/target/index.jsonl --view target --rva <rva>
```

### ctor (0x6ed280)
Identical shape to show ctor: `call weapon_core_base_state::weapon_core_base_state`
(weapon, true) [base aware-state passes `true`], two vtable stores (fsm_state @0,
unmanaged_resource @0x18), the `add ecx,138h; call resource_ptr<base_scene>::ctor`
member init, `mov [edx+140h], eax` (m_is_shown ref store from [ebp+0Ch]), then
`mov dword ptr [ecx+130h], 0FFFFFFFDh` = m_body_part_mask_for_user =
body_part_whole_body_but_hands (-3), overriding the base ctor's -1. -> body:
`m_body_part_mask_for_user = animation::body_part_whole_body_but_hands;`

### initialize (0x6ed2f0)
`call weapon_core_animation_end_aware_state::initialize`; then `mov byte[ebp-1],0;
lea eax,[ebp-1]; call fixed_size_allocator::finalize_impl` (LTCG-folded empty
out-of-line call, unreproducible - documented artifact); then `mov ecx,[ecx+128h];
call weapon_core::instant_toggle_start`. -> base init + m_weapon.instant_toggle_start().

### finalize (0x6ed250)
`call weapon_core_animation_end_aware_state::finalize`; `mov ecx,[eax+128h];
call weapon_core::instant_toggle_end`. -> base finalize + m_weapon.instant_toggle_end().
No folded-call artifact here (matches show finalize = 100%).

### on_animation_end_impl (0x6ed220)
`mov byte[ebp-1],0; lea eax,[ebp-1]; call fixed_size_allocator::finalize_impl`
(folded empty call); then `mov edx,[ecx+140h]; mov byte ptr [edx], 0` =
**m_is_shown = false** (show set it to 1); then `mov eax,[ebp+8]; mov byte ptr
[eax], 1` = animation_player_tick_result = true.

## Mangled access chars (target COFF weapon_core_hide_state_base.cpp.obj)
- ctor:                 `??0...@@IAE@...`     I = protected
- initialize:           `...@@MAEXXZ`         M = protected virtual
- finalize:             `...@@MAEXXZ`         M = protected virtual
- on_animation_end_impl:`...@@EAEXAA_N@Z`     E = private virtual
Header updated to match (was all-public + private impl): ctor/init/finalize ->
protected, on_animation_end_impl -> private, friend the anchor.

## Anchor
Mirror of show: `use_game_core_weapon_core_hide_state_base` in temp_include_all.cpp -
concrete derived stub (overrides pure weapon_and_hands_expression), qualified
non-virtual calls to initialize/finalize/on_animation_end_impl, escape &state /
&is_shown / &tick_result through example_callback. Added include + IncludeAll call.

## Expected outcome (pre-build, by mirror)
- ctor: 100% (show ctor was 100%, same shape).
- finalize: 100% (show finalize was 100%).
- initialize: ~75% PARTIAL (LTCG-folded empty call, same as show initialize 75.13%).
- on_animation_end_impl: ~70% PARTIAL (same folded call, same as show 69.93%).

## Build
```
python3 scripts/rebuild.py      # NO module arg - relinks the EXE
```

## Build 1 failure: pre-existing brace loss in temp_include_all.cpp (inherited)
First `rebuild.py` failed: `temp_include_all.cpp(313): error C2601 'use_character_
dispersion_calculator': local function definitions are illegal` + `C1075 EOF before
matching brace`. Root cause was NOT my edit (verified `git diff HEAD` clean except
my additions; `git show HEAD:` confirmed the breakage is committed on the show branch
tip). FIVE anchor functions were missing their closing `}` (each ran into the next
`void use_*`): use_dispersion_calculator, use_character_dispersion_calculator,
use_weapon_dispersion_calculator, use_client_player_update, use_game_core_weapon_state.
Inserted the five missing `}`. This is the non-target anchor TU, so it cannot regress
any matched object's bytes; it only lets the file compile so the anchors work. Brace
balance verified 0 (never negative) after the fix. Rebuilt.

## Results (FINAL, after ASSERT recovery)
report.json fuzzy_match_percent - ALL FOUR 100% DONE:
- ctor                  100.0    DONE
- finalize              100.0    DONE
- initialize            100.0    DONE   (was 75.13 pre-ASSERT; recovered, see CORRECTION + SWEEP below)
- on_animation_end_impl 100.0    DONE   (was 69.93 pre-ASSERT; recovered, see CORRECTION + SWEEP below)

CORRECTION (new guidelines): the `mov byte[ebp-1],0; lea eax,[ebp-1]; call empty_stub`
(delinker-misnamed `finalize_impl`) in initialize (between base init() and
instant_toggle_start(), target offset 0x11) and at the head of on_animation_end_impl
(target offset 0x09) is a COMPILED-OUT ASSERT, NOT an LTCG-folded empty call. Recovered
both with `ASSERT( UNKNOWN_EXPRESSION );` at the matching source statement (mirrors the
weapon_core_show_state_base fix). New scores TBD by the sweep rebuild.

Exactly mirrors weapon_core_show_state_base (same numbers). is_ready_for_transition
(inline header accessor, not in this unit) scores None - same as show, unpaired.

## Regressions: NONE (all 57 in report-changes are stale-baseline artifacts)
report-changes.json: 57 "regressed" but 56 are `100.0 -> 0.0` of unrelated
CRT/library/other-module symbols (btSphereShape/Scaleform/boost binders/empty_stub/
float3+float4+float4x4 default ctors) - the empty-function ICF/strip visibility flip
class (assembly_patterns.md). The 57th is `scheduler::on_frame 52.33 -> 46.38`, which
is its DOCUMENTED true PARTIAL value (46.39, the size()/operator[] inline-vs-call) -
the 52.33 was the stale-baseline noise, not a real prior score. Triage per
loop_performance.md: my git diff touches ONLY hide_state_base.{cpp,h} + the
temp_include_all anchors; none of the regressed symbols are reachable through my
change. The prior report.json was generated on a tree where temp_include_all.cpp did
NOT compile (the inherited brace bug), so its baseline reflected a different reachable-
symbol set. The show functions (ctor/finalize 100, init 75.13, on_anim 69.93) are
intact and unchanged - confirming no real regression.

SWEEP RESULT (rebuild after ASSERT recovery): initialize 75.13% -> 100% DONE,
on_animation_end_impl 69.93% -> 100% DONE. The recovered ASSERT closed the gap.
