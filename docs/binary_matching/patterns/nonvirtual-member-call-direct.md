# Non-virtual member call through a member reference = DIRECT `call`
tags: cpp:member cpp:virtual | asm:call asm:mov | topic:codegen-idiom
symptoms: direct call survarium::Class::method, no vtable indirection
confidence: 9/10
variants: virtual-call-through-vtable.md

`m_ref.method()` where method is NON-virtual compiles to a direct `call`. The identical
source shape with a virtual method dispatches via `[vtbl+off]` - the asm differs ONLY by
the method's virtual-ness; read the .h `virtual` keyword to decide.

```cpp
m_weapon.instant_toggle_start( );   // non-virtual member of weapon_core
```
```asm
mov ecx,[this]; mov ecx,[ecx+128h]                    ; ecx = m_weapon (weapon_core& @0x128)
call survarium::weapon_core::instant_toggle_start     ; direct, no vtable
```
Evidence: weapon_core_show_state_base::{initialize,finalize} (non-virtual instant_toggle_start/end) vs aimed_state_base (virtual instant_aim_start/end).
