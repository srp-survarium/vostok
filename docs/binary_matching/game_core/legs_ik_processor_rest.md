# game_core::legs_ik_processor (remaining functions, first pass)

One unit matching all remaining STUB/PARTIAL functions in legs_ik_processor.cpp.

Branch off `chore/remove-slow-machine-notes`.

## Functions in scope
- s_ik_legs_debug_draw_cc / s_ik_foot_capsule_radius_cc / s_ik_legs_rot_axis_cc /
  s_ik_adjust_hip_position_cc dynamic initializers + atexit destructors (console vars)
- leg_params::leg_params(), leg_params::activate(skeleton,pcstr), leg_params::tick(float)
- legs_ik_processor::legs_ik_processor(), ~legs_ik_processor(), activate(skeleton),
  process(float4x4*,float4x4 const&), tick(u32)
- get_additional_length (re-verify PARTIAL, LTCG inline-vs-call, stays)

## Key discoveries
- Target RVAs are shifted vs the carcass comments: index uses 0x6ea.. not 0x6fa..,
  and cc initializers are at 0x7cb.. not 0x7db...
- The s_ik_*_cc are `console_commands::cc_bool` / `cc_float` static initializers,
  same shape as dispersion_calculator.cpp / bullet.cpp. Names from the ??_C@ mangled
  string constants. There is a FIFTH cc not in the STUB list: s_ik_adjust_hip_position_cc
  (rva 0x7cb290), referenced by process(); also declared.
- cc_bool ctor asm: push <serializable>, push &value, push name; eax=1, ecx=<filter>,
  esi=&cc. cc_float: push 1, push <max via fld/fstp>, push &value, push name; eax=1,
  ecx=0, xmm0=<min>.
- skeleton_bone is 0x14 bytes; m_parent at +4, m_children_begin at +8. leg_params::
  activate walks the parent chain (foot->leg->knee->up_leg) + children_begin for toe;
  each index = skeleton::get_bone_index(bone) = (&bone-&get_root()) + R_ASSERT_CMP.
- ik_processor::m_last_time_in_ms moved private->protected (no byte change) so tick()
  can read/write it.
- transition_time_calculator::tick(float) = `m_value += arg`; is_on_ground() =
  heel || toe (decoded from process() ground-state tests).

## Commands run (verbatim, in order)
- git checkout chore/remove-slow-machine-notes
- pdb_rich_query --index binaries/rich/target/index.jsonl --function '<name>' --list
- pdb_fetch --target-index binaries/rich/target/index.jsonl --rva <rva> --view target/structure/callees/info
- python3 scripts/rebuild.py   (full, no module arg)

## Iterations
(see below, appended per rebuild)

## Iterations (scores from binaries/objdiff/report.json)

1. INPUT: first cut of all functions (cc statics; leg_params ctor/activate/tick; legs_ik ctor/activate/tick;
   dtor=DELETE(m_drawer); process full ladder). ASSERT(UNKNOWN_EXPRESSION_T(success)).
   BUILD: two compile errors - DELETE not found (game_core_memory.h has the macro commented out -> define DELETE
   locally), try_invert on const (had transform.try_invert(inverted); correct is inverted.try_invert(transform)).
2. INPUT: fixed both.
   BUILD: leg_params ctor 100, leg_params::activate 100, legs_ik::activate 100, legs_ik::tick 100,
   leg_params::tick 97.42, legs_ik ctor 78.79, dtor 85.71, process 86.43. cc inits unscored (report.json None; pairing artifact).
   DIFF(ctor): target writes m_*_transition_time_calculator.m_value right after the interpolators; my empty
   calculator ctor skipped it. DIFF(process): ASSERT block entirely target-only (ASSERT(success) compiled out -
   warning C4189 'success unreferenced'); matrices->c.xyz().y emitted an out-of-line xyz() call + temp.
3. INPUT: transition_time_calculator() : m_value(0.1f); ASSERT->ASSERT(success); c.xyz().y -> c.y.
   BUILD: legs_ik ctor 78.79 -> 100 (0.1f const guess was exact), process 86.43 -> 90.
   DIFF(process): ASSERT(success) STILL produced nothing (macro no-op in Gold) -> target ASSERT references success.
4. INPUT: ASSERT(success) -> R_ASSERT(success).
   BUILD: process stays 90 (R_ASSERT now emits the assert-enabled predicate the target also has, but target
   additionally evaluates+reports success: `movzx success; push; push 0; call; add esp,8; movzx; test; je` - an
   assert-variant difference). Net more matched instructions; kept R_ASSERT.

## Outcome
All remaining functions implemented + anchored. 100%: leg_params ctor/activate, legs_ik ctor/activate/tick.
97.42% leg_params::tick (extra LTCG frame slot). 85.71% dtor (ICF-folded member-dtor this-setup).
90% process (get_skeleton temp-spill 0xC frame shift + R_ASSERT report half). cc inits effectively DONE (report.json
None - universal cc dynamic-init/atexit name-pairing artifact, not 0%). get_additional_length stays 65.38% (operator| inline-vs-call). Regressions: layout churn on unrelated
symbols from added statics (objdiff name-pairing), not code regressions; report-changes.json clean on final rebuild.

## Structure-verifier v2 pass (2026-06-07)

REAL FIX (process, 90.00 -> 92.60%): the `R_ASSERT( success )` after `try_invert`
was the wrong assert macro. The target at 0xff emits the MASTER_GOLD `_U` assert form
- `VOSTOK_EMPTY_EXPRESSION_U_VA_ARGS` = `if ( identity(false) ) { expression_eater(
assert_type, success ); } else (void)0` - i.e. it materializes and passes the asserted
expression to the eater: `mov byte[ebp-169h],0; lea eax,..; call stub; movzx [eax];
test; je; push success; push 0; call`. Plain `R_ASSERT` expands to
`VOSTOK_EMPTY_EXPRESSION_VA_ARGS` (no expression reference) and emitted only the 0x12
stub. Changed to `R_ASSERT_U( success )`; the R_ASSERT row is now byte-identical and the
fn jumped 90.00 -> 92.60. Sole residual now is the get_skeleton()/*m_skeleton spill (0xC
per get_bone_matrix_in_object_space call; target inlines [m_skeleton]) - a call-boundary
LTCG spill, the L155/L157-vs-3rd-branch swap in the diff is an aligner artifact, the 4-way
ground ladder is fully present.

CONFIRMED: the prior "is_full_on_ground vs is_on_ground" suspicion for `process`'s final
branch is RESOLVED as `is_on_ground()` (the asm at .9/0x4b2 and the discarded
`m_right_leg_params.is_on_ground()` at .12/0x516 both test heel `[+2Ch]` THEN-OR toe
`[+2Dh]` = `heel || toe`; `is_full_on_ground` would be `heel && toe`). Source is correct,
no change. Same check on get_foot_fixed_transform: first branch tests both bytes AND-wise
(heel `je` next THEN toe `je` next) = `is_full_on_ground()` (correct); second = heel-only
`is_heel_on_ground()`; third = toe-only `is_toe_on_ground()`. All correct.

WALLS confirmed (structure MATCH, non-steerable residual; carcasses replaced with embeds):
- ~legs_ik_processor 85.71%: 1/1 stmt clean; residual is the auto-emitted member-dtor
  epilogue (target sets this+0x7Ch/this+0x70h before each ~fermi_interpolator; base
  ICF-folds the this-setups). Codegen, not source.
- leg_params::tick 97.42%: 3/3 clean; one extra 4-byte frame slot (LTCG phantom temp at
  math::max boundary).
- set_heel_on_ground 98.84% / set_toe_on_ground 98.59%: sole SIZE on the
  m_*_interpolator = fermi_interpolator(...) row = lea ecx,[ebp-0Ch] for the COMDAT-folded
  ~fermi_interpolator temp dtor this arg (ICF/LTCG).
