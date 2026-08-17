# Base inlines what target out-lines: a compiled-out ASSERT paid the target's inline budget
tags: cpp:assert cpp:inline | asm:call | topic:inline-vs-call topic:assert-eater topic:source-fidelity
symptoms: base inlines a callee the target CALLs (or a base body folds where the target kept one), no other size/flag explanation; target line geometry shows empty/0-byte line entries at the start of the callee body
confidence: 7/10
variants: speculative-assert-inline-ctor.md, od-helper-inline-depth-wall.md

Old MSVC decides inline-vs-call on the PRE-optimization body: a statement the
optimizer later deletes still counted when the inliner sized the function. An
assert flavor that compiles out at the COMPILER stage (constant-false guard, not
a preprocessor-empty macro) therefore emits ZERO final bytes yet pushed the
target's function over the inline threshold - so retail keeps it out-of-line
while our assert-less reconstruction inlines it.

Detection: in the target's line table / statement geometry, a source line at the
START of the callee body that maps to no bytes (or the first real statement sits
one line lower than the brace implies). NOT proof by itself - an empty line can
be other compiled-out code - but combined with a base-inlines/target-calls
divergence it names the fix.

Fix: restore the assert at that spot (the original source plausibly had one -
this is source reconstruction, not an inline-shaping hack). The body grows
pre-optimization, the inline decision flips to match, final bytes are unchanged,
and line numbers below it shift toward the target's `__LINE__` geometry. Then
verify the standard way: out-of-line call-site counts on BOTH sides must agree
with the target's before the change is kept (sushi 2026-08-17; see
noinline-audit rule).

Calibration data (math::color_rgba float x4, render 2026-08-17, REVERTED): at
call sites whose args are ALL CONSTANTS the knob does not bite - 4x plain
`ASSERT` (Master Gold discards the expression, leaving only
`if(identity(false)){}`) moved nothing, and 4x `ASSERT_U` (range exprs kept in
the dead branch) still failed to flip the 8 renderer.cpp inline sites to the
target's calls while its dead branches PERTURBED the already-inlined constant
expansions (draw_luminance_picker_info cur 67.0->49.8). The constant-args
inline bonus dominates the pre-opt size estimate; use this pattern on sites
with non-constant args, and prefer detection via the line-geometry signature
first. Also note `ASSERT_U( valid(x) )` in a core header can break PCHs that
reach math_color.h before math_functions.h - keep kept-expressions
self-contained.
