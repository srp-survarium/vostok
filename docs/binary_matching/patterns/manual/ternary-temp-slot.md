# `dst = c ? a : b` materializes a `tv` temp + a final copy (bigger frame); if/else stores direct
tags: cpp:ternary cpp:local | asm:cmp asm:jcc asm:jmp asm:mov | topic:structure-shape topic:pdb-locals
symptoms: a tv<N> compiler temp written in both arms then copied to dst, an extra push ecx/sub esp for the slot, vs if/else writing dst directly with no temp
confidence: 9/10
variants: od-ifelse-inversion.md, bool-store-ternary-boolize.md

A `?:` whose result is consumed (assigned, passed, returned) lowers through a **compiler
temp** `tv<N>`: both arms write the temp, then ONE copy moves it to the destination — and
the frame grows by a slot (`push ecx`/`sub esp,4`) to hold it. The equivalent
`if (c) dst=a; else dst=b;` writes `dst` DIRECTLY in each arm — no temp, no extra slot. So a
`tv` temp + a trailing copy = a ternary, not an if/else. Probe-verified:

```cpp
void val ( int c, int a, int b ) { g = c ? a : b; }            // -> tv temp
void ifeq( int c, int a, int b ) { if ( c ) g = a; else g = b; } // -> direct stores
```
```asm
?val@@YAXHHH@Z PROC                       ?ifeq@@YAXHHH@Z PROC
  51        push ecx          ; tv slot     83 7d 08 00 cmp [c],0
  83 7d 08 00 cmp [c],0                      74 0a je $LN2@ifeq
  74 08 je $LN3@val                          8b 45 0c mov eax,[a]
  8b 45 0c mov eax,[a]                       a3 .. mov g,eax      ; dst written DIRECT
  89 45 fc mov tv65,eax                      eb 09 jmp $LN3@ifeq
  eb 06 jmp $LN4@val                       $LN2@ifeq:
$LN3@val: 8b 4d 10 mov ecx,[b]              8b 4d 10 mov ecx,[b]
  89 4d fc mov tv65,ecx                      89 0d .. mov g,ecx   ; dst written DIRECT
$LN4@val: 8b 55 fc mov edx,tv65            $LN3@ifeq:
  89 15 .. mov g,edx    ; copy temp->dst
```
Steerable: a `tv<N>` slot written in both arms + a final copy = a ternary (write `?:`); per-arm direct stores = if/else. A constant-pair ternary `c ? K : K+1` instead goes branchless: `xor eax,eax / setcc al / add eax,K` (cf. bool-store-ternary-boolize.md).
