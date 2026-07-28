# `length()`/`normalize()` mix SSE and x87: SSE self-square sum, then `call sqrtf` + `fld1; fdivrp` reciprocal
tags: cpp:float cpp:vector | asm:call asm:fld1 asm:fdivrp asm:mulss | topic:codegen-idiom
symptoms: squared_length sqr(x)+sqr(y)+sqr(z) via movaps+mulss, then call sqrtf (float on the int stack), fld1; fdivrp, mixed fmul/mulss
confidence: 9/10

`squared_length` is `sqr(x)+sqr(y)+sqr(z)` via `movaps`+`mulss` self-square + `addss` sum — but
the `sqrt` is a runtime **`call sqrtf`** (the float pushed on the integer stack, NOT `sqrtss`).
`normalize()` is `*this /= length()`, and `operator/=` computes `1.f/value` first: the reciprocal
lands on the **x87 stack** (`fld1; fdivrp`), then the three component scalings are a MIX of
`fmul ...[esi+N]` (x87) and `mulss xmm0,[esp+N]` (SSE). A pure-SSE `divss` spelling will not match.

```cpp
inline type    length()   const { return sqrt( squared_length() ); }
inline float3& operator/=(type value) { float const invert_value = 1.f/value; *this *= invert_value; ... }
```
```asm
movaps xmm3, xmm0 / mulss xmm3, xmm0   ; sqr(x)  (y,z follow, addss-summed)
movss  [esp], xmm3
call   sqrtf                            ; x87 call, not sqrtss
fld1
fdivrp                                  ; 1.f / length on the FPU stack
fmul   st, dword ptr [esi+4]            ; y component on x87
mulss  xmm0, [esp+8]                     ; x component on SSE  (mixed!)
```
Wall-ish: the FPU/SSE mix is dictated by `1.f/value` in `operator/=` plus `/fp:fast`; match the source spelling exactly (`length()` via `sqrt(squared_length())`, normalize via `/= length()`). cite: vostok::math::length(float3 const&) math_float3_inline.h:194/199; float3_pod::normalize() :140 (operator/= :71).
