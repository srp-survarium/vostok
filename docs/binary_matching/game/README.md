# game - matching notes

- Namespace: `survarium::` (no module sub-namespace - shared with `game_core`
  and the pc exe module).
- Build: `/Od` (optimization off) + LTCG, `Master Gold` - see [../MATCHING.md](../MATCHING.md).
- Sources: `sources/vostok/game/` (infra headers at module root, everything
  else under `sources/`).
- Reachability anchors: `survarium::game_module::{create_world,destroy_world,
  register_memory_allocators,allocator}` in `game_entry_point.cpp`, consumed by
  the compiled `survarium/game_module_proxy.cpp` (99.9%); render's
  `game_renderer.cpp` includes `game/sources/flash_factory.h`.
- Carcass rebuilt from canonical structure (see
  [../library_carcass_rebuild.md](../library_carcass_rebuild.md)); the 2011/12
  code it replaced was parked in `temp/game_legacy/`, since deleted from the
  tree - `git show :temp/game_legacy/<file>` for body reference.

## The shared-namespace header triage

`binaries/structure/target/headers/survarium/` is one flat pool for the whole
`survarium::` namespace - game, game_core and the pc exe together (728 files).
The rebuild queue (`temp/structure_queue/`) was seeded with ALL of them, then
triaged; the triage log (`git show :temp/triage_log.md`) records every
removal with its reason:

- pass 1 (238): basename / nested `outer__inner` / `_N`-variant matches against
  `sources/vostok/game_core/` - the type already lives there.
- pass 2 (32): definition-grep over `sources/vostok/` - types defined inside
  *other-named* game_core files (TU-local predicates etc.), plus
  `application.h` / `game_module_proxy.h` (pc exe module).
- A name hit in a `vostok::*`-namespace module is NOT coverage (e.g.
  `flash_renderer`, `render_visual` also exist in `vostok::render`) - those
  stayed in the queue, flagged CHECK in the triage log.
- pass 3 (147 - 2 restored): instantiation dumps of the engine USER_ALLOCATOR
  container machinery (`survarium::map/vector/std_allocator` come from
  `vostok/map.h` + `std_containers.h` via the kept `game_memory.h`); restored
  the 2 `weapon_sound_events_handler_state_cook` variants pass 2 dropped by
  matching the template ARGUMENT type - that family is game's.

What remains is game's work-list (301 headers + 144 compiland files after pass
3, minus whatever later batches reproduce): a file is deleted from the queue
when its real counterpart is reproduced in the tree. Empty queue = carcass
complete.

## Module-wide notes

- **The module is an evolution of the parked legacy code** - many type/file
  names survived into the shipped build, so the legacy bodies
  (`git show :temp/game_legacy/<file>`) are strong priors, not verified
  matches. The simulation core was split out into
  `game_core` (matched separately); the legacy network client wiring
  (`game_net_client.*`) was replaced by `vostok::network`'s
  `login_client`/`match_client`.
- **`game_entry_point.cpp` is an interim refresh**: canonical statics order is
  `s_input_allocator, s_ui_allocator, s_ai_navigation_allocator,
  s_ai_allocator, s_game_allocator, s_game, s_generate_shaders,
  s_generate_shaders_world` (lines 46-55); the last three wait on `game.h` /
  `game_generate_shaders.h`, so the file stays in the queue until they land.
- **The flash/scaleform code belongs to an original `vostok/scaleform` module
  that the structure generator never emitted** (the ONLY compiland gap in the
  whole tree - the symbol lists were fabricated from the rich index, ~275
  addressed fns, incl. out-of-line `flash_movie`/`flash_value` bodies in
  `movie.cpp`/`value.cpp` and the Scaleform D3D11 HAL `d3d1x_*` pair files).
  **Batch 12 built this module** (`sources/vostok/scaleform/`, see
  [../scaleform/README.md](../scaleform/README.md)): the flash/scaleform headers
  moved there from `game/sources/`, their `{ /* no source */ }` inline stubs
  became declarations + .cpp definitions in the 10 compilands, and game's
  `temp_include_all.cpp` + render now include them via `<vostok/scaleform/sources/...>`.
- `survarium::collision_object_types` at the module root (`collision_object_types.h`)
  is a keeper infra enum shared with the editor.

## Carcass-rebuild baseline (2026-06-12)

Parking the legacy TUs cut the only real references into the engine_user world
cone - `/OPT:REF` stripped game_core/ai/ui/animation almost whole (game_core
509->0 exact). Restored by two temp anchors: `IncludeAll temp_anchor;` in
`game_entry_point.cpp::create_world` (the legacy root was `game_world.cpp:99`)
plus `use_engine_user_world_cone()` in `temp_include_all.cpp` (the anchor TU,
now owned by game - moved from game_core)
(ai/ui/animation/input/rtp/ai_navigation create/destroy_world).

Residual vs the pre-rebuild report: 880 lost exact fns / 635 gained (net exact
count UP: 8,703 vs 8,683; overall fuzzy 49.22% vs 50.67%). Of the lost: 517
STRIPPED - template COMDATs (boost/stlport/animation-mixing) whose only
instantiating TUs were the legacy game ones; they return as rebuilt game TUs
land (prioritize `animations_selector.cpp` + the animation controllers - the
`mixing_n_ary_tree_*` family alone is ~80 fns). 363 CHANGED - LTCG codegen
drift from the smaller consumer web (bullet/game_core/stlport); only real
consumers restore these, do NOT chase them with anchors. The gained side is
mostly the fresher target regen (this worktree's target is newer than the main
repo's 2026-06-02 artifacts).

## Per-function logs
One `<function>.md` in this folder per function that needed real effort (see
[../agentic_loop.md](../agentic_loop.md) section 7). Live status is in the
`STATE[...]` markers in the sources; these files hold the deeper write-ups.
