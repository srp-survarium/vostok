# A `this`-unused trivial member is FRAMELESS in the target, framed under /Od (None|PARTIAL)
tags: cpp:member cpp:return | asm:fld asm:ret | topic:scoring-artifact topic:convention
symptoms: target body d9 ee c3 fldz ret, b8 reloc c2 0400, base full frame 11+ bytes, fuzzy None
confidence: 8/10
variants: optimized-comdat-in-od-unit.md

A trivial member that never references `this` (returns a literal/0.0f/NULL) gets
frame-pointer omission in the original build (3-5 bytes) while /Od always emits the
frame + `mov [ebp-4],ecx` - too large a gap for objdiff to pair (None) though the
semantic body is correct. Not steerable under /Od (build flag). These survive STANDALONE
at a real rva (present-but-divergent): mark None|PARTIAL. CONTRAST: a member that USES
this keeps its frame in BOTH and matches 100% - frame presence tracks this-usage.

```asm
; target: d9 ee c3                   ; fldz; ret           (get_speed)
;         b8 <reloc> c2 0400        ; mov eax,lit; ret 4  (use_info)
; base:   55 8bec 51 894dfc ... 8be5 5d c3
```
Evidence: artefact_container_core::use_info, booby_trap_core::get_speed (both None|PARTIAL); damage_protector ctor/dtor (this-using, 100%).
