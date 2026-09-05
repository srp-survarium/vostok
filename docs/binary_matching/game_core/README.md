# game_core - matching notes

- Namespace: `survarium` (the pch does `using namespace vostok;`).
- Build: `/Od` (optimization off) + LTCG, `Master Gold` - see [../MATCHING.md](../MATCHING.md).
- Sources: `sources/vostok/game_core/sources/`.
- Reachability anchor: every matched function must be referenced from
  `game/sources/temp_include_all.cpp` (a `use_*` driven by `IncludeAll`,
  instantiated in `game/sources/game_entry_point.cpp::create_world`; the TU
  moved out of game_core during the game carcass rebuild) or it is
  dead-stripped - see
  [../agentic_loop.md](../agentic_loop.md) section 3.
- Best reversed references: `player_stamina.cpp`, `collision_sensor.cpp`,
  `damage_model.cpp`, `bullet.cpp`.

## Module-wide notes
Shared types, recurring gotchas, and asm quirks that span functions - add as you
find them.

The [inline-body audit](inline_recovery.md) records the `jump_logic` no-source
declaration reviewed with the PR 569 animation/AI batch.

### Foundational gap: `vostok::ai::fsm_state::~fsm_state` has no body in our sources
The whole game_core state hierarchy (weapon_core_*_state, jump_logic_state_*,
player_logic_*_state, ...) derives from `vostok::ai::fsm_state`, whose destructor
is declared pure-virtual (`= 0`) with **no definition anywhere in our sources**
(`sushi/v0.100b/match-fsm` matches only the `fsm` container, not `fsm_state`;
target defines the dtor @ rva 0x3f210). So you cannot construct/anchor any
concrete fsm_state-derived state without a dtor body to link against.
**Workaround (use it, it doesn't regress any matched obj):** define a local empty
`vostok::ai::fsm_state::~fsm_state() {}` in the non-target `temp_include_all.cpp`
TU alongside your anchor (as PR #121 did). It is not a matched object, just a
link stopgap. *Proper fix:* match `fsm_state::~fsm_state` as a foundational unit
(it lives in the `ai` module, out of the game_core lane) and drop the stopgaps.

### Suspected BLOCKED cluster: config (`vostok::configs::binary_config_value`)
`load(binary_config_value const&)` methods and `(binary_config_value const&)`
ctors (the params structs: weapon_dispersion_params, character_dispersion_params,
weapon_recoil_params, ...) likely hit the same never-compiled-header wall as the
packet cluster - no compiled `.cpp` references `binary_config_value`. Grep before
dispatching; if its header doesn't compile standalone, defer them BLOCKED.

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
`weapon_user_animations_selector::{serialize,deserialize}`,
`hand_to_weapon_ik_processor::{serialize,deserialize}`,
`base_player::{send_game_world_object,deserialize_game_world_object}`.
Unblocking task: build out the `udp_match_packet` / `packet_reader` header cluster
(its own work item, likely shared with the `network_core` module).

### Constant-only ctors: anchor so the object is OBSERVED (revised)
A ctor that only stores constants is dead-store-eliminated by LTCG **only if no
reachable code observes the object**. The fix is the same opaque-sink escape used
for setters (loop_performance.md): the anchor must construct an instance and
**escape `&obj` through an opaque external sink** so the member stores are
observed - then the ctor matches at full %. Proven incidentally: `weapon_state`'s
default ctor hit 100% for free once `operator=`'s anchor (PR #115) escaped a
`weapon_state` instance.

So the earlier "constant-only ctor caps at ~18%" (`weapon_recoil_params`, PR #107)
was an **inadequate anchor**, not a hard limit. **CONFIRMED**: the #107 retry
re-anchored `weapon_recoil_params` default ctor via `use_game_core_weapon_recoil_params`
(construct, then `example_callback( reinterpret_cast< pcstr >( &params ) )`) and it
jumped 18% -> **100%** with the body untouched. General rule: anchor every
accessor/ctor with a properly *observed* instance (the same escape), don't just
instantiate-and-discard.

## Per-function logs
One `<function>.md` in this folder per function that needed real effort (see
[../agentic_loop.md](../agentic_loop.md) section 7). Live status is in the
`STATE[...]` markers in the sources; these files hold the deeper write-ups.
