# `strings::equal(a,b)` is a register-args helper call (NO pushes); 1st arg->EAX, 2nd->ECX
tags: cpp:string cpp:call | asm:call asm:mov asm:movzx asm:test | topic:codegen-idiom
symptoms: mov eax ??_C@ literal / mov ecx <other> / call vostok::strings::equal / movzx eax,al / test / jcc, NO push before the call
confidence: 9/10

The engine's universal string equality `strings::equal(left,right)` (~296 sites) is an inlined
`!strcmp` whose two `pcstr` args are passed in **registers, not pushed**: the 1st C++ arg goes
to EAX, the 2nd to ECX. So a string literal lands in EAX when written first, ECX when written
second; the bool result returns in AL and is immediately `movzx`/`test`/`jcc`. Stack pushes
around the compare mean a different overload — not this helper.

```cpp
if ( affect == affects_type_leg_damage && strings::equal( "left_leg", bodypart ) )
```
```asm
mov   ecx, [ebp+8]                              ; bodypart   (2nd arg -> ECX)
mov   eax, ??_C@_08OMGMOFDM@left_leg?$AA@       ; "left_leg" (1st arg -> EAX)
call  vostok::strings::equal
movzx eax, al
test  eax, eax
je    short $LN..
```
Steerable: the arg ORDER in source picks the EAX-vs-ECX assignment — swap operands if the literal is in the wrong register. cite: survarium::damage_model::on_broken_limb_affect (rva 0x6ff660) damage_model.cpp:317; mirror order at weapon_core::on_animation_ik_interval weapon_core.cpp:743-750.
