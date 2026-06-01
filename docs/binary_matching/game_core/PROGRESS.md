# game_core matching progress (orchestrator ledger)

Module is large: 529 STUB markers across ~90 files at run start, plus 73 PARTIAL
and 55 SKIPPED. Workers dispatched one-per-function, leaf/small-first.

## Ledger
One line per dispatched function: `module::function -> STATE -> PR (regressions)`.

- game_core::game_core_initialize -> STATE[100%|DONE] -> PR #104 (regressions: none)
- game_core::client_player_update::serialize -> STATE[BLOCKED] -> PR #105 (regressions: none)
  - body matched 1:1 from asm but parked: udp_match_packet.h cluster is never-compiled stubs.
- game_core::inventory_item::inventory_item(action_behaviour_type) -> STATE[78.8%|PARTIAL] -> PR #106 (regressions: none)
- game_core::weapon_recoil_params::weapon_recoil_params() -> STATE[18.18%|PARTIAL] -> PR #107 (regressions: none)
  - low %: default ctor, likely inlined member constructors / layout not reproduced. Worth a revisit.

