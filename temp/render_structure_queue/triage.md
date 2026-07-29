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
- `headers/vostok/render/engine__renderer.h` |
  represented in the live tree |
  `sources/vostok/render/facade/engine_renderer.h` carries the target four-byte
  facade owner and its editor-aware initialization contract.
- `sources/vostok/render/facade/sources/engine_renderer.cpp` |
  represented in the live tree |
  Construction, initialization forwarding, and frame-id access live at the
  target source path.
- `headers/vostok/render/game__renderer{,__draw_scene_params}.h` |
  represented in the live tree |
  `sources/vostok/render/facade/game_renderer.h` and its source carry the
  target `0x14` facade owner, the `0x20` draw parameters, and the shipped
  movie, text-manager, Scaleform, output, and scene APIs.
- `headers/vostok/render/functor_with_big_buffer_to_copy_command_vostok__render__game__renderer__draw_scene_params_.h` |
  represented by the live generic template |
  `sources/vostok/render/facade/sources/functor_with_big_buffer_to_copy_command.h`
  is instantiated with the live target-layout `game::renderer::draw_scene_params`.
- `sources/vostok/render/facade/sources/game_renderer.cpp` |
  represented in the live tree |
  The owner construction, command marshalling, renderer accessors, movie and
  text-manager routing, Scaleform dispatch, and scene submission live at the
  target source path. Downstream engine scene-view/output implementations stay
  in their own structure-queue entries until those owners are rebuilt.
- `headers/vostok/render/scene_view{,_1}.h` |
  merged and represented in the live tree |
  `sources/vostok/render/engine/sources/scene_view.h` carries the target `0x5e0`
  owner layout, renderer-visible collections, movie/text-manager state, and
  inline access surface. The `_1` record differs only by an incomplete particle
  enum from another compiland.
- `sources/vostok/render/engine/sources/scene_view.cpp` |
  represented in the live tree |
  Construction, destruction, movie ordering/removal, and text-manager ownership
  live at the target path. The corresponding engine-world forwarding bodies are
  also live; the larger engine-world source entry remains queued for its other
  unresolved bodies.
- `headers/vostok/render/post_process_parameters.h` |
  represented in the live tree |
  `sources/vostok/render/engine/sources/post_process_parameters.h` carries the
  complete target `0x2d4` layout, including all four resource-owning texture
  pointers and the atmosphere, clouds, rain, filmic, and skylight parameters.
- `sources/vostok/render/engine/sources/post_process_parameters.cpp` |
  represented in the live tree |
  The constructor initializes every target field and acquires the base color
  grading LUT at the target path. Consumers that still belong to larger legacy
  material and post-process units remain queued for their own reconstruction.
- `headers/vostok/render/output_window_configuration.h` |
  represented in the live tree |
  `sources/vostok/render/facade/common_types.h` carries the target `0x14`
  output configuration, including explicit dimensions, windowed state, and the
  Scaleform command-queue dependency.
- `headers/vostok/render/render_output_window.h` |
  represented in the live tree |
  `sources/vostok/render/engine/sources/render_output_window.h` carries the
  target `0x2cf0` owner layout and the resize, explicit-size, fullscreen,
  client-size, output, target-context, and Scaleform surfaces.
- `sources/vostok/render/engine/sources/render_output_window.cpp` |
  represented in the live tree |
  The owner constructs its target context and swap-chain output, manages the
  optional Scaleform renderer, and implements the target resize and fullscreen
  paths. `renderer_context_targets` and `res_render_output` remain separately
  queued dependencies; their current compatibility surfaces do not constitute
  completion of those owners.
- `headers/vostok/render/enums/enum_render_target_index.h` |
  represented in the live tree |
  `sources/vostok/render/engine/sources/renderer_context_targets.h` carries all
  70 target-family indices and the nine-entry luminance-family count with the
  target values.
- `headers/vostok/render/enums/enum_rt_usage.h` |
  represented by the live DX11 core owner |
  `sources/vostok/render/core/dx11/render_target.h` carries the target
  depth-stencil and render-target usage values consumed by the engine owner.
- `headers/vostok/render/render_target_instance.h` |
  represented in the live tree |
  `sources/vostok/render/engine/sources/renderer_context_targets.h` carries the
  target `0xa0` instance layout: original name, generated name, render target,
  and texture.
- `headers/vostok/render/renderer_context_targets.h` |
  represented in the live tree |
  `sources/vostok/render/engine/sources/renderer_context_targets.h` carries the
  target `0x2bd0` owner with `m_family[70]`, size at `0x2bc0`, id at `0x2bc8`,
  and memory accounting at `0x2bcc`.
- `sources/vostok/render/engine/sources/renderer_context_targets.cpp` |
  represented in the live tree |
  The live owner names and creates the complete target family, owns resize/id/
  memory behavior, creates luminance lockables, and initializes history
  targets. Temporal SSAO allocation is currently always enabled until the
  target engine-options owner exposes its controlling field. Legacy stage
  consumers that predate the target family have explicit transitional
  mappings: normal-downsampled to `rt_final_frame_downsampled_temp`, tangents
  to `rt_normal_copy`, position-ex to `rt_position`, emissive to
  `rt_object_motion_vectors`, MLAA edges/weights to the final-frame
  downsampled pair, SSAO-small to `rt_ssao_temporal_mask`, light-scattering to
  `rt_light_scattering_result`, and color to `rt_albedo`. Those consumers
  remain queued and must recover their target semantics when rebuilt.
