# `class X : public A, public B` ctor = `add ecx, sizeof(A)` before `B::B` + one vftable store per polymorphic base
tags: cpp:ctor cpp:inherit cpp:virtual | asm:add asm:call asm:mov | topic:codegen-idiom topic:structure-shape
symptoms: call A::A (ecx=this), add ecx,sizeof(A), call B::B, mov [this],??_7..@A, mov [this+sizeof(A)],??_7..@B; NO [thunk]/adjustor symbols
confidence: 8/10

`class X : public A, public B` lays B at `this + sizeof(A)`. The ctor calls `A::A` with
`ecx=this`, then `add ecx, sizeof(A); call B::B`, and stores ONE vftable pointer per polymorphic
base at offsets 0 and sizeof(A). An EMPTY second base (e.g. `boost::noncopyable`) adds no offset.
This engine has **no adjustor/vtordisp thunks** (verified: zero `[thunk]:`/`vtordisp` symbols in
the target) — the this-adjust is always the inline `add` at the base-ctor/cast site.

```cpp
class weapon_core_base_state : public ai::fsm_state, public resources::unmanaged_resource, public boost::noncopyable
```
```asm
call  vostok::ai::fsm_state::fsm_state              ; ecx=this, base A @ +0
add   ecx, 18h                                      ; this + sizeof(fsm_state)
call  vostok::resources::unmanaged_resource::unmanaged_resource
mov   dword ptr [eax],     ??_7..@fsm_state@..       ; vftable A @ +0
mov   dword ptr [ecx+18h], ??_7..@unmanaged_resource@..  ; vftable B @ +0x18
```
Steerable: the `+sizeof(A)` adjust and dual vftable stores follow from base order/sizes; an upcast `X* -> B*` is likewise a plain `add`. cite: weapon_core_base_state ctor weapon_core_base_state.h:27.
