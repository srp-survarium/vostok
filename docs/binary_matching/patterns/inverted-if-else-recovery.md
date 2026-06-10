# Recover an INVERTED if/else from line records + branch sizes + jmp widths
tags: cpp:if | asm:jmp asm:jcc | topic:structure-shape
symptoms: TRGT_ONLY pair 0x1a+0x5 after the if row, BASE_ONLY pair 0x2+0x1a after the then-block, equal stmt counts shifted
confidence: 8/10
variants: inverted-guard-early-return.md, rel8-rel32-brace-exit.md

When the record right AFTER the if line is the size of the ELSE-branch call followed by
a 5-byte jmp record, the original put that call in the THEN branch - i.e. wrote the
NEGATED condition `if ( !cond ) simple_call; else { big block }`. Cross-check jmp widths:
je is rel8 over a small then-branch, rel32 (+0x4) over a big one; the jmp-over-else is
rel32 (0x5) over a big else, rel8 (0x2) over a small one.

```cpp
if ( !cond )
	process_hand( ... );    // 0x1a
else
{ /* big block */ }
```
Evidence: hand_to_weapon_ik_processor::process 81.51 -> 94.64 (12/12, sole residual the documented mix_transformations wall).
