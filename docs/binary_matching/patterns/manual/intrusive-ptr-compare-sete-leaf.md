# intrusive_ptr `a == b` / `a == raw` = `mov [this]; xor eax,eax; cmp; sete al` leaf
tags: cpp:operator cpp:bool cpp:template | asm:cmp asm:sete asm:xor | topic:codegen-idiom
symptoms: mov ecx,[eax] / xor eax,eax / cmp ecx,[edx] / sete al / ret (self) or ret 4 (raw rhs)
confidence: 9/10

All six relational operators forward to `c_ptr() <cmp> object.c_ptr()` (or a raw pointer). They
compile to a 3-5 instruction leaf: load `m_object`, zero eax, compare, set-condition al. The
self-vs-self form takes both ptrs in registers (`ret`); the raw-pointer form reads the rhs off
the stack (`ret 4`). This is what `ptr != NULL` and `a == b` between two resource_ptrs lower to
(contrast the safe-bool `if(ptr)` mask, intrusive-ptr-safe-bool-mask.md).

```cpp
if ( iitem != NULL )      // iitem is inventory_item_ptr
```
```asm
; operator==(self const&)        ; operator==(object_type* const)
mov  ecx, [eax]                    mov  edx, [ecx]
xor  eax, eax                      xor  eax, eax
cmp  ecx, [edx]                    cmp  edx, [esp+4]
sete al                            sete al
ret                                ret  4
```
Leaf, usually inlined; near-100% when it survives. cite: inventory.cpp:108 `if ( iitem != NULL )`; symbols intrusive_ptr<...managed_resource...>::operator==(const&) and ...inventory_item...::operator==(... const).
