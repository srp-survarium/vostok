# Explicit `p->~T()` vs `delete p` (non-virtual): same skeleton, the flag immediate (0 vs 1) decides the free
tags: cpp:dtor cpp:new-delete | asm:call asm:and asm:jcc | topic:codegen-idiom
symptoms: call ??1T then <flag> & 1 / je over operator delete; flag built by xor eax,eax (0, explicit dtor) or mov reg,1 (1, delete)
confidence: 8/10
variants: placement-new-null-guarded-ctor.md, delete-polymorphic-scalar-deleting-dtor.md

For a type with a **non-virtual** destructor, both an explicit `p->~T()` and `delete p`
compile to the SAME skeleton: `call ??1T` (the dtor, direct thiscall) followed by a
`<flag> & 1 / je .skip` guard over a `push p / call ??3@YAXPAX@Z` (operator delete). The
ONLY difference is the immediate fed into the flag: `xor eax,eax` (= 0) for the explicit
destructor — the free is guarded out — vs `mov reg,1` for `delete p`. Probe-verified:

```cpp
void manual_dtor( Obj*   p ) { p->~Obj(); }   // flag 0 -> dtor only, NO free
void del_plain  ( Plain* p ) { delete p; }    // flag 1 -> dtor + operator delete
```
```asm
?manual_dtor@@YAXPAUObj@@@Z PROC          ?del_plain@@... (after the null check)
  mov ecx,[p]                               mov ecx,$T
  e8 .. call ??1Obj@@QAE@XZ    ; ~Obj        e8 .. call ??1Plain@@QAE@XZ   ; ~Plain
  33 c0 xor eax,eax            ; flag = 0    ba 01.. mov edx,1             ; flag = 1
  83 e0 01 and eax,1                         83 e2 01 and edx,1
  74 0c je  $LN1 (skip free)                 74 0c je  $LN6 (skip)
  ...push p / call ??3 (DEAD) ...            ...push p / call ??3@YAXPAX@Z (RUNS)...
```
Steerable: a `??1T` dtor call followed by an always-false (`xor reg,reg / and reg,1 / je`) `operator delete` branch = explicit `p->~T()` (destroy in place); the same shape with the flag built as `1` = `delete p`. For a virtual dtor it is neither — see delete-polymorphic-scalar-deleting-dtor.md.
