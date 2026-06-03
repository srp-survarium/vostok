# Structure verification: weapon_user_animations_selector::look_time_calculator

PR #181 (match/game_core-jump_logic-animations). Verified via
`pdb_fetch --view structure`, target vs our base. No rebuild, no logic change.
Dependency of jump_logic::selected_animations.

## Verdict

STRUCTURE MATCH

report.json fuzzy_match_percent = 100.0 (size 42 bytes). The 100% is over the
CORRECT structure - no trap here.

## Skeletons (target vs base)

Target (rva 0x584cf0):  ; 3 statements, 0x2a bytes
Base   (rva 0x448a50):  ; 3 statements, 0x2a bytes

```
offset  size   target (srcline)   base (statement)
0x00    <0x7>  L318               {
0x07    <0x1d> L319               return look_time_functor( this, &weapon_user_animations_selector::look_time_factor_calculator );
0x24    <0x6>  L320               }
```

Statement quantity: 3 == 3. Per-statement sizes: 0x7 / 0x1d / 0x6 identical.
The single return statement constructs the look_time_functor (FastDelegate) from
`this` plus the member-fn pointer to look_time_factor_calculator, in one source
statement of 0x1d bytes on both sides.

## Divergences

None - neither QUANTITY nor SIZE.

## Source fix

None required. Structure already reproduces the target.
