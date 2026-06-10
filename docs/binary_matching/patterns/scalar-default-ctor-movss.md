# Float-POD default ctor: sequential `movss [this+off], const` in declaration order
tags: cpp:ctor cpp:init-list cpp:float | asm:movss asm:mov | topic:codegen-idiom
symptoms: movss xmm0 [__real], per-member stores in offset order, gap in offset sequence
confidence: 9/10
variants: member-ctor-call-plus-inline-stores.md

A member-init list of scalar float constants maps 1:1 to `movss` stores in DECLARATION
order (MSVC ignores init-list order). A member with NO store (offset gap, e.g. [this+1Ch]
then [this+24h]) is simply absent from the init list - leave it out. Each /Od float
literal gets its OWN rdata slot.

```cpp
weapon_recoil_params::weapon_recoil_params( ) : m_a( 0.5f ), m_b( 1.2f ) /* gap = omitted member */ { }
```
```asm
mov   eax, [ebp-4]            ; this
movss xmm0, [rdata_slot]
movss [eax], xmm0             ; member @0
mov   ecx, [ebp-4]
movss xmm0, [rdata_slot]
movss [ecx+4], xmm0           ; member @4 ...
```
Read the constants from the obj's .rdata + .text relocations (type 20 = DIR32) - objdiff/rich mask them as [0]; a NAMED reloc symbol (e.g. epsilon_3 = .001f) means a named engine constant (vostok::math::epsilon_3, math_constants.h).
Evidence: game_core/weapon_recoil_params::weapon_recoil_params() (target rva 0x5830c0).
