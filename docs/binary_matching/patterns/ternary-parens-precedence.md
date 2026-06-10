# Ternary precedence trap: `a * b * c ? x : y` makes the whole product the condition
tags: cpp:ternary cpp:float | asm:test asm:movzx | topic:structure-shape
symptoms: target tests the bool ALONE then multiplies, base tests the float product
confidence: 9/10

`mult * koef * first_shoot ? side_a : side_b` parses as `(mult*koef*first_shoot) ? ...`
(* binds tighter than ?:). If the target tests the bool alone
(`movzx ecx, byte[first_shoot]; test ecx,ecx; je .else`) then multiplies the selected
value, the original parenthesized the ternary. Fully steerable source bug.

```cpp
mult * koef * ( first_shoot ? side_a : side_b )
```
Evidence: weapon_recoil_calculator::fire 79.80 -> 91.69 (both the recoil and recoil_amount ternaries).
