# Builder/setter chain on ONE object: target wrote SEPARATE statements, not a fluent chain
tags: cpp:method cpp:inline | asm:lea asm:call | topic:structure-shape topic:source-fidelity
symptoms: N tiny per-line statements each `lea &obj; call obj::setter`, base collapses the whole chain into ONE big row, statement count too LOW base-side
confidence: 8/10
variants: chained-temporary-params-object.md, mixing-expression-operator-plus.md

A fluent builder `obj.a(x).b(y).c(z);` is ONE C++ statement, so the compiler emits it
as ONE debug-info statement row (one big base-side row). If the TARGET instead shows
each setter as its OWN tiny statement on its OWN source line - and the `this` for each
call is a FRESH `lea eax, [ebp-obj]` (the SAME object slot), not the threaded return of
the previous call - then the original source wrote them as SEPARATE statements:

```cpp
obj.a( x );   // line L
obj.b( y );   // line L+1
obj.c( z );   // line L+2
```
NOT `obj.a(x).b(y).c(z);`. The setters still `return *this` (chainable) but the author
discarded the return and re-named the object each line. Tell from the asm: every call
site reloads `lea eax,[ebp-N]` with the SAME N (the object), never `mov ecx,eax` off the
prior call's result.

Splitting the chain into per-statement calls reproduces the target's statement COUNT and
per-line line-numbers (a large structure win), even though the trivial `inline` setters
may still differ inline-vs-call (base inlines to a member store, target keeps the `call`)
- that residual is a separate, non-steerable wall; take the structure first.

Evidence: game_core get_weapon_lexeme_pair_impl - fluent main/offset chains collapsed to
10 stmts (58%); split to 20 per-line stmts line-aligned to the target's 21 (65.7%); the
one missing stmt is the unresolved L40 lone-byte (lone-byte-store-zero.md), and the
per-setter residual is the mixing-builder inline-vs-call wall.
