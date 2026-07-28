# `VOSTOK_NEW`/`VOSTOK_DELETE` family: a `strip_pointer` call feeding a `new_helper<T>::call` / `delete_helper<T>`
tags: cpp:new-delete cpp:template cpp:macro | asm:call asm:push | topic:codegen-idiom topic:convention
symptoms: call strip_pointer then new_helper<T>::call then a null-guarded ctor; or strip_pointer + delete_helper<T>(alloc,ptr); allocator pushed as OFFSET g_*_allocator
confidence: 8/10
variants: placement-new-null-guarded-ctor.md, allocator-new-delete-helpers.md

The engine never writes raw `new`/`delete`; every module aliases `NEW(T)`/`DELETE(p)` to the
`VOSTOK_*_IMPL` family in `memory_macros.h` (release expansions). `VOSTOK_NEW_IMPL(alloc,T)`
= placement-new at `new_helper<T>::call( strip_pointer(alloc) )` — so the site is
`call strip_pointer` -> `call new_helper<T>::call` -> a **null-guarded ctor** (the placement
`operator new` emits no code, just forwards its pointer, see placement-new-null-guarded-ctor.md).
`VOSTOK_DELETE_IMPL(alloc,p)` = `delete_helper<T>( strip_pointer(alloc), p )` — the dtor +
free live INSIDE `delete_helper`, not at the site. Probe-verified (modelled expansion):

```cpp
// NEW( Widget )  ==  VOSTOK_NEW_IMPL( *g_allocator, Widget )
new ( vostok::memory::new_helper<Widget>::call( vostok::memory::strip_pointer(g_allocator) ) ) ( Widget );
// DELETE( w )    ==  VOSTOK_DELETE_IMPL( *g_allocator, w )
vostok::memory::delete_helper( vostok::memory::strip_pointer(g_allocator), w );
```
```asm
; NEW:                                              ; DELETE:
  push OFFSET ?g_allocator@@...                       mov eax,[w] / push eax
  call ?strip_pointer@memory@vostok@@...              push OFFSET ?g_allocator@@...
  add esp,4 / push eax                                call ?strip_pointer@memory@vostok@@...
  call ?call@?$new_helper@UWidget@@...                add esp,4 / push eax
  add esp,4 / mov $T,eax                              call ??$delete_helper@UWidget@@...
  cmp $T,0 / je .skip                                 add esp,8
  mov ecx,$T / call ??0Widget@@QAE@XZ   ; ctor      ; (dtor + free are inside delete_helper)
```
Steerable: a `strip_pointer` call whose result feeds `new_helper<T>::call` + a guarded ctor = `VOSTOK_NEW( T )`; feeding `delete_helper<T>` = `VOSTOK_DELETE`. `VOSTOK_MALLOC`/`VOSTOK_ALLOC` are the same shape into `malloc_helper`. Reproduce the macro, never a bare `new`/`delete`.
