# Counted loop exiting on `je` (not `jae`) = source wrote `i != N`, not `i < N`
tags: cpp:for | asm:cmp asm:jcc | topic:codegen-idiom
symptoms: cmp [i] N je .end vs jae .end
confidence: 9/10

MSVC /Od preserves the exact comparator: `!=` -> je-exit, unsigned `<` -> jae-exit. Match
the operator the target used.

```cpp
for ( u32 i = 0 ; i != 2 ; ++i )    // je-exit; `i < 2` would emit jae
```
Evidence: weapon_core_idle_state/weapon_core_aimed_state ctors (two nested for(... != 2 ...)), 100%.
