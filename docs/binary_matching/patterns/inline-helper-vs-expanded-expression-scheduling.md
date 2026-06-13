# Calling a thin inline accessor vs inlining its body by hand changes operand scheduling
tags: cpp:member cpp:float | asm:cvtsi2ss asm:divss asm:call | topic:codegen-idiom topic:inline-vs-call
symptoms: int->float convert/divide emitted eagerly (next to randI) in base but DEFERRED past a sibling call in target; one extra ebp temp slot in target
confidence: 7/10

When a value is produced by a trivial inline accessor (e.g. `float randF() { return float(randI())/maxF(); }`)
that has a side-effecting sub-call (randI advances the seed), writing `randF()` at the
use site vs hand-inlining `float(randI())/maxF()` gives MSVC /Od DIFFERENT freedom to
schedule the float convert/divide. Calling the accessor keeps the int side-effect ordered
early but lets the `cvtsi2ss`/`divss` float-finish slide later (past a neighbouring call in
the same statement), which is what the target often does; the hand-inlined form pins the
divide right after the int. Prefer the real accessor call when the target defers the float
math - it also tends to match the target's extra spill temp.

```cpp
// rand_n rejection loop: randF() (not float(randI())/maxF()) made the divide defer past expf
do { y = -logf( randF() ); } while ( randF() > expf( -math::sqr(y - 1.0f) * 0.5f ) );
```
Evidence: normal_random::rand_n 90.2 -> 94.2 swapping the two `float(randI())/maxF()`
inlines for `randF()` member calls (gave randF the deferred `cvtsi2ss/divss` the target
shows). Residual ~6% is a pure spill-vs-register / left-right-evaluation scheduling diff
inside the one `while` statement (target spills -0.5*sqr to a temp and computes randF
BEFORE calling expf; base keeps it in xmm0 and calls expf first) - structure-diff is a
single `SIZE -0x14` on that statement, every other statement byte-exact; not steerable from
source without the original's exact form. Also here: the half-normal magic literal was the
imprecise `1.2539185f` (0x3fa08067), NOT the true sqrt(pi/2)=1.2533141 - match the literal.
