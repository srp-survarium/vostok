# Structure-divergence queue — goal: drive `pdb_divergence` to zero

Authoritative structure verification compares our **base** gold PDB against the
**target** `survarium.pdb` straight off both type streams — self-comparison is 0
divergences (no false positives), unlike `vostok diff layout` which both over- and
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

- The retail-vs-base record-name STYLE divergence (`T<enum E,...>` and
  `const `-with-trailing-space in retail, bare in base) is a Wine artifact,
  mechanism proven and fixed build-side — see
  `patterns/pdb-names-undname-crt-wine.md`. The pdb-parser canon
  normalization that fenced it becomes redundant once builds run on the
  native msvcr90.
- `[fn-order]` is dominated by top-level-`const`-on-value-param spelling diffs
  (target `const T`, base `T`) — byte-neutral but a real divergence; sweep them
  (see the const-param sweep). `uninitialized_reference<T>` `[size]` rows are
  derivative — they track the wrapped type, fix the wrapped type and they follow.
- Drop a row when a re-run shows the type at 0 divergences.
- **Before believing a `[member]` flag, confirm against the emitted asm — some are
  PHANTOM** (target carries a duplicate `_1` type record, OR the base PDB is stale
  from incremental relink). See `patterns/divergence-phantom-duplicate-type-record.md`.
  `report.json` is authoritative for "did my fix land"; `pdb_divergence` is
  trustworthy only after a CLEAN RELINK (`rm` exe+pdb, rebuild). Confirmed-phantom
  `[member]` flags (do NOT re-chase): network_core `udp_match_connection`/session/
  server/`udp_match_client` `sequence_number<u8>`/`0xB20`, network `match_client_impl`
  `boost::array<...,2048>` — base is asm-correct.

## Member-levers sweep (tip 396d3dbe, 2026-06-25 — match/member-levers-2)

FIXED (clean relink confirms 0 divergences + report.json wins, 0 regressions):
- `particle::particle_system_lod` — field REORDER (`m_parent` first @0x0) — particle_system
  load_lod_actions_binary/index_to_action/load_binary → 100%.
- `tasks::task_allocator` `max_task_count` 950000→4096 (target buffer `u8[393216]`,
  granularity 0x60, ctor loop `cmp edx,1000h`) — also clears the derivative `task_manager`
  `[size]` rows (it embeds the allocator); task_allocator ctor/allocate/deallocate +
  task_manager ctor → 100%.
- `physics::character_move_test_callback` — `const` on `m_up_vector`/`m_minSlopeDot`
  + ctor param (byte-neutral, faithful; stays 100%).
- `network_core` `delayed_packets_predicate` — member rename `m_delayed_packets_to_appear`
  →`m_packets` (byte-neutral; predicate op() stays 100%). The `boost::noncopyable` BASE
  could NOT be added (std::remove_if copy-constructs the predicate → C2248); sushi@TODO +
  review_todos row left.
PARKED (out of layout-only scope — body/render-coupled rewrites, leave for a particle/input matcher):
  `particle_emitter_instance` / `particle_system_instance(_impl)` (reference→pointer +
  new `render::base_scene_ptr m_scene` + removed `self_ptr` + ctor/method-set rewrite),
  `input::receiver::keyboard` (DirectInput event-buffer rewrite, 0x210→0x914),
  `console_commands::cc_bool` (`cc_value<bool>` base refactor), `core` `mutex_mt_raii`
  (+`const bool m_is_tasks_aware`, out-of-line ctor logic), `core` `regions_filler`
  (+`m_high_memory_regions`, 2-arg ctor + region-routing body), `game_test_suite`
  (`m_rtp_world` is base-only but used by rtp_learn/rtp modules — can't remove without
  touching them). All others in scope are type-spelling SKIP (`enum X`/`const X`/`char const `).
