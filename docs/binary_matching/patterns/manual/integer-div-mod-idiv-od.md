# Integer `/` and `%` in a /Od TU = one `cdq;idiv` (or `xor edx;div`); quotient EAX, remainder EDX
tags: cpp:operator | asm:idiv asm:div | topic:codegen-idiom
symptoms: x / y, x % y, cdq, idiv, xor edx edx, div dword ptr, remainder used from edx
confidence: 9/10

In a /Od TU (tu-optimization-level-od-vs-ox.md), `/` and `%` emit a real divide — signed:
`cdq; idiv r/m`; unsigned: `xor edx,edx; div r/m`. Quotient stays in EAX, remainder in EDX, so a
sibling `/` and `%` on the same operands fold into ONE divide. This holds even for a CONSTANT
divisor (no magic-number multiply) because the TU is unoptimized.

```cpp
tv.tv_sec  = usec / 1000000;     // signed long, constant divisor, /Od TU
tv.tv_usec = usec % 1000000;
size_t bucket = calculate_hash_value(k) % num_buckets_;   // unsigned
```
```asm
; signed, constant divisor 1000000     ; unsigned, divisor = member [ecx+18h]
cdq                                      xor   edx, edx
idiv  ecx                                div   dword ptr [ecx+18h]
                                         mov   [ebp-8], edx   ; remainder = bucket
```
Steerable: emit the right signedness and reuse the divide for a paired `/`,`%`. In a /Ox TU a constant divisor becomes a magic-number multiply instead (integer-div-mod-magic-ox.md). cite: boost select_reactor::get_timeout select_reactor.ipp:285; hash_map::find hash_map.hpp:116.
