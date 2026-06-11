# Trivial COMDAT template method: target `call`s it, base inlines (vectora::size)
tags: cpp:template cpp:member cpp:inline | asm:call asm:idiv | topic:inline-vs-call
symptoms: call vectora<T>::size vs inlined [+4]-[+0] cdq idiv, imul edx 0x38 operator[]
confidence: 6/10
variants: inline-vs-call-trivial-getter.md, fixed-string-ctor-inline.md, float3-dot-operator-inline.md

For `container.size()` / `container[i]` on a `vostok::vectora<T>` (thin std::vector
wrapper) the TARGET emits a real `call` while the BASE inlines (`size()` becomes
`[+4]-[+0]; cdq; idiv sizeof` and `operator[]` becomes `imul edx,sizeof; add edx,base`).
Under /Od+/GL whether a trivial inline COMDAT template method is emitted-and-called or
inlined is a whole-program decision - not forceable from the caller. Mark PARTIAL.

```asm
; target: call vostok::vectora<T>::size          ; base: mov; sub; cdq; mov ecx,38h; idiv ecx
```
TELL it is LTCG: pdb_rich_query finds the wrapper out-of-line in TARGET's index but the base query returns nothing (inlined everywhere). Corollary: a wrong member with a shorter inlined form can score HIGHER fuzzy % - match the offset the target reads, not the metric.
Evidence: game_core/scheduler::on_frame(u32,u32) (target rva 0x77de80, 46.39% PARTIAL; m_active_objects @0x10, not m_inactive @0x00).
