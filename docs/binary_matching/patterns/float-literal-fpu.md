# Bare float literals 0.0f / 1.0f load as fldz / fld1
tags: cpp:float | asm:fld asm:movss | topic:codegen-idiom
symptoms: fldz, fld1, movss xmm [__real@...] when a named constant was used instead
confidence: 9/10
variants: named-float-const-global.md

A bare `0.0f` / `1.0f` literal in an FPU expression emits `fldz` / `fld1`. If the base
instead does `movss xmm, [__real@...]` (constant-pool load), the source used a named
constant or variable, not the bare literal.

```cpp
if ( direction.squared_length( ) < 1.f )   // -> fld1
```
Evidence: game_core/sources/bullet.cpp.
