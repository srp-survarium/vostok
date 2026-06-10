# Named float-const global vs literal at a `float const&` parameter
tags: cpp:float cpp:const | asm:push asm:movss asm:lea | topic:codegen-idiom
symptoms: push <global> vs movss to a temp then lea/push of the temp
confidence: 6/10
variants: float-literal-fpu.md

At a `float const&` parameter the target pushing the ADDRESS of a named global (e.g.
epsilon_3) means the original named the constant; a literal `0.001f` binds the const&
through a fresh stack temp (movss to slot, lea, push).

```cpp
bullet_direction * math::epsilon_3    // not * 0.001f
```
Evidence: bullet::collide_front_face 95.13 -> 97.40 (with the min-order fix).
