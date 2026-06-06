# Structure verification: player_logic_base_state {ctor, set_user}

PR #141, branch `match/game_core-player_logic_base_state-ctor-set_user`.
Source: `sources/vostok/game_core/sources/player_logic_base_state.cpp`.

Both functions report `fuzzy_match_percent = 100.0` in `binaries/objdiff/report.json`.
This is a structure-only check: does the base SOURCE SHAPE reproduce the target SHAPE.

## Verdict

- `player_logic_base_state::player_logic_base_state(weapon_user_animations_selector&, weapon_user_state_enum)`
  -> STRUCTURE MATCH (0 vs 0 statements, 0x5b vs 0x5b bytes).
- `player_logic_base_state::set_user(base_player&)`
  -> STRUCTURE MATCH (3 vs 3 statements, 0x16 vs 0x16 bytes, per-statement sizes identical).

No divergence. No downgrade. The 100% is over the correct structure for both.

## Function 1: constructor

Mangled `??0player_logic_base_state@survarium@@QAE@AAVweapon_user_animations_selector@1@W4weapon_user_state_enum@1@@Z`, size 91 bytes.
Target rva 0x58c220, base rva 0x44ae70.

```
TARGET  ; 0 statements, 0x5b bytes
BASE    ; 0 statements, 0x5b bytes
```

Both sides emit ZERO body statements. The seven member initializers are written as a
member-initializer list (`: m_owner( owner ), m_user( NULL ), ... m_is_ready_to_be_deactivated( true )`),
so all of them are attributed to the ctor decl line and collapse into the function's
header rather than producing per-statement source lines. The target does exactly the
same (0 body statements). Byte size agrees exactly (0x5b = 91).

This is the textbook "member-init list vs body assignments" case, and the base is on
the CORRECT side of it: had these seven inits been written as body assignments
(`m_owner = ...; m_user = NULL; ...`) the base would show 7+ statements against the
target's 0 - a quantity divergence the % could hide. It does not happen here; the
init-list form matches.

## Function 2: set_user

Mangled `?set_user@player_logic_base_state@survarium@@UAEXAAUbase_player@2@@Z`, size 22 bytes.
Target rva 0x58c200, base rva 0x44ae50.

```
TARGET  ; 3 statements, 0x16 bytes        BASE  ; 3 statements, 0x16 bytes
0x00  <0x7>  L28   (open brace `{`)        0x00  <0x7>  {
0x07  <0x9>  L29   (m_user = &user;)       0x07  <0x9>  m_user = &user;
0x10  <0x6>  L30   (close brace `}`)        0x10  <0x6>  }
```

Statement-by-statement:
- 0x00 / size 0x7: prologue / open scope. MATCH.
- 0x07 / size 0x9: the single assignment `m_user = &user;`. MATCH (same offset, same size).
- 0x10 / size 0x6: epilogue / close scope. MATCH.

Quantity matches (3 vs 3), every per-statement size matches (0x7, 0x9, 0x6), total
matches (0x16 = 22). The body is a single brace-wrapped assignment on both sides; the
function-brace-on-its-own-line style and the brace-wrapped single statement reproduce
the target's three-statement skeleton exactly.

## report.json context

- ctor: size 91, fuzzy_match_percent 100.0 - over the correct (0-statement) structure.
- set_user: size 22, fuzzy_match_percent 100.0 - over the correct (3-statement) structure.

No high-%-over-wrong-shape trap present in either function. Both remain clean `100%|DONE`.

(Note: `movement_animation_index` in the same unit is PR #123 and was NOT part of this
verification.)
