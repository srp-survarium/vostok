# An empty-bodied derived state ctor: the vtable store is sunk into the `new`-expression, no standalone ctor symbol
tags: cpp:ctor cpp:virtual cpp:new-delete | asm:call asm:mov | topic:codegen-idiom topic:inline-vs-call
symptoms: call operator new / null-guard / call BaseState::BaseState / mov dword ptr [obj], ??_7DerivedState@@6B@ / push; call add_state
confidence: 9/10

When `VOSTOK_NEW_IMPL(alloc, DerivedState)(args)` builds an FSM state whose derived ctor body is
empty (init-list only), the only thing the derived ctor contributes is the vtable store — and LTCG
SINKS it into the new-expression. So the new-site shows: `call Base::Base` (the base-state ctor,
with the derived args), then `mov [new_obj], ??_7DerivedState@@6B@`, then the object is pushed into
`add_state`. The derived ctor has NO standalone symbol (contrast state-ctor-vtable-stores.md, where
the store sits inside an out-of-line ctor).

```cpp
jump_logic_base_state* inactive = VOSTOK_NEW_IMPL( g_allocator, jump_logic_state_inactive )( *this );
m_logic->add_state( inactive );
```
```asm
call  operator new
cmp   dword ptr [ebp-10h], 0                     ; null-guard
call  survarium::jump_logic_base_state::jump_logic_base_state   ; BASE ctor
mov   dword ptr [edx], ??_7jump_logic_state_inactive@survarium@@6B@   ; DERIVED vtable @ new-site
call  vostok::ai::fsm::add_state
```
Steerable: do NOT write the vtable store in the ctor — an empty derived ctor + the new-expression reproduce it. cite: jump_logic::initialize_logic jump_logic.cpp:102 / jump_logic_state_start.h:18-24.
