# Array brace-init: one literal per LINE = one statement per element
tags: cpp:string cpp:local | asm:mov | topic:pdb-locals topic:structure-shape
symptoms: SIZE diff on a captions[N] line, N separate 0x7 statements vs one N*0x7, EMPTY only base rows
confidence: 9/10
variants: same-line-statement-merges.md

/Od attributes each array-initializer element-store to the SOURCE LINE its literal sits
on. A single-line `= { "a", "b" }` collapses all stores onto one statement; the original
wrote each element on its own line. Bytes identical, fuzzy unchanged - only the
structure-diff flags it. The blank-line gaps show as harmless `EMPTY only base` rows.

```cpp
pcstr captions[ 2 ] =
{
	"a",
	"b"
};
```
Evidence: all 7 get_weapon_lexeme_pair variants in pistol/double_barreled idle/aimed_idle/show states (SIZE-diff 1 -> 0; report.json unchanged 99.92%).
