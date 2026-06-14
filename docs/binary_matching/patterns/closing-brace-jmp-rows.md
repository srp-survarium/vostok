# A 2-byte `jmp short` row = a closing brace `}` (case blocks, braced loop bodies)
tags: cpp:switch cpp:for | asm:jmp | topic:structure-shape
symptoms: +0x002 carcass step, N trailing 0x2 rows after a loop body, base body row +2 per missing one
confidence: 8/10
variants: else-block-double-jmp.md, rel8-rel32-brace-exit.md, for-head-statement-merges.md

/Od emits a 2-byte `jmp` for a block's `}` even when the block's last statement returns
(dead, not removed) - this is how the carcass reveals where braces were. A `case`/`if`
showing a `+0x002` row was braced; one without was brace-less. A BRACED loop body
attributes the backjump to the `}` line as its OWN 0x2 statement (nested loops stack: 3
braced levels = 3 trailing 0x2 rows); a brace-less body merges the backjump into the body
statement (size-sum tell: base body row +2 per missing 0x2 row).

```cpp
case x:
{               // the { } emits the +0x002 row
	...
	break;
}
```
Do NOT decide braces from "has a local" alone (mis-braced get_target_koef and earlier switches that way).
Evidence: double_barreled fire_state ctor (16/16, 0x1c2 both sides - braces around all nested for levels, mirroring the reload sibling).

## Brace PLACEMENT decides opening-vs-closing attribution (3+ nested loops)
For a 3-deep nested `for`, the brace STYLE flips which brace gets the line records:
- brace on the NEXT line (Allman) -> MSVC attributes records to the OPENING `{`s and FOLDS
  the closing backjumps into the body statement (base 7 stmts vs target 10; the missing
  rows show TRGT_ONLY 0x2 at the `}`s, body row +size). Byte-identical, structure-WRONG.
- brace on the SAME line as the `for` (K&R) -> the three fors land on consecutive line
  records and each closing `}` gets its OWN 0x2/0x5 statement -> STRUCTURE MATCH.
  (Outermost `}` of a 3-iteration inner loop is rel32 = a 0x5 row, inner two are 0x2.)
A 2-deep loop emits the closing `}` rows under EITHER style (see weapon_core_aimed_state
ctor, Allman, STRUCTURE MATCH). The flip only bites at depth >= 3.
Tell in the target structure: the `for`s sit on CONSECUTIVE source lines (e.g. 23,24,25)
followed by the body then the `}` lines (27,28,29) -> the author wrote K&R.
Evidence: pistol_/double_barreled_weapon_core_aimed_idle_state ctors (10/10 & 11/11 STRUCTURE
MATCH after switching Allman->K&R; Allman gave 7/10 & 8/11 at the same 100% bytes).
