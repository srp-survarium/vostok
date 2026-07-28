# cdecl vs thiscall at the call site: caller `add esp,N` + `YA`/`SA` vs `mov ecx,this` + no cleanup
tags: cpp:call cpp:member cpp:static | asm:push asm:call asm:mov | topic:convention topic:mangling
symptoms: add esp,N after the call (cdecl caller-cleanup), mov ecx,<this> right before a member call, no add esp for thiscall, YA/SA vs QAE/AAE mangling
confidence: 9/10
variants: virtual-call-vtable-dispatch.md, param-pointer-const-mangling.md

Args always push **right-to-left**. A free function or `static` member is **cdecl**: after
the `call`, the caller pops with `add esp,N`; mangling is `YA` (free) / `SA` (static) — a
static-member call is byte-identical to a free call. A non-static member is **thiscall**:
args push, then `mov ecx,<this>` immediately before the `call`, and **no `add esp`** (callee
cleans via `ret N`); mangling carries the access char + `AE` (`QAE`/`IAE`/`AAE`...).
Probe-verified:

```cpp
void call_free( )      { g = freefn( 1, 2 ); }
void call_mem ( C* c ) { g = c->mem( 1, 2 ); }
void call_stat( )      { g = C::stat( 1, 2 ); }
```
```asm
?call_free@@YAXXZ PROC
  6a 02 / 6a 01   push 2 / push 1
  e8 .. call ?freefn@@YAHHH@Z              ; YA = free cdecl
  83 c4 08        add  esp, 8              ; <-- caller cleans (cdecl)

?call_mem@@YAXPAUC@@@Z PROC
  6a 02 / 6a 01   push 2 / push 1
  8b 4d 08        mov  ecx, DWORD PTR _c$[ebp]   ; <-- this in ECX, after the arg pushes
  e8 .. call ?mem@C@@QAEHHH@Z              ; QAE = public thiscall member
  ;               (NO add esp -> callee cleaned via ret 8)

?call_stat@@YAXXZ PROC
  6a 02 / 6a 01   push 2 / push 1
  e8 .. call ?stat@C@@SAHHH@Z              ; SA = static, cdecl
  83 c4 08        add  esp, 8              ; same shape as a free call
```
Steerable: `add esp,N` after a `call` = cdecl callee (free/static); a `mov ecx,*` feeding the call with no cleanup = thiscall member. Top-level const on a by-value pointer param flips the mangle and gates pairing — param-pointer-const-mangling.md.
