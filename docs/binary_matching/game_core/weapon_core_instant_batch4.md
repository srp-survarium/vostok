# weapon_core batch4: instant_show/hide/aim_start/aim_end, chamber_a_round, get_dispersion

All six in `sources/vostok/game_core/sources/weapon_core.cpp`, class `survarium::weapon_core`.
Branch `match/game_core-weapon_core-batch4` off `origin/int/game_core` in worktree
`/home/sheep/Projects/surv/vostok_3`.

## Setup / target asm
```
nix develop -c pdb_rich_query --index binaries/rich/target/index.jsonl --list | grep instant_...
nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl --rva <rva> --view target
```
Target RVAs (PDB): instant_show 0x592c70, instant_hide 0x592c50, instant_aim_end 0x592c30,
instant_aim_start 0x593070, chamber_a_round 0x5930d0, get_dispersion 0x594760.

Access chars (from index.jsonl mangled names) - all already correct in the header:
- instant_show/instant_hide/chamber_a_round: `QAE` (public)
- instant_aim_start/instant_aim_end: `UAE` (public virtual)
- get_dispersion: `QBE` (public const)

Member offsets (weapon_core.h): 0x488 m_aimed, 0x48c m_is_firing, 0x48e m_is_round_chambered,
0x491 m_aiming_state_transition, 0x47a m_ammo_in_magazine, 0x44c m_user, 0x328 m_dispersion_calculator.

## Bodies
- `instant_show`  : `m_aimed = false; on_show();`  (on_show virtual vtable+0xB0). 100%.
- `instant_hide`  : `on_hide();`  (vtable+0xB4). 100%.
- `instant_aim_end`: `m_aimed = false; m_aiming_state_transition = true;`. 100%.
- `chamber_a_round`: 3x `finalize_impl` calls = 3 compiled-out ASSERTs, then
  `--m_ammo_in_magazine; m_is_round_chambered = true;`. ASSERT placeholder = `m_ammo_in_magazine`. 100%.
- `get_dispersion`: `return m_dispersion_calculator.get_dispersion();`. 100%.
- `instant_aim_start`: see iteration below. 100%.

## instant_aim_start iteration
First try: `if (!m_is_firing && !m_user->input().is_aiming()) reset_fire_queue(); m_aimed=true; m_aiming_state_transition=true;`
- gave 60.33%. `--view diff` showed TWO shape mismatches:
  1. target reads m_is_firing into a stack temp `[ebp-1]` (`mov cl,[eax+48Ch]; mov [ebp-1],cl`)
     before testing - i.e. the bool came through the inlined `is_firing()` accessor
     (returns m_is_firing by value), not a direct member read.
  2. target tests the aiming bit RAW (`and ecx,20h; jne`) with NO boolean normalization
     (no neg/sbb/neg) - so it is NOT a bool-returning `is_aiming()` call; it is the integer
     expression `input().actions_mask & 0x20` used directly in the `&&` short-circuit.
- Fix: `if ( !is_firing( ) && !( m_user->input( ).actions_mask & 0x20 ) ) reset_fire_queue();`
  -> 100%. (Reverted the speculative `player_input::is_aiming()` body; not used.)

`m_user->input()` is base_player's virtual `input()` (vtable+0x24) returning `player_input const&`;
`actions_mask` is at player_input+0x10. `reset_fire_queue()` is a member call.

## Anchor
Added the four new calls to the existing `use_game_core_weapon_core_small_setters()` in
`temp_include_all.cpp` (instant_show/hide already there): instant_aim_start, instant_aim_end,
chamber_a_round, get_dispersion.

## Build
`touch sources/vostok/game_core/sources/weapon_core.cpp` (header-edit gotcha) then
`nix develop -c python3 scripts/rebuild.py` (no module arg). Scores from report.json
top-level units[].functions[].fuzzy_match_percent. Final: all six 100%, report-changes
regressed count 0.
