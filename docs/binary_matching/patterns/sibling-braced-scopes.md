# N sequential `[1]` block-opens = N SEPARATE sibling braced scopes (not nesting)
tags: cpp:local | asm:mov | topic:pdb-locals topic:structure-shape
symptoms: depth resets to [1] each time never [2], slot-rename storm collapses when braced
confidence: 7/10
variants: decl-in-if-guard.md, per-call-ref-return-spill.md

N `[1]` block-opens at distinct srclines with no enclosing block are N sibling braced
`{ ... }` stages; block-scoped locals then REUSE stack slots across blocks (the target's
slot overlap), collapsing the `[ebp-N]` slot-rename storm flat function-scope source
produces. Read the FIRST statement after each `[1]` from the asm - a recompute the flat
source wrote BETWEEN stages belongs as the first statement INSIDE the next block; also
re-read intra-block statement ORDER (the target may recompute a matrix earlier so a
dependent normalize reads the fresh value).

```cpp
{ knee_obj = m[knee] * up_leg; ... }
{ leg_obj  = m[leg]  * knee;  ... }   // each stage its own { } scope
```
Evidence: game_core/legs_ik_processor::process_leg (78.81 -> 80.96).
