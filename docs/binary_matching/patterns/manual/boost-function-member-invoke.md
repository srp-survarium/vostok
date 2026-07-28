# Invoking a `boost::function` MEMBER (`m_cb();`) = `mov ecx,this; add ecx,<member_off>; call functionN::operator()`
tags: cpp:call cpp:template | asm:add asm:mov asm:call | topic:codegen-idiom
symptoms: add ecx,N then call boost::function0<void>::operator(), no functor copy at the call site, member offset picks the callback
confidence: 9/10

Calling a `boost::function` data member is NOT inlined: load `this`, `add ecx,<byte offset of the
function slot>` so ecx points at the embedded function object, and `call functionN::operator()`
(thiscall — the function object IS the implicit `this`). The `add ecx,N` immediate is exactly the
member offset, so two sibling callbacks in one class differ only by that immediate. Args push first.

```cpp
m_initialize_callback( );   // +0x28
m_finalize_callback( );     // sibling slot +0x48
```
```asm
mov   ecx, [ebp-130h]                 ; this
add   ecx, 28h                        ; -> &m_initialize_callback (48h for finalize)
call  boost::function0<void>::operator()
```
Steerable: source is just `m_cb();`; the offset follows from struct layout. cite: player_logic_sprint_state::initialize / ::finalize player_logic_sprint_state.cpp:109,115.
