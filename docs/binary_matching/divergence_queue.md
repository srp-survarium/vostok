# Structure-divergence queue — goal: drive `pdb_divergence` to zero

Authoritative structure verification compares our **base** gold PDB against the
**target** `survarium.pdb` straight off both type streams — self-comparison is 0
divergences (no false positives), unlike `layout_diff.py` which both over- and
under-reported. This is the worklist; re-running the tool is the scoreboard.

Tool: `pdb_divergence` (vostok-pdb-parser, branch `structure-builder-all-enums+scaleform`,
flake-pinned). Run from the parser clone:

    nix develop --command cargo run --release --quiet --bin pdb_divergence -- \
      --base-pdb   <repo>/binaries/Win32/survarium-dx11-win32-gold.pdb \
      --base-engine-path   'z:\home\...\<worktree>\sources\'   # lowercased Wine path, trailing \
      --target-pdb $SURVARIUM_BIN/survarium.pdb \
      --target-engine-path 'c:\survarium\sources' \
      --skip bullet --skip opcode --skip stlport --skip vorbis --skip ogg --skip zlib \
      --skip render --skip sound --skip scaleform --skip flash

Categories — headers: `[size]` (instance size / STATIC_SIZE_ASSERT), `[member]`
(type/offset/one-sided/reorder), `[fn-order]` (member-fn decl order), enum
`[values]`/`[underlying]`. Sources (joined by engine-relative path): `[fn-order]`
(definition order, LCS relative), `[stmt]` (per-fn statement count), `[const]`
(constants by type+value; rename = misname). Use the base PDB from a FRESH tip
build (a stale base shows already-fixed types).

**Deferred (skipped above):** render (matched last), sound (dedicated rewrite),
scaleform / `flash_*` / `Scaleform::` (vendor GFx), third-party
(bullet/opcode/stlport/vorbis/ogg/zlib).

## Scoreboard — tip 70d369b5e, 2026-06-25

| category | count |
|---|---|
| [size]   | 42 (26 our-type) |
| [member] | 142 |
| [fn-order] | 630 |
| [stmt]   | 616 |
| [values] (enum) | 15 |
| [const]  | 4 |

(Raw first run, no deferral skips, was 322 / 452 / 1104 / 971; the drop is the
skips + the integrated fixes engine_world / ui×4 / base_particle / brain_unit_cook_params.)

## Open `[size]` layout levers (our types) — each can ripple like engine_world(+38)/ui(+35)

- **survarium scene/menu:** free_fly_camera, game_scene, main_menu, main_menu_external_handler, object_light, object_particle_visual
- **ai:** ai_world, planning::search, planning::search_base (+ vertex_allocator_impl_type)
- **network:** network_world, match_client_impl · **network_core:** udp_match_client / _client_session / _connection
- **resources:** resources_manager · **vfs:** fat_node_info, pack_archive_args, patch_args, save_archive_args · **tasks:** task_allocator, task_manager
- **misc:** animation::skeleton_animation_cook, game_test_suite

## Enum `[values]` divergences (overlaps `enum_queue.md`)

keyboard_key_group, scene_ready_type, ai::predicate_types_enum,
animation::mixing::time_event_types_enum, animation::reserved_channel_ids_enum,
core::log_flags_enum, input::mouse_button, login_client/match_client/match_server
message_types_enum, network_core::udp_match_packets_count_enum,
particle::enum_particle_entity_type, resources::class_id_enum.

## Notes

- `[fn-order]` is dominated by top-level-`const`-on-value-param spelling diffs
  (target `const T`, base `T`) — byte-neutral but a real divergence; sweep them
  (see the const-param sweep). `uninitialized_reference<T>` `[size]` rows are
  derivative — they track the wrapped type, fix the wrapped type and they follow.
- Drop a row when a re-run shows the type at 0 divergences.
