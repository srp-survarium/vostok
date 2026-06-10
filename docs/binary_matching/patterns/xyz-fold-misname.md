# `.i/.j/.k/.c.xyz()` folds: misnamed `finalize_impl` call, row offset in the `add`
tags: cpp:member cpp:float | asm:add asm:call asm:lea | topic:fold-icf
symptoms: add eax 30h call finalize_impl, lea eax [mat] call finalize_impl, add 10h/20h
confidence: 9/10
variants: fold-misname-empty-fn.md, cxyz-reference-hoist.md

The trivial `float4_pod::xyz()` (returns float3 const& = first 12 bytes) ICF-folds and
the delinker misnames the call (commonly `fixed_size_allocator<...>::finalize_impl`).
The row is identified by the `add`: +0 = `.i`, +0x10 = `.j`, +0x20 = `.k`, +0x30 = `.c`
(float4x4 = 0x40, position row .c @0x30). NOT an allocator call and NOT an ASSERT.

```cpp
matrix.c.xyz( )       // a following call float3_pod::length = .xyz().length()
```
```asm
lea eax,[mat]; call finalize_impl     ; mat.i.xyz()
add eax,30h;  call finalize_impl      ; mat.c.xyz() - returned eax then used as float3 const&
```
Evidence: legs_ik_processor.cpp uses .c.xyz() throughout; legs_ik_drawer::draw_leg; the get_root_bones_count entry's add eax,0x30 sites.
