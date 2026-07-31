# Render legacy triage

Legacy entries are removed only after their target replacement is live or their
incompatibility with the shipped interface is recorded here.

## Obsolete predecessor interfaces

- `engine/sources/render_output_window_cook.{h,cpp}`: implemented the earlier
  `resources::unmanaged_cook` allocation interface. The shipped type derives
  from `resources::translate_query_cook` and owns `translate_query` and
  `delete_resource`; no legacy method body maps faithfully to that target
  virtual surface.
- `engine/sources/scene_view_cook.{h,cpp}`: implemented the earlier
  `resources::unmanaged_cook` allocation interface. The shipped type derives
  from `resources::translate_query_cook`; its old allocation/create/destroy
  methods do not map to the target `translate_query`/`delete_resource` surface.
- `engine/model_format.h`: the target-owned enums, model header, quantized basis,
  and skinned vertex layouts were consolidated into the live header. Legacy
  `s24_tc_base`, `vert_static`, and `vb_sw_struct` have no retail PDB record or
  live consumer and were omitted.

## Dead retail code (zero target-symbol evidence; SYMBOL-bucket refinement 2026-07-31)

DX9/dev-era subsystems absent from the shipped DX11 exe (class names have zero
hits across all 19,449 target functions; heirs listed where one exists):

- `common/sources/res_rt.cpp`: DX9 `res_rt` (D3DFORMAT/hw_wrapper); the DX11
  sibling `core/sources/res_rt.cpp` is the real `render_target` ancestor.
- `common/sources/terrain_texture_pool.cpp`, `engine/sources/terrain_texture_pool.cpp`:
  tile-atlas pool; shipped `texture_pool` is an unrelated occupancy-slot design.
- `engine/stage_sun_materials.h`, `engine/sources/stage_sun_materials.{h,cpp}`:
  heirs are `stage_sun.cpp`/`stage_shadow_direct.cpp`; silhouette-hull
  `dumb_convex_volume` is not the shipped plane-based `convex_volume`.
- `engine/sources/{effect_combine,effect_deffer_base,stage_combine,stage_fr_test,stage_texture_mip_clamp_test,stage_skybox}.cpp`:
  `render_dx10::*` stages/effects with no shipped counterpart (`stage_skybox`
  replaced by `stage_clouds`/`sky_dome_geometry`).
- `engine/sources/scene_render.cpp`, `engine/sources/scene_render_effects.cpp`:
  `render_dx10::scene_render`; heir `renderer.cpp`/`renderer_register_effects.cpp`
  has a different effect list.
- `engine/sources/{terrain,terrain_render_model}.cpp`: the whole render-side
  terrain subsystem is retired in the target (only `effect_gstage_terrain_materials` remains).
- `engine/sources/visual.cpp`: `render_visual`/`*_visual` hierarchy replaced by
  `render_model_static`/`render_model_skeleton`/`speedtree_*`; the target's
  `render_visual` hits are the unrelated `survarium::render_visual`.
- `engine/sources/grass.{h,cpp}`: `#if 0` cell/layer prototype; shipped
  patch/template grass shares no member.
- `core/sources/dds.cpp` + `core/dx11/sources/dds.cpp`: no DDS parsing in the
  target (zero `dds` tokens).
- `core/sources/gpu_timer.cpp` + `core/dx11/sources/gpu_timer.cpp`: replaced by
  `statistics_cpu_gpu`/`event_query`.
- `core/sources/{index_stream,vertex_stream}.cpp`: dynamic-discard streams
  replaced by `index_buffer`/`vertex_buffer`.
- `core/sources/texture_converter_cook.{h,cpp}` + `core/dx11/sources/` duplicates:
  entire body `#ifndef MASTER_GOLD` (offline AMD compressor), compiled out of
  the shipped exe.
- `base/{editor_renderer,engine_wrapper,extended_renderer,platform,renderer_dx11}.h`,
  `base/sources/platform_{dx9,dx11}.cpp`: the pre-facade `render_base` layer
  (`engine::wrapper`, `render::platform`, `dx10::renderer`) has no shipped
  counterpart; superseded by the facade renderers + `engine::world`.
- `base/{engine_renderer.h,world.h}`, `base/sources/engine_renderer.{h,cpp}`
  (removed earlier this pass): dead `render_base` interfaces (`base_world`,
  command processors) with no shipped counterpart.
- `facade/platform.h`: entire body `#if 0`.
- `engine/sources/render_engine_world_pc_opengl.cpp`: never-implemented GL
  backend (every body `NOT_IMPLEMENTED()`).

## Parked for reconstruction elsewhere (not render-tree ports)

- `facade/editor_renderer.h` + `facade/sources/editor_renderer.cpp`: shipped exe
  has zero `editor::` symbols (VOSTOK_STATIC_LIBRARIES compiles the editor
  renderer out); kept as the ancestor if editor builds are ever restored.
- `engine/sources/flash_renderer.cpp`: class moved to `survarium::flash_renderer`
  (scaleform side); legacy bodies are the ancestor for that module, not render.
- `core/sources/include_handler.h` (+dx11 duplicate): ID3DInclude callback must
  be reconstructed inside `shader_binary_source_cook.cpp`/`effect_compiler.cpp`
  (wave 3), not ported as-is.

## UNMAPPED-bucket dispositions (2026-07-31; full evidence in agent report)

INFRA superseded by kept/live files: `common/sources/d3d_defs.h`,
`{common/sources,engine}/namespace_wrapper.h` (bodies `#if 0`),
`engine/sources/HLSLShaders.pm` (build tooling),
`engine/sources/register_effect_descriptors.h` (live decl in
`renderer_register_effects.h`), `engine/sources/render_engine_dx11.vcproj`,
`*_library_linkage.cpp` TUs (pragmas live in `library_linkage.h` files),
`core/{backend_handlers,custom_config_value,effect_manager,resource_manager,
sampler_state_descriptor,state_cache,untyped_buffer}_inline.h` +
`core/res_xs_hw_impl.h` (10-line `VOSTOK_RENDER_CORE_FILE` dispatch stubs;
mechanism lives as `core/platform_helper.h`), `core/{dx10_defines,
blender_defines,gpu_timer,pix_event_wrapper,res_sampler_state}.h` (dispatch
stubs to dead pointees), `core/dx11/dx10_defines.h` (426-line DX10 typedef map
superseded by `dx11_defines.h`), `core/sources/render_core_dx11.vcproj`,
`base/render_allocator.h`, `base/sources/render_base{.vcproj,_entry_point.cpp,
_library_linkage.cpp}` (retired render_base lib).

DEAD (zero symbol AND zero string evidence in the shipped exe):
- common/ pre-rewrite library: `du_sphere.{h,cpp}` (exe mangling matches the
  engine/ variant instead), `environment.{h,cpp}`, `hw_wrapper_base_dx10.h`
  (zero-byte), `hw_wrapper_dx9.cpp`, `hw_wrapper_inline.h`, `render_device.cpp`,
  `res_common_inline.h`, `res_const_table.cpp` (DX9 D3DX variant),
  `shader_manager.cpp` (blender system), `shared_names.h` (old `$user$` set;
  live subset covered by engine's), `simulator_states.h`, `utils_inline.h`
  (only `utils::wform`).
- engine/ render_dx10 era: `environment.h`, `model_manager_inline.h` (all
  commented out), `shared_names.h` (old set), `terrain_texture_pool_inline.h`
  (both copies), `visual.h`, `sources/engine_options.cpp` (all commented out),
  `sources/environment.{h,cpp}`, `sources/flash_renderer.h` (exe has only
  `survarium::flash_renderer`), `sources/model_converter.cpp`,
  `sources/polymorph_vector{,_inline}.h` (`#error`-guarded / all commented),
  `sources/render_engine_pc_opengl.vcproj`, `sources/terrain_cook.cpp`.
- core/ render_dx10 leftovers: `constant_inline.h`, `device_inline.h`,
  `res_buffer_inline.h`, `res_common.h`, `res_common_inline.h`,
  `res_constant_buffer_inline.h`, `shader_declarated_macros.h` (consumer stub
  is addressless; revisit if it ever gets an address), `sorted_vector_inline.h`,
  `sources/res_sampler_state.cpp`, `sources/shader_declarated_macros.cpp`,
  `dx11/res_sampler_state.h`, `dx11/sources/res_sampler_state.cpp`,
  `dx11/sources/shader_declarated_macros.cpp`.
- base/ retired lib + console/DX9 backends: `command_common.h`,
  `sources/command_processor.cpp`, `sources/command_terrain_cell_set_visible.{h,cpp}`,
  `sources/command_update_visual_vertex_buffer.cpp`, `sources/platform_api.h`,
  `sources/platform_dx9{.h,_inline.h,_static.cpp}`, `sources/platform_ps3.{h,cpp}`,
  `sources/platform_xbox360{.h,.cpp,_inline.h}`. (`sources/platform_dx11.h` kept
  temporarily as a structural reading reference for the engine-world wave.)
- facade/ editor-only: `sources/editor_draw_screen_lines_command.cpp`,
  `sources/editor_draw_3d_screen_lines_command.cpp` (editor paths stripped from
  the shipped build).

## DEAD-bucket sweep (2026-07-31; 195 rows verified, 170 retired, 25 overturned)

Retired families (zero symbol AND zero string evidence in the shipped exe;
per-function source-file list from match.db used as ground truth; every family
has its newer legacy heir still queued where one exists):

- `blender_*` (30): D3D9 shader system; heirs are the queued `effect_*` files
  (`blender_forward_system`->`effect_forward_system`, `blender_light_mask`->
  `effect_light_mask`, `blender_deffer*`->`effect_gstage_default_materials`,
  `blender_compiler`->`effect_compiler`, `blender_tree`->speedtree effects).
  `blender_combine.{cpp,h}` KEPT as the only surviving reading reference for
  the ancestor-less `stage_resolve_lighting`/`effect_resolve_lighting`.
- `base/sources/command_*` (40): pre-facade per-operation commands; shipped
  design is `functor_command`/`functor_with_big_buffer_to_copy_command`.
- `editor_*` (16): zero `editor::` symbols; `facade/sources/editor_renderer.cpp`
  stays parked separately for a hypothetical editor restore.
- terrain (19): render-side terrain subsystem retired in the target; only
  `effect_gstage_terrain_materials`/`effect_post_process_terrain_debug_materials`
  survive and have direct queued ancestors. Ports of files that include
  terrain headers drop those branches.
- old `effect_*` generation (19): `effect_deffer*`/`effect_light_spot`/
  `effect_notexture`/`effect_tree`/`effect_radiance_volume_*` (bodies fully
  commented out) - heirs queued (`effect_gstage_default_materials`,
  `effect_light_propagation_volumes`, ...).
- 46 singles: DX9 device/caps/state era (`hw_*`, `render_device*`,
  `simulator_states.cpp`), orphan headers of already-retired `.cpp`s
  (`index/vertex_stream.h`, `dds.h`, `scene_render.h`, stage headers),
  `render_options.h` (->`options.h`), broken-at-write-time stubs
  (`shader_cache.h`, `parallel_light_effect.h` - folded into `effect_sun`),
  `render_model_composite` (different shipped design), CPU skinning (target
  ships GPU-only), editor gizmos (`transform_control*`), platform dispatch
  (`platform_api_*`), pre-facade world/scene (`render_base_world.cpp`,
  `scene_renderer_impl.cpp`), `render_output_window_base.h`,
  `render_statistics.h`, `float4x4b.h`, `engine_options.h` (body fully in
  comment - consumers drop the include), `model_converter.h`.

Overturned -> PORT (renamed-effect ancestors, feed wave 4):
`effect_light_direct.h` -> `effect_sun.cpp`;
`effect_post_process_antialiasing.{cpp,h}`+`effect_post_process_aliasing.cpp`
-> `effect_post_process_sraa.cpp` (cross-check `effect_temporal_antialiasing`);
`effect_post_process_deferred_transparency.{cpp,h}` -> `effect_translucency.cpp`;
`effect_gather_sun_light_scattering_zone.{cpp,h}` -> `effect_god_rays.cpp`;
`make_vcm_effect.{cpp,h}` -> `effect_motion_vectors_accumulation.cpp` (lower
confidence; make_vcm shaders still ship).

Overturned -> REFERENCE (held): `blender_combine.{cpp,h}` (resolve_lighting),
`effect_light_direct.cpp`+`engine/effect_light_direct.h` (second reading for
effect_sun), `core/{res_rt,res_constant_table,res_constant_buffer,constant,
constant_bindings}.h`+`common/sources/res_common.h` (declaration headers for
queued wave-3 res_* ports; also supply shader_constant member ordering),
`core/dx11/gpu_timer.h` (D3D query idiom for statistics/event_query),
`pix_event_wrapper` x4 (macro recognition while porting stages; target has
zero PIX symbols - ported bodies DROP the scopes; retire with waves 4/5).

## DX9-bucket sweep (2026-07-31; 46 rows verified, 43 retired, 3 held)

Whole `dx9/` tree retired as unused retail code (match.db has zero dx9 units)
- every stage/backend/stream/visual family has its DX10/DX11 heir still queued
in `temp/render_legacy/core|engine` or already live. Exceptions held as
REFERENCE:

- `dx9/sources/model_manager.cpp` - the ONLY surviving portal/sector traversal
  bodies in the corpus (`traverse`, `clip_polygon_against_volume`,
  `portal::create`, `detect_sector` x2, `select_visible_sectors`, sector
  `marker` idiom - ancestor of `sector_double_query_preventer`). Every other
  legacy copy is declarations-only or commented out. Feeds ~50 stubs across
  `portal_sector_system.{h,cpp}`, `sector_double_query_preventer.cpp`,
  `culling/portal_sector_structure.h`, `culling/possible_sectors_holder.h`,
  `facade/sources/portal_sector_structure.cpp`,
  `portal_sector_structure_cook.cpp`. Read with the retired declaration header
  `engine/model_manager.h` (in git history). Retire with the culling wave.
- `dx9/sources/stage_combine.{cpp,h}` - stage-side pair of the held
  `blender_combine.{cpp,h}`; only remaining stage ancestor for
  `stage_resolve_lighting.cpp` / `effect_resolve_lighting.cpp` (lower
  confidence: DX9 draws a fullscreen quad, shipped stage adds a per-material
  `render_models` pass). Retire with wave 4.
