# `movzx; neg; sbb; neg` on a bool array index = `arr[ b != false ]`, not `arr[ b ]`
tags: cpp:bool | asm:movzx asm:neg asm:sbb | topic:codegen-idiom
symptoms: full normalize before the index scale, bare movzx for plain bool index
confidence: 8/10
variants: bool-from-mask-test.md, bool-addend-explicit-compare.md

A bare `arr[bool_param]` is just `movzx eax, byte[param]` (already 0/1). The full
neg/sbb/neg normalize before the scale means the source wrote the explicit comparison.

```cpp
m_weapon_animations[ is_third_view != false ][ user_state_id == type_crouch ]
// 2nd index: a plain == compare -> cmp 1; sete
```
Evidence: weapon_core_idle_state::get_weapon_lexeme_pair (100%).
