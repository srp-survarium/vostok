# Integer `/` or `%` by a CONSTANT in a /Ox TU = magic-number multiply (`mov eax,<magic>; mul; shr`), NO idiv
tags: cpp:operator | asm:mul asm:shr asm:imul | topic:codegen-idiom
symptoms: mov eax,<reciprocal magic e.g. 51EB851Fh>, mul, shr edx,N, imul edx,<divisor>, sub (remainder), no idiv
confidence: 9/10

In a /Ox TU a constant divisor is reciprocal-multiplied: load the magic constant, `mul`, shift
`edx`, then (for `%`) `imul` the divisor back + `sub` to recover the remainder. Seeing
`mov eax,<magic>; mul; shr edx,N; imul edx,K` IS `/ K` / `% K` — do not try to write an idiv.

```cpp
u32 const extra = ((years % 400 == 0) || ((years % 4 == 0) && (years % 100 != 0))) ? 1 : 0;   // /Ox TU
```
```asm
; years % 400  (51EB851Fh = 2^39/400 reciprocal)
mov   eax, 51EB851Fh
mul   ecx
shr   edx, 7
imul  edx, 190h          ; * 400
sub   eax, edx           ; remainder
```
Wall-ish: the magic constant + shift are codegen — reproduced only by writing the same `/`,`%` on the same-signedness type in a /Ox TU. The /Od form is a plain idiv (integer-div-mod-idiv-od.md); which one you see is set per-TU (tu-optimization-level-od-vs-ox.md). cite: core day_count build_extensions.cpp:42-66.
