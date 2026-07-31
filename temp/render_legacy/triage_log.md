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

## res_* / buffer / constant-table harvest (wave 7 slice, 2026-07-31)

Ported into `sources/vostok/render/core/dx11/sources/` (render_target,
res_texture, res_render_output, untyped_buffer + header map/unmap,
index_buffer, vertex_buffer, shader_constant_table, shader_constant_buffer,
res_state, res_declaration, res_effect, res_texture_list, res_geometry).
Retired without port:

- `common/sources/res_shader.cpp`, `common/sources/res_atomic.cpp`,
  `common/sources/res_common.h` - DX9-generation bodies
  (shader_manager/hw_wrapper era, dtor-side manager releases, res_base/
  res_named ancestry); every covered class has its same-generation DX11
  ancestor which was ported instead. `res_texture_list::clear/clear_not_free/
  find_texture_stage/apply` and `res_shader`/`res_id` have no counterpart in
  the shipped generation.
- `core/sources/res_rt.cpp` + `core/res_rt.h`,
  `core/sources/res_constant_table.cpp` + `core/res_constant_table.h`,
  `core/sources/res_constant_buffer.cpp` + `core/res_constant_buffer.h`,
  `core/sources/res_buffer.cpp`, `core/constant.h`, `core/constant_bindings.h`
  - renamed pre-rename generation (`render_dx10` namespace, u8 slot indices,
  `_free()`); superseded by the same-generation
  `core|core/dx11/sources/*.cpp` copies that were ported.
- `core/index_buffer_inline.h`, `core/vertex_buffer_inline.h`,
  `core/shader_constant_binding_inline.h`, `core/dx11/untyped_buffer_inline.h`
  - verified against the canonical in-class inline bodies (binding ctor
  asserts are absent from the shipped 0x66d40/0x67540 instances, so the
  canonical empty body is faithful); map/unmap were filled into
  `core/untyped_buffer.h`.

Kept as HARVEST REMAINDER (ancestors of still-stubbed `compare` functions):
`core/dx11/sources/res_effect.cpp` (res_pass::equal,
res_shader_technique::equal), `core/dx11/sources/res_texture_list.cpp`
(equal(texture_slots)), `core/dx11/sources/res_geometry.cpp` (equal).

## core/sources twins of core/dx11 (2026-07-31)

26 files under `core/sources/` verified byte-identical (git diff vs the
campaign-start dx11 copies at 8bb5b3dfc) to their `core/dx11/sources/`
siblings - the same source committed twice in the old tree layout. The dx11
copies are the harvest sources; the twins add nothing. Retired:
resource_manager, device, effect_manager, state_cache, state_descriptor,
state_utils, custom_config, custom_config_value, decl_utils, effect_cook,
effect_descriptor, effect_options_descriptor, render_core_entry_point,
res_gs_hw, res_input_layout, res_ps_hw, res_sampler_list, res_signature,
res_vs_hw, shader_binary_source_cook, shader_macros, texture_cook,
texture_cook_wrapper, texture_options, texture_pool, texture_storage, utils
(.cpp each). Four differing siblings kept for reconciliation: backend,
options, effect_compiler, backend_handlers.

## Final-push dispositions (2026-07-31; worker C + closeout sweep verdicts)

base/ RETIRED wholesale (22 files) - the pre-facade base_world/platform
dispatch layer, wholly superseded by the already-ported facade generation:
debug_renderer.{h,sources/*} (fn set fully covered by canonical facade
debug_renderer, 0 stubs), game_renderer/ui_renderer pairs (base ctors take
base_world&/platform&), debug_draw_{lines,triangles}_command x4 (facade
generation ported wave 1), platform_dx11.{cpp,h} (dissolved into core
backend/device; wave-2 reading hold expired), platform_dx9.cpp,
editor_renderer/engine_wrapper/extended_renderer/renderer_dx11/platform.h
(NOVTABLE interface shells; triage-entered earlier, physical deletion was
pending), model.h (unique terrain_model_instance already noted dead in the
facade remainder).

common/ RETIRED (11): light/lights_db/material_manager pairs (older
generations of already-harvested engine copies), res_shader.h
(IDirect3DVertexBuffer9 era), res_texture.{cpp,h} (DX9 set_surface era;
DX11 copy harvested), utils.h (X-Ray property_container helpers, zero
evidence), convex_volume.cpp (comment-level twin of the consumed engine
copy). HELD: blender_combine.{cpp,h} (resolve_lighting reading reference),
pix_event_wrapper.h (stage-porting macro recognition).

Also deleted: facade/platform.h (all-#if 0, entered earlier),
engine/sources/terrain_texture_pool_inline.h (entered earlier), plus the
closeout sweep's 33 fully-consumed shells (dispatch headers, consumed
facade pch/allocator/command twins - see sweep report in the PR).

core/sources reconciliation four (backend/options/effect_compiler/
backend_handlers.cpp): RETIRE verdicts recorded (older generations, deltas
nil or deliberately-disabled); physical deletion follows the final port
round which uses backend.cpp as a port source.

## Engine/core drain dispositions (2026-07-31; worker "drain engine/core", 147 deleted / 12 kept)

Holds expired (11): dx9 model_manager pair (5 ported chunks noted in
portal_sector_system.cpp + detect_sector x2 in portal_sector_structure.cpp;
remaining portal stubs all "no legacy ancestor"), dx9 stage_combine pair
(resolve_lighting notes record the member mismatch), gpu_timer.h
(declarations-only; event_query.cpp spells the D3D11_QUERY_EVENT idiom),
flash_renderer.cpp (hold REFUTED - survarium::flash_renderer is a name
collision, not the same type), pix_event_wrapper x3 (zero PIX symbols in
target), core/sources/include_handler.h (byte-twin of dx11 copy).
Duplicate twins (5): last of the core/sources "26 twins" family
(texture_cook_wrapper/texture_pool/texture_storage/shader_binary_source_cook
.h). Covered-by-canonical-notes (~62): all 47 HARVEST-REMAINDER banner
files whose leftovers map to a STATE[STUB]+claude@NOTE, the 9 untrimmed
stage_*.cpp (each execute stub carries its divergence note), and
render_engine_world_pc_dx11.cpp (unported header inlines land on world_pc.h's
22 target-proven-empty { /* no source */ } inlines). Dead machinery (~68):
verified against match.db target symbols, e.g. resource_manager.cpp (every
leftover has an UNPORTED: cause - m_sources subsystem / non-MASTER_GOLD / no
target symbol), effect_manager.cpp (no find/delete/recompile/create-by-name
in target).

## Facade/common drain dispositions (2026-07-31; worker "drain facade+common", 22/22 retired)

Fully-ported byte-equivalents (4): render_facade_world.cpp (9 bodies),
render_facade_entry_point.cpp (3 bodies + s_world/allocator defs),
cull_mode.h, base_command.h. Canonical-is-target-superset (5):
common_types.h, scene_view_mode.h (target 0x1a-entry set),
render_stage_types.h (target 0x1d-stage set), vertex_input_type.h (target
0x0f-type set), scene_renderer.h (pre-v0.100b interface; named as ancestor
by the set_model_visible note). Zero-target-evidence (5): model.h
(terrain_model_instance - terrain subsystem retired), particles.h
(particle_emitter_info heir = particle::preview_particle_emitter_info),
render_stage_types.cpp (string_to_stage_type; target ships only
stage_type_to_string 0x5872f0), vertex_input_type.cpp (both string
converters dead), debug_renderer.cpp remainder. Plus common/ retirements
recorded in the final-push entry above (blender_combine + pix_event_wrapper
holds released with the stage waves complete).

## Final port round (2026-07-31; the 12 kept-portable files -> EMPTY)

speedtree.cpp: initialize_speedtree [0x75a880] (IsAuthorized guard,
CAllocatorInterface local - atexit-dtor evidence, SetCoordSys, both
register_cook pairs; statics at NAMESPACE scope per dynamic-initializer
symbols), finalize_speedtree [0x75aa10], print_speedtree_errors [0x75910].
NOT ported: speed_tree_allocator::Alloc [0x54440] 76B vs one-line MALLOC
forward (legacy adds num_speedtree_memory_used counter + throw bad_alloc) -
claude@NOTE left, matcher follow-up. material.cpp:
initialize_nomaterial_material [0x6fcbf0] header-inline -> out-of-line
(geometry_render_stage -> gbuffer_render_stage; effect instantiation
confirmed by create_effect's observed-instantiation list).
scene_manager.cpp 7/7 (ctor = only stub left, new-in-target).
system_renderer.cpp 16 ported at legacy positions; draw_debug_terrain does
NOT map (needs dropped m_terrain_debug_material + retired render-terrain
subsystem) - git-history note in source. shader_binary_source_cook:
converted_shader_loaded [0x5589e0] COMPLETE (else-arm only; blob+copy
collapses to raw pointer into pinned managed resource = fits 0x18d);
create_resource [0x558b70] PARTIAL (define path-suffix loop elided - needs
dropped get_shader_source_by_short_name/found_shader_declarated_macroses;
note cites 0x558b70 disassembly as the resolution path).
include_handler.h hold MOOT: sole consumer compile_shader_task not
canonical, zero ID3DInclude call sites in render (runtime shader compile
stripped from MASTER_GOLD); effect_compiler uses canonical
shader_include_getter instead. texture_pool/texture_storage/core-utils
verified consumed (ctor formula + dtor [0x781f60], get/release inlines,
utils::log_2).

## Closing evidence lines

- res_xs_hw mark_registered/set_name: R_ASSERT guards kept verbatim from
  legacy (MASTER_GOLD emits empty_stub per assert site - they are bytes).
- effect_manager::create_effect: legacy proves the ALLOCA-descriptor form
  (stack-allocated effect_options_descriptor filled then passed by ref),
  kept in the canonical template.

## Recovery pointer

Every file this campaign deleted from temp/render_legacy/ is recoverable at
the clean-room baseline commit: `git show 8bb5b3dfc:temp/render_legacy/<path>`
(full ever-existed list: `git ls-tree -r --name-only 6931a537b -- temp/render_legacy`).
Dangling claude@NOTE references to temp/render_legacy paths in sources/
resolve through that same pointer.
