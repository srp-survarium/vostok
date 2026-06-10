# u32 difference to float: store low/zero high, `fild qword`, fdiv
tags: cpp:float cpp:cast | asm:fild asm:mov | topic:codegen-idiom
symptoms: fild qword, zeroed high dword, fdiv [__real@447a0000]
confidence: 9/10

`float dt = ( u32a - u32b ) / 1000.0f`: there is no unsigned-32 FPU load, so MSVC stores
the u32 difference as the LOW dword of a 64-bit slot, zeroes the HIGH dword, `fild qword`,
then `fdiv` (0x447a0000 = 1000.0f). One statement, ~0x1f bytes. Do NOT mistake the
zeroed-high for a real u64/__int64 local.

```cpp
float const dt = ( current_time - last_time ) / 1000.0f;
```
```asm
mov [lo], eax; mov dword ptr [hi], 0
fild qword ptr [slot]
fdiv dword ptr [__real@447a0000]
```
Evidence: character_dispersion_calculator::tick (L59, dt the lone PDB local).
