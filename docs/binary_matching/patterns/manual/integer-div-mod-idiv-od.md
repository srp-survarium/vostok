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

Power-of-two exception: retail network_core `operator-<u16>` at RVA `0x127b20`
retains its frame and comparison call but lowers signed `% 65536` to
`and eax,8000ffffh; jns; dec eax; or eax,ffff0000h; inc eax`.
The preceding `add eax,10000h` belongs to the source numerator. This is not a
signed-16-bit cast: at the serial-number half-window boundary the cast changes
the result's sign. Reconstruct the arithmetic before blaming an inline boundary;
the surrounding acknowledgement consumer calls this real helper.
