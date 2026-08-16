# Enum work queue (target vs base)

Generated/reconciled by `python3 -m vostok diff enums --write-queue`.

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

    python3 -m vostok diff enums                 # human-readable diff
    python3 -m vostok diff enums --write-queue   # reconcile this file


_Live rows: 22 (18 MISSING + 4 FIELD-MISMATCH). Deferred render enums: 15._

| Enum (fully-qualified) | Target location | Intended base location | Status | Diff detail | BLOCKED? + cause |
|---|---|---|---|---|---|
| `survarium::lobby::client_state_enum` | client_state_enum.h (enums/ or owning class header) | sources/vostok/game_core/client_state_enum.h (or owning class header) | MISSING | 5 enumerators to add |  |
| `survarium::messaging::client_state_enum` | client_state_enum.h (enums/ or owning class header) | sources/vostok/game_core/client_state_enum.h (or owning class header) | MISSING | 4 enumerators to add |  |
| `vostok::animation::collection_playback_types` | collection_playback_types.h (enums/ or owning class header) | sources/vostok/animation/... (collection_playback_types - enum file or owning class) | MISSING | 2 enumerators to add |  |
| `vostok::command_line::key::type_enum` | type_enum.h (enums/ or owning class header) | sources/vostok/command_line/... (type_enum - enum file or owning class) | MISSING | 5 enumerators to add |  |
| `vostok::fs_new::assert_on_fail_bool` | assert_on_fail_bool.h (enums/ or owning class header) | sources/vostok/fs_new/... (assert_on_fail_bool - enum file or owning class) | MISSING | 2 enumerators to add |  |
| `vostok::memory::chunk_reader::chunk_type` | chunk_type.h (enums/ or owning class header) | sources/vostok/memory/... (chunk_type - enum file or owning class) | MISSING | 4 enumerators to add |  |
| `vostok::network::login_client::client_state_enum` | client_state_enum.h (enums/ or owning class header) | sources/vostok/network/... (client_state_enum - enum file or owning class) | MISSING | 2 enumerators to add |  |
| `vostok::ppmd_compressor::model_restoration_enum` | model_restoration_enum.h (enums/ or owning class header) | sources/vostok/ppmd_compressor/... (model_restoration_enum - enum file or owning class) | MISSING | 3 enumerators to add |  |
| `vostok::resolve_error_types_enum` | resolve_error_types_enum.h (enums/ or owning class header) | sources/vostok/... (locate owning class/header) | FIELD-MISMATCH | successfully_resolved: base=0x0 target=<absent>; cannot_resolve: base=0x1 target=<absent>; resolve_error_type: base=0xff target=<absent> |  |
| `vostok::resources::assert_on_fail_bool` | assert_on_fail_bool.h (enums/ or owning class header) | sources/vostok/resources/... (assert_on_fail_bool - enum file or owning class) | MISSING | 2 enumerators to add |  |
| `vostok::resources::class_id_enum` | class_id_enum.h (enums/ or owning class header) | sources/vostok/resources/... (class_id_enum - enum file or owning class) | FIELD-MISMATCH | config_class: base=0x20a target=<absent>; spl_class: base=0x20b target=<absent>; animation_controller_class: base=0x20c target=<absent>; sound_stream_class: base=0x20d target=<absent>; game_project_class: base=0x20e target=<absent>; game_project_simple_class: base=0x20f target=<absent>; game_cell_class: base=0x210 target=<absent>; game_object_class: base=0x211 target=<absent>; game_object_scene_class: base=0x212 target=<absent>; sound_player_logic_class: base=0x213 target=<absent>; sound_player_editor_class: base=0x214 target=<absent> |  |
| `vostok::resources::cook_base::result_enum` | result_enum.h (enums/ or owning class header) | sources/vostok/resources/... (result_enum - enum file or owning class) | MISSING | 6 enumerators to add |  |
| `vostok::resources::cook_base::reuse_enum` | reuse_enum.h (enums/ or owning class header) | sources/vostok/resources/... (reuse_enum - enum file or owning class) | MISSING | 3 enumerators to add |  |
| `vostok::resources::fs_task::type_enum` | type_enum.h (enums/ or owning class header) | sources/vostok/resources/... (type_enum - enum file or owning class) | FIELD-MISMATCH | type_fs_iterator: base=<absent> target=0x1; type_mount_physical: base=<absent> target=0x3; type_mount_archive: base=<absent> target=0x4; type_unmount: base=<absent> target=0x5; type_erase_file: base=0x5 target=0x6; type_mount_composite: base=0xc target=0x7; type_mount_operations_end: base=0xd target=0x8; type_fs_iterator_task: base=0x1 target=<absent>; type_update_file_size_in_fat: base=0x3 target=<absent>; type_update_disk_node_name: base=0x4 target=<absent>; type_mount_disk: base=0x6 target=<absent>; type_mount_db: base=0x7 target=<absent>; type_mount_disk_node: base=0x8 target=<absent>; type_unmount_disk: base=0x9 target=<absent>; type_unmount_db: base=0xa target=<absent>; type_unmount_disk_node: base=0xb target=<absent> |  |
| `vostok::resources::mount_by_config_helper::type_enum` | type_enum.h (enums/ or owning class header) | sources/vostok/resources/... (type_enum - enum file or owning class) | MISSING | 2 enumerators to add |  |
| `vostok::resources::query_result_for_user::error_type_enum` | error_type_enum.h (enums/ or owning class header) | sources/vostok/resources/... (error_type_enum - enum file or owning class) | MISSING | 14 enumerators to add |  |
| `vostok::resources::resource_base::creation_source_enum` | creation_source_enum.h (enums/ or owning class header) | sources/vostok/resources/... (creation_source_enum - enum file or owning class) | MISSING | 7 enumerators to add |  |
| `vostok::sound::collection_playback_types` | collection_playback_types.h (enums/ or owning class header) | sources/vostok/sound/... (collection_playback_types - enum file or owning class) | MISSING | 2 enumerators to add |  |
| `vostok::sound::new_sound_propagator::propagation_state` | propagation_state.h (enums/ or owning class header) | sources/vostok/sound/... (propagation_state - enum file or owning class) | MISSING | 3 enumerators to add |  |
| `vostok::sound::sound_type` | sound_type.h (enums/ or owning class header) | sources/vostok/sound/... (sound_type - enum file or owning class) | FIELD-MISMATCH | hud: base=<absent> target=0x3; sound_types_count: base=<absent> target=0x4 |  |
| `vostok::vfs::assert_on_fail_bool` | assert_on_fail_bool.h (enums/ or owning class header) | sources/vostok/vfs/... (assert_on_fail_bool - enum file or owning class) | MISSING | 2 enumerators to add |  |
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
| `vostok::render::enum_constant_class` | FIELD-MISMATCH |
| `vostok::render::enum_options_changes_result` | MISSING |
| `vostok::render::enum_render_stage_type` | FIELD-MISMATCH |
| `vostok::render::enum_vertex_input_type` | FIELD-MISMATCH |
| `vostok::render::light_type` | FIELD-MISMATCH |
| `vostok::render::scene_view_mode` | FIELD-MISMATCH |
| `vostok::render::stage_atmosphere::stage_type` | MISSING |
| `vostok::render::stage_forward::stage_type` | MISSING |
