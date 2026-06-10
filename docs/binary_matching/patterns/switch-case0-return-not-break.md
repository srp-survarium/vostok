# Case whose body equals the default: `case 0: return X;`, NOT `case 0: break;`
tags: cpp:switch cpp:return | asm:cmp asm:jcc | topic:structure-shape
symptoms: explicit cmp [v] 0 je to the shared return block, scores None with break
confidence: 9/10
variants: switch-default-nodefault.md, switch-redundant-case-compare.md

A target with an explicit `cmp [v],0 / je <block>` where that block is the SAME return
the fall-through reaches (one block, two predecessors): write `case 0: return X;` plus a
tail `return X;` - /Od emits the cmp/je and FOLDS the two identical return blocks. A
`case 0: break;` gets folded into the default path (NO cmp 0 at all) and the base loses
the whole comparison - objdiff reports None (too divergent; the symbol is present).

```cpp
case 0:	return 1.0f;     // distinct labeled block
...
return 1.0f;             // the default path - MSVC folds the two fld1 blocks
```
Evidence: character_dispersion_calculator::get_broken_hands_penalty - break -> None, return 1.0f -> 82.89%.
