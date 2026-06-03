# Structure verification: jump_logic::selected_animations

PR #181 (match/game_core-jump_logic-animations). Verified via
`pdb_fetch --view structure`, target vs our base. No rebuild, no logic change.

## Verdict

STRUCTURE MATCH

report.json fuzzy_match_percent = 100.0 (size 81 bytes). The 100% is over the
CORRECT structure - no trap here.

## Skeletons (target vs base)

Target (rva 0x57d9d0):  ; 3 statements, 0x51 bytes
Base   (rva 0x44d2f0):  ; 3 statements, 0x51 bytes

```
offset  size   target (srcline)   base (statement)
0x00    <0x9>  L209               {
0x09    <0x42> L210               );        (the wrapped return-call)
0x4b    <0x6>  L211               }
```

Statement quantity: 3 == 3. Per-statement sizes: 0x9 / 0x42 / 0x6 identical on
both sides. The single wrapped statement (the `static_cast<...>(...)->
selected_animations( buffer, is_third_view, m_owner.look_time_calculator(),
weapon_parameters )` return) is one statement of 0x42 bytes on both sides; the
opening brace (0x9) and closing brace (0x6) align.

## Divergences

None - neither QUANTITY nor SIZE.

## Source fix

None required. Structure already reproduces the target.
