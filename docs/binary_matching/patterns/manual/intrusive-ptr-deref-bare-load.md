# intrusive_ptr `operator->`/`operator*` are a bare `mov eax,[eax]; ret` (ASSERT compiled out, ICF-folded)
tags: cpp:operator cpp:template | asm:mov | topic:codegen-idiom topic:fold-icf
symptoms: mov eax,[eax]; ret (3-byte leaf, no ASSERT/empty_stub call, every instantiation ICF-folds to one COMDAT)
confidence: 9/10

`operator->`/`operator*` are `ASSERT(m_object); return m_object;`. In MASTER_GOLD the ASSERT
drops entirely (here not even an `empty_stub` — it is a pure accessor), leaving `mov eax,[eax];
ret`. Every instantiation is byte-identical, so they ICF-fold to one COMDAT and the PDB name
attached to a given `ptr->` site is arbitrary. At a real `m_obj->member` site this inlines to a
single load.

```cpp
m_model->m_render_model   // m_model is render::static_model_ptr
```
```asm
mov   eax, [eax]
ret
```
Not a steering lever (3 bytes, folded) — note it so a 0%/ICF-named `operator->` symbol isn't mistaken for a structure bug. cite: object_solid_visual.cpp:56 `m_model->m_render_model`; symbols intrusive_ptr<...inventory_item...>::operator-> and ...booby_trap_core...::operator*.
