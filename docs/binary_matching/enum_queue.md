# Enum work queue (target vs base)

Generated/reconciled by `python3 scripts/enum_diff.py --write-queue`.

This is a PERSISTENT queue of every engine enum (`survarium::` / `vostok::`,
**excluding** `vostok::render::` - render is matched last; see the deferred
bucket at the bottom) whose definition in our base sources does NOT yet match the
original game (the TARGET pdb-parser dump under
`binaries/structure/target/headers`).

## Semantics

* One row per UNHANDLED enum.
* `MISSING` - defined in target, absent from base sources.
* `FIELD-MISMATCH` - present in both, but the enumerator set or values differ.
* An enum is **removed** from this queue once handled (base matches target):
  re-run `--write-queue` and the row drops out.
* `BLOCKED` rows **stay** (persistent) with their cause - we iterate until the
  file is empty. When you add a real block (a missing type/header, a cross-module
  dependency), set Status to `BLOCKED:<cause>` and `--write-queue` preserves it.
* The reconciler preserves the human-authored `Status` (BLOCKED) and the
  `Intended base location` of any row that is still defective; it only drops rows
  that are now handled and appends newly-discovered defects.

Re-derive the live set anytime:

    python3 scripts/enum_diff.py                 # human-readable diff
    python3 scripts/enum_diff.py --write-queue   # reconcile this file


_Live rows: 63 (49 MISSING + 14 FIELD-MISMATCH). Deferred render enums: 31._

| Enum (fully-qualified) | Target location | Intended base location | Status | Diff detail | BLOCKED? + cause |
|---|---|---|---|---|---|
| `survarium::ScaleformLogMessageType` | ScaleformLogMessageType.h (enums/ or owning class header) | sources/vostok/game_core/ScaleformLogMessageType.h (or owning class header) | MISSING | 4 enumerators to add |  |
| `survarium::action_type_enum` | action_type_enum.h (enums/ or owning class header) | sources/vostok/game_core/action_type_enum.h (or owning class header) | MISSING | 32 enumerators to add |  |
| `survarium::animation_additivity_layers` | animation_additivity_layers.h (enums/ or owning class header) | sources/vostok/game_core/animation_additivity_layers.h (or owning class header) | MISSING | 6 enumerators to add |  |
| `survarium::animation_offset_enum` | animation_offset_enum.h (enums/ or owning class header) | sources/vostok/game_core/animation_offset_enum.h (or owning class header) | MISSING | 4 enumerators to add |  |
| `survarium::controllers_options_enum` | controllers_options_enum.h (enums/ or owning class header) | sources/vostok/game_core/controllers_options_enum.h (or owning class header) | MISSING | 3 enumerators to add |  |
| `survarium::game_material_manager_cook::res_type` | res_type.h (enums/ or owning class header) | sources/vostok/game_core/res_type.h (or owning class header) | MISSING | 4 enumerators to add |  |
| `survarium::gameplay_options_enum` | gameplay_options_enum.h (enums/ or owning class header) | sources/vostok/game_core/gameplay_options_enum.h (or owning class header) | MISSING | 10 enumerators to add |  |
| `survarium::keyboard_key_group` | keyboard_key_group.h (enums/ or owning class header) | sources/vostok/game_core/keyboard_key_group.h (or owning class header) | MISSING | 5 enumerators to add |  |
| `survarium::lobby::client_state_enum` | client_state_enum.h (enums/ or owning class header) | sources/vostok/game_core/client_state_enum.h (or owning class header) | MISSING | 5 enumerators to add |  |
| `survarium::messaging::client_state_enum` | client_state_enum.h (enums/ or owning class header) | sources/vostok/game_core/client_state_enum.h (or owning class header) | MISSING | 4 enumerators to add |  |
| `survarium::player_body_parts` | player_body_parts.h (enums/ or owning class header) | sources/vostok/game_core/player_body_parts.h (or owning class header) | MISSING | 9 enumerators to add |  |
| `survarium::scene_ready_type` | scene_ready_type.h (enums/ or owning class header) | sources/vostok/game_core/scene_ready_type.h (or owning class header) | FIELD-MISMATCH | login_scene_ready: base=<absent> target=0x0; lobby_scene_ready: base=<absent> target=0x1; scene_ready_login: base=0x0 target=<absent>; scene_ready_lobby: base=0x1 target=<absent> |  |
| `survarium::sound_options_enum` | sound_options_enum.h (enums/ or owning class header) | sources/vostok/game_core/sound_options_enum.h (or owning class header) | MISSING | 8 enumerators to add |  |
| `survarium::weapon_time_synchronization_group_enum` | weapon_time_synchronization_group_enum.h (enums/ or owning class header) | sources/vostok/game_core/weapon_time_synchronization_group_enum.h (or owning class header) | MISSING | 9 enumerators to add |  |
| `vostok::ai::planning::actions_type_enum` | actions_type_enum.h (enums/ or owning class header) | sources/vostok/ai/... (actions_type_enum - enum file or owning class) | MISSING | 12 enumerators to add |  |
| `vostok::ai::planning::block_location_type_enum` | block_location_type_enum.h (enums/ or owning class header) | sources/vostok/ai/... (block_location_type_enum - enum file or owning class) | MISSING | 2 enumerators to add |  |
| `vostok::ai::planning::parameters_type_enum` | parameters_type_enum.h (enums/ or owning class header) | sources/vostok/ai/... (parameters_type_enum - enum file or owning class) | MISSING | 6 enumerators to add |  |
| `vostok::ai::planning::predicates_type_enum` | predicates_type_enum.h (enums/ or owning class header) | sources/vostok/ai/... (predicates_type_enum - enum file or owning class) | MISSING | 13 enumerators to add |  |
| `vostok::ai::predicate_types_enum` | predicate_types_enum.h (enums/ or owning class header) | sources/vostok/ai/... (predicate_types_enum - enum file or owning class) | FIELD-MISMATCH | predicate_type_sound_played: base=0xa target=0x9; predicate_type_animation_with_sound_played: base=0xc target=0xa; predicate_type_is_at_position: base=0xd target=0xb; predicate_type_animation_playing: base=0x9 target=<absent>; predicate_type_sound_playing: base=0xb target=<absent>; predicate_type_is_moving: base=0xe target=<absent> |  |
| `vostok::animation::animation_event_type_enum` | animation_event_type_enum.h (enums/ or owning class header) | sources/vostok/animation/... (animation_event_type_enum - enum file or owning class) | MISSING | 13 enumerators to add |  |
| `vostok::animation::collection_playback_types` | collection_playback_types.h (enums/ or owning class header) | sources/vostok/animation/... (collection_playback_types - enum file or owning class) | MISSING | 2 enumerators to add |  |
| `vostok::animation::mixing::time_event_types_enum` | time_event_types_enum.h (enums/ or owning class header) | sources/vostok/animation/... (time_event_types_enum - enum file or owning class) | FIELD-MISMATCH | time_event_user_defined: base=<absent> target=0x20; time_event_channel_callback_should_be_fired: base=0x20 target=<absent> |  |
| `vostok::animation::reserved_channel_ids_enum` | reserved_channel_ids_enum.h (enums/ or owning class header) | sources/vostok/animation/... (reserved_channel_ids_enum - enum file or owning class) | FIELD-MISMATCH | channel_id_on_animation_lexeme_end: base=<absent> target=0x3; channel_id_max: base=0x3 target=0x4 |  |
| `vostok::collision::box_plane_types_enum` | box_plane_types_enum.h (enums/ or owning class header) | sources/vostok/collision/... (box_plane_types_enum - enum file or owning class) | MISSING | 6 enumerators to add |  |
| `vostok::collision::cylinder_plane_types_enum` | cylinder_plane_types_enum.h (enums/ or owning class header) | sources/vostok/collision/... (cylinder_plane_types_enum - enum file or owning class) | MISSING | 3 enumerators to add |  |
| `vostok::command_line::key::type_enum` | type_enum.h (enums/ or owning class header) | sources/vostok/command_line/... (type_enum - enum file or owning class) | MISSING | 5 enumerators to add |  |
| `vostok::core::ReplacesCorHdrNumericDefines` | ReplacesCorHdrNumericDefines.h (enums/ or owning class header) | n/a - SDK leak | BLOCKED:sdk-leak | 23 enumerators to add | .NET COR header enum (COMIMAGE_FLAGS_*); also emitted under others/enums - leaked into vostok::core by the type-stream walk, NOT a real engine type. Do not author. |
| `vostok::core::debug_initialization_enum` | debug_initialization_enum.h (enums/ or owning class header) | sources/vostok/core/... (debug_initialization_enum - enum file or owning class) | MISSING | 2 enumerators to add |  |
| `vostok::core::log_flags_enum` | log_flags_enum.h (enums/ or owning class header) | sources/vostok/core/... (log_flags_enum - enum file or owning class) | FIELD-MISMATCH | log_to_stdout: base=<absent> target=0x0 |  |
| `vostok::core::stdstream_enum` | stdstream_enum.h (enums/ or owning class header) | sources/vostok/core/... (stdstream_enum - enum file or owning class) | MISSING | 2 enumerators to add |  |
| `vostok::fs_new::assert_on_fail_bool` | assert_on_fail_bool.h (enums/ or owning class header) | sources/vostok/fs_new/... (assert_on_fail_bool - enum file or owning class) | MISSING | 2 enumerators to add |  |
| `vostok::logging::log_file_usage_enum` | log_file_usage_enum.h (enums/ or owning class header) | sources/vostok/logging/... (log_file_usage_enum - enum file or owning class) | FIELD-MISMATCH | no_log: base=0x3 target=0x0; uninitialized_log: base=0x0 target=<absent> |  |
| `vostok::match_client_message_types_enum` | match_client_message_types_enum.h (enums/ or owning class header) | sources/vostok/... (locate owning class/header) | FIELD-MISMATCH | connection_request: base=<absent> target=0x40; get_startup_info: base=<absent> target=0x41; join_match: base=<absent> target=0x42; client_player_update: base=<absent> target=0x43; client_player_commit_suicide: base=<absent> target=0x44; time_synchronization_request: base=<absent> target=0x45; time_synchronization_confirmation: base=<absent> target=0x46; bullets_info_request: base=<absent> target=0x47; team_bases_initialize_info: base=<absent> target=0x48; force_finish_match: base=<absent> target=0x49; world_synchronization_confirmation: base=<absent> target=0x4a; match_client_invalid_message_type: base=<absent> target=0x7f |  |
| `vostok::match_server_message_types_enum` | match_server_message_types_enum.h (enums/ or owning class header) | sources/vostok/... (locate owning class/header) | FIELD-MISMATCH | match_server_connection_successful: base=<absent> target=0x80; match_options_message_type: base=<absent> target=0x81; server_player_input: base=<absent> target=0x82; kill_player: base=<absent> target=0x83; spawn_player: base=<absent> target=0x84; team_base_capture_progress: base=<absent> target=0x85; match_time_changed: base=<absent> target=0x86; respawn_time_changed: base=<absent> target=0x87; player_kd_stats_changed: base=<absent> target=0x88; hit_player: base=<absent> target=0x89; affect_damage_model: base=<absent> target=0x8a; sync_response: base=<absent> target=0x8b; match_finished: base=<absent> target=0x8c; server_bullet_added: base=<absent> target=0x8d; server_bullet_removed: base=<absent> target=0x8e; server_bullet_moved: base=<absent> target=0x8f; server_bullet_collided: base=<absent> target=0x90; player_visibility_changed: base=<absent> target=0x91; player_profile_message_type: base=<absent> target=0x92; team_bases_message_type: base=<absent> target=0x93; initialize_victory_items: base=<absent> target=0x94; victory_item_take_or_put: base=<absent> target=0x95; trap_placed: base=<absent> target=0x96; trap_removed: base=<absent> target=0x97; trap_fired: base=<absent> target=0x98; trap_disarmed: base=<absent> target=0x99; game_status_changed: base=<absent> target=0x9a; match_wait_time_changed: base=<absent> target=0x9b; game_world_object_state: base=<absent> target=0x9c; world_synchronization_request: base=<absent> target=0x9d; damage_model_state: base=<absent> target=0x9e; match_server_invalid_message_type: base=<absent> target=0xc0 |  |
| `vostok::math::get_frustum_vertices::__l2::frustum_plane_id` | frustum_plane_id.h (enums/ or owning class header) | sources/vostok/math/... (inside get_frustum_vertices body) | BLOCKED:function-local | 6 enumerators to add | Function-LOCAL enum (`__l2` = a lexical block inside get_frustum_vertices); recovered only when that function is matched, not as a header enum. Handle with the function. |
| `vostok::memory::chunk_reader::chunk_type` | chunk_type.h (enums/ or owning class header) | sources/vostok/memory/... (chunk_type - enum file or owning class) | MISSING | 4 enumerators to add |  |
| `vostok::messaging_client_message_types_enum` | messaging_client_message_types_enum.h (enums/ or owning class header) | sources/vostok/... (locate owning class/header) | MISSING | 5 enumerators to add |  |
| `vostok::messaging_server_message_types_enum` | messaging_server_message_types_enum.h (enums/ or owning class header) | sources/vostok/... (locate owning class/header) | MISSING | 5 enumerators to add |  |
| `vostok::network::login_client::client_state_enum` | client_state_enum.h (enums/ or owning class header) | sources/vostok/network/... (client_state_enum - enum file or owning class) | MISSING | 2 enumerators to add |  |
| `vostok::network_core::udp_match_packets_count_enum` | udp_match_packets_count_enum.h (enums/ or owning class header) | sources/vostok/network_core/... (udp_match_packets_count_enum - enum file or owning class) | FIELD-MISMATCH | udp_match_single_packet: base=<absent> target=0x0; udp_match_multiple_packets: base=<absent> target=0x1; udp_match_regular_packet: base=? target=<absent>; udp_match_low_level_packet: base=? target=<absent> |  |
| `vostok::particle::enum_particle_entity_type` | enum_particle_entity_type.h (enums/ or owning class header) | sources/vostok/particle/... (enum_particle_entity_type - enum file or owning class) | FIELD-MISMATCH | particle_action_gravity_type: base=<absent> target=0x20; particle_action_unsupported_type: base=0x20 target=0x21 |  |
| `vostok::physics::collision_filter_groups` | collision_filter_groups.h (enums/ or owning class header) | sources/vostok/physics/... (collision_filter_groups - enum file or owning class) | MISSING | 12 enumerators to add |  |
| `vostok::physics::model_type` | model_type.h (enums/ or owning class header) | sources/vostok/physics/... (model_type - enum file or owning class) | MISSING | 3 enumerators to add |  |
| `vostok::physics::primitive_type` | primitive_type.h (enums/ or owning class header) | sources/vostok/physics/... (primitive_type - enum file or owning class) | MISSING | 6 enumerators to add |  |
| `vostok::platform_pointer_enum` | platform_pointer_enum.h (enums/ or owning class header) | sources/vostok/... (locate owning class/header) | FIELD-MISMATCH | platform_pointer_default: base=0x0 target=0x1 |  |
| `vostok::ppmd_compressor::model_restoration_enum` | model_restoration_enum.h (enums/ or owning class header) | sources/vostok/ppmd_compressor/... (model_restoration_enum - enum file or owning class) | MISSING | 3 enumerators to add |  |
| `vostok::resources::assert_on_fail_bool` | assert_on_fail_bool.h (enums/ or owning class header) | sources/vostok/resources/... (assert_on_fail_bool - enum file or owning class) | MISSING | 2 enumerators to add |  |
| `vostok::resources::class_id_enum` | class_id_enum.h (enums/ or owning class header) | sources/vostok/resources/... (class_id_enum - enum file or owning class) | FIELD-MISMATCH | composite_sound_class: base=0x0 target=0x101; sound_collection_class: base=0x1 target=0x102; weapon_show_state_class: base=0x0 target=0x12d; weapon_hide_state_class: base=0x1 target=0x12e; weapon_idle_state_class: base=0x2 target=0x12f; weapon_reload_state_class: base=0x3 target=0x130; weapon_chamber_a_round_state_class: base=0x4 target=0x131; weapon_chamber_a_round_aimed_state_class: base=0x5 target=0x132; weapon_fire_state_class: base=0x6 target=0x133; weapon_aimed_state_class: base=0x7 target=0x134; weapon_aimed_fire_state_class: base=0x8 target=0x135; weapon_shotgun_reload_state_class: base=0x9 target=0x136; weapon_shotgun_reload_start_substate_class: base=0xa target=0x137; weapon_shotgun_reload_one_substate_class: base=0xb target=0x138; weapon_shotgun_reload_finish_substate_class: base=0xc target=0x139; double_barreled_weapon_show_state_class: base=0xd target=0x13a; double_barreled_weapon_hide_state_class: base=0xe target=0x13b; double_barreled_weapon_idle_state_class: base=0xf target=0x13c; double_barreled_weapon_reload_state_class: base=0x10 target=0x13d; double_barreled_weapon_fire_state_class: base=0x11 target=0x13e; double_barreled_weapon_aimed_state_class: base=0x12 target=0x13f; double_barreled_weapon_aimed_fire_state_class: base=0x13 target=0x140; pistol_weapon_show_state_class: base=0x14 target=0x141; pistol_weapon_hide_state_class: base=0x15 target=0x142; pistol_weapon_idle_state_class: base=0x16 target=0x143; pistol_weapon_reload_state_class: base=0x17 target=0x144; pistol_weapon_fire_state_class: base=0x18 target=0x145; pistol_weapon_aimed_state_class: base=0x19 target=0x146; pistol_weapon_aimed_fire_state_class: base=0x1a target=0x147; composite_animation_class: base=0x0 target=0x201; animation_collection_class: base=0x1 target=0x202; flash_movie_class: base=0x2 target=0x203; game_world_class: base=0x3 target=0x204; last_resource_class: base=0x4 target=0x205; platformed_raw_data_class: base=0x5 target=0x3; converted_model_class: base=0x6 target=0x3; binary_config_class: base=0x7 target=0x22; inherits_binary_config_class: base=0x8 target=0x23; config_class: base=0x9 target=<absent>; spl_class: base=0xa target=<absent>; animation_controller_class: base=0xb target=<absent>; sound_stream_class: base=0xc target=<absent>; game_project_class: base=0xd target=<absent>; game_project_simple_class: base=0xe target=<absent>; game_cell_class: base=0xf target=<absent>; game_object_class: base=0x10 target=<absent>; game_object_scene_class: base=0x11 target=<absent>; sound_player_logic_class: base=0x12 target=<absent>; sound_player_editor_class: base=0x13 target=<absent> |  |
| `vostok::resources::cook_base::result_enum` | result_enum.h (enums/ or owning class header) | sources/vostok/resources/... (result_enum - enum file or owning class) | MISSING | 6 enumerators to add |  |
| `vostok::resources::cook_base::reuse_enum` | reuse_enum.h (enums/ or owning class header) | sources/vostok/resources/... (reuse_enum - enum file or owning class) | MISSING | 3 enumerators to add |  |
| `vostok::resources::fs_task::type_enum` | type_enum.h (enums/ or owning class header) | sources/vostok/resources/... (type_enum - enum file or owning class) | FIELD-MISMATCH | type_fs_iterator: base=<absent> target=0x1; type_mount_physical: base=<absent> target=0x3; type_mount_archive: base=<absent> target=0x4; type_unmount: base=<absent> target=0x5; type_erase_file: base=0x5 target=0x6; type_mount_composite: base=0xc target=0x7; type_mount_operations_end: base=0xd target=0x8; type_fs_iterator_task: base=0x1 target=<absent>; type_update_file_size_in_fat: base=0x3 target=<absent>; type_update_disk_node_name: base=0x4 target=<absent>; type_mount_disk: base=0x6 target=<absent>; type_mount_db: base=0x7 target=<absent>; type_mount_disk_node: base=0x8 target=<absent>; type_unmount_disk: base=0x9 target=<absent>; type_unmount_db: base=0xa target=<absent>; type_unmount_disk_node: base=0xb target=<absent> |  |
| `vostok::resources::mount_by_config_helper::type_enum` | type_enum.h (enums/ or owning class header) | sources/vostok/resources/... (type_enum - enum file or owning class) | MISSING | 2 enumerators to add |  |
| `vostok::resources::query_result_for_user::error_type_enum` | error_type_enum.h (enums/ or owning class header) | sources/vostok/resources/... (error_type_enum - enum file or owning class) | MISSING | 14 enumerators to add |  |
| `vostok::resources::resource_base::creation_source_enum` | creation_source_enum.h (enums/ or owning class header) | sources/vostok/resources/... (creation_source_enum - enum file or owning class) | MISSING | 7 enumerators to add |  |
| `vostok::sound::collection_playback_types` | collection_playback_types.h (enums/ or owning class header) | sources/vostok/sound/... (collection_playback_types - enum file or owning class) | MISSING | 2 enumerators to add |  |
| `vostok::sound::new_sound_propagator::propagation_state` | propagation_state.h (enums/ or owning class header) | sources/vostok/sound/... (propagation_state - enum file or owning class) | MISSING | 3 enumerators to add |  |
| `vostok::sound::processing_stage` | processing_stage.h (enums/ or owning class header) | sources/vostok/sound/... (processing_stage - enum file or owning class) | MISSING | 3 enumerators to add |  |
| `vostok::sound::sound_type` | sound_type.h (enums/ or owning class header) | sources/vostok/sound/... (sound_type - enum file or owning class) | FIELD-MISMATCH | hud: base=<absent> target=0x3; sound_types_count: base=<absent> target=0x4 |  |
| `vostok::sound::speakers` | speakers.h (enums/ or owning class header) | sources/vostok/sound/... (speakers - enum file or owning class) | MISSING | 10 enumerators to add |  |
| `vostok::vfs::assert_on_fail_bool` | assert_on_fail_bool.h (enums/ or owning class header) | sources/vostok/vfs/... (assert_on_fail_bool - enum file or owning class) | MISSING | 2 enumerators to add |  |
| `vostok::vfs::erase_from_hashset_enum` | erase_from_hashset_enum.h (enums/ or owning class header) | sources/vostok/vfs/... (erase_from_hashset_enum - enum file or owning class) | MISSING | 2 enumerators to add |  |
| `vostok::vfs::exchange_nodes_action` | exchange_nodes_action.h (enums/ or owning class header) | sources/vostok/vfs/... (exchange_nodes_action - enum file or owning class) | MISSING | 2 enumerators to add |  |
| `vostok::vfs::vfs_iterator::type_enum` | type_enum.h (enums/ or owning class header) | sources/vostok/vfs/... (type_enum - enum file or owning class) | MISSING | 5 enumerators to add |  |

## Deferred: render-module enums (`vostok::render::`)

Render is matched last; these are tracked but not part of the live queue above. Re-run with `--include-render` to diff them.

| Enum | Status |
|---|---|
| `vostok::render::D3D11_CULL_MODE` | MISSING |
| `vostok::render::D3D_FEATURE_LEVEL` | MISSING |
| `vostok::render::D3D_PRIMITIVE_TOPOLOGY` | MISSING |
| `vostok::render::DXGI_FORMAT` | MISSING |
| `vostok::render::_D3DDECLTYPE` | MISSING |
| `vostok::render::_D3DDECLUSAGE` | MISSING |
| `vostok::render::_D3D_CBUFFER_TYPE` | MISSING |
| `vostok::render::enum_geometry_type` | FIELD-MISMATCH |
| `vostok::render::enum_options_changes_result` | MISSING |
| `vostok::render::enum_render_stage_type` | FIELD-MISMATCH |
| `vostok::render::enum_render_target_index` | MISSING |
| `vostok::render::enum_uro_ambient_occlusion_quality_values` | MISSING |
| `vostok::render::enum_uro_anisotrophic_filter_values` | MISSING |
| `vostok::render::enum_uro_antialiasing_method_values` | MISSING |
| `vostok::render::enum_uro_decorations_quality_values` | MISSING |
| `vostok::render::enum_uro_geometry_quality_values` | MISSING |
| `vostok::render::enum_uro_graphics_quality_values` | MISSING |
| `vostok::render::enum_uro_lighting_quality_values` | MISSING |
| `vostok::render::enum_uro_motion_blur_quality_values` | MISSING |
| `vostok::render::enum_uro_particles_quality_values` | MISSING |
| `vostok::render::enum_uro_post_process_quality_values` | MISSING |
| `vostok::render::enum_uro_shading_quality_values` | MISSING |
| `vostok::render::enum_uro_shadow_quality_values` | MISSING |
| `vostok::render::enum_vertex_input_type` | FIELD-MISMATCH |
| `vostok::render::light_type` | FIELD-MISMATCH |
| `vostok::render::mesh_type_enum` | FIELD-MISMATCH |
| `vostok::render::model_chunk_enum` | FIELD-MISMATCH |
| `vostok::render::render_surface_instance_flags` | MISSING |
| `vostok::render::scene_view_mode` | FIELD-MISMATCH |
| `vostok::render::stage_atmosphere::stage_type` | MISSING |
| `vostok::render::stage_forward::stage_type` | MISSING |
