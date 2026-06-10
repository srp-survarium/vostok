# Bone matrix index re-called per use: do NOT hoist `get_root_bones_count()`
tags: cpp:local cpp:member | asm:call asm:sub-esp | topic:structure-shape
symptoms: repeated mov eax [m_skeleton] call get_root_bones_count per matrix site, shl edx 6
confidence: 8/10
variants: accessor-call-result-temp.md, cxyz-reference-hoist.md

`matrices[params.<bone>_bone_index - get_skeleton().get_root_bones_count()]` is called
FRESH at every matrix site (the index helper inlines each time). A hoisted
`u32 root_count` local caches it once and DROPS the repeated load+call the target keeps
(objdiff shows + call/+ mov insertions per matrix).

```cpp
matrices[ params.up_leg_bone_index - get_skeleton( ).get_root_bones_count( ) ]   // per site
```
```asm
mov ecx,[this]; mov eax,[ecx]                 ; m_skeleton (private base member via inline accessor)
call vostok::animation::skeleton::get_root_bones_count   ; this in EAX (LTCG)
mov edx,[params]; mov edx,[edx+BONEOFF]; sub edx,eax
shl edx,6; add edx,[matrices_arg]             ; * sizeof(float4x4)=0x40
```
A `float4x4 const&` ref bound to `matrices[...] * hip_world_matrix` materializes the product and stores its address; a stored-then-reloaded-once ref local is a declared-but-unused source local present in BOTH binaries - keep it (C4189 matches the target).
Evidence: legs_ik_processor::get_foot_fixed_transform (84.16%, rva 0x6ebae0; un-hoisting was the single biggest fix, 81.55 -> 84.16).
