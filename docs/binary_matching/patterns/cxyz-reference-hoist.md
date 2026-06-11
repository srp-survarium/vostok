# Hoisting a temporary's `.c.xyz()` binds a float3 REFERENCE, not a full-matrix local
tags: cpp:local cpp:float cpp:const | asm:add asm:mov | topic:structure-shape
symptoms: add eax offset then accessor call then mov [slot] eax pointer store, no full struct copy
confidence: 8/10
variants: xyz-fold-misname.md, byvalue-temp-named-local-hoist.md

When the target keeps `get_X().c.xyz()` of a returned temporary as its OWN statement, the
materialized local is the `.c.xyz()` RESULT bound by reference - the asm stores a POINTER
to the sub-object (call returns &temp in eax; `add eax,<offset>`; accessor call;
`mov [slot],eax`), never copies the struct. A full-struct hoist allocates an extra NRV
slot the target never uses and REGRESSES.

```cpp
float3 const& p = get_bone_matrix_in_object_space( ... ).c.xyz( );
// NOT: float4x4 m = get_bone_matrix_in_object_space( ... ); float3 p = m.c.xyz();
```
Evidence: process_hand - full-float4x4 hoist 89.69 -> 89.54; the reference hoist 89.54 -> 90.37, restoring 37==37 statements.
