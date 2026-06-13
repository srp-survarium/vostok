# Contiguous-iterator loop under /Ox: `++it` = `add reg, sizeof(T)`; `end()-begin()` count = reciprocal `imul`
tags: cpp:iterator cpp:for | asm:add asm:imul | topic:codegen-idiom topic:ltcg
symptoms: add esi,<sizeof element>, imul <magic>; sar/shr for the element count, iterator held in a register, cmp esi,edi je
confidence: 9/10

For a contiguous container (`vector`/`fixed_vector`) the iterator IS a raw `T*`, so an optimized
(/Ox) loop keeps it in a **register** and advances with `add reg, sizeof(T)` — no iterator object,
no helper call (contrast the /Od tree/list forms). When the code needs `end()-begin()` as a count
it appears as the signed-division-by-constant reciprocal sequence (`imul <magic>; sar`) — i.e.
divide the byte span by `sizeof(T)`.

```cpp
planes_type::const_iterator const end_it = m_planes.end();           // fixed_vector<aabb_plane,...>
for ( planes_type::const_iterator it = m_planes.begin(); it != end_it; ++it )
```
```asm
mov   esi, ebx        ; it = begin (raw ptr)
cmp   esi, edi        ; it != end_it
...
add   esi, 14h        ; ++it  (sizeof aabb_plane == 0x14)
cmp   esi, edi
jne   short ..
sub   edi, ebx        ; end - begin
mov   eax, 66666667h  ; / 0x14 via reciprocal -> element count
imul  edi
sar   edx, 3
```
Steerable: the `add reg,<sizeof T>` stride is exact — a wrong element type yields the wrong immediate. This is the /Ox world (tu-optimization-level-od-vs-ox.md); a /Od vector loop instead keeps the iterator in a slot. cite: vostok::math::convex::test_inexact math_convex.cpp:264.
