# boost::bind args: by-VALUE vs boost::cref (reference_wrapper) - read the bind's listN type
tags: cpp:template | asm:call asm:sub-esp | topic:codegen-idiom
symptoms: call boost::addressof per arg, reference_wrapper<float3 const> in assign_to/bind_t, value<float3> struct copies
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
Evidence: bullet_manager play_particle 67.87 -> 98.81, update_tracer 58.56 -> 89.38 (cref on the float3/resource args collapsed the bind region).
