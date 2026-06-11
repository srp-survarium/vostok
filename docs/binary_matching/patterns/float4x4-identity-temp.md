# `lea &slot; call <misnamed ctor>; mov ecx,eax; call identity` = `float4x4( ).identity( )`
tags: cpp:ctor cpp:float | asm:lea asm:call | topic:fold-icf topic:codegen-idiom
symptoms: call stlp_std::allocator<char>::allocator<char> before float4x4::identity
confidence: 6/10
variants: fold-misname-empty-fn.md, xyz-fold-misname.md

A default-constructed float4x4 temporary with `identity()` called on it (identity returns
float4x4&, eax = this). The first call is the trivial default ctor, COMDAT-folded and
delinker-misnamed (e.g. allocator<char>) - the SHAPE is the tell, not the symbol.

```cpp
float4x4( ).identity( )
```
```asm
lea ecx,[ebp-0C8h]
call stlp_std::allocator<char>::allocator<char>   ; misname for float4x4()
mov ecx, eax
call vostok::math::float4x4::identity
```
Evidence: get_bone_matrix_in_object_space_impl (the parent==NULL leaf).
