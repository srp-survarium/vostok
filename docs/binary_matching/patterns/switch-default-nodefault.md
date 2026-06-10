# Switch jump-table bounds check (`cmp/ja`) tracks the `default:`: NODEFAULT() drops it
tags: cpp:switch | asm:cmp asm:jmp asm:jcc | topic:structure-shape
symptoms: cmp [tmp] N ja default before jmp [table], head SIZE diff 0xa, BASE_ONLY 0x2 break row
confidence: 9/10
variants: switch-jump-table-relocs.md, switch-case0-return-not-break.md, switch-footer-vs-report.md

`default: NODEFAULT();` (= `__assume(0)` in Master Gold, debug_macros.h) makes the
default unreachable, so MSVC drops the range check and emits a tight contiguous table.
Read the direction of the diff:
- BASE has `cmp/ja` the TARGET lacks => you are missing the top case(s) plus
  `default: NODEFAULT();` (a terminal value reached THROUGH the table is an explicit
  case, not the default). NOT an LTCG artifact.
- TARGET has `cmp max; ja .end` the BASE lacks (head +0xa) => the original had NO default
  clause (or a reachable one); remove NODEFAULT. Often pairs with a BASE_ONLY 0x2 row:
  the original's LAST case fell out with NO break, while our written `break;` emits a
  2-byte jmp with its own line record - remove it too.

```cpp
case type_preview: return 1.0f;
default: NODEFAULT( );             // -> no cmp/ja, full table
```
Evidence: character_dispersion_calculator::get_target_koef (enum 0..4 - missing case + NODEFAULT); items_cook::create_item_and_finish_query 98.76 -> 100 (29/29, 0x370 both) removing NODEFAULT + break.
