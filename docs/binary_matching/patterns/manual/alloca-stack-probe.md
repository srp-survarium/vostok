# `_alloca` / `ALLOCA` = `mov eax,size; call __alloca_probe_16; mov slot,esp` — no matching free
tags: cpp:local cpp:macro | asm:call asm:sub-esp asm:mov | topic:codegen-idiom topic:convention
symptoms: call __alloca_probe_16 (or __chkstk), the result pointer IS esp after the probe, lea esp,[ebp-N] on the way out, no free call
confidence: 9/10

`ALLOCA` is `_alloca` (see `cs/core/macro_extensions.h`). It is a compiler intrinsic, not a
call into the allocator: MSVC puts the byte count in `eax`, calls the stack-probe helper
`__alloca_probe_16` (touches each guard page, adjusts `esp` by the rounded size), and the
allocation IS the new `esp`. There is **no matching free** — the memory lives until the frame
is torn down (`mov esp,ebp`). Probe-verified:

```cpp
void f( int n ) { void* p = _alloca( n ); use( p ); }
```
```asm
?f@@YAXH@Z PROC
  83 ec 08  sub  esp, 8
  8b 45 08  mov  eax, DWORD PTR _n$[ebp]   ; size in EAX
  e8 ..     call __alloca_probe_16         ; stack-probe helper (NOT the engine allocator)
  89 65 f8  mov  DWORD PTR tv66[ebp], esp  ; the allocation == adjusted esp
  ... use(p) ...
  8d 65 f8  lea  esp, DWORD PTR [ebp-8]    ; reclaimed implicitly by frame teardown
```
Wall-ish: a `call __alloca_probe_16`/`__chkstk` with the count in `eax` and the result taken from `esp` = `_alloca`/`ALLOCA` — write the `ALLOCA` macro, never a heap alloc. Distinct from `VOSTOK_NEW`/`VOSTOK_MALLOC` (vostok-memory-macros.md), which `call` real helpers.

Multiple probes also expose source allocation order. Follow each adjusted `esp`
into its consumers, not just the saved stack-slot number: placement-constructor
field offsets and loop strides identify the array element type. In render's
`effect_manager::recompile_shaders_async`, the target allocates a pointer table,
then `user_data_variant` storage, then `creation_request` storage. Their byte
counts are `4*n`, `48*n`, and `16*n`; variant initialization at offsets `0x28`
and `0x2c` and the `0x30` versus `0x10` strides confirm which buffer is which.
Reversing the last two declarations changes actual stack movement and instruction
scheduling despite preserving the logical contents of all three arrays. Match
that raw-storage declaration order without moving placement construction or
introducing individual frees.

The count's lifetime is separate evidence. A count stored before the probes and
reloaded after the construction loop represents a cached value; spelling
`vector.size()` again at the query can instead recompute it. A direct size-test
early return followed by the cached-count declaration lets the optimizer hoist
the shared calculation into the guard while retaining that value for the query.
In this example the optimized PDB omits the cached local in that source shape,
even though the assembly preserves its value. Do not infer that the source had
no such local solely from its absence in the optimized PDB.
