# Out-of-line strip_pointer call on the DELETE path = allocator passed as POINTER; the original dereferenced it
tags: cpp:new-delete cpp:macro | asm:call asm:mov | topic:allocator topic:inline-vs-call
symptoms: allocator load then call strip_pointer fold then push eax on DELETE only, target pushes the load directly
confidence: 8/10
variants: strip-pointer-delete-resource.md, allocator-wrapper-inline.md

`VOSTOK_DELETE_IMPL( g_allocator, p )` with a `doug_lea_allocator*` argument routes
through the real `strip_pointer(T*)` overload - an out-of-line call under /Od. The
original passed the DEREFERENCED allocator: `strip_pointer(T&)` is the identity overload
and compiles to nothing. The NEW path (`VOSTOK_NEW_IMPL`) keeps a folded helper call
after the allocator load in BOTH binaries - that one is correct; only the DELETE-side
strip call is the tell.

```cpp
VOSTOK_DELETE_IMPL ( *g_allocator, p );   // deref -> identity strip_pointer, no call
```
```asm
; base (pointer arg): mov eax,[g_allocator]; call <strip_pointer fold>; push eax
; target (deref'd):   mov ecx,[g_allocator]; push ecx
```
`DELETE(p)` from network_memory.h cannot be used in asio TUs (WinSDK headers eat the macro -> C3861).
Evidence: destroy_client / destroy_http_client 90.83% -> up.
