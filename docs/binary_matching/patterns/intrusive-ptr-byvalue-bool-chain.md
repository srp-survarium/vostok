# Smart-ptr returned by value in a `&&` chain: temp construct + safe-bool test + dtor
tags: cpp:if cpp:bool | asm:sete asm:cmp asm:call | topic:codegen-idiom
symptoms: or dword [ebp-18h] 1 EH guard, intrusive_ptr::set then cmp 0 sete then ::dec, and 0FFFFFFFEh
confidence: 6/10

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

`operator unspecified_bool_type` body is `if(!m_object) return 0; return &intrusive_ptr::c_ptr;`
so `if ( ptr )` (or `ptr &&`) emits `test raw,raw; je` AND a `mov reg, <&::c_ptr member-fn>; test reg,reg; je`
(the member-fn-pointer materialize is the safe-bool true value - the `mov reg, <c_ptr symbol>`
the delinker mislabels). When the raw pointer is ALREADY loaded in a register (e.g. just
stored by an `operator=`), the target spells the guard `if ( ptr.c_ptr() )` instead - a single
`test raw,raw; je`, NO safe-bool double-test, reusing the live register. Use `.c_ptr()` (not the
bare safe-bool) wherever the asm shows a lone `test;je` with no `mov reg,<c_ptr>` materialize.
Evidence: weapon_sound_effect::on_sound_event (rva 0x5bd340) - `if(instance.c_ptr())` 94.7% vs
`if(instance)` which adds the stray safe-bool member-fn load + test.
