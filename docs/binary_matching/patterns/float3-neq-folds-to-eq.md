# float3 `a != b` folds to `!(operator==)` when the target has only operator==
tags: cpp:operator cpp:float | asm:call asm:jcc | topic:inline-vs-call
symptoms: call operator== with inverted branch polarity vs call operator!=, no operator!= symbol in target
confidence: 8/10
variants: float3-dot-operator-inline.md

`x != float3(...)`: TARGET calls `vostok::math::operator==` (@0x14b90) with je/jne
polarity OPPOSITE to base; BASE keeps a standalone float3 `operator!=` (@0x6f250,
math_float3_inline.h) and calls it directly. operator!= is `inline { return !(a==b); }`,
folded at the call site in target. Source `a != b` is correct; mark PARTIAL.

```cpp
if ( position != float3( 0.f, 0.f, 0.f ) )
```
Evidence: redundant_bullet_predicate::operator() (87.98%).
