# game_core::legs_ik_drawer (5-function same-class cluster)

`legs_ik_drawer` is a thin debug-render forwarder over `render::debug::renderer`.
All five public draw_* methods forward to `m_renderer.draw_*( m_scene, ... )`.
m_renderer is at this+0x0, m_scene (scene_ptr) at this+0x4.

Functions (all STATE STUB -> targeted DONE):
- draw_leg          rva 0x7a1d70
- draw_cross        rva 0x7a1d40
- draw_origin       rva 0x7a1d10
- draw_line_capsule rva 0x7a1ef0
- draw_solid_capsule rva 0x7a1ec0

## Commands run (verbatim, in order)
- git checkout match/game_core-legs_ik_processor-process_leg-restructure  (HEAD c83e30ac)
- pdb_rich_query --index binaries/rich/target/index.jsonl --function legs_ik_drawer::<fn> --list   (x5)
- pdb_fetch --target-index binaries/rich/target/index.jsonl --rva <rva> --view target/callees   (x5)
- pdb_fetch --target-index binaries/rich/target/index.jsonl --rva 0x7a1d70 --view structure
- read binaries/structure/target/headers/vostok/math/float4x4.h, float4_pod.h, float4.h
- read sources/vostok/render/facade/debug_renderer.h  (full renderer decl with all draw_* methods)
- python3 scripts/rebuild.py   (NO module arg; timed)

## Asm decode

### draw_cross / draw_origin / draw_line_capsule / draw_solid_capsule
Single-statement forwarders. Each pushes the trailing args, then m_scene
(= this+4 via `add ecx,4`), then dereferences m_renderer (= *(this+0)) and calls
`render::debug::renderer::draw_*`. Source:
```
m_renderer.draw_cross       ( m_scene, p, half_size, c, use_depth );
m_renderer.draw_origin      ( m_scene, matrix, half_size, use_depth );
m_renderer.draw_line_capsule( m_scene, matrix, size, color, use_depth );
m_renderer.draw_solid_capsule( m_scene, matrix, size, color, use_depth );
```
The two capsule variants only differ in the compiler's push order of the
matrix/size args; source shape is identical (scene, matrix, size, color, use_depth).

### draw_leg (10 statements: L27 prologue, L28-31 origins, L33-36 lines, L37 epilogue)
4x `draw_origin( m_scene, {up_leg,knee,leg,foot}, cross_half_size, false )` then
4x `draw_line`. Each draw_line endpoint is `matrix.c.xyz()`:
- `add reg,0x30` = `&matrix.c` (float4_pod c at offset 0x30 in float4x4)
- `call finalize_impl` = COMDAT-folded `float4_pod::xyz()` returning `float3 const&`
  (delinker misnames the folded thunk `fixed_size_allocator<...>::finalize_impl`).

Pattern: a `add reg,0x30; call <folded>::finalize_impl; mov esi/ecx,eax` that feeds a
`float3 const&` argument is `matrix.c.xyz()` on a float4x4 — NOT an allocator call and
NOT an ASSERT. Confirmed against sibling legs_ik_processor.cpp which uses `.c.xyz()`
throughout.

Endpoint/color mapping (start, end, color):
- L33: up_leg.c.xyz(), knee.c.xyz(), up_leg_color
- L34: knee.c.xyz(),  leg.c.xyz(),  knee_color
- L35: leg.c.xyz(),   foot.c.xyz(), leg_color
- L36: up_leg.c.xyz(),foot.c.xyz(), foot_color
all use_depth=false. `draw_line(scene, start, end, color, false)`.

## Anchor
temp_include_all.cpp: added `#include <vostok/game_core/legs_ik_drawer.h>`, a
`use_game_core_legs_ik_drawer()` that constructs a drawer over NULL renderer/scene
refs and calls all five draw_* (never run), and a call to it from IncludeAll::IncludeAll.

## Iterations
1. INPUT: all 5 bodies as forwarders (above), one anchor block in temp_include_all.
   BUILD (report.json fuzzy_match_percent):
     draw_cross         100.0%
     draw_line_capsule  100.0%
     draw_solid_capsule  79.43%
     draw_leg            73.36%
     draw_origin         62.88%
   Regressions vs previous: only the usual delinker COMDAT-fold churn (24 regressed /
   43 improved, all unrelated dtors/storage/boost binders), NONE of them legs_ik_*.
   Overall scoreboard +0.02 code / +15 functions.
   DIFF:
   - draw_origin: base passes `half_size` via `fld [ebp+0Ch]; fstp [esp]` (stack);
     target passes via `movss xmm0,[ebp+0Ch]` (register). Everything else identical.
   - draw_solid_capsule: base keeps `size` in eax + pushes the renderer-deref as a stack
     arg; target receives the renderer as the __thiscall `this` in ecx (mov ecx,[edx]) and
     pushes `size`. Register-vs-stack arg assignment only.
   - draw_leg: structure exact (4 draw_origin + 4 draw_line, all `.c.xyz()` endpoints and
     colors resolved); residual is the same draw_origin xmm0-vs-stack float pass x4.

   No second rebuild: the residuals are all call-boundary arg-passing (the only sanctioned
   LTCG stop reason). Identical forwarding source produced 100% for draw_cross /
   draw_line_capsule and only diverges where the (whole-program) callee's float/int arg
   convention differs - not steerable from the drawer's source.

## Build timings (rebuild.py, NO module arg; incremental - only 2 .cpp changed)
From /tmp/legs_ik_drawer_build.log:
- Building survarium via ninja (22:10:30) -> Build OK (22:12:02) = 1m32s
    (= full compile + lib + EXE relink; incremental, only legs_ik_drawer.cpp +
     temp_include_all.cpp recompiled, so far below the ~20m cold relink)
- Build OK (22:12:02) -> All done (22:12:07) = 5s   (= delink + structure/COFF/rich refresh)
- Hang after All done: NONE. rebuild.py exited cleanly at 22:12:07 (exit 0); no Wine
  stuck-child wait observed this run.

## Outcome
STATE:
  draw_cross          100%   DONE
  draw_line_capsule   100%   DONE
  draw_solid_capsule  79.43% PARTIAL (LTCG call-boundary register-vs-stack arg assignment)
  draw_leg            73.36% PARTIAL (LTCG draw_origin float pass: xmm0 vs fld/fstp x4)
  draw_origin         62.88% PARTIAL (LTCG draw_origin float pass: xmm0 vs fld/fstp)
Blocker (3 partials): call-boundary arg-passing convention dictated by the unmatched
render module's whole-program LTCG state; not influenceable from one drawer function's
source (proven by the two 100% siblings using the identical forward pattern).
Regressions caused: none (only unrelated delinker COMDAT-fold churn).
Inlining: none. float4_pod::xyz() shows as a COMDAT-folded `...::finalize_impl` thunk
in the target (delinker misnames it) - it is `matrix.c.xyz()`, not an allocator/ASSERT.

## Structure-verifier pass (2026-06-07, chain/game_core-sv-item_object_batch)
Re-diffed all three PARTIALs with `--view structure-diff --condensed` after a clean rebuild:
- draw_leg          target 9 / base 9 stmts, aligned 1, size-diffs 8, quantity-diffs 0  -> 73.36%
- draw_origin       target 1 / base 1 stmts, aligned 0, size-diffs 1, quantity-diffs 0  -> 62.88%
- draw_solid_capsule target 1 / base 1 stmts, aligned 1, size-diffs 0, quantity-diffs 0 -> 79.43%
All STRUCTURE MATCH: statement counts identical, no quantity divergence. Residuals are the
call-boundary float/reg-vs-stack arg passing into the unmatched render module (movss xmm0 vs
fld/fstp [esp]; renderer-in-ecx vs pushed) - the only sanctioned LTCG stop reason. Embedded the
condensed diffs + VERDICT lines; deleted the carcasses. No logic change, no % movement expected.
