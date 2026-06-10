# Typed pointer subtraction = `sub; cdq; idiv sizeof`
tags: cpp:cast | asm:idiv asm:sub-esp | topic:codegen-idiom
symptoms: sub eax cdq mov ecx sizeof idiv ecx
confidence: 9/10

`p - q` for `T const*` operands: the compiler does the byte-diff / sizeof division itself
(`idiv` because pointer difference is signed). The divisor IS the element size - read it
to confirm which type's array you index. A surrounding `u32(...)` just narrows.

```cpp
u32 const matrix_index = u32( &bone - first_non_root_bone );   // skeleton_bone = 0x14
```
```asm
mov eax,[ebp+0Ch]; sub eax,[ebp+14h]
cdq
mov ecx, 14h; idiv ecx
```
Evidence: game_core/get_bone_matrix_in_object_space_impl.
