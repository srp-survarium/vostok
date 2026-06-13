# `if ( ptr )` on an intrusive_ptr = `mov eax,[eax]; neg; sbb eax,eax; and eax,&c_ptr` (safe-bool mask)
tags: cpp:bool cpp:operator cpp:template | asm:neg asm:sbb asm:and | topic:codegen-idiom topic:fold-icf
symptoms: mov eax,[eax] / neg eax / sbb eax,eax / and eax,<...::c_ptr>
confidence: 8/10

`intrusive_ptr::operator unspecified_bool_type` returns `&intrusive_ptr::c_ptr` (a member-fn
pointer) when non-null, else 0. MSVC8 lowers the whole `ptr != 0 ? &c_ptr : 0` to the branchless
`neg/sbb/and` mask — the `and` operand is the address of some `c_ptr` instantiation (ICF-folded,
so the named type is noise). This is the body for `if ( m_model )`-style tests on a member/by-value
intrusive_ptr.

```cpp
if ( m_model )            // m_model is render::static_model_ptr
```
```asm
mov   eax, [eax]
neg   eax
sbb   eax, eax
and   eax, vostok::intrusive_ptr<...sound_emitter...>::c_ptr   ; &c_ptr, ICF-folded name
ret
```
Steerable: write `if ( ptr )`, NOT `if ( ptr.c_ptr() )` or `if ( ptr != NULL )` (those give a plain `cmp`/`sete` — see intrusive-ptr-compare-sete-leaf.md). Usually inlined at the test site. cite: object_solid_visual.cpp:54 `if( m_model )`; symbol intrusive_ptr<...inventory_item...>::operator inventory_item*.
