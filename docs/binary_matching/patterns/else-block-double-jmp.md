# Two identical `jmp .end` back-to-back = a returning if-block's `}` jumping OVER an else
tags: cpp:if cpp:return | asm:jmp | topic:structure-shape
symptoms: +0x002 brace row sandwiched between if-body last statement and else statement, two jmp short .end
confidence: 8/10
variants: closing-brace-jmp-rows.md, early-returns-vs-else-join.md

A returning if-body followed immediately by a second `jmp .end` (the block's `}` jumping
past the else to the epilogue) means a REAL `else` block, not a fall-through: a plain
if-then + trailing return falls THROUGH (no jmp over it) and /Od folds the two identical
return jmps into one.

```cpp
if ( !x ) { ...; return a; }
else { return mul; }          // the else recovers the second jmp .end
```
```asm
mov eax,[ebp+8]; jmp short .end   ; the return's own jmp
jmp short .end                    ; the if-} jmp, jumping OVER the else body
.else: ...; mov eax,[ebp+8]
.end:
```
A 1-byte `int3` inside such a block is `DEBUG_BREAK( )` (= __debugbreak, debug_macros.h), NOT an empty Master-Gold ASSERT (zero bytes).
Evidence: math::get_relative_matrix 90.2 -> 97.5 (int3) -> 100 (the else).
