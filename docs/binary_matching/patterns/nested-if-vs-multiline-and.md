# A multi-line `&&` if = ONE record at its LAST line; TWO records = NESTED ifs
tags: cpp:if | asm:test asm:jcc | topic:structure-shape topic:pdb-locals
symptoms: condition split into two statements whose sizes SUM to the base's single if row, off-by-one stmt counts
confidence: 8/10
variants: same-line-statement-merges.md, decl-in-if-guard.md

A single `if ( a && b && c )`, however many lines it wraps, anchors ONE line record on the
LAST condition line. Per-leg records mean the original NESTED the ifs - byte-identical
when there is no else (exit labels coincide); only the line attribution changes. A
line-number gap before the inner if suggests a `{` line.

```cpp
if ( a )
{
	if ( b && c )
		body;
}
```
Evidence: weapon_core_shotgun_reload_start_substate::initialize (4 -> 5 stmts, 5/5; 0x1a + 0x2f records summing to the single if row).
Evidence: weapon_core_reload_state_base::initialize - target split `!deserializing()` (@29) from `chamber_a_round_on_reload() && round_is_chambered()` (@31) with a line-30 gap (the `{`); a flat `if (!deserializing() && chamber && round)` folds to ONE record (3 stmts) no matter the blank-line/multi-line spelling, while `if(!deserializing()){ if(chamber && round) body; }` emits 4 stmts byte-identical (0x61 both) -> STRUCTURE MATCH. The leftover 99.83% is only `round_is_chambered`'s `this`-load reg (eax target / ecx base), a /Od scratch-reg choice.
