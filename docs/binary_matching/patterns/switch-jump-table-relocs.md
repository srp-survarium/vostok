# Jump-table switch: read the case order from the obj's own .text relocs
tags: cpp:switch | asm:jmp asm:movss | topic:structure-shape topic:tooling
symptoms: jmp dword ptr [edx*4+table], trailing garbage after ret, add eax 6B00595Fh
confidence: 9/10
variants: switch-default-nodefault.md, switch-footer-vs-report.md

`jmp dword ptr [edx*4 + <table>]` with the table inside the function's .text (the
trailing "garbage" after ret in --view target). The N table entries are N dwords with
type-20 (REL32) relocs back to the function symbol; the stored dword IS the case target
as a function-relative offset. Parse the COFF (find the function's .text offset from the
symbol table, read the table dwords), then cross-reference each target block's leaf
`movss xmm0,[reg+off]` against the struct layout to label the case.

```cpp
switch ( type ) { case type_stand: ... }   // case order recovered from the relocs
```
A `case X: return cond ? a : b;` lowers each leaf to `movss xmm0,[m_params+off]; movss [tmp],xmm0; ...; fld [tmp]; jmp end` and matches modulo register/slot.
Evidence: character_dispersion_calculator::get_target_koef - type_stand(0)->0x25, type_crouch(1)->0x8b, type_sprint(2)->0xef, type_jump(3)->0xf9, default(4)->0x103 (fld1).
