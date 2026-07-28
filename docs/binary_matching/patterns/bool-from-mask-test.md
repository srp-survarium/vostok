# `and X,mask; neg; sbb X,X; neg` = `bool b = ( val & MASK ) != 0;`
tags: cpp:bool | asm:and asm:neg asm:sbb | topic:codegen-idiom
symptoms: and ecx 1 neg sbb ecx ecx neg, mask literal in the and operand
confidence: 6/10
variants: bool-index-explicit-compare.md, bool-addend-explicit-compare.md, bool-store-ternary-boolize.md, ternary-pointer-or-null.md

MSVC /Od emits neg/sbb/neg to normalize any-nonzero to exactly 1 for the explicit `!= 0`
spelling. The mask literal reads straight from the `and` operand.

```cpp
bool b = ( val & MASK ) != 0;
```
```asm
mov ecx, [eax+10h]
and ecx, 1
neg ecx; sbb ecx, ecx; neg ecx
mov [ebp-2], cl
```
Evidence: player_logic_base_state::movement_animation_index (actions_mask @ player_input+0x10, masks 1/2/4/8).
evidence-basis: named site but no stated outcome; the byte-proofs live in sibling entries
