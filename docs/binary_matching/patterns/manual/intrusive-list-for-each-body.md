# `intrusive_list::for_each` body = empty-guard, mutex_raii, then a save-next-BEFORE-pred-call loop
tags: cpp:for cpp:template cpp:iterator | asm:cmp asm:je asm:call | topic:codegen-idiom topic:stlport
symptoms: cmp [list+24h],0 sete movzx, mutex_raii_impl ctor/dtor, mov next=current->m_next THEN call operator(), locals raii/current/next, ret 4
confidence: 9/10

The inlined `for_each(Predicate const&)` listener-fire loop, order is load-bearing: (1) `if(empty())`
via `cmp [m+24h],0; sete cl; movzx; test; je`; (2) construct `ThreadingPolicy::mutex_raii`; (3) loop
reads `current=m_first`, and INSIDE the body loads `next = current->m_next` **before** calling the
predicate (so a listener may unsubscribe/delete itself mid-fire); (4) `void_predicate_ref`
double-derefs (`mov edx,[ebp+8]; mov eax,[edx]`) to reach the real functor. Locals are exactly
`raii`, `current`, `next`.

```cpp
typename ThreadingPolicy::mutex_raii raii( *this );
for ( PointerType current = m_first; current; ) {
    PointerType const next = get_next_of_object( current );
    pred( &*current );          // fire this listener
    current = next;
}
```
```asm
cmp   dword ptr [eax+24h], 0   ; empty()
sete  cl
call  ...mutex_raii_impl<mutex>::mutex_raii_impl
cmp   dword ptr [ebp-0Ch], 0   ; current != 0 / je .end
mov   ecx, [eax+20h]           ; next = current->m_next  (SAVED FIRST)
mov   [ebp-10h], ecx
call  boost::function1<...>::operator()   ; pred(&*current)
mov   [ebp-0Ch], eax           ; current = next
call  ...mutex_raii_impl<mutex>::~mutex_raii_impl
ret   4
```
Steerable: the whole body is header-generated — match by writing the `for_each` call; do NOT hand-roll the loop. cite: intrusive_list<damage_subscriber,...>::for_each<...> (rva 0x108410) intrusive_list_inline.h:320.
