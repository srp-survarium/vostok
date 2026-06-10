# `val != u32(-1) ? &obj : NULL` = `sub r,-1; neg; sbb r,r; lea; and`
tags: cpp:ternary | asm:neg asm:sbb asm:and asm:lea | topic:codegen-idiom
symptoms: sub edx 0FFFFFFFFh neg sbb edx edx lea and edx eax push
confidence: 8/10
variants: bool-from-mask-test.md

A pointer-or-NULL select on an unsigned `!= -1` test: MSVC /Od lowers the compare to
`sub r,-1; neg; sbb r,r` (mask = all-ones iff non-equal) and ANDs it with the address.
Write the ternary in source; it reproduces byte-for-byte.

```cpp
time_synchronization_group != u32( -1 ) ? &main_lexeme : NULL
```
```asm
mov  edx, [ebp+20h]      ; val
sub  edx, 0FFFFFFFFh     ; val + 1
neg  edx                 ; CF iff val != -1
sbb  edx, edx            ; 0xFFFFFFFF or 0
lea  eax, [ebp-0E0h]     ; &obj
and  edx, eax            ; &obj or NULL
push edx
```
Evidence: game_core/get_weapon_lexeme_pair_impl (the offset lexeme's time_driving_animation arg).
