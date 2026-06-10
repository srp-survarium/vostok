# Lone 4-byte `mov byte[ebp-N],0` = unused `bool b = false;`, NOT an ASSERT
tags: cpp:bool cpp:local | asm:mov | topic:assert-eater
symptoms: <0x4> statement, mov byte ptr [ebp-N],0 with no lea/call
confidence: 8/10
variants: assert-eater-recover.md

A statement of size <0x4> whose only instruction is the byte store, with NO following
`lea eax; call <empty>`, is a dead `bool` local init kept under /Od (no DCE). An assert
eater is <0xc>; writing `ASSERT(UNKNOWN_EXPRESSION)` here over-produces the lea+call.

```cpp
bool b = false;   // declared, never used
```
```asm
mov byte ptr [ebp-5], 0    ; nothing follows
```
Disp size (4 vs 7 bytes) is slot-allocation noise. Evidence: game_core/get_weapon_lexeme_pair_impl L40.
