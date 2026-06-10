# A 2-byte `jmp short` row = a closing brace `}` (case blocks, braced loop bodies)
tags: cpp:switch cpp:for | asm:jmp | topic:structure-shape
symptoms: +0x002 carcass step, N trailing 0x2 rows after a loop body, base body row +2 per missing one
confidence: 9/10
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
