# game carcass rebuild - batch plan

Working agreement for the queue grind (`temp/structure_queue/`). One agent per
batch, SERIAL (every batch touches `game.vcproj` + `temp_include_all.cpp`).
Per batch: reproduce each file (headers + same-stem `*_inline.h`), wire headers
into `temp_include_all.cpp` + a vcproj `<File>` entry, `git rm` from the queue,
`vostok build game` green, ONE commit. Queue `.cpp` compilands are reproduced
with their subsystem but added to the vcproj `ExcludedFromBuild="true"` for
every configuration - TU enablement is a separate leaf-first pass at the end
(an enabled-but-uncompilable TU breaks the build for everyone).

Method: [../library_carcass_rebuild.md](../library_carcass_rebuild.md).
Module notes: [README.md](README.md). Legacy priors: `git show 3320ded27:temp/game_legacy/<file>`.

| # | Batch | Scope (queue stems) | Status |
|--|-------|---------------------|--------|
| 1 | enums | 10 standalone survarium enums | DONE (0732995a) |
| 2 | predicates | DISSOLVED - TU-local types, reproduced inside their owner `.cpp` (mapping in the triage log, `git show 3320ded27:temp/triage_log.md`); 9 owner-unknown stay for batch 12 | DONE (analysis) |
| 3 | weapon templates | `weapon_sound_events_handler_state*` (60), `shotgun_weapon_reload_state{,_cook}`, `rifle_scope{,_cook}`, `weapon_sound_effect`, `weapon_cook`, `weapon`, `weapon_user_dead_state` + pulled `fingers_to_weapon_corrector` (value member of `weapon`) - reconstruct the PRIMARY templates (union across `_N` variants, per-instantiation STATIC_SIZE_ASSERTs) | DONE (51951c87) |
| 4 | flash/scaleform | `flash_*` (rest), `scaleform_*`, `swf_input_translator`, `vostok_scaleform_log`, `ui_label`, `text_translator` | DONE (8e7765dc) |
| 5 | options/menus/keys | `options_*`, `main_menu*`, `lobby_menu*`, `login_menu*`, `chat_tab`, `chat_handler`, `key_*`, `keyboard_key_descr`, `game_action_descr`, `graphic_preset` + pulled `base_game_scene.h` (base of the menu scenes; its compiland stays for batch 9) | DONE (0fe95219) |
| 6 | player/stats/economy | `player_*` (incl. `player_input_handler` + nested enum, pulled from batch 11), `profile_*`, `account_*`, `npc_stats`, `stats*`, `statistics_item`, `base_point_stats`, `faction_price`, `price_item`, `service_prices`, `slot_def`, `items_compatibility`, `client_player_*`, `human_npc*` (+ TU-local `hit_object` in human_npc.cpp) + pulled headers `game_object_.h`/`object_weapon.h` (batch 7: human_npc base / npc_game_attributes member-pointer), `game_camera.h` (batch 9: player_input_handler base), `game_material_visibility_parameters.h` (batch 11: human_npc value member) - their compilands stay queued | DONE (a5f400d0) |
| 7 | scene objects | `object_*`, `render_visual` (CHECK resolved: name collision with vostok::render, distinct survarium type - reproduced; its insert/remove bodies stay queued in batch 9's game_project.cpp), `game_object_` inline carcass folded (sources/object.h), `game_object_static` | DONE (this commit) |
| 8 | animation | `animations_selector`, `*animation_controller*`, `animation_space_*`, `animations_search_service`, `base_animation_controller`, `animated_model_instance{,_cook}` (`fingers_to_weapon_corrector` moved to batch 3) - PRIORITY: re-instantiates the stripped `mixing_n_ary_tree_*` COMDATs. Pulled the ai/search `vertex_allocator::fixed_count::impl` canonical layout refresh (shipped impl stores `m_allocator`, 0x10 - ai's `search_base` + ai_navigation's `path_find_channel` wrappers updated with it) | DONE (fae2d846) |
| 9 | cameras/scenes/projects | `game_camera`, `free_fly_camera`, `lobby_camera`, `levelmap_camera`, `camera_director`, `base_game_scene`, `simple_game_project` (+ nested `loading`; lives in `game_project.{h,cpp}` per the PDB compiland - carries `render_visual::insert/remove` too), `project_cooker_simple`, `generate_shaders_world` (lives in `game_generate_shaders.{h,cpp}`), `game_world_ui` (`_1` variant identical) | DONE (103283c9) |
| 10 | network-facing | `network_client*`, `base_network_client`, `match_client*`, `lobby_client`, `messaging_client`, `network_packets_orderer`, `circular_buffer_*`, `client_player_history_item` + pulled the rebuilt vostok::network public client headers (login/match/tcp_packet/http) and login_server/constants.h verbatim from worktree-network-carcass (value members need them; identical blobs merge clean at integration) | DONE (e749af1b) |
| 11 | world/game cascade | `game` (game.h), `game_world*`, `game_options`, `game_map_description`, `game_material_visibility_parameters` (header landed with batch 6), `global_input_handler`, `step_manager`, `generic_anomaly`, `damage_zone*`, `booby_trap*`, `artefact_container`, `victory_item*`, `empty_hands*`, `ai_collision_object`, `ai_sound_player`, `vostok_file_opener` (deferred to batch 12 - its methods live in the missing scaleform module), `sound_player_cook`, `damage_model_stats`, `first_predicate`, `game__debug_window_enum` | DONE (a5e3096c) |
| 12 | scaleform module | NEW: `vostok/scaleform` - the one module the structure generator never emitted (parser gap; queue = `temp/structure_queue/sources_scaleform/`, symbol lists from the rich index). Scaffold `sources/vostok/scaleform/` (vcproj/pch/api), move the flash/scaleform headers there from game/sources, declarations in headers + definitions in the 10 compilands. Wired via a `game_core -> scaleform` `ProjectDependencies` edge in the .sln (mirrors `game_core -> network_core`) so vcproj2ninja emits the lib; only pch.cpp builds, all 10 TUs `ExcludedFromBuild`. The d3d1x_* HAL family is a fork of a NEWER Scaleform SDK than the vendored 4.0.15 - reproduced as documentation, but its headers are reached by no enabled TU (only their own excluded .cpp), so the SDK gap stays contained until TU enablement. | DONE (1bcb53f5) |
| 13 | remaining compilands | every `temp/structure_queue/sources/` file not consumed by 2-11 + the 9 owner-unknown TU-locals | DONE (2972258c) - 9 orphan predicates triaged out (pass 6); header queue empty |
| 14 | TU enablement | ALL 144 game compilands un-`ExcludedFromBuild` for Master Gold; 31 needed buildability fixes (3 serial waves A/B/C, see [enablement_recipe.md](enablement_recipe.md)) - init-list placeholders, intrusive_ptr complete-type includes, signature/override drift, 1 brace bug, plus a missing `class bullet;` fwd-decl in game_core's bullet_manager_engine.h. Module compiles clean, links, and the **full survarium.exe links green** - all 141 game units now MEASURED (0% addressed stubs, paired against targets) instead of excluded. `game_entry_point.cpp` real-form refresh (s_game) + interim-anchor retirement deferred into the matcher loop. | DONE (waves 3e012a87 / 8439f42a / 7a94bb1b) |

Batches 2-11 also pull the SAME-STEM files from `temp/structure_queue/sources/`
(`<stem>.cpp`, `<stem>.h`, `<stem>_inline.h`) so a subsystem leaves the queue
whole. Update the Status column (+ commit SHA) when a batch lands.

## CARCASS REBUILD COMPLETE (2026-06-13)

Both acceptance gates met: `temp/structure_queue/` is EMPTY (every canonical
class/struct/enum/carcass reproduced or triaged) and no addressless `FUNCTION
BODY` remains in `sources/vostok/game/sources/`. The game module's entire type
skeleton is real, compilable C++; `python3 -m vostok build` is green; the
new `vostok/scaleform` module is scaffolded. ~144 game compilands (+10
scaleform) sit `ExcludedFromBuild` as addressed stub carcasses, ready for the
matcher loop (batch 14). `game_entry_point.cpp` keeps its interim form (returns
NULL, `create_world` carcass preserved in comments) because the real `s_game`
instantiation needs `game::game` from the still-excluded `game.cpp` - that
unblocks only as enablement proceeds.

Interim devices still in place (retire during/after batch 14):
- `temp_include_all.cpp` world-cone anchors (`use_engine_user_world_cone`, the
  `use_network_core_*` block + its CRT-ban `#undef` pair) and `IncludeAll
  temp_anchor;` in `create_world` - keep the engine_user cone alive under
  /OPT:REF; removed when the rebuilt game TUs reference those subsystems for real.
- `temp/game_legacy/` - body-reference priors; deleted from the tree, retrievable
  with `git show 3320ded27:temp/game_legacy/<file>`.
