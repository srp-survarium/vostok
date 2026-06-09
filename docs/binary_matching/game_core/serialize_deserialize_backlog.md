# serialize/deserialize backlog (post udp_match_packet keystone)

Keystone `45cde188` built the `udp_match_packet`/`packet_reader`/`packet<T>` header cluster
in game_core, unblocking the serialize/deserialize bodies that were `STATE[BLOCKED]` on
"the cluster is never-compiled". This trail covers the bodies filled in that wake.

All `r<T>()` reads / `packet.append(...)` writes were read off the TARGET asm
(`pdb_rich_query --index binaries/rich/target/index.jsonl --function <name>`). Most are
virtuals reached through their class vtables (no extra anchor needed); the free helpers
(`serialize_affect`/`deserialize_affect`, `call_item_*`) are reached through the
body_part / inventory serialize chains.

## Cleared (built + logically faithful, PARTIAL = inline/LTCG/assert residual)

| function | shape |
|---|---|
| inventory_item::serialize/deserialize | append/r<u16> m_amount [+0x114]; client_offset dropped (LTCG) |
| weapon_state::deserialize | three r<bool> into u8 slot/ammo_slot/state (target uses r<bool> instantiation) |
| player_stamina::deserialize | r<float> m_value, r<u32> x2 timestamps, r<bool> threshold flag |
| weapon_core_base_state::serialize/deserialize | serialize_animation_state-guarded playback_state probe (get_animation_playback_state, mask -3 = body_part_whole_body_but_hands) + append id/time; deserialize reads id/time |
| weapon_core_reload/chamber_a_round/chamber_a_round_aimed_state_base | append/r<bool> m_animation_has_been_ended [+0x135] (inherited from weapon_core_base_state) |
| hand_to_weapon_ik_processor::serialize/deserialize | pack both hands' is_active into a u8 (bit0/bit1) + per-hand start_transition_time biased by client_offset |
| weapon_user_animations_selector::serialize/deserialize | walk m_logic fsm state list to map current<->index; serialize appends index + forwards state serialize; deserialize promotes indexed state to initial + forwards deserialize |
| weapon_core_shotgun_reload_state::serialize/deserialize | same fsm-index codec over the internal reload-substate fsm (m_logic at +0x138) |
| weapon_core::serialize/deserialize | base inventory_item + scalar state (random/normal seeds, target, actions mask, magazine/queue counts, fire-queue type, ammo slot), chamber flag; under live logic fsm: is_shown/hand-ik/state index + forwarded state & selector codec; deserialize rebinds m_ammunition from inventory slot + force-selects user animations; guarded by m_deserializing |
| base_player::deserialize_game_world_object | read source slot, fetch inventory item, forward deserialize_game_world_object |
| inventory::deserialize + call_item_serialize/deserialize | for_each + boost::bind over slot array; per-slot helper skips items whose profile slot is in ignored_slots_for_serialization (7 armor slots) |
| damage_model::deserialize | intrusive_list for_each forwarding body_part_parameters::deserialize |
| body_part_parameters::serialize/deserialize + serialize_affect/deserialize_affect | health + last-hit time + active-affect list; affect = (u8 type, u32 expiry-biased-by-client_offset) |
| booby_trap_core::serialize/deserialize | owner serialize_game_world_object_header + trap state + transform position/angles; deserialize rebuilds transform (rotation*translation), owner insert_trap, switch_to_state |
| booby_trap_set_core::serialize_game_world_object_header/deserialize_game_world_object | append/read trap index, forward to the resolved trap |
| oxygen_tank / medkit / weapon_ammunition serialize/deserialize | pure forward to inventory_item base (ICF-folded, rva 0xbcd90/0xbcdb0) - the {} stubs were WRONG |
| relocate_item_descr::serialize (game_net_defines.h) | 6 tcp_packet appends (LTCG-inlined in target) |

## Still BLOCKED (precise reason)

- **inventory_item::serialize_game_world_object_header / deserialize_game_world_object**
  (header inline, rva 0x869f0 / 0x9b250): target copies the args (game_world_object,
  udp_match_packet, packet_reader) BY VALUE into the VOSTOK_UNREFERENCED_PARAMETERS eater.
  The by-value copy needs the COMPLETE types, but inventory_item.h only forward-declares
  them. Matching the eater needs an out-of-line body (own .cpp) or heavy includes in the
  header. Left as `{}` stub with the reason inline.
- **match_options::deserialize (game_net_defines.h)**: LTCG-inlined (register this/reader),
  with a memcpy for map_name[32] and many field reads; standalone bytes won't match. Left STUB.

## Out of scope (not the cluster wall)

- **animation_player::deserialize_state**: takes `void* buffer`, not packet_reader - it is a
  separate STUB unrelated to the udp_match_packet cluster, left as-is.

## Notes / residuals to chase against the diff

- `random32::seed()` was made `const` so weapon_core's const serialize can read the seed
  (pure member read; the engine's const serialize requires it).
- The compiled-out `ASSERT( found/current )` in the fsm-index codecs and the trailing
  `call empty_stub` (delinker `finalize_impl`) asserts do NOT re-emit in MASTER_GOLD, so
  those rows are PARTIAL residuals, not source-shape bugs.
- The boost::bind / fixed_vector / intrusive_list iteration shapes (body_part_parameters
  serialize, inventory::deserialize, damage_model::deserialize) are logically faithful but
  the exact bind-list bytes await a structure-diff pass.
