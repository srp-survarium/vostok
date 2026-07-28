# bool -> u32 member store through neg/sbb/neg = a `? 1u : 0u` ternary
tags: cpp:bool cpp:ternary cpp:member | asm:movzx asm:neg asm:sbb | topic:codegen-idiom
symptoms: final store +0x6 in target, movzx neg sbb neg before the member store
confidence: 8/10
variants: bool-addend-explicit-compare.md, bool-from-mask-test.md

Storing a bool local into a u32 member: a plain bool read stores movzx directly; the
target's boolize (+0x6) means the original spelled the rvalue as a ternary.

```cpp
m_last_shot_animation_index = last_shot ? 1u : 0u;
```
Evidence: pistol_weapon_core_fire_state::initialize 92.62 -> 99.76 (3/3, 0x85 both sides), matching the aimed sibling that already carried the ternary.
