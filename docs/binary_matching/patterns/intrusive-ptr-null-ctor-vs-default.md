# `set(0)` after the zero-store = `resource_ptr( NULL )`, not `resource_ptr( )`
tags: cpp:ctor cpp:template | asm:push asm:call asm:lea | topic:structure-shape
symptoms: mov dword [slot] 0 then push 0 lea ecx call intrusive_ptr::set, base 10 bytes short per site
confidence: 8/10
variants: static-cast-resource-ptr-copy.md

The intrusive_ptr/resource_ptr DEFAULT ctor compiles to a single `mov dword ptr [slot], 0`.
The POINTER ctor (`intrusive_ptr( T* )` = `m_p(0) { set(p) }`) compiles to the zero-store
PLUS `push <p>; lea ecx,[slot]; call intrusive_ptr<...>::set` (~10 extra bytes). A ternary
false-arm / empty-fallback that shows the set(0) call was written `resource_ptr( NULL )`,
not `resource_ptr( )` - swap the source and the statement closes.

```asm
; target false arm:  mov dword ptr [ebp-60h],0 ; push 0 ; lea ecx,[ebp-60h] ; call intrusive_ptr<...>::set
; base  false arm:   mov dword ptr [ebp-60h],0                                ; SIZE -0xa per site
```
Evidence: weapon_core_cook::on_weapon_states_ready (the two chamber_a_round ternaries;
98.74% -> 100.0% after `weapon_core_base_state_ptr( NULL )`).
