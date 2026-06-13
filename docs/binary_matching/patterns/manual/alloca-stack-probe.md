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
