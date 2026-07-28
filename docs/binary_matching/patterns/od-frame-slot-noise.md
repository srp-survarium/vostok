# Byte-identical body, `sub esp` off by 4-8 = /Od frame-slot allocation noise (stop at PARTIAL)
tags: cpp:local | asm:sub-esp asm:mov | topic:convention topic:pdb-locals
symptoms: sub esp 1Ch vs 18h, sub esp 38h vs 30h, saved-this slot shifted, all [ebp-N] shifted by a constant
confidence: 8/10
variants: single-spill-prologue.md, disp8-disp32-size-rows.md, per-call-ref-return-spill.md

When the base matches the target instruction-for-instruction, offset-for-offset,
call-for-call, but the TARGET reserved a few more stack bytes (one or two unused /Od
frame slots) so the saved-`this` and every `[ebp-N]` shift by the constant - that is pure
stack-allocation noise, NOT a missing local/brace/ASSERT/statement. The PDB local set
maps, the carcass structure matches 1:1. Non-steerable under /Od; mark PARTIAL at the
resulting %.

```asm
sub esp, 1Ch     ; target - one extra unused dword slot
sub esp, 18h     ; base   - this@[ebp-0Ch] vs target this@[ebp-10h]
```
Evidence: character_dispersion_calculator::tick (99.67%, 16 stmts, only diff the sub esp); breath_vibration_calculator::tick (94.23%: sub esp 38h vs 30h, 2 extra temp slots swap which register holds the vtable at the 2nd virtual call - locals all map).
