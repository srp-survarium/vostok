# Structure verification: jump_logic::activate

PR #181 (match/game_core-jump_logic-animations). Verified via
`pdb_fetch --view structure`, target vs our base. No rebuild, no logic change.

## Verdict

STRUCTURE MATCH

report.json fuzzy_match_percent = 100.0 (size 77 bytes). The 100% is over the
CORRECT structure - no trap here.

## Skeletons (target vs base)

Target (rva 0x57d880):  ; 4 statements, 0x4d bytes
Base   (rva 0x44d2a0):  ; 4 statements, 0x4d bytes

```
offset  size   target (srcline)   base (statement)
0x00    <0x9>  L185               {
0x09    <0x24> L186               m_jumping_direction      = get_move_direction( m_user->input( ) );
0x2d    <0x1c> L187               m_is_jump_from_right_leg = !m_owner.is_right_leg_supporting( );
0x49    <0x4>  L188               }
```

Statement quantity: 4 == 4. Two body assignment statements, each its own line
(0x24 and 0x1c), plus opening/closing braces - matching the target exactly.
These are written as body assignments (not a member-init list), which is correct:
activate() is a regular method, not a ctor, so each assignment is one statement.
The `!m_owner.is_right_leg_supporting()` logical-NOT (neg;sbb;add1) lives inside
the single 0x1c statement; no extra block.

## Divergences

None - neither QUANTITY nor SIZE.

## Source fix

None required. Structure already reproduces the target.
