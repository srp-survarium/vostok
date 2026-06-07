# game_core::legs_ik_processor::get_foot_fixed_transform

Target: `vostok::math::float4x4 survarium::legs_ik_processor::get_foot_fixed_transform(
leg_params const&, float4x4 const&, float4x4 const*, float&) const`
- target rva 0x6ebae0, base file `legs_ik_processor.cpp` (~line 415).
- Mangled access: private const member `ABE` (B = const-this; declared `private:` in header already).
- Large IK math function: ~0xb5a bytes, 64 statements, source lines L294-L400.
  Expected to end PARTIAL: lots of inline-vs-call of trivial math COMDATs
  (`xyz()`, `operator-/+/^/*`, `get_root_bones_count`, `float4x4()`, `is_similar`,
  `length`, `squared_length`, `deg2rad`, `transform_position`, `color::color`).

## Commands run (verbatim, in order)
- git checkout -b match/game_core-get_foot_fixed_transform
- pdb_rich_query --index binaries/rich/target/index.jsonl --function get_foot_fixed_transform --list
- pdb_fetch --target-index ... --function get_foot_fixed_transform --view info
- pdb_fetch --target-index ... --function get_foot_fixed_transform --view callees
- pdb_fetch --target-index ... --function get_foot_fixed_transform --view target
- pdb_fetch --target-index ... --function get_foot_fixed_transform --view structure

## Statement decode (line -> source, from asm)
this = [ebp-5F8h]; args: [ebp+8]=hidden ret, [ebp+0Ch]=params, [ebp+10h]=hip_world_matrix,
[ebp+14h]=matrices, [ebp+18h]=delta_len.

The recurring pattern `mov ecx,this; mov eax,[ecx](=m_skeleton); call get_root_bones_count;
mov edx,params; mov edx,[edx+BONEOFF]; sub edx,eax; shl edx,6; add edx,matrices` =
`matrices[params.<bone>_bone_index - m_skeleton->get_root_bones_count()]`.
(get_root_bones_count is leaf, `this` passed via eax under LTCG.)
`add eax,30h; call <xyz fold 0x3f210>` = `matrix.c.xyz()` (the .c position float3 @ +0x30).

- L295 up_leg_world_matrix = matrices[up_leg_bone_index(@0x10) - root] * hip_world_matrix  (float4x4 const&)
- L296 knee_world_matrix   = matrices[knee_bone_index(@0x0c)   - root] * hip_world_matrix
- L297 leg_world_matrix    = matrices[leg_bone_index(@0x08)    - root] * hip_world_matrix
- L298 foot_world_matrix   = matrices[foot_bone_index(@0x00)   - root] * hip_world_matrix
- L299 toe_world_matrix    = matrices[toe_bone_index(@0x04)    - root] * hip_world_matrix
- L300 if ( is_similar( foot_world_matrix.c.xyz(), toe_world_matrix.c.xyz() ) ||
            is_similar( foot_world_matrix.c.xyz(), leg_world_matrix.c.xyz() ) )
- L301     return foot_world_matrix;   (rep movsd 0x10 -> [ebp+8]; jmp end)
- L303 foot_to_leg_dir = normalize( leg_world_matrix.c.xyz() - foot_world_matrix.c.xyz() )  (float3 const&)
- L304 foot_to_toe_dir = normalize( toe_world_matrix.c.xyz() - foot_world_matrix.c.xyz() )  (float3 const&)
- L305 left_dir = normalize( foot_to_leg_dir ^ foot_to_toe_dir )                            (float3 const&)
- L307 result.identity()  (lea result[-40h]; call float4x4(); call identity())   result @ [ebp-40h]
- L309 result.i.xyz() = left_dir            (mov [esi]->[eax] x3; result+0 = .i)
- L310 result.j.xyz() = foot_to_leg_dir     (result+0x10? actually .i @0 .j? see below)
        NB asm 0x2a5 writes result.i (@0), 0x2c3 writes result.j... need to verify offsets
- L311 up_dir = normalize( foot_to_leg_dir ^ left_dir )   (float3 up_dir @ [ebp-20h])
- L313 rotation_angle = deg2rad( 30.0f )  (default_fps==30.0f symbol; const float)
- L314 result = create_rotation( left_dir, rotation_angle ) * result   (rep movsd into result)
- L315-317 three `mov byte[ebp-N],0; lea; call <empty>` = 3 ASSERTs
- L318 start = foot_world_matrix.c.xyz()   (float3 start @ [ebp-218h]) -- actually float3(...) ctor; see asm
        asm 0x3d4: float3( 0.f, 0.082f(3da7ef9e), satisfaction_equality_tolerance )  @ [ebp-218h]
        -> L318 = `start` float3 ctor? then transform; revisit
- L320 finish = result.transform_position( start )   (float3 finish @ [ebp-10h])  -- see asm 0x3f9
- L323 capsule_size = float3( radius, 0.12f(3df5c28f), radius )   radius=s_ik_foot_capsule_radius_value @ [ebp-20Ch]
- L324 foot_to_cube_center_offset = float3( 0.f, 1.f, 0.f )   @ [ebp-0C0h]
- L326 float3 up_dir/foot_to_center_rel(?) allocator<char> (float4x4 default ctor folds) @ [ebp-1FCh],[ebp-0A4h]
- L327 original_color = color( 0x80, 0xC8, 0, 0 )  (u32 r,g,b: 0x80,0xC8,0 + a) @ [ebp-170h]
- L328 fixed_color    = color( 0x80, 0, 0xC8, 0 )  @ [ebp-168h]
- L329 rotation_interpolation_koef = <offset symbol float> @ [ebp-160h]
- L300-block (L329..L357): big if/else-if chain on params.m_heel_on_ground(@0x2C)/m_toe_on_ground(@0x2D)
  computing start/finish (foot_to_cube_center_offset etc) and rotation_interpolation_koef per branch.
- L364 foot_center_transform = get_relative_matrix( result??, foot_world_matrix )  @ [ebp-108h]
- L366 if ( s_ik_legs_debug_draw_value && m_drawer ) m_drawer->draw_line_capsule(...)
- L371 m_character_controller->adjust_foot_transform( ... ) (member @ +0x0C)
- L375 if ( s_ik_legs_debug_draw_value && m_drawer ) m_drawer->draw_solid_capsule(...)
- L384 result = foot_center_transform * result??  (operator* mat*mat) -- revisit
- L386-388 leg_len/up_leg_len/knee_len = length of bone position deltas
- L390 up_leg_to_fixed_foot_dist = length( ... )
- L391 *delta_len = leg_len + up_leg_len + knee_len - up_leg_to_fixed_foot_dist  (xmm adds, [ebp+18h])
- L392 up_leg_to_original_foot_dist_sqr = squared_length( ... )
- L393 if ( sqr( up_leg_to_fixed_foot_dist ) > up_leg_to_original_foot_dist_sqr ) ...
- L395-397 result/foot_center_transform blend with koef
- L399 return result;  (rep movsd 0x10 from [ebp-2A8h] -> [ebp+8])

(decoding in progress; offsets to be verified against build diff. This is an
overview; exact branch bodies refined while iterating.)

## Setup needed to compile
- get_relative_matrix (target rva 0xbb050) is an inline math helper absent from our
  headers. First tried declaring in math_float4x4.h + defining in math_float4x4_inline.h;
  that FAILED to compile ("get_relative_matrix is not a member of vostok::math") because
  those headers are inside the precompiled pch and the PCH did NOT pick up the edit (PCH
  staleness; a direct #include in the .cpp is a guard no-op). FIX: define
  vostok::math::get_relative_matrix directly in legs_ik_processor.cpp (after pch),
  reverting both header edits. Self-contained, no ODR/PCH dependency.
- character_controller.h needs VOSTOK_PHYSICS_API defined -> #include <vostok/physics/api.h>
  BEFORE it. Real adjust_foot_transform sig: (float3 half_size, float3 start, float3 finish,
  float rotation_koef0, float __formal, float4x4& transform).
- get_root_bones_count needs full skeleton type -> #include <vostok/animation/skeleton.h>.
- m_skeleton is private in base ik_processor; added a protected inline get_skeleton() accessor.
- get_foot_fixed_transform is private; anchored via a friend free fn
  use_game_core_legs_ik_processor_get_foot_fixed_transform (friend decl emits no bytes),
  called from IncludeAll.
- s_ik_legs_debug_draw_value (bool) + s_ik_foot_capsule_radius_value (float): the cc
  machinery is STUB; declared as plain file statics to reproduce the reads.

## Iterations
1. INPUT: full reconstruction (5 bone matrices*hip, is_similar early-out, 3 normalized
   dirs, identity+i/j/k build, create_rotation*result, 3 ASSERTs, result.c=foot pos then
   =transform_position(up_dir), capsule_size/colors, 4-way ground if-chain,
   get_relative_matrix, 2 debug-draw guards, adjust_foot_transform, 3 lengths + delta_len,
   sqr-distance blend, return). Compiles clean (warns up_leg_world_matrix/knee_world_matrix
   unused -> my tail recomputes matrices[...] directly instead of via those refs; a known
   structural gap to refine).
   BUILD: 81.55% (report.json). Regressions: 53/57 are 100->0 empty-dtor/COMDAT
   ICF fold churn (rva 0x3f210 class), balanced by 49 0->100 improvements (net neutral
   link-time noise); NO matched game_core fn regressed. 4 non-fold: render::fix_view_matrix
   100->62.79 (out-of-scope render obj, fold-landed-elsewhere), buffer_string::operator=
   68->0, a render dtor 99.92->0, a boost list1 ctor 8->0 - all fold movement, not source.
   draw_line/solid_capsule went 0->33 (now kept alive by my anchor).
   DIFF: (a) root_count was HOISTED into one local in base but the target RE-CALLS
   get_skeleton().get_root_bones_count() per matrix (7 sites) - base cached [ebp-0BCh],
   target inserts a fresh `call` each time. (b) up_leg_to_fixed_foot_dist /
   up_leg_to_original_foot_dist_sqr used `matrices[up_leg-root]` (fresh) in base but the
   target uses the cached `up_leg_world_matrix` ref ([ebp-94h], the *hip-multiplied* world
   matrix). (c) knee_world_matrix ([ebp-0C4h]) is genuinely unused in BOTH (declared,
   never read) - the C4189 warning matches the target; keep it. (d) else-branch does
   `finish = start` (copies the op+ result) + a single-byte original_color write (0x64),
   not the full color ctor I wrote. (e) the rest are register/[ebp-N] slot renames (cheap).
2. INPUT: un-hoist root_count (inline get_skeleton().get_root_bones_count() at all 7
   sites); up_leg distance lines use up_leg_world_matrix ref; else-branch `finish = start`.
   color(0x64) left diverging (TODO: single-byte channel write, exact source unknown).
   BUILD: 84.16% (report.json). Regressions: NONE (report-changes shows only the
   single 81.55->84.16 improvement; the prior build's 53 100->0 fold entries were a
   transient relink shuffle relative to the pre-function baseline and netted out).
   DIFF: 370 `~` (mostly register/[ebp-N] slot renames - cheap), 51 base-only `-`,
   36 target-only `+`. The remaining `+`/`-` clusters are: (i) push-order / temp
   materialization at the math operator call boundaries (operator+/-/^/*, normalize,
   create_rotation, transform_position, is_similar) - the target stages the intermediate
   `offset*dist` temp and passes it as the operator's `this`/arg differently (LTCG
   arg-passing class); (ii) a couple of inline-vs-call decisions of trivial COMDAT math
   helpers; (iii) the else-branch single-byte original_color write. All statement byte
   sizes (`; <0xNN>`) agree with the carcass (e.g. start `<0x47>`, finish `<0x3e>`,
   is_full_on_ground `<0x53>`, rotation_angle `<0x15>`), confirming the control structure
   matches; the divergence is sub-statement arg passing, not structure.

3. INPUT: resolve the @TODO else-branch single-byte original_color write. Read the
   ACTUAL color type in play (math_color.h lines 115-164: the `union{u8 b,g,r,a; u32 m_value;}`
   variant, NOT the float4 color_pod variant). Target asm at else-branch (rva 0x6ebae0 +0x82a):
     0x82a: mov byte ptr [ebp-167h], 64h     ; materialize 0x64 & 0xff into a temp byte
     0x831: mov cl, [ebp-167h]
     0x837: mov [ebp-170h], cl               ; store into original_color's offset-0 byte = b
   original_color is at [ebp-170h]; its lowest byte is the `b` channel (b,g,r,a order). The
   3-instruction shape (const byte -> temp -> single channel store) is exactly the body of
   `set_B(u32 val){ b = val & 0xff; }` with val=0x64u. NOT a full color ctor (the sibling
   init at L476 IS a full 4-arg ctor: target emits `color::color` at +0x484 with r=0x80
   (ecx), pushed g=0xC8,b=0,a=0 -> color(0x80,0xC8,0,0); leave that as-is). Rewrote the
   else-branch statement to `original_color.set_B( 0x64u );`.
   BUILD: 84.656% (report.json), up from 84.158%. get_relative_matrix stays 100%.
   Regressions: report-changes shows 46 regressed / 52 improved, ALL the documented ICF /
   COMDAT empty-dtor/ctor/inline-helper fold churn from the EXE relink (net +459, balanced)
   - none is an in-scope matched function. The one notable entry is color::color(u32,u32,u32)
   99.78->0: that is the 3-arg ctor I just REMOVED the sole call to, so it is no longer
   emitted/folded - a removal artifact, not a real regression. No matched legs_ik_processor
   or get_relative_matrix function regressed (verified by name filter).

## Outcome
STATE[84.66%|PARTIAL]. Structure fully matched (all 64 statements / the carcass shape).
@TODO RESOLVED: else-branch original_color is a single `set_B(0x64u)` channel setter
(reproduces the exact `mov byte[tmp],64h; mov cl,[tmp]; mov [color],cl` write at +0x82a),
not the full color ctor previously written.
Residual: register/slot renaming + LTCG arg-passing & temp-materialization at the many
math-operator/helper call boundaries (the permitted call-boundary class) + a few
trivial-COMDAT inline-vs-call decisions. All are sub-statement register/slot/arg-passing,
not structure. Regressions caused: none (in-scope). Inlining: get_relative_matrix (target
rva 0xbb050) defined in this TU; knee_world_matrix is a declared-but-unused local in BOTH
binaries (the C4189 warning matches the target - keep it).


## Deep pass (anchor-removal, match/game_core-legs_ik_processor-deep)
Removed the fake-observation direct anchor `use_game_core_legs_ik_processor_get_foot_fixed_transform`
(NULL-cast processor/params, escaped result) + its IncludeAll dispatch line. Now reached only
transitively via the real `processor` instance (process() -> get_foot_fixed_transform).
- COMMAND: python3 scripts/rebuild.py (no module arg); python3 scripts/legs_scores.py
- RESULT: 84.23% UNCHANGED. Function still SCORES. Residual is genuine (the documented
  call-boundary LTCG / slot-rename class), NOT the anchor-observation distortion.

## Structure-verifier v2 pass (2026-06-07) - 84.23 -> 84.65%

REAL FIX (the prior @TODO): the else-branch single-byte original_color write. Target at
0x82a does `mov byte[ebp-167h],64h; mov cl,[ebp-167h]; mov [ebp-170h],cl` - a single low
byte (offset 0 = the `b` channel of math::color's {b,g,r,a} union) write = `set_B(0x64u)`.
The stale base form `original_color = math::color( 0x64u, 0x00u, 0x00u )` emitted a full
color ctor + 4-byte copy (0x1c). Changed to `original_color.set_B( 0x64u )`; that row is
now a single-byte write (0x10) and the fn moved 84.23 -> 84.65.

Remaining residual is the documented call-boundary class: get_root_bones_count() spill
(0xC per `matrices[idx]*hip_world_matrix` and per length() index; target inlines the helper),
the is_similar operator inline (0x78 vs 0xb6), and the original_color/fixed_color ctor +
draw_*capsule block placement (the ONLY base / L356/L357/L375/L379 ONLY target rows are
codegen block placement, not source shape). 86/93 stmts, structure MATCH.
