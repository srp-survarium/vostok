# Optimization level is PER-TU (.vcproj Optimization 0 vs 3) — read the asm to know which world you're in
tags: cpp:for cpp:operator | asm:idiv asm:mul asm:add | topic:convention topic:ltcg
symptoms: same source, two codegens; idiv vs magic-number multiply for /,%, verbose STL iterator objects vs a raw register pointer, ebp frame vs omitted
confidence: 9/10

The `Master Gold|Win32` config sets `Optimization` **per source file** in each `.vcproj`
(game_core.vcproj alone has 23 per-file blocks mixing `Optimization="0"` and `="3"`; core is
mostly `="3"`). So a "module" is NOT uniformly one opt level — each TU is `/Od` (0) or `/Ox`
(3), all with `WholeProgramOptimization="1"` (LTCG). The level dictates the shape, and the
asm itself is the tell:
- **/Od TU**: ebp frame kept, one store per source statement, `cdq;idiv`/`xor edx;div` for
  `/` `%` (even by a constant), STL `it != end` is a verbose iterator-object compare.
- **/Ox TU**: aggressive inlining, frame-pointer omission, **magic-number multiply** for
  constant `/` `%` (no idiv), STL iterators collapse to a raw register pointer (`add reg,
  sizeof(T)`), values cached in registers across statements.

```cpp
x / 1000000;   // /Od TU -> cdq; idiv     |   /Ox TU -> mov eax,magic; mul; shr
```
Steerable indirectly: confirm the target TU's `Optimization=` in its `.vcproj` before predicting body shape; when in doubt READ the target asm (`idiv` vs a `mov eax,<magic>; mul` = the opt level). The patterns below split their /Od and /Ox forms accordingly. cite: game_core.vcproj per-file blocks; core day_count (/Ox, magic) vs boost select_reactor (/Od, idiv).
