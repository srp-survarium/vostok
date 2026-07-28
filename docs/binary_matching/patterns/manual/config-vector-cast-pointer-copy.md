# A float3/by-ref config cast returns a POINTER + a member-wise 3-dword copy (scalars return in a register)
tags: cpp:operator cpp:cast cpp:float | asm:call asm:mov | topic:codegen-idiom
symptoms: call operator[] / call operator float3 (ICF-folded onto operator*) / mov [dst]/[dst+4]/[dst+8] copy triplet
confidence: 8/10

The vector casts (`operator math::float3 const&` etc.) return a `const&` — a **pointer in eax** —
so unlike the scalar case the result is copied member-wise, three dwords. `position =
config["position"]` (implicit cast) and `(float3)config["rotation"]` lower identically. ICF folds
the cast body's symbol onto an unrelated `operator*`; the call-target name is a red herring.

```cpp
position = config["position"];                 // implicit operator float3 const&
math::create_rotation( (float3)config["rotation"] );
```
```asm
call  ...binary_config_value::operator[]
call  ...::operator*        ; <- ICF-folded float3 cast, eax = float3 const*
mov   ecx, [ebp-158h]
add   ecx, 8
mov   edx, [eax]    / mov [ecx], edx
mov   edx, [eax+4]  / mov [ecx+4], edx
mov   eax, [eax+8]  / mov [ecx+8], eax
```
Steerable: keep the idiomatic cast; expect the 3-dword copy (vs a register store for a scalar, config-index-then-cast.md). cite: respawn_point_core::load respawn_point.cpp:24-25.
