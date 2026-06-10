# `math::pow(x, INT)` binds pow(float,int): target inlines the sign-dispatch, base calls
tags: cpp:float cpp:cast | asm:call asm:test | topic:inline-vs-call
symptoms: mov edx 5 test jne call pow_impl inlined vs mov ecx 5 call vostok::math::pow
confidence: 8/10
variants: float3-dot-operator-inline.md

`math::pow( f, 5 )` (INTEGER second arg) binds `pow(float,int)`, whose body
(`test ecx; jne; movss[1.0]; ret / jge; divss; neg; jmp pow_impl / jmp pow_impl`) the
TARGET inlines whole-program while the BASE calls it. `pow_impl(float,uint)` is
out-of-line in BOTH indexes; `pow(float,int)` only in base. The inline reorders operand
evaluation and shifts x87-vs-xmm result codegen. Source is correct; mark PARTIAL.

```cpp
math::pow( x, 5 );     // int -> pow(float,int);  pow(x,5.0f) would bind pow(float,float) - different fn
```
Evidence: game_core/pseudo_random::random_f (target rva 0x57e420, 60.74%).
