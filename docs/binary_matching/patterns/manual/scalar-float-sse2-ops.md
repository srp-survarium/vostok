# Scalar float math = `movss`/`addss`/`mulss` on xmm, ONE component at a time, base ptr reloaded per component (/Od)
tags: cpp:float | asm:movss asm:addss asm:mulss | topic:codegen-idiom
symptoms: f3 0f 10/58/59 (movss/addss/mulss), base pointer reloaded before every component, __fltused EXTRN, __real@<hex> const COMDAT, float global mangled @@3MA
confidence: 9/10
variants: scalar-default-ctor-movss.md, float4x4-identity-temp.md

The engine's pervasive `float3`/`float4x4` math (`/arch:SSE2 /fp:fast`) lowers to **scalar
SSE**: `movss xmm,<src>` / `addss` / `mulss` / `subss` / `divss`, one component per group.
Under /Od the base pointers are **reloaded before every component** (no caching across `.x`/
`.y`/`.z`), so an N-component op is N near-identical movss/op/store groups. A float global is
mangled `@@3MA` (`M` = float), float literals become `__real@<ieee-hex>` const COMDATs, and
any float use drags in the `__fltused` symbol. Probe-verified:

```cpp
void add( float3* r, float3 const* a, float3 const* b ) { r->x=a->x+b->x; r->y=a->y+b->y; r->z=a->z+b->z; }
void dot( float3 const* a, float3 const* b ) { g = a->x*b->x + a->y*b->y + a->z*b->z; }
```
```asm
; add (per component; note the base reloads):       ; dot (mul into xmm0, extra terms via xmm1+addss):
  8b 45 0c mov eax,[a] / 8b 4d 10 mov ecx,[b]          f3 0f 10 00 movss xmm0,[a]
  f3 0f 10 00 movss xmm0,[eax]                         f3 0f 59 01 mulss xmm0,[b]
  f3 0f 58 01 addss xmm0,[ecx]                         f3 0f 10 4a 04 movss xmm1,[a+4]
  8b 55 08 mov edx,[r] / f3 0f 11 02 movss [edx],xmm0  f3 0f 59 48 04 mulss xmm1,[b+4]
  ... reload a,b,r; repeat at +4, then +8 ...          f3 0f 58 c1 addss xmm0,xmm1
                                                       ... +8 term ... / movss g,xmm0
```
Steerable: `movss`/`addss`/`mulss` per component with full base reloads = straight `float3`-style member arithmetic written component-wise; the operator order (`addss` vs `mulss` vs `subss`) and the xmm0/xmm1 accumulation pattern dictate the exact expression. The float-POD ctor's per-member `movss <const>` is scalar-default-ctor-movss.md.
