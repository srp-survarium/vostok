# `push 0; call [vptr+0]` = explicit VIRTUAL dtor call (flag 0: destroy, don't free)
tags: cpp:dtor cpp:virtual | asm:push asm:call asm:mov | topic:codegen-idiom
symptoms: push 0 mov edx [eax] call eax, 0xe destroy row vs 0x1b direct dtor + flag dance
confidence: 9/10
variants: explicit-dtor-call-not-delete.md, compiler-dtor-icf-this-setup.md, free-vs-delete-impl.md

An explicit dtor call on a POLYMORPHIC member dispatches through the vtable's scalar
deleting destructor with flag=0 (0xe bytes). Calling the dtor of a wrapper struct that
merely CONTAINS the polymorphic member resolves statically and LTCG inlines the deleting
stub (0x1b: direct `call T::~T` + `xor eax,eax; and eax,1; je; push this; call operator
delete`). The vptr load offset names WHICH subobject the original destroyed.

```cpp
infl.protector.~damage_protector( );   // virtual dispatch; NOT infl.~item_influence( );
```
```asm
push 0                       ; flag: destroy only
mov eax,[obj]; mov edx,[eax] ; vptr of the polymorphic subobject
mov ecx,[obj]; mov eax,[edx]; call eax
```
Pairs with VOSTOK_FREE_IMPL on the storage (free undestroyed-rest, medkit pattern).
Evidence: oxygen_tank::~oxygen_tank 61.61 -> 100.0 (virtual dtor call + DELETE->FREE).
