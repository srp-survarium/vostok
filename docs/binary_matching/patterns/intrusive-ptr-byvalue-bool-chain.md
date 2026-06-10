# Smart-ptr returned by value in a `&&` chain: temp construct + safe-bool test + dtor
tags: cpp:if cpp:bool | asm:sete asm:cmp asm:call | topic:codegen-idiom
symptoms: or dword [ebp-18h] 1 EH guard, intrusive_ptr::set then cmp 0 sete then ::dec, and 0FFFFFFFEh
confidence: 9/10

A getter returning a resource_ptr/intrusive_ptr BY VALUE used in boolean context
materializes the temp, tests it via the safe-bool operator, and destroys it before the
next `&&` term - the whole construct/test/destroy is ONE term.

```cpp
if ( a && m_weapon->ammunition( ) && b )    // ammunition() returns weapon_ammunition_ptr by value
```
```asm
mov dword ptr [ebp-18h], 0          ; EH guard word
or  dword ptr [ebp-18h], 1          ; temp live
lea ecx,[ebp-0Ch]; push <&member>; call intrusive_ptr<...>::set
xor ecx,ecx; cmp dword ptr [ebp-0Ch],0; sete cl
and dword ptr [ebp-18h], 0FFFFFFFEh
lea ecx,[ebp-0Ch]; call intrusive_ptr<...>::dec
```
Base LTCG may lower the safe-bool through an EXTRA intermediate bool slot (mov [slot],0; jmp; cmp [slot],0) vs the target's direct cmp/sete - not steerable; match the control flow + by-value getter call.
Evidence: dispersion_calculator::get_dispersion (rva 0x586970, 87.49% PARTIAL).
