<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Game inline interface and scene-switch consumers

PR 571 source-only audit of all **34 in-class/inline methods in `game.h`**,
plus the existing `game_world::test_action_portal_system` declaration needed
by the game-level forwarding model. This is 35 reviewed entries, not 35 newly
verified implementations. Thirty game markers and one world marker are removed;
`query_render_window(bool)` remains individually open. No build or score refresh.

## Individual register

| # | Function | Disposition and evidence |
|---:|---|---|
| 1 | `ui_world` | Existing virtual getter verified directly at retail RVA 0x7d920: load game+0x90, return. Keep dereferenced pointer/reference semantics. |
| 2 | `input_world` | Existing virtual getter verified directly at RVA 0x7d930: load game+0x8c, return. |
| 3 | `game_time_ms` | Existing unmarked cached u32 getter. read_ping_server_answer reads game+0x3f4 at +0x15; tick updates it from the pausable timer. Do not replace each getter with a fresh clock sample. |
| 4 | `game_permanent_time_ms` | Existing cached u32 getter. free_fly_camera::tick reads game+0x3e8 at +0x1e. This is the separately sampled permanent clock, not game+0x3f4. |
| 5 | `game_time_sec` | Replace zero placeholder with float(game_time_ms()) * math::epsilon_3 as an explicit cached-clock model. No original consumer distinguishes cached milliseconds from a fresh timer sample or multiplication from division. Preserve the PDB's non-const method; keep the question open. |
| 6 | `last_frame_time` | Existing float getter at +0x3ec verified in player::update_speed_info (+0x95/+0x17a), also used by NPC foot adjustment. Despite the name, update_stats stores elapsed seconds since the first permanent frame, not a per-frame delta. |
| 7 | `input_translator` | Existing value-member reference at +0x3c0, verified by base_game_scene::input_translator's load/add/return. Keyboard consumers retain this interface. |
| 8 | `get_game_world() const` | Keep existing const reference to the +0x98 value member. PDB overload/CV and storage agree; inspected ordinary mutable consumers do not independently identify this const overload. |
| 9 | `get_game_world()` | Existing mutable reference verified by network_client::get_game_world's +0x98 address return and game_options::activate's scene comparison. Not a resource-pointer copy. |
| 10 | `get_game_options` | Existing value-member reference at +0x81c, verified by options_monitor_index_selector::refill_resolutions_data (+0x19). |
| 11 | `get_sound_world` | Existing stored reference at +0x84, verified by game_world::get_sound_world and NPC construction; also present in legacy game source. Const game still returns mutable sound world as declared. |
| 12 | `get_network_world` | Existing stored reference at +0x88. Network-client construction uses it at +0x1e/+0x3a/+0x8a for login/match/HTTP ownership. Legacy source agrees. |
| 13 | `get_key_binder` | Existing dereferenced pointer at +0x78. game_options::process_key_input loads it at +0x1e before key-name lookup. |
| 14 | `get_chat_handler` | Existing dereferenced pointer at +0x37c, verified in game_world::show_ui at +0x3f/+0x72 and real chat display/focus consumers. |
| 15 | `engine` | Existing reference at +0x7c, verified in lobby_camera::on_mouse_move at +0x48 before virtual get_render_window_size. Legacy source agrees. |
| 16 | `render_world` | Keep existing reference to +0x80. PDB and the legacy getter agree; no separate named getter consumer was found in the current game/game_core sources. This does not establish that it was editor-only or absent originally. |
| 17 | `get_stats` | Existing dereferenced pointer at +0x74, verified in player::update_speed_info (+0xf3/+0x1e7). Legacy source agrees. |
| 18 | `renderer` | Existing stored reference at +0x94, verified in base_game_scene::renderer. Preserve renderer ownership; do not return a scene renderer instead. |
| 19 | `render_output_window` | Existing resource-pointer return **by value** at +0x58. base_game_scene::output_window_size performs the temporary's refcount increment and decrement; tick also copies it for draw_scene. Changing to const reference would drop real operations. |
| 20 | `viewport` | Existing const rectangle reference at +0x5c, passed by address at tick+0x225. Legacy declaration/body agree. Unlike the output window, this is not an owning value copy. |
| 21 | `main_menu` | Keep existing pointer dereference at +0x370. PDB declares main_menu&, and scene-switch/activation code uses the same object. No separate named accessor consumer found; do not invent a new call to make it observable. |
| 22 | `lobby_menu` | Existing pointer dereference at +0x374, verified in read_ping_server_answer (+0x35) and numerous packet/UI consumers. |
| 23 | `login_menu` | Keep existing pointer dereference at +0x378. PDB declares login_menu&, and switch_to_login uses that object. No separate named accessor consumer found; no extra activation side effect belongs in this getter. |
| 24 | `text_translator` | Existing reference to +0x3dc, verified in game_world_ui::initialize (+0x33/+0x80). Keep stored translator state. |
| 25 | `items_dictionary` | Existing dereferenced resource at +0x3b4, verified in game_world_ui::on_player_killed (+0xa8), with player/weapon cook consumers. Const game returns the declared mutable dictionary reference, not an owning copy. |
| 26 | `active_scene` | Existing raw pointer at +0x3ac, verified in options_gamma_selector::revert (+0x1b). It can name a menu or world; do not replace it with the world member. |
| 27 | `scheduler` | Existing reference to +0x380, verified by base_game_scene::scheduler's load/add/return and victory-item scheduling consumers. |
| 28 | `current_frame_id` | Keep existing u32 getter at +0x3f0. Tick's +0x89 store comes from its frame-ID argument, independently of timer fields. No named getter consumer found; not a reason to delete the method or return a clock. |
| 29 | `permanent_timer` | Existing const reference to timer +0x28. send_sync_request adds 0x28 and samples it; process_sync_response does the same. These need fresh permanent elapsed time, unlike the cached permanent-time getter. |
| 30 | `test_action_portal_system` | Replace empty body with forwarding to the existing game_world helper. Explicit model: the matching name/scene ownership suggest this route, but the original invocation, active-scene selection and loading guard are not established. No key/console binding added. |
| 31 | `get_network_client` | Existing pointer at +0x3b8, verified by game_world::on_keyboard_action (+0x115/+0x138) and HUD consumers. Preserve actual virtual client dispatch. |
| 32 | `project_resource_name` | Existing fixed-string c_str read through +0x610, verified in lobby_menu::update_level_loading_progress (+0x130). Return the buffer's stored begin pointer, not an assumed inline buffer address. |
| 33 | `query_render_window(bool)` | OPEN: a window query/configuration helper is the semantic hypothesis, but bool meaning and callback/recreation ownership remain unknown. on_configs_loaded records its configuration locals itself; do not extract that whole live block to populate this method. See below. |
| 34 | `toggle_debug_window` | Restore the legacy none/resources/tasks/none conditional cycle. Retail retains the enum, initialized state, draw dispatch and separately owned window. Source-prior model, not a verified original call/inline expansion. |
| 35 | `game_world::test_action_portal_system` | Model forwarding its render_scene() through existing scene_renderer().test_action_portal_system. Use the complete facade header; no fabricated helper or renderer edit. Original invocation, guard and precise forwarding spelling remain open. The rest of game_world is a separate unfinished audit. |

## Public scene interface replaces invented friendship

Retail `network_client::connect_to_login` (RVA 0x6f4af0) has a spectator-path
statement at +0x1e2 that obtains game+0x98, compares it with active_scene at
+0x3ac, conditionally deactivates the old scene through slot +4, stores the
world and activates it through slot +0. The public
`game::switch_to_game_world` (RVA 0x5d5840) performs those same operations.
Restore `m_game.switch_to_game_world()` at this consumer instead of calling the
private generic switch and granting friendship solely to permit it. The target
statement has no added caller local. A future build must check its source/call
boundary; the equivalent expanded operation does not prove original spelling.
The frozen two-sided structure has eight statements on both sides, 0x299
target bytes versus 0x278 base bytes; the scene-switch statement is 0x30 versus
0x2c. The remaining sign-in statement also differs. These are pre-edit
measurements, not a new result, and current source-line text printed against
old base line records must not be mistaken for the compiled source snapshot.

The other `friend class game_world` was justified by direct writes to
`hide_game_stats`, but retail and source both make that field public at +0xb.
`game_world::update_npc_stats` writes it through get_game(); it requires no
friendship. Searches of both classes' implementation files found no other
private game-member access or private-base conversion needing either grant.
Neither removal changes PDB member visibility to make source compile.

`pdb_topology --classes --class survarium::game --show-identical` reports the
same one semantic variant on each frozen side, size 0x878 and 150 declarations.
Retail TI 0x939a occurs once; base has two equal records, 0x6c2e/0x1c3cc. This
is record multiplicity, not an access/layout mismatch or whole-PDB equality.
Generated base-class access is still subject to the separately documented
renderer bug; keep the actual private input/UI/physics/Scaleform bases.

## Clock and debug-window models

Tick samples the pausable timer at +0x10 into +0x3f4, separately samples the
permanent timer at +0x28 into +0x3e8, and writes its frame argument to +0x3f0.
`update_stats` at RVA 0x5d60b0 subtracts first permanent frame time +0x3e0,
converts to seconds, and stores that coordinate to +0x3ec. Its reciprocal uses
the difference between the previous and current coordinates. Preserve the
apparently odd existing last_frame_time getter, including its NPC consumer.
The new game_time_sec body only models conversion of cached game time; it is
not evidence of a live zero-time bug or proof of the original clock choice.

`git show 885e1d4a4:temp/game_legacy/game.cpp` contains the complete
toggle_debug_window body at lines 881–889. The enum and state match current
retail, whose tick draws only when type != none and the console is inactive.
Window creation/destruction are owned by resource startup/cleanup, not each
toggle. Reuse that source body without inventing allocation or visibility
changes. Retail global_input_handler::on_keyboard_action only handles console
toggle and Alt-F4; the checked game console registration has no debug/portal
binding. These observations leave their original activation unresolved; they
do not establish editor-only ownership.

## Why the render-window helper stays open

Retail `on_configs_loaded` at RVA 0x5d7760 records eight named locals:
result, window_data, data[1], window_configuration, output_window_data,
requests[1], resolution_xy and fullscreen_command. Its actual code fills the
window configuration and queries game_render_output_window with
on_render_output_window_created. Preserve those locals and the query in that
owner. The unused-looking output_window_data is also recorded; do not drop it.

The nearby bool in `load_cc_script` really means create_renderer: the true
branch creates the separate renderer resource using renderer.engine_world and
on_renderer_created. `on_fullscreen_alttab(bool)` instead calls the existing
output window's fullscreen operation and does not query a window. Neither
proves what query_render_window's bool meant. Candidate hypotheses include
window-mode versus Flash-renderer creation/recreation control; a retained
consumer or sibling original implementation must distinguish them before
wiring a new async query/callback sequence into live startup.

For the portal models, the existing render facade accepts a base_scene_ptr and
queues the engine-world portal action. No named facade procedure was found by
the current sema query, so source API and corresponding scene ownership are
the evidence, not a claimed direct retail xref. No render implementation is
changed. Both model anchors remain TODOs rather than erasing this evidence gap.

## Deferred validation

No C++/tool build and no ledger/README regeneration. Static review checks the
public switch signature, complete portal facade declaration, existing scene
accessors, overload/return ownership and absence of duplicate new methods.
Later validation must compile all game.h/game_world.h consumers and inspect
the spectator switch's structure/expansion. The new unreferenced models are
not byte-proven merely because they may compile out. Do not add reachability
anchors or new input bindings to manufacture that proof.
