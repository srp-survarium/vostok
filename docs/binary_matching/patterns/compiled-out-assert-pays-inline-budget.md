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
