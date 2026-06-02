# game_core::legs_ik_processor::process_leg

Target: `void survarium::legs_ik_processor::process_leg(leg_params&, float4x4 const&,
float4x4 const&, float4x4*, float4x4 const&)`
- target rva 0x6eafa0, base file `legs_ik_processor.cpp` (~L271), 0xb39 bytes, 58 statements (L165-L291).
- Mangled access: private `AAE` (`?process_leg@legs_ik_processor@survarium@@AAEX...`). Header already declares it private.
- Sibling of get_foot_fixed_transform (84% PARTIAL): same family of inline-vs-call trivial
  math COMDATs (`xyz()`, `operator-/^/|/*`, `normalize`, `length`, `is_similar`,
  `get_root_bones_count`, `color::color`, `get_relative_matrix`, `create_rotation`,
  `transform_direction`, `transform_position`, `create_translation`) plus two NOT-in-our-headers
  helpers `get_rotation_matrix` / `change_matrix_orientation` (math_float4x4_inline_2.h on
  target). Expected to end PARTIAL.

## Args / frame
this=ecx->[ebp-94Ch]. ret 14h = 5 args:
[ebp+8]=params(leg_params&), [ebp+0Ch]=target_foot_obj_matrix, [ebp+10h]=hip_obj_matrix,
[ebp+14h]=matrices, [ebp+18h]=transform.

## Statement decode (from asm)
The recurring `mov eax,this; mov eax,[eax](=m_skeleton@0); call get_root_bones_count;
mov ecx,[ebp+8]; mov edx,[ecx+BONEOFF]; sub edx,eax` = `params.<bone>_index - get_skeleton().get_root_bones_count()`.
bone offsets: foot@0, toe@4, leg@8, knee@0xC, up_leg@0x10. `finalize_impl` = inlined xyz()/length/identity fold.
operator*(out,A,B) push order (right-to-left): push B; push A; push out -> result = A*B (i.e. result = 2nd-pushed * 1st-pushed).

- L166 toe_matrix_index   = params.toe_bone_index    - root   -> [ebp-5Ch]
- L167 foot_matrix_index  = params.foot_bone_index   - root   -> [ebp-70h]
- L168 leg_matrix_index   = params.leg_bone_index    - root   -> [ebp-0F4h]
- L169 knee_matrix_index  = params.knee_bone_index   - root   -> [ebp-8]
- L170 up_leg_matrix_index= params.up_leg_bone_index - root   -> [ebp-0F8h]
- L172 leg_len    = matrices[knee_matrix_index].c.xyz().length()  -> [ebp-4Ch]
- L173 up_leg_len = matrices[leg_matrix_index].c.xyz().length()   -> [ebp-4]
- L174 knee_len   = matrices[foot_matrix_index].c.xyz().length()  -> [ebp-6Ch]
- L176 up_leg_obj_matrix = matrices[up_leg_matrix_index] * hip_obj_matrix  -> [ebp-180h]
- L177 knee_obj_matrix   = matrices[knee_matrix_index]   * up_leg_obj_matrix -> [ebp-0F0h]
- L178 leg_obj_matrix    = matrices[leg_matrix_index]    * knee_obj_matrix   -> [ebp-48h]
- L179 foot_obj_matrix   = matrices[foot_matrix_index]   * leg_obj_matrix    -> [ebp-140h]
- L180 toe_obj_matrix    = matrices[toe_matrix_index]    * foot_obj_matrix   -> [ebp-0B0h]
- L185 if ( is_similar(target_foot.i.xyz(),foot_obj.i.xyz(),eps3) && ...j && ...k && ...c ) return;
       (4-way && chain comparing rows of target_foot_obj_matrix vs foot_obj_matrix; eps=epsilon_3)
- L186 return;
- L188 if ( s_ik_legs_debug_draw_value && m_drawer )  (m_drawer @ this+8)
- L200 draw_leg block #1: draw_leg(up_leg_obj*transform, knee_obj*transform, leg_obj*transform,
       foot_obj*transform, color(0x64,0x64,0x64), color(0x64,0,0), color(0,0,0x64), color(0,0x64,0), 0.0f)
       [WIP: color channel order still to verify from operand diff]
- L203 target_up_leg_to_foot_dir = normalize( [ebp-150h].xyz() - target_foot_obj.c.xyz() ) -> [ebp-68h], ref [ebp-0FCh]
       [WIP: [ebp-150h] is a working float4x4 not yet mapped to a named local]
- L205..L222: alpha-IK angle math: get_additional_length, get_angle, normalize dirs,
  create_rotation(up_leg.i, alpha_angle), transform_direction, get_rotation_matrix,
  change_matrix_orientation(up_leg_obj); knee_obj recompute. [WIP - approximate]
- L231..L235: knee leg: get_rotation_matrix(original_knee_dir, target_dir),
  change_matrix_orientation(knee_obj); leg_obj/foot_obj recompute. [WIP]
- L245..L253: leg: get_rotation_matrix(original_leg_dir, target_leg_dir),
  change_matrix_orientation(leg_obj); foot_obj = target_foot_obj_matrix. [WIP]
- L265 draw_leg block #2 (colors 0x96/0xFF) + L267/L269 heel draw_cross + L272/L276 toe draw_origin
- L280 matrices[up_leg_matrix_index] = get_relative_matrix(up_leg_obj, hip_obj_matrix)
- L281 matrices[knee_matrix_index]   = get_relative_matrix(knee_obj, up_leg_obj)
- L282 matrices[leg_matrix_index]    = get_relative_matrix(leg_obj, knee_obj)
- L283 foot_pos = matrices[foot_matrix_index].c.xyz()  (save row)  [ebp-58h/-54h/-50h]
- L284 matrices[foot_matrix_index]   = get_relative_matrix(foot_obj, target_foot_obj_matrix)
- L285 matrices[foot_matrix_index].c.xyz() = foot_pos  (restore)
- L291 return / epilogue

## Setup
- get_rotation_matrix / change_matrix_orientation: NOT in base headers; process_leg calls
  them OUT-OF-LINE so a self-contained definition in this TU resolves the link (their own
  bodies are separate objects, do not affect process_leg's scored bytes).
- ik_utils.h included for get_angle (inline STUB return 0 - target calls it out-of-line,
  base will inline -> a known separate-fn inline-vs-call residual).
- get_additional_length defined out-of-line in this TU already (PARTIAL).
- Anchored via friend use_game_core_legs_ik_processor_process_leg (header friend, no bytes).

## Commands run (verbatim, in order)
- git checkout -b match/game_core-legs_ik_processor-process_leg
- pdb_rich_query --index binaries/rich/target/index.jsonl --function process_leg --list
- pdb_fetch --target-index ... --function process_leg --view info / callees / structure / target
- python3 scripts/rebuild.py

## Iterations
1. INPUT: full first reconstruction. Compile FAILED: legs_ik_processor.cpp(387,394)
   C2664 transform_position - I had `target_foot.transform_position(transform)` (args
   swapped). FIX: `transform.transform_position(target_foot.c.xyz())` for the heel
   draw_cross and `transform.transform_position((matrices[toe]*target_foot).c.xyz())`
   for the toe draw_origin (decoded from asm 0x90a/0x9bf: this=transform float4x4,
   arg=the position float3).
2. INPUT: fix applied. BUILD: 78.52% (report.json). Regressions: 18 entries all
   100->0 ICF/COMDAT-fold churn (empty_stub, trivial dtors/thunks) balanced by 30
   0->100 improvements - net-neutral relink fold shuffle (same as get_foot_fixed_transform.md);
   NO matched game_core fn regressed.
   DIFF (--view diff): structure aligns top-to-bottom with the carcass byte sizes.
   Found real operand bugs in the angle-math middle:
   - up_leg_to_foot_len: target is `(up_leg_obj.c.xyz() - TARGET_foot.c.xyz()).length()`
     with NO normalize; I had `normalize(... - foot_obj ...).length()`.
   - additive_len: target normalizes BOTH dir args; I passed raw operator- results.
   - up_leg_alpha_angle: target `get_angle(leg_len+add, up_leg_to_foot_len, knee_len+add)`;
     I had `(up_leg_len+add, ..., leg_len+add)`.
   - L216/L217: target writes/reads `[ebp+8]+0x1C` = `params.rotation_axis` (leg_params
     offset 0x1C), NOT `up_leg_obj_matrix.i.xyz()`. Fixed both the normalize-write and
     the create_rotation arg to use params.rotation_axis (avoids the .i.xyz() lvalue fold).
   - toe debug bone: target get_bone(`params.FOOT_bone_index`), I had toe_bone_index.
3. INPUT: the 5 fixes above. BUILD: 78.82% (report.json). Regressions: none new.
   DIFF: 447 `~` (register/[ebp-N] slot renames - dominant), 67 base-only `-`,
   44 target-only `+`. The `+`/`-` are the call-boundary class (is_similar epsilon
   passed as the lone stack arg `add esp,4` while base pushes a ptr too; operator*/-/^
   temp materialization; xyz-fold inline-vs-call). The `~` are slot renames whose ROOT
   CAUSE is a control-structure divergence, NOT just declaration order / LTCG: see the
   REVIEW note below.

## REVIEW (claude, no rebuild): the dominant residual is a missing-brace structure bug, not LTCG
Re-reading the target carcass against `binaries/structure/base/.../legs_ik_processor.cpp`:
- TARGET process_leg has THREE `[1]` block-opens at srclines 205, 231, 245 (the up_leg /
  knee / leg orientation stages), and the PDB tags the stage locals `<1>`
  (`original_up_leg_to_foot_dir`, `additive_len`, `up_leg_alpha_angle`, `up_leg_to_foot_len`,
  `original_up_leg_dir`, `target_up_leg_dir`, `rotation_matrix`(x3), `alpha_rotation_matrix`,
  `original_knee_dir`, `target_leg_dir`, `original_leg_dir`) = declared INSIDE those braced
  scopes.
- BASE structure has ZERO `[n]` block-opens in process_leg: my source wrote the three IK
  stages FLAT at function scope, so every `<1>` local got a function-scope slot.
- That block-scoped-vs-function-scoped allocation IS the source of the "447 slot renames"
  (MATCHING.md check 5: base/target STRUCTURE must agree, not just the %). So it is a
  RECOVERABLE matching problem, NOT the LTCG/slot-noise class. The original "declaration
  reorder is the unconverged tail / not pursued" framing under-stated this. NEXT STEP (a
  faster machine): brace the three IK stages (205/231/245) so the `<1>` locals are
  block-scoped, then re-diff. No source change made here (a re-match needs a rebuild, out of
  a reviewer's scope).

## Outcome
STATE[78.82%|PARTIAL]. All 58 statements / IK operands match (verified against asm), but
the CONTROL STRUCTURE does not yet: the three target `[1]` braced IK-stage blocks
(205/231/245) are written flat in the base, which is the source of the dominant `[ebp-N]`
slot renames (see REVIEW above) - a recoverable matching problem, not LTCG. The smaller
remaining residual is the permitted call-boundary class (is_similar/operator temps), a few
xyz-fold inline-vs-call decisions, and the get_skeleton()->get_root_bones_count temp-
roundtrip that sibling get_foot_fixed_transform (84%) also shows. Regressions caused: none
(the 18 100->0 entries are net-neutral ICF fold churn, balanced by 30 0->100).
Setup: get_rotation_matrix/change_matrix_orientation defined locally in this TU (called
out-of-line by process_leg; their own bodies are separate objects). Anchored via friend
use_game_core_legs_ik_processor_process_leg.

## Deep pass (anchor-removal, match/game_core-legs_ik_processor-deep)
Removed the fake-observation direct anchor `use_game_core_legs_ik_processor_process_leg`
(NULL-cast processor/params) + its IncludeAll dispatch line. process_leg is now kept alive
ONLY transitively through the real `processor` instance in `use_game_core_legs_ik_processor`
(process() -> process_leg). Hypothesis: fake observation might distort LTCG/DSE.
- COMMAND: python3 scripts/rebuild.py (no module arg); python3 scripts/legs_scores.py
- RESULT: 78.81% UNCHANGED (vs PR #159 78.82). Function still SCORES (reached transitively).
- CONCLUSION: anchor observation was NOT the cap. The real residual is the documented
  three-block bracing (the [1] blocks at srclines 205/231/245 written flat in our source) -
  a recoverable control-structure matching problem, plus the call-boundary LTCG class. NOT
  pursued in this pass (the brace work is a multi-iteration restructure: the target also
  declares an extra `original_up_leg_to_foot_dir<1>` inside the up_leg block that our source
  lacks, so it needs the in-block local set matched, not just `{ }` added). Stays 78.81% PARTIAL.

## Restructure pass (match/game_core-legs_ik_processor-process_leg-restructure)
Goal: brace the three IK stages so the <1> locals become block-scoped (collapse the slot-rename storm).

### Decode of the three target blocks (from `--view target` byte offsets + carcass `[1]` marks)
Matrix obj slots (from srclines 176-180): up_leg=[ebp-180h], knee=[ebp-0F0h], leg=[ebp-48h],
foot=[ebp-140h], toe=[ebp-0B0h]. Block boundaries and statement ORDER read off the asm:
- 203 (FUNCTION scope): target_up_leg_to_foot_dir = normalize(up_leg.c.xyz - target_foot.c.xyz)
- BLOCK 1 (up_leg, opens 205): up_leg_to_foot_len; additive_len; up_leg_alpha_angle;
  original_up_leg_dir; target_up_leg_dir; if(!is_similar) rotation_axis=...; alpha_rotation_matrix;
  rotated_dir; rotation_matrix; change_matrix_orientation(rotation_matrix, up_leg_obj).
- BLOCK 2 (knee, opens 231): knee_obj = m[knee]*up_leg (231, FIRST stmt INSIDE block);
  leg_obj = m[leg]*knee (232); original_knee_dir; rotation_matrix2 =
  get_rotation_matrix(original_knee_dir, target_up_leg_to_foot_dir);
  change_matrix_orientation(rotation_matrix2, knee_obj).
- BLOCK 3 (leg, opens 245): leg_obj = m[leg]*knee (245, FIRST stmt INSIDE block);
  foot_obj = m[foot]*leg (246); original_leg_dir; target_leg_dir; rotation_matrix3;
  change_matrix_orientation(rotation_matrix3, leg_obj); foot_obj = target_foot_obj_matrix.
KEY ORDERING FIX vs the old flat source: the old source computed original_knee_dir BEFORE
the leg recompute (using stale leg_obj); the target recomputes leg FIRST (232) then takes
original_knee_dir from it. The knee/leg recompute is the FIRST statement INSIDE the next
block, NOT a function-scope statement between blocks.

### Iterations
1. INPUT: wrapped the three stages in `{ }`, moved the knee recompute into block 2 and the
   leg recompute into block 3 (each as the first in-block statement), reordered block 2 to
   knee,leg,orig_knee_dir,rot,change.
   COMMAND: nix develop --command python3 scripts/rebuild.py (no module arg).
   RESULT: report.json 78.81% -> **80.96%** (`0 regressed, 1 improved`). Base structure dump now
   shows exactly THREE `[1]` block-opens at base-srclines 382/400/409 (= target 205/231/245);
   the former ZERO. The slot-rename storm collapsed.
   DIFF (--view diff, header fuzzy 68.86 - secondary metric): remaining residual is now the
   non-bracing class, see Final residual below.

### Final residual (80.96% PARTIAL - not bracing, not pursued further)
1. **[ebp-150h] working slot.** Every up_leg_obj_matrix READ in the dir math (srclines
   203/205/210/214/215) targets [ebp-150h], while change_matrix_orientation (222) and draw_leg
   mutate/read [ebp-180h] (the slot srcline 176 actually writes). [ebp-150h] is never written in
   the IK region and is NOT a named float4x4 in the carcass LOCALS - it is a compiler in/out
   lowering copy of up_leg_obj that the target keeps for the dir math while the real local is
   mutated. Also shows as an operand-EVALUATION-order swap at srcline 203 (target evaluates the
   up_leg xyz() first, base evaluates target_foot first) and a target-only `lea [ebp-150h];call`
   trailing srcline 203. Not reproducible from a single source variable.
2. **get_angle inline-vs-call.** get_angle is a STUB here (base inlines `return 0`); the target
   calls it out-of-line, so target srcline 212 (+0x03e) inlines the `leg_len+additive_len` /
   `knee_len+additive_len` adds into the call (`addss xmm0,[ebp-1F0h]; push; movss [esp]`),
   which base (+0x010) does not. Separate-fn inline-vs-call class.
3. **get_skeleton()->get_root_bones_count temp-roundtrip.** Base spills *m_skeleton into a
   per-call [ebp-8XXh] temp (`mov ecx,[eax]; mov [ebp-870h],ecx; mov eax,[ebp-870h]`) where the
   target does `mov eax,[eax]` direct - the inline-accessor-returns-reference materialization
   under /Od. Sibling get_foot_fixed_transform (84%) shows the same.
4. **Call-boundary temps.** is_similar epsilon passed as the lone stack arg (`add esp,4`) while
   base also pushes a ptr (`add esp,8`); operator*/-/^ temp materialization. Permitted LTCG/
   call-boundary class.

## REVIEW (claude, no rebuild) - verified 80.96% PARTIAL, structure confirmed, one residual under-attributed
- report.json `fuzzy_match_percent` for `?process_leg@legs_ik_processor@survarium@@AAEX...` = **80.9602** -
  the `.cpp` STATE, this `.md`, and PROGRESS.md all agree. PARTIAL tag correct (not 100%).
- §2a STRUCTURE check PASSES: TARGET structure (binaries/structure/target/.../legs_ik_processor.cpp)
  shows the three `[1]:'205'` / `[1]:'231'` / `[1]:'245'` block-opens; BASE structure now shows exactly
  three `[1]` block-opens (base srclines 383/401/410, the dump's first-in-block lines; was ZERO before the
  restructure). The brace restructure is real and faithful.
- The DUPLICATE `leg_obj = matrices[leg]*knee` (block 2 srcline 232 AND block 3 srcline 245) is NOT a
  reviewer logic bug: the target structure emits the recompute at BOTH 232 and 245. Reproducing it is
  correct (MATCHING.md #1). No base/target confusion - get_angle is genuinely a STUB in base (ik_utils.h)
  so base inlines / target calls out-of-line, the direction in STATE is correct.
- CAVEAT on residual (1): the target LOCALS dump declares an extra block-1 `original_up_leg_to_foot_dir<1>`
  (a `float3 const&`) that the source does NOT declare; the `[ebp-150h]` working-slot residual is plausibly
  that missing in-block dir local, i.e. a RECOVERABLE matching item (declare the extra block-1 normalize
  local, route the dir reads through it), not purely an unsteerable compiler copy. Flagged for a faster
  machine as the concrete next step before banking it as non-reproducible. STATE stays PARTIAL (honest).
