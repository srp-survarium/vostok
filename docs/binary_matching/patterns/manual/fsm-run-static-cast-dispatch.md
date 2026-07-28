# FSM consume idiom: `fsm.tick(); static_cast<Derived*>(fsm.current_state()); derived->virtual(dt)`
tags: cpp:virtual cpp:cast | asm:mov asm:call | topic:codegen-idiom
symptoms: mov edx,[fsm+10h] current_state inlined, redundant temp->temp->local mov chain (static_cast no-op), mov eax,[this]; mov edx,[eax+slot]; call edx
confidence: 8/10

After driving the generic FSM, a consumer reads `current_state()` (inlined `mov reg,[fsm+0x10]`),
`static_cast`s it to the concrete state base (a NO-OP that under /Od leaves a redundant
temp->temp->local `mov` chain — real statement rows, not waste to delete), then dispatches a
state-specific virtual whose slot is PAST the 4 `fsm_state` pures (e.g. +0x18).

```cpp
m_logic.tick();
breath_state* const current_state = static_cast< breath_state* >( m_logic.current_state() );
current_state->tick( dt );
m_target_multiplier = current_state->get_multiplier();
```
```asm
mov   edx, [ecx+10h]       ; current_state() inlined
mov   [ebp-18h], edx       ; static_cast no-op temp chain
mov   eax, [edx]           ; current_state->tick(dt)
mov   edx, [eax+18h]       ;   slot +0x18 (after the 4 fsm_state pures)
call  edx
movss xmm0, [eax+24h]      ; get_multiplier() inlined: load m_multiplier
```
Steerable: keep the `static_cast` (its no-op temps are real rows); the +0x18 slot is fixed by the derived vtable. cite: breath_vibration_calculator::tick breath_vibration_calculator.cpp:97-101.
