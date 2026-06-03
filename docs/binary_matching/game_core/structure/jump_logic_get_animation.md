# Structure verification: jump_logic::get_animation

PR #181 (match/game_core-jump_logic-animations). Verified via
`pdb_fetch --view structure`, target vs our base. No rebuild, no logic change.

## Verdict

STRUCTURE MATCH

report.json fuzzy_match_percent = 100.0 (size 136 bytes). The 100% is over the
CORRECT structure - no trap here.

## Skeletons (target vs base)

Target (rva 0x57df30):  ; 3 statements, 0x88 bytes
Base   (rva 0x44d350):  ; 3 statements, 0x88 bytes

```
offset  size   target (srcline)   base (statement)
0x00    <0x9>  L161               {
0x09    <0x79> L162               );        (the wrapped return-call)
0x82    <0x6>  L163               }
```

Statement quantity: 3 == 3. Per-statement sizes: 0x9 / 0x79 / 0x6 identical on
both sides. The single 0x79-byte body statement carries the inlined
get_jump_animation (with the bounds ASSERT temp that pushed this fn from 91.93%
to 100% - see jump_logic_animations.md). The inlined index calc and array
access stay inside one source statement on both sides; no extra block opens.

## Divergences

None - neither QUANTITY nor SIZE.

## Source fix

None required. Structure already reproduces the target.
