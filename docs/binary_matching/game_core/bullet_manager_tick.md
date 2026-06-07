# bullet_manager::tick (0x591d80)

~95.11% PARTIAL (marker was stale 98.49%). Structure MATCH (23/23 stmts, 0 diffs).

## Residual (non-steerable wall)
Pure frame-size LTCG: target `sub esp,1ECh` vs base `sub esp,1F0h` (base 4 bytes
more), cascading into every `[ebp-N]` slot (`[ebp-1ECh]` vs `[ebp-1F0h]`, etc.). No
structural difference at all. The boost::function/boost::bind temporaries in the
tick-task spawn and the redundant_bullet_predicate remove_if are lowered onto the
stack with a 4-byte different layout under whole-program LTCG. Source correct.

## Commands
pdb_fetch --function tick --rva 0x591d80 --view structure-diff/diff
