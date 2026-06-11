# Bool added to an int through neg/sbb/neg = source wrote `+ ( b != 0 )`, not `+ b`
tags: cpp:bool | asm:movzx asm:neg asm:sbb asm:add | topic:codegen-idiom
symptoms: movzx neg sbb neg add, plain movzx add for the bare bool
confidence: 9/10
variants: bool-from-mask-test.md, bool-index-explicit-compare.md, bool-store-ternary-boolize.md

A bare bool->int promotion is `movzx ecx,[b]; add eax,ecx`. The neg/sbb/neg normalize
BEFORE the add means the addend was spelled as a comparison. The SAME bool read directly
in a sibling `if (b)` branch stays a plain read - the normalize is specific to the
arithmetic-add context.

```cpp
m_ammo_in_magazine + ( m_is_round_chambered != 0 )
```
```asm
movzx ecx, byte [b]; neg ecx; sbb ecx, ecx; neg ecx; add eax, ecx
```
Evidence: weapon_core::reset_fire_queue else-branch (bare + gave a direct add @94%; the explicit compare reproduced neg/sbb/neg -> 99.65%); weapon_core::maximum_ammo_in_weapon (with the const-bool fix -> 100%).
