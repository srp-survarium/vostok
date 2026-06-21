# Cast a concrete global allocator to `base_allocator&` to keep malloc/free VIRTUAL

cpp:cast cpp:new-delete cpp:member | asm:call asm:mov | topic:inline-vs-call topic:codegen-idiom topic:convention

## Symptom

A `VOSTOK_NEW_IMPL`/`VOSTOK_DELETE_IMPL`/`VOSTOK_FREE_IMPL` site where the TARGET
loads the allocator's vtable and dispatches the allocation through it:

```
mov   eax, [?g_mt_allocator@memory@vostok@@3Vpthreads3_allocator@12@A]
mov   edx, [eax+10h]      ; vtable slot 0x10 = malloc_impl  (0x18 = free for delete)
push  60h
mov   ecx, ?g_mt_allocator@memory@vostok@@3Vpthreads3_allocator@12@A
call  edx
```

while our `/Od` base instead emits a DEVIRTUALIZED direct call (`call pt3malloc` /
`jmp pt3free`), often with an extra `push esi`/`mov esi,eax`/`pop esi` spill around
the following ctor/dtor call. The structure (statement count) matches; only the
malloc/free instruction diverges.

## Cause

`VOSTOK_NEW_IMPL( a, T )` expands to `new_helper<T>::call( strip_pointer(a) )`, and
`strip_pointer<A>(A&)` returns `A&` preserving the **static** type. The helper is a
template on the allocator type `A`:

- Pass the CONCRETE global (`memory::g_mt_allocator`, a `pthreads3_allocator`, or
  `*survarium::g_allocator`, a `doug_lea_allocator`) and MSVC sees the final override
  of `malloc_impl`/`free` and devirtualizes it to the concrete `pt3malloc`/`pt3free`.
- The TARGET keeps the virtual `[vtable+0x10]`/`[vtable+0x18]` dispatch, i.e. its
  source passed the allocator through the abstract `base_allocator&` interface (or
  the optimizer never had the concrete type, e.g. an LTCG const-folded `base_allocator*`
  parameter resolved to the global symbol but still typed as the base).

## Fix

Bind the global through the base reference so `A = base_allocator` and the call
stays virtual, while the loaded SYMBOL is still the concrete global (matches the
target's `[g_mt_allocator]` address):

```cpp
VOSTOK_NEW_IMPL   ( static_cast<memory::base_allocator&>( memory::g_mt_allocator ), T )( ... );
VOSTOK_DELETE_IMPL( static_cast<memory::base_allocator&>( memory::g_mt_allocator ), p );
```

The `static_cast` to a single-inheritance base reference is a no-op (same address),
so it adds NO codegen of its own. In vostok's physics entry point this took
`create_world_bt` 20%->76% and `destroy_world` 65%->100% in one rebuild.

## Notes

- If the target loads `g_mt_allocator` directly where the source has a
  `base_allocator*` PARAMETER, the symbol is an LTCG const-fold of the sole caller's
  argument; writing the global directly (with this cast) reproduces the target bytes
  better than the parameter, which under /Od also drags in a `strip_pointer` deref
  call (`strings::detail::tuples::helper<2>::string`) the target lacks.
- Any residual after the cast (e.g. an `esi` spill around the following ctor/dtor)
  is the CALLEE's optimized-COMDAT convention, a separate cross-unit wall.
