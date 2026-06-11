# Derived state ctor: base delegation + compiler-emitted vtable stores, EMPTY body
tags: cpp:ctor cpp:init-list cpp:virtual | asm:call asm:mov | topic:codegen-idiom
symptoms: mov [ecx] ??_7, mov [edx+18h] ??_7, vtable stores after base-ctor call
confidence: 9/10
variants: base-ctor-call-vs-inlined-init.md, derived-ctor-overwrites-inherited.md

The `mov [this+off], ??_7...` stores after a base-ctor call are the compiler-emitted
vtable pointers for each base subobject of a multiply-inherited class - never write them.
A ctor that is ONLY an init-list base delegation matches with an empty `{}` body.

```cpp
weapon_core_idle_state_base::weapon_core_idle_state_base( weapon_core& weapon )
	: weapon_core_base_state( weapon, false ) { }
```
```asm
push 0; mov eax,[ebp+8]; push eax; mov ecx,[ebp-4]
call survarium::weapon_core_base_state::weapon_core_base_state
mov [ecx],     ??_7...@6Bfsm_state@ai@vostok@@            ; primary vtable @0x00
mov [edx+18h], ??_7...@6Bunmanaged_resource@...@          ; secondary vtable @0x18
```
Evidence: game_core/weapon_core_idle_state_base ctor, 100% with the empty body.
