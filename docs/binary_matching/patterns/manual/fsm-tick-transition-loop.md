# `fsm::tick()` = guard, walk the current state's transition list, dispatch finalize/initialize/execute through 4 vtable slots
tags: cpp:virtual cpp:while | asm:call asm:mov | topic:codegen-idiom topic:structure-shape
symptoms: mov edx,[fsm+10h] current_state, [edx+24h] next walk, boost::function0<bool>::operator(), call [edx+0Ch] finalize/[edx+4] initialize/[edx+8] execute
confidence: 9/10

The engine's one generic FSM driver: guard on `current_state->is_ready_for_transition()` (vtable
slot +0x10); walk the intrusive transition list (`next` at +0x24) evaluating each
`boost::function0<bool>` predicate; on the first true predicate call the OLD state's `finalize`
(slot +0x0C), store `m_current_state = transition->target_state` (+0x20 -> fsm+0x10), call the NEW
state's `initialize` (+0x04) then `execute` (+0x08). Those four slot calls in that exact order are
the signature.

```cpp
if ( !m_current_state->is_ready_for_transition() ) return;
for ( fsm_state_transition* i = m_current_state->transitions.front(); i; i = i->next )
    if ( i->predicate() ) { m_current_state->finalize(); m_current_state = i->target_state;
                            m_current_state->initialize(); break; }
m_current_state->execute();
```
```asm
mov   eax, [edx+10h] / call eax        ; is_ready_for_transition
mov   eax, [edx+24h]                   ; i = i->next
call  boost::function0<bool>::operator()   ; predicate()
mov   eax, [edx+0Ch]                   ; old->finalize
mov   eax, [edx+20h] / mov [ecx+10h],eax   ; m_current_state = target
mov   eax, [edx+4]                     ; new->initialize
mov   eax, [edx+8]                     ; current_state->execute
```
Steerable: the 4-slot order (+0xC,+0x4,+0x8 around the +0x20 store) is fixed by `fsm_state`'s vtable. cite: vostok::ai::fsm::tick fsm.cpp:56-75.
