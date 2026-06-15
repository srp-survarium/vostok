# boost::bind args: by-VALUE vs boost::cref (reference_wrapper) - read the bind's listN type
tags: cpp:template | asm:call asm:sub-esp | topic:codegen-idiom
symptoms: call boost::addressof per arg, reference_wrapper<float3 const> in assign_to/bind_t, value<float3> struct copies, boost::ref( *ptr )
confidence: 9/10
variants: boost-asio-placeholders.md, boost-bind-icf-sibling-fold.md

STEERABLE: when the target bind's `listN<...>` names `reference_wrapper<T const>` for
large value args (float3, resource_ptr) while the base packs `value<T>` (inline struct
copy or out-of-line `_bi::value<T>::value` per arg), wrap those args in `boost::cref(...)`.
Scalars often stay by value (`value<unsigned short>`, `value<float>`) - read the target's
listN to decide WHICH args are wrappers.

```cpp
boost::bind( &f, this, boost::cref( functor->position ), boost::cref( functor->direction ), speed )
```
```asm
call boost::addressof<...>      ; == boost::cref, one per wrapped arg
```
Same tell for `boost::ref( *ptr )` at a ctor/bind arg: a plain `*ptr` to a reference
parameter pushes the pointer with NO call, while `push <ptr-value>; call
boost::addressof<...>; add esp,4; mov [slot],eax` (often misnamed under an unrelated
bind_t) means the lvalue was wrapped - ref() builds the reference_wrapper via the real
addressof() call and the wrapper's `operator T&()` materializes the extra slot.
Disambiguates `*m_client` vs `boost::ref( *m_client )`.
Evidence: bullet_manager play_particle 67.87 -> 98.81, update_tracer 58.56 -> 89.38 (cref on the float3/resource args collapsed the bind region); boost::ref(*m_client) confirmed across network/match_client::{disconnect,enqueue,send_queued_packets,connect,on_packet_received} (targets 0x74ca80 +0x7a, 0x74cf90 +0xd1, 0x74d740 +0x7f/+0xee, 0x74d8e0 +0x12c).
