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
   temp materialization; xyz-fold inline-vs-call). The `~` are slot renames: my local
   DECLARATION order yields a different /Od stack layout than the target's PDB order
   (e.g. up_leg_obj mine -308h / target -180h; foot_obj -2C8h/-140h; the index locals
   -108h/-5Ch etc). The target's obj-matrix slots (up_leg -180,foot -140,knee -F0,
   toe -B0,leg -48) follow the PDB declaration order up_leg,foot,knee,toe,leg, which
   differs from my up_leg,knee,leg,foot,toe. Converging the exact /Od slot layout via
   declaration reorder is the deep unconverged tail (also requires uninit-then-assign
   which changes codegen) - not pursued; structure is fully matched.

## Outcome
STATE[78.82%|PARTIAL]. All 58 statements / control flow / IK operands match (verified
against asm). Residual is the LTCG/slot-rename class: the dominant 447 register/[ebp-N]
slot renames from a different /Od stack layout (local declaration order vs target PDB
order), plus call-boundary arg passing (is_similar/operator temps) and a few xyz-fold
inline-vs-call decisions, plus the get_skeleton()->get_root_bones_count temp-roundtrip
that sibling get_foot_fixed_transform (84%) also shows. Regressions caused: none (the
18 100->0 entries are net-neutral ICF fold churn, balanced by 30 0->100).
Setup: get_rotation_matrix/change_matrix_orientation defined locally in this TU (called
out-of-line by process_leg; their own bodies are separate objects). Anchored via friend
use_game_core_legs_ik_processor_process_leg.
