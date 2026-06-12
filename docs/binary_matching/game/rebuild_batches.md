# game carcass rebuild - batch plan

Working agreement for the queue grind (`temp/structure_queue/`). One agent per
batch, SERIAL (every batch touches `game.vcproj` + `temp_include_all.cpp`).
Per batch: reproduce each file (headers + same-stem `*_inline.h`), wire headers
into `temp_include_all.cpp` + a vcproj `<File>` entry, `git rm` from the queue,
`rebuild.py game` green, ONE commit. Queue `.cpp` compilands are reproduced
with their subsystem but added to the vcproj `ExcludedFromBuild="true"` for
every configuration - TU enablement is a separate leaf-first pass at the end
(an enabled-but-uncompilable TU breaks the build for everyone).

Method: [../library_carcass_rebuild.md](../library_carcass_rebuild.md).
Module notes: [README.md](README.md). Legacy priors: `temp/game_legacy/`.

| # | Batch | Scope (queue stems) | Status |
|--|-------|---------------------|--------|
| 1 | enums | 10 standalone survarium enums | DONE (0732995a) |
| 2 | predicates | DISSOLVED - TU-local types, reproduced inside their owner `.cpp` (mapping in `temp/triage_log.md`); 9 owner-unknown stay for batch 12 | DONE (analysis) |
| 3 | weapon templates | `weapon_sound_events_handler_state*` (60), `shotgun_weapon_reload_state{,_cook}`, `rifle_scope{,_cook}`, `weapon_sound_effect`, `weapon_cook`, `weapon`, `weapon_user_dead_state` + pulled `fingers_to_weapon_corrector` (value member of `weapon`) - reconstruct the PRIMARY templates (union across `_N` variants, per-instantiation STATIC_SIZE_ASSERTs) | DONE (86d34529) |
| 4 | flash/scaleform | `flash_*` (rest), `scaleform_*`, `swf_input_translator`, `vostok_scaleform_log`, `ui_label`, `text_translator` | |
| 5 | options/menus/keys | `options_*`, `main_menu*`, `lobby_menu*`, `login_menu*`, `chat_tab`, `chat_handler`, `key_*`, `keyboard_key_descr`, `game_action_descr`, `graphic_preset` | |
| 6 | player/stats/economy | `player_*` (non-input), `profile_*`, `account_*`, `npc_stats`, `stats*`, `statistics_item`, `base_point_stats`, `faction_price`, `price_item`, `service_prices`, `slot_def`, `items_compatibility`, `client_player_*`, `human_npc*` | |
| 7 | scene objects | `object_*`, `render_visual` (CHECK), `game_object`, `game_object_static` | |
| 8 | animation | `animations_selector`, `*animation_controller*`, `animation_space_*`, `animations_search_service`, `base_animation_controller`, `animated_model_instance{,_cook}` (`fingers_to_weapon_corrector` moved to batch 3) - PRIORITY: re-instantiates the stripped `mixing_n_ary_tree_*` COMDATs | |
| 9 | cameras/scenes/projects | `game_camera`, `free_fly_camera`, `lobby_camera`, `levelmap_camera`, `camera_director`, `base_game_scene`, `simple_game_project`, `project_cooker_simple`, `generate_shaders_world`, `game_world_ui` | |
| 10 | network-facing | `network_client*`, `base_network_client`, `match_client*`, `lobby_client`, `messaging_client`, `network_packets_orderer`, `circular_buffer_*`, `client_player_history_item` | |
| 11 | world/game cascade | `game` (game.h), `game_world*`, `game_options`, `game_map_description`, `game_material_visibility_parameters`, `global_input_handler`, `player_input_handler` (+nested enum), `step_manager`, `generic_anomaly`, `damage_zone*`, `booby_trap*`, `artefact_container`, `victory_item*`, `empty_hands*`, `ai_collision_object`, `ai_sound_player`, `vostok_file_opener`, `sound_player_cook`, `damage_model_stats`, `first_predicate`, `game__debug_window_enum` | |
| 12 | remaining compilands | every `temp/structure_queue/sources/` file not consumed by 2-11 | |
| 13 | TU enablement | flip `ExcludedFromBuild` leaf-first, drive full build green, refresh `game_entry_point.cpp` statics (s_game etc.), retire interim anchors | |

Batches 2-11 also pull the SAME-STEM files from `temp/structure_queue/sources/`
(`<stem>.cpp`, `<stem>.h`, `<stem>_inline.h`) so a subsystem leaves the queue
whole. Update the Status column (+ commit SHA) when a batch lands.
