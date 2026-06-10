# NEW_ARRAY allocations are freed with VOSTOK_DELETE_ARRAY_IMPL (delete_array_helper_impl)
tags: cpp:new-delete cpp:macro | asm:push asm:lea asm:call | topic:allocator
symptoms: call delete_array_helper_impl, lea edx [&ptr] push - takes the pointer's ADDRESS, dealloc row +0x14
confidence: 8/10
variants: free-vs-delete-impl.md, allocator-wrapper-inline.md

A dealloc row +0x14 in base where the target slice calls
`delete_array_helper_impl<alloc,T,call_destructor_predicate>` taking the ADDRESS of the
pointer (it NULLs it): the allocation used VOSTOK_NEW_ARRAY_IMPL, so the original freed
with VOSTOK_DELETE_ARRAY_IMPL, not VOSTOK_DELETE_IMPL.

```cpp
VOSTOK_DELETE_ARRAY_IMPL( allocator, m_items );
```
```asm
push &pred_bool; lea edx,[&ptr]; push edx; push allocator
call delete_array_helper_impl<alloc,T,call_destructor_predicate>; add esp,0Ch
```
Evidence: items_dictionary_cook::on_subresources_loaded 93.95 -> 97.72 (row +0x14 -> +0x4, residual strip_pointer inline-vs-call).
