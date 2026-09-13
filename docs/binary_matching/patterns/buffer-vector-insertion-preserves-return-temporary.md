<!-- SPDX-License-Identifier: GPL-3.0-or-later -->
# A returned value copied after the null guard can be buffer_vector::push_back
tags: cpp:template cpp:new-delete cpp:call | asm:call asm:test asm:mov | topic:structure-shape topic:source-fidelity
symptoms: vertex sret into stack temporary before test destination; movq plus dword copy; direct placement new elides copy; extra caller const-reference statement
confidence: 9/10
variants: manual/placement-new-null-guarded-ctor.md

When a value-returning call precedes a null-guarded placement copy and destination
advance in one statement, check the existing container insertion helper.
`buffer_vector::push_back(const_reference)` evaluates the argument before its
inlined `construct` guard; direct placement construction from the returned value
can instead pass the destination as the return buffer and eliminate the copy.

```cpp
buffer_vector< float3 > vertices( ALLOCA( count * sizeof( float3 ) ), count );
vertices.push_back( box.vertex( id ) );
```
```asm
lea  eax, [temporary]
call aabb::vertex
test edi, edi
je   advance
movq xmm0, [eax]
movq [edi], xmm0
mov  eax, [eax+8]
mov  [edi+8], eax
advance:
add  edi, 0Ch
```

Measured in `portal_sector_system::make_frustum_images`, target RVA `0x5eb340`:
existing-helper insertion gives 100%, STRUCTURE MATCH (7 statements, 281 bytes);
direct placement construction gave 87.3563% and removed the target copy.
