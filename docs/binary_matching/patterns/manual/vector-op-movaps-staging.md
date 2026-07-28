# `cross_product`/`mul4x3` stage operands with `movaps xmm,xmm` register copies — NOT base reloads — because the source caches `self = *this`
tags: cpp:float cpp:operator | asm:movaps asm:mulss asm:subss | topic:codegen-idiom
symptoms: float3_pod self = *this; movaps xmmN,xmmM before each mulss/subss, x = self.y*other.z - self.z*other.y
confidence: 9/10

The generic /Od rule is "reload the base pointer per component" (manual/scalar-float-sse2-ops.md).
`cross_product` and `mul4x3` are the **exception**: they snapshot `float3_pod self = *this;` so
all inputs are live xmm values, and each multiplicand is staged with `movaps xmmN, xmmM` (a
register-register copy) before the `mulss`/`subss` — no memory re-reads.

```cpp
float3_pod self = *this;
x = self.y*other.z - self.z*other.y;   // cross_product
```
```asm
movaps xmm0, xmm3        ; self.z staged
mulss  xmm0, xmm2        ; * other.y
movaps xmm1, xmm5        ; self.y staged
mulss  xmm1, xmm4        ; * other.z
subss  xmm0, xmm1
movss  [eax], xmm0       ; -> result.x (sret)
```
Steerable: the `self = *this` local is load-bearing — drop it and codegen reverts to per-component memory reloads (scalar-float-sse2-ops.md). cite: vostok::math::cross_product math_float3_inline.h:124 (member `^=` :96); mul4x3 math_float4x4_inline.h:146.
