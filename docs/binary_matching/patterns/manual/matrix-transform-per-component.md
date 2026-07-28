# `float4x4::transform_position/direction` = 3 scalar dot-products; the `1.f*e30` translation folds to a bare `addss`
tags: cpp:float cpp:operator | asm:movss asm:mulss asm:addss | topic:codegen-idiom
symptoms: transform_position/transform_direction, result.x = vector.x*e00 + vector.y*e10 + vector.z*e20 + 1.f*e30
confidence: 9/10

`float4x4::transform_position`/`transform_direction` expand into three scalar dot-products, one
per output component: load the vector lane into an xmm, `mulss` against the matrix entry, `addss`
the running sum. The `+ 1.f*e30` translation term is NOT a `mulss` by 1.0 — the compiler folds it
to a plain `addss xmm,[ecx+30h]`. `transform_direction` is the identical shape MINUS that final
translation addss.

```cpp
result.x = vector.x*e00 + vector.y*e10 + vector.z*e20 + 1.f*e30;   // transform_position
```
```asm
movss xmm3, [ecx+10h]     ; e10
mulss xmm3, xmm1          ; * vector.y
movaps xmm4, xmm2         ; vector.x staged
mulss xmm4, [ecx]         ; * e00 (folded memory operand)
addss xmm3, xmm4
addss xmm3, [ecx+30h]     ; + 1.f*e30  -> bare add, NO mulss
movss [eax], xmm3         ; result.x via hidden sret in eax
```
Steerable: keep the exact source term order `vector.x*e00 + ... + 1.f*e30` — operand order picks which lane folds into the memory operand. cite: vostok::math::float4x4::transform_position math_float4x4_inline.h:76 / transform_direction :87.
