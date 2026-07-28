# if/else under /Od: the condition INVERTS, the then-block ends in a `jmp` over the else
tags: cpp:if cpp:return | asm:cmp asm:jcc asm:jmp | topic:codegen-idiom
symptoms: cmp/jge (or jne) skipping the then-block, jmp short over the else, else-if = chained cmp/jne .next then jmp .end
confidence: 9/10
variants: od-early-return-guard.md, inverted-if-else-recovery.md, flat-ifs-vs-else-if.md, else-block-double-jmp.md

`if (C) T; else E;` emits the **negated** test jumping PAST the then-block; the then-block
ends with a `jmp` over the else; the else falls through. The trailing `jmp` (vs none) is the
tell that an `else` exists. An `else if` chain is N of these stacked: each `cmp; jne .next`
falls through to its body + `jmp .end`; the final bare `else` has no compare. Probe-verified
(`/FAcs`, /Od):

```cpp
void ifelse( int a, int b ) { if ( a < b ) g = 1; else g = 2; }
```
```asm
?ifelse@@YAXHH@Z PROC
  8b 45 08          mov  eax, DWORD PTR _a$[ebp]
  3b 45 0c          cmp  eax, DWORD PTR _b$[ebp]
  7d 0c             jge  SHORT $LN2@ifelse          ; a<b INVERTED -> jge skips the then
  c7 05 .. 01000000 mov  DWORD PTR ?g@@3HA, 1       ; g = 1
  eb 0a             jmp  SHORT $LN3@ifelse          ; then-block jumps OVER the else
$LN2@ifelse:
  c7 05 .. 02000000 mov  DWORD PTR ?g@@3HA, 2       ; g = 2
$LN3@ifelse:
  c3                ret  0
; else-if chain: each arm is  cmp [a],K / jne .next / mov g,K / jmp .end ; final else = bare mov
```
Steerable: an extra `jmp .end` after the then-block = there is an `else`; no else folds the join away. else-if = one `cmp/jne .next` per arm (flat-ifs-vs-else-if.md).
