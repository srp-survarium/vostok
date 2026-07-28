# Event dispatch = build a stack functor holding the payload, then `m_subscribers.for_each( functor )` — never a raw `(*it)->on_x()` loop
tags: cpp:template cpp:member | asm:lea asm:call asm:push | topic:codegen-idiom topic:stlport
symptoms: lea slot; push; call intrusive_list<...>::for_each<void_predicate_ref<...callback_predicate<...>>>, subscribers_callback_predicate, ICF-folded ctor name
confidence: 9/10

Firing an event over a listener list is NEVER a hand-written `for(it) (*it)->on_x()`: source builds
a stack functor holding the event by const-ref, then calls `intrusive_list::for_each(pred)`. The fire
function is just: stack the functor (a `lea`/ctor), push its address, `call for_each<...>`. ICF folds
the functor ctor / finalize names — that naming is noise; the SHAPE (lea slot; push; call for_each) is
the match. (The npc variant first does a `_Rb_tree::_M_find` to pick the per-key list.)

```cpp
subscribers_callback_predicate<SubscriberType,EventParameterType> callback_predicate( parameter );
m_subscribers.for_each( callback_predicate );   // -> subscriber->m_subscription_callback( m_parameter )
```
```asm
lea   ecx, [ebp-0Ch] / push ecx       ; &payload wrapper
call  boost::_bi::list1<...>::list1<...>   ; ICF-fold name = the predicate ctor
push  eax                              ; &predicate
call  vostok::intrusive_list<...>::for_each<...void_predicate_ref<...callback_predicate<...>>>
ret   8
```
Steerable: write the functor + `for_each` exactly; expect ICF-misnamed ctor/finalize callees. cite: ai::npc_subscriptions_manager<damage_subscriber,sensed_hit_object>::on_event (rva 0x107f50) subscriptions_manager_inline.h:51.
