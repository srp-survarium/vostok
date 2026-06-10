# A single low-byte store into a multi-byte member = a single-FIELD write, not a multi-arg ctor
tags: cpp:ctor cpp:member | asm:mov | topic:structure-shape
symptoms: one byte store where source builds the whole object, mov byte[tmp] 64h
confidence: 6/10

A full `T( a, b, c )` ctor writes ALL fields (4 byte stores for a packed color); ONE byte
store means the statement touched exactly ONE field - a single-field ctor or a per-field
setter. The type's STRUCTURE (ctor/accessor list) is the menu; the byte count says how
many fields the statement may touch.

```cpp
original_color.set_B( 0x64u );    // one channel - NOT color( 0x64u, 0x00u, 0x00u )
```
```asm
mov byte[tmp],64h; mov cl,[tmp]; mov [member],cl
```
Evidence: legs_ik_processor::get_foot_fixed_transform else-branch (0x64 = 100 on the packed-union color).
