# Render structure queue

Frozen from `binaries/structure/target/` on 2026-07-29 after a fresh
`scripts/generate_structure.py target`.

## Scope

- `headers/vostok/render/`: 1,126 synthetic namespace-keyed type records shared
  by all render sublibraries.
- `sources/vostok/render/`: 432 path-preserved function-bearing files.
- Shipped build projects: `render_facade`, `render_core_pc_dx11`, and
  `render_engine_pc_dx11`.
- Legacy `base`, DX9, generic DX11, and OpenGL projects are evidence/triage
  sources, not automatic owners in the shipped target.

## Drain rules

- Reconstruct owner-first: facade roots, DX11 core owners, then engine owners.
- Remove a structure entry only after its declaration/body is represented in the
  live tree or this log records another proven owner/disposition.
- Merge `_N` path variants by symbol and source-line evidence.
- Use target layouts and symbols as ABI truth; use legacy render and analogous
  engine subsystems only for topology, implementation candidates, and style.
- Use the existing per-module `anchor_<module>()` system for temporary
  reachability. Do not recreate `temp_include_all`.

## Dispositions

Record each non-live removal here as:

`queue path | disposition | evidence`

- `headers/vostok/render/world.h` | represented in the live tree |
  `sources/vostok/render/world.h` now carries the target `0x190` layout and
  facade ownership.
- `sources/vostok/render/facade/sources/render_facade_entry_point.cpp` |
  represented in the live tree |
  `sources/vostok/render/facade/sources/render_facade_entry_point.cpp` now
  exposes the target allocator and world-factory signatures.
- `sources/vostok/render/facade/sources/render_facade_world.cpp` |
  represented in the live tree |
  `sources/vostok/render/facade/sources/render_facade_world.cpp` now owns the
  target facade-world construction and renderer wiring.
- `headers/vostok/render/{base_command,base_scene,base_scene_view,base_output_window}.h` |
  represented in the live tree |
  `sources/vostok/render/facade/base_command.h` and
  `sources/vostok/render/engine/base_classes.h` carry the target layouts,
  ownership aliases, and public/protected surfaces.
- `headers/vostok/render/one_way_render_channel{,__null_render_command}.h` |
  represented in the live tree |
  `sources/vostok/render/facade/one_way_render_channel.h` carries the target
  `0xb8` channel owner and its nested null command.
- `sources/vostok/render/facade/one_way_render_channel_inline{,_1}.h` |
  merged into the live header |
  Both compiland variants are represented in
  `sources/vostok/render/facade/one_way_render_channel_inline.h`.
- `sources/vostok/render/facade/sources/one_way_render_channel.cpp` |
  represented in the live tree |
  The owner methods and scene-list draining live at the same real path.
