# `bool const` local IS codegen-visible: drops xor-before-sete, flips && temps to byte
tags: cpp:bool cpp:const cpp:local | asm:sete asm:mov | topic:codegen-idiom topic:pdb-locals
symptoms: xor ecx ecx before sete in base only, mov dword [ebp-N] 1/0 vs mov byte
confidence: 9/10

`bool const x = ...` makes MSVC8 /Od emit the bare sete (no register pre-zeroing) and
BYTE-sized &&-temps; plain `bool x` zeroes the full register and uses dword temps. The
target PDB locals record (`; locals` in --view structure) shows `const bool` - restore
the const. An exception to the general rule that const on locals is codegen-invisible;
check `const u8`/`const bool` locals FIRST on a sete/temp-width diff.

```cpp
bool const x = ( a == K );    // bare sete, byte && temps
```
Evidence: weapon_core::on_hand_ik_event 96.55 -> 100 (xor); weapon_core::maximum_ammo_in_weapon 88.41 -> 100 (dword -> byte temp, with the != 0 addend).
