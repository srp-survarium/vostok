# Early-return guard `if(!c) return;` = inverted test `jne .body` + a bare `jmp` epilogue
tags: cpp:if cpp:return | asm:cmp asm:jcc asm:jmp | topic:structure-shape
symptoms: cmp 0 / jne .body / jmp .epilogue pair, two branches where a wrapping-if has one je
confidence: 9/10
variants: od-ifelse-inversion.md, inverted-guard-early-return.md, early-returns-vs-else-join.md

A top-of-function guard `if (!p) return;` does NOT fold to a single `je .end`. MSVC keeps the
inverted test as `jne .body` and emits `return;` as a separate **unconditional `jmp` to the
epilogue** — so a guard shows TWO branch rows (`jne` over a `jmp`). The equivalent wrapping
`if (p) { ...; }` emits ONE `je .end`. Probe-verified, the two compiled side by side:

```cpp
void guard( int* p ) { if ( !p ) return;  *p = 1; }   // -> two branches
void wrap ( int* p ) { if ( p ) { *p = 1; } }         // -> one branch
```
```asm
?guard@@YAXPAH@Z PROC
  83 7d 08 00   cmp  DWORD PTR _p$[ebp], 0
  75 02         jne  SHORT $LN1@guard        ; p != 0 -> run the body
  eb 09         jmp  SHORT $LN2@guard         ; the `return;`  (bare unconditional jmp)
$LN1@guard:
  8b 45 08      mov  eax, DWORD PTR _p$[ebp]
  c7 00 01..    mov  DWORD PTR [eax], 1
$LN2@guard:     ret

?wrap@@YAXPAH@Z PROC
  83 7d 08 00   cmp  DWORD PTR _p$[ebp], 0
  74 09         je   SHORT $LN2@wrap          ; ONE branch, straight to the join
  ... body ...
$LN2@wrap:      ret
```
Steerable: a lone 2-byte `TRGT_ONLY` branch after the guard test = inverted guard + early `return;` (inverted-guard-early-return.md); flatten else-chains to early returns when the target shows per-leaf double `jmp` (early-returns-vs-else-join.md).
