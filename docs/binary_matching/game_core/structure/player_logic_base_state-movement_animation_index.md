# Structure verification: player_logic_base_state::movement_animation_index

Function: `static unsigned int survarium::player_logic_base_state::movement_animation_index(survarium::player_input const&)`
Unit: vostok/game_core/sources/player_logic_base_state.cpp
PR: #122 (branch match/game_core-player_logic_base_state-movement_animation_index)

Target rva: 0x58c100
Base rva:   0x44d120

## Verdict

STRUCTURE MATCH

29 target statements vs 29 base statements; every offset and every per-statement
size is identical; 0xfa (250) bytes on both sides. fuzzy_match_percent = 100.0.
This is a genuine clean 100% DONE - the high % is over the CORRECT structure.

## The two skeletons (side by side)

Both: `; 29 statements, 0xfa bytes`

```
offset  size   TARGET (Lnn)   BASE (source statement)
0x00    <0x4>  L33            {
0x04    <0x12> L34            bool move_fwd_pressed   = ( input.actions_mask & 0x1 ) != 0;
0x16    <0x12> L35            bool move_bwd_pressed   = ( input.actions_mask & 0x2 ) != 0;
0x28    <0x12> L36            bool move_left_pressed  = ( input.actions_mask & 0x4 ) != 0;
0x3a    <0x12> L37            bool move_right_pressed = ( input.actions_mask & 0x8 ) != 0;
0x4c    <0x10> L39            if ( move_fwd_pressed && move_bwd_pressed )
0x5c    <0x4>  L40            move_fwd_pressed  = false;
0x60    <0x4>  L41            move_bwd_pressed  = false;
0x64    <0x10> L44            if ( move_left_pressed && move_right_pressed )
0x74    <0x4>  L45            move_left_pressed  = false;
0x78    <0x4>  L46            move_right_pressed = false;
0x7c    <0x8>  L49            if ( move_fwd_pressed )
0x84    <0x8>  L50            if ( move_left_pressed )
0x8c    <0x7>  L51            return move_fwd_left_anim;
0x93    <0x8>  L53            if ( move_right_pressed )
0x9b    <0x7>  L54            return move_fwd_right_anim;
0xa2    <0x7>  L56            return move_fwd_anim;
0xa9    <0x8>  L59            if ( move_bwd_pressed )
0xb1    <0x8>  L60            if ( move_left_pressed )
0xb9    <0x7>  L61            return move_bwd_left_anim;
0xc0    <0x8>  L63            if ( move_right_pressed )
0xc8    <0x7>  L64            return move_bwd_right_anim;
0xcf    <0x7>  L66            return move_bwd_anim;
0xd6    <0x8>  L69            if ( move_left_pressed )
0xde    <0x7>  L70            return move_left_anim;
0xe5    <0x8>  L72            if ( move_right_pressed )
0xed    <0x7>  L73            return move_right_anim;
0xf4    <0x2>  L75            return idle_anim;
0xf6    <0x4>  L76            }
```

Note: the target view labels statements by its own source line numbers (L33..L76)
and the base view by our source text. The line numbers differ slightly from our
.cpp line numbers (the target's original source had a marginally different vertical
layout - e.g. the two false-assignment statements are attributed L40/L41 in the
target vs the same statements appearing a line lower in our file because of an extra
intervening comment), but that is purely line-numbering noise. The STRUCTURAL units
- statement count, offsets, sizes, control-flow shape - are identical.

## Divergences

None. No QUANTITY divergence (29 vs 29) and no SIZE divergence (every `+delta`
matches: 4,12,12,12,12,10,4,4,10,4,4,8,8,7,8,7,7,8,8,7,8,7,7,8,7,8,7,2,4 bytes on
both sides).

## Control-structure shape confirmed

- The two top guards (`if ( move_fwd_pressed && move_bwd_pressed )` and
  `if ( move_left_pressed && move_right_pressed )`) are braced two-statement blocks;
  both reproduce, with the two contained `= false;` assignments emitted as separate
  statements after the `if`. Matched on both sides.
- The decision cascade is a flat sequence of early-return guards (no `else` /
  no `else if`). The base source already encodes this (claude@MATCH note in the
  .cpp explains the flat-returns choice avoids per-level join jumps). The target
  shows the same flat shape - the final terminal `return idle_anim;` is a bare
  <0x2> statement at 0xf4, exactly as a fall-through to the single epilogue, not a
  join from an else chain. Matched.
- The two outer `if ( move_fwd_pressed )` / `if ( move_bwd_pressed )` blocks each
  contain nested `if`s and returns; sizes (0x8 for the guard, 0x7 for each return)
  line up one-for-one.

No member-initializer-list / body-assignment ambiguity applies (free-standing
static, no ctor). No switch, so no case-brace scoping concern.

## Source-shape conclusion

No restructure needed. The 100%|DONE label is correct and the carcass was rightly
stripped on a clean 100% DONE. STATE left as `// STATE[100%|DONE]`.
