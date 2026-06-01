# game_core - matching notes

- Namespace: `survarium` (the pch does `using namespace vostok;`).
- Build: `/Od` (optimization off) + LTCG, `Master Gold` - see [../MATCHING.md](../MATCHING.md).
- Sources: `sources/vostok/game_core/sources/`.
- Reachability anchor: every matched function must be referenced from
  `temp_include_all.cpp` (a `use_*` driven by `IncludeAll`, instantiated in
  `game/sources/game_world.cpp`) or it is dead-stripped - see
  [../agentic_loop.md](../agentic_loop.md) section 3.
- Best reversed references: `player_stamina.cpp`, `collision_sensor.cpp`,
  `damage_model.cpp`, `bullet.cpp`.

## Module-wide notes
Shared types, recurring gotchas, and asm quirks that span functions - add as you
find them.

### BLOCKED cluster: packet serialize/deserialize (`udp_match_packet` / `packet_reader`)
Every `serialize(network_core::udp_match_packet&)` / `deserialize(network_core::packet_reader&)`
function in game_core is **blocked on the same root cause**: `network_core/udp_match_packet.h`
and `packet_reader` (with their `sequence_number` / `boost::array` / `base_packet`
deps) are never-compiled stubs - no built `.cpp` `#include`s them, so the first
include breaks the whole game_core build. The bodies are matchable 1:1 from asm
(see PR #105), but cannot compile or be scored until that header cluster is built
out. **Do not dispatch these until the cluster is built** - park `BLOCKED` cheaply
(keep the verified body as a comment), don't burn rebuilds rediscovering it.

Affected (as of the queue scan): `client_player_update::serialize` (PR #105),
`player_state::{serialize,deserialize}`, `weapon_state::deserialize`,
`weapon_core::{serialize,deserialize}`, `weapon_core_base_state::{serialize,deserialize}`,
`weapon_core_reload_state_base::{serialize,deserialize}`,
`weapon_core_chamber_a_round_state_base::{serialize,deserialize}`,
`weapon_core_chamber_a_round_aimed_state_base::{serialize,deserialize}`,
`weapon_core_shotgun_reload_state::{serialize,deserialize}`,
`hand_to_weapon_ik_processor::{serialize,deserialize}`,
`base_player::{send_game_world_object,deserialize_game_world_object}`.
Unblocking task: build out the `udp_match_packet` / `packet_reader` header cluster
(its own work item, likely shared with the `network_core` module).

### Deprioritize constant-only default ctors
A default ctor that only stores constants, reachable solely via the
`temp_include_all` anchor, compiles **empty** under `/Od`+`/GL` - LTCG
dead-store-eliminates every member store because no real consumer observes the
object. So it caps at a low PARTIAL (~18% on `weapon_recoil_params::weapon_recoil_params()`,
PR #107) until its real game callers are matched. Write the correct body once,
anchor once to confirm the symbol + score, then STOP - don't iterate. Prefer
functions with observable side effects / real consumers; come back to these
ctors after their callers exist.

## Per-function logs
One `<function>.md` in this folder per function that needed real effort (see
[../agentic_loop.md](../agentic_loop.md) section 7). Live status is in the
`STATE[...]` markers in the sources; these files hold the deeper write-ups.
