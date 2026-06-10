# A write/read at `[ebp+arg]+0xNN` is a MEMBER of an argument struct, not a local
tags: cpp:member | asm:mov asm:add | topic:structure-shape
symptoms: mov eax [ebp+8] add eax 0x1C store there, re-read by a later call
confidence: 6/10

`[ebp+8]` is the by-ref struct ARG; +0x1C is a member offset - the statement is
`arg.member = ...`, not a local. Mis-decoding it as a matrix-row write over-produces an
lvalue xyz-fold.

```cpp
params.rotation_axis = normalize( a ^ b );        // leg_params +0x1C, public float3
create_rotation( params.rotation_axis, angle );   // re-read
```
Evidence: game_core/legs_ik_processor::process_leg (originally mis-decoded as up_leg_obj_matrix.i.xyz() write).
