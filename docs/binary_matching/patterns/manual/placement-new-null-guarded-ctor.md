# Placement `new (p) T` = a null-guarded ctor call, NO allocation; the placement operator is codeless
tags: cpp:new-delete cpp:ctor | asm:cmp asm:jcc asm:call | topic:codegen-idiom
symptoms: cmp slot,0 / je over the ctor, then mov ecx,slot / call ??0T, no operator new call, an extra $T temp + tv result slot
confidence: 9/10
variants: vostok-memory-macros.md, explicit-dtor-vs-delete-flag.md

Placement new constructs in already-owned memory: the placement `operator new(size_t,void*)`
just returns its pointer (no code), so `new (mem) T` is a **null guard** on the address plus
the ctor — `cmp mem,0 / je .skip / mov ecx,mem / call T::T`. No allocation, no `operator
new` call. It is the inner half of `VOSTOK_NEW` (vostok-memory-macros.md) and the partner of
an explicit destructor. Probe-verified:

```cpp
void placement( void* mem ) { new ( mem ) Obj; }
```
```asm
?placement@@YAXPAX@Z PROC
  8b 45 08  mov eax,[mem]
  89 45 fc  mov $T3211,eax
  83 7d fc 00  cmp $T3211,0
  74 0d     je  SHORT $LN3@placement        ; standard placement-new null guard
  8b 4d fc  mov ecx,$T3211
  e8 ..     call ??0Obj@@QAE@XZ             ; ctor only -- NO operator new
$LN3@placement:
```
Steerable: a `cmp,0/je` guarding a lone ctor call with no `operator new` = placement new; reproduce `new (p) T`. The matching `p->~T()` is the destroy-in-place partner (explicit-dtor-vs-delete-flag.md).
