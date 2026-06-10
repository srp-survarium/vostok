# VOSTOK_FREE_IMPL vs VOSTOK_DELETE_IMPL: read the dealloc statement shape
tags: cpp:new-delete cpp:macro | asm:call asm:cmp | topic:allocator
symptoms: identical bytes per member regardless of triviality, if(p){free;p=0} shape, no element dtor
confidence: 9/10
variants: delete-array-impl.md, allocator-new-delete-helpers.md, virtual-dtor-explicit-call.md

A dealloc row that SIZE-diffs oddly per member - base BIGGER on trivially-destructible
members (+0xb: predicate-bool store + doubled slot copy from inlined delete_helper_impl)
yet SMALLER on a non-trivial one (base out-of-line `call delete_helper<alloc,T>`, target
shows NO dtor) - means the original used FREE, not DELETE: the free_helper shape is
exactly `if (p) { call free_impl; p = 0; }` with NO element destructor, identical bytes
for every member.

```cpp
VOSTOK_FREE_IMPL( allocator, m_damage_protection );   // raw free, symmetric with VOSTOK_MALLOC_IMPL
```
Evidence: medkit::~medkit 67.06 -> 100 (three FREE rows 0x40 each; the boost::function-bearing damage_protection freed undestroyed).
