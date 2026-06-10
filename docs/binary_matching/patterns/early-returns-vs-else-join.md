# Double join-`jmp` per return leaf = an else chain; flatten to early returns
tags: cpp:if cpp:return | asm:jmp | topic:structure-shape
symptoms: jmp .13 jmp .5 double jump per leaf, scores None too divergent
confidence: 8/10
variants: inverted-guard-early-return.md, flat-ifs-vs-else-if.md, else-block-double-jmp.md

An `if (a) {...} else if (b) {...} else {...}` chain whose branches all return creates a
join point before the function end, so MSVC /Od emits jmp-to-join then jmp-to-end (DOUBLE
jump per leaf). The target written as FLAT early returns (`if (a) { ...; return; }
if (b) { ...; return; } return;` - no else) jumps ONCE per leaf to the shared epilogue.
objdiff can score None until the jumps match.

```cpp
if ( a ) { ...; return; }
if ( b ) { ...; return; }
return;                     // drop the else keywords
```
Evidence: game_core/player_logic_base_state::movement_animation_index (None -> 100%).
