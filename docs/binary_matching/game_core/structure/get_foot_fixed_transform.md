# Structure audit: legs_ik_processor::get_foot_fixed_transform

- target rva 0x6ebae0  |  base rva 0x514fb0
- sig: `vostok::math::float4x4 survarium::legs_ik_processor::get_foot_fixed_transform(leg_params const&, float4x4 const&, float4x4 const*, float&) const`
- report.json `fuzzy_match_percent`: 84.23 (PR title cites 84.66) - PARTIAL
- structure-diff totals: target 87 stmts / base 95 stmts; aligned 45, size-diffs 37, quantity-diffs 18

## Verdict

STRUCTURE MATCH (source shape). The 84% is HONEST, not a high-percent-over-wrong-
structure trap: every aligned statement is SIZE-only and the quantity divergences are
codegen block placement, not a source-shape defect. The function is correctly tagged
PARTIAL; no downgrade and no restructure are warranted. Left STATE[84.66%|PARTIAL] as is.

## What the diff shows

All 45 aligned statements that carry text are SIZE-only diffs - the body matrix/vector
math (matrix indexing, normalize, ^, create_rotation, transform_position, is_similar,
length, get_root_bones_count, interpolated_value, adjust_foot_transform). These are the
LTCG arg-passing / temp-materialization residuals already described in the STATE note.
None is a quantity divergence; the statement carcass lines up one-for-one.

The quantity divergences (the 8-statement gap, target 87 vs base 95) are all the same
phenomenon - the target OUTLINES the debug-draw branches and relocates the epilogue:

```
--          | 0x523 <0x19> | math::color original_color( 0x80u,0xc8u,0,0 );    ONLY base
--          | 0x53c <0x19> | math::color fixed_color( 0x80u,0,0xc8u,0 );        ONLY base
--          | 0x8f7 <0x25> | m_drawer->draw_line_capsule( ... );               ONLY base
0x89c <0x30>| --           | L375                                              ONLY target
--          | 0x966 <0x25> | m_drawer->draw_solid_capsule( ... );              ONLY base
0x8e3 <0x22>| --           | L379                                              ONLY target
--          | 0xbe6 <0x10> | return foot_center_transform;                     ONLY base
0xb41 <0x13>| --           | L399                                              ONLY target
```

The two `if ( s_ik_legs_debug_draw_value && m_drawer )` guards are present and ALIGNED on
both sides (rows 0x863/0x8e0 and 0x8cc/0x94f are `.. same ..`). The difference is purely
where each compiler put the guarded body:

- base: emits the draw-call body (`draw_line_capsule` / `draw_solid_capsule`) and the
  `original_color` / `fixed_color` constructions in place, as ordinary statements.
- target: jumps from the guards to OUT-OF-LINE cold blocks (L375 / L379); the color
  temps are materialized only along that cold path, so they do not appear as front-of-
  function statements. The closing `return foot_center_transform;` is likewise placed in
  a separate trailing block (target L399 vs base 0xbe6) because the function has multiple
  returns and the target lays the final return out last.

This is control-flow / cold-block placement chosen by LTCG, not a source-shape choice.

## Source-shape checks (all clean)

- Member-init vs body assigns: N/A (free-standing locals; not a ctor).
- Braces: both draw guards are single-statement bodies, brace-less on both sides; the
  four ground-state `if/else if/else` blocks are braced on both sides and align. No
  added/omitted block.
- Early-return guard: the `is_similar(...) return foot_world_matrix;` early-out aligns
  (SIZE-only on the is_similar pair); flat guard, no spurious block. Correct.
- Definition order: statements appear in original order; no reordering/regrouping.

## Fix

None. The source already reproduces the target's shape. The remaining byte gap is the
documented codegen residual (math-call temp materialization + the target's outlining of
the debug-draw branches and multi-return epilogue), which a source edit cannot move.
Closing the last ~16% would require coaxing the same cold-block/outlining decision out of
our toolchain, which is a matcher's codegen task, not a structure fix. Rebuild not run
(no bytes changed).
