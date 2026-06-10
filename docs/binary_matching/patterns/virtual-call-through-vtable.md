# Virtual call on a member reference: `mov edx,[obj]; mov eax,[edx+slot]; call eax`
tags: cpp:virtual cpp:member | asm:call asm:mov | topic:codegen-idiom
symptoms: vtable slot load [edx+8Ch] call eax, slot loaded twice under /Od
confidence: 9/10
variants: nonvirtual-member-call-direct.md

Calling a method declared `virtual` on the reference's class dispatches via the vtable.
Same source shape as a non-virtual member call; only the callee's virtual-ness differs.

```cpp
m_weapon.instant_aim_start( );   // virtual on weapon_core
```
```asm
mov eax,[ebp-4]; mov ecx,[eax+128h]   ; m_weapon (weapon_core& @0x128)
mov edx,[ebp-4]; mov eax,[edx+128h]   ; /Od loads the slot twice
mov edx,[ecx]                         ; vtable
mov ecx,eax                           ; this
mov eax,[edx+8Ch]; call eax           ; vtable slot 0x8c
```
Evidence: weapon_core_aimed_state_base::{initialize,finalize} (instant_aim_start/end @ slots 0x8c/0x90) vs idle_state_base's direct calls.
