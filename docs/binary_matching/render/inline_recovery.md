<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Render missing-body audit

The initial render inventory contained 26 `/* no source */` methods: 21 on
`engine::world`, four on `scene_renderer`, and
`renderer_cook::destroy_resource`. It also exposed two unmarked placeholders,
`engine::world::window_client_size` and `engine::world::type`, which returned
zero and `NULL` respectively.

## Recovered from retail code

`renderer_cook::destroy_resource` is retained as an 18-byte virtual procedure
at RVA `0x507a0`. Its one statement invokes the resource's virtual destructor,
and retail folds it with
`bi_spline_skeleton_animation_baked_cook::destroy_resource`. The identical
surviving implementation is:

```cpp
resource->~unmanaged_resource( );
```

The old inline empty body emitted only a three-byte `ret 4`. The recovered
definition is out-of-line immediately after `create_resource`, matching the
retail procedure and definition order.

## Recovered from the GSC render ancestor

The deleted authentic render source remains available at:

```text
git show 660448d46^:temp/render_legacy/engine/sources/render_engine_world_pc_dx11.cpp
git show 660448d46^:temp/render_legacy/facade/sources/scene_renderer.cpp
```

It proves that the following methods were ordinary out-of-line bodies, not
functions the original developers replaced with empty inline stubs:

- `engine::world::{draw_text,window_client_size,draw_debug_lines,
  draw_debug_triangles,draw_editor_lines,draw_editor_triangles,
  setup_grid_render_mode,remove_grid_render_mode,
  setup_rotation_control_modes,update_system_model,draw_terrain_debug,
  set_selection_parameters,update_model_index_buffer,setup_view_and_output,
  clear_zbuffer,draw_screen_lines,draw_3D_screen_lines,
  draw_3D_screen_point,type}`;
- `scene_renderer::window_client_size`.

Those bodies are restored with the surviving v0.100b base-pointer signatures.
They forward to `system_renderer`, `backend`, `render_output_window`, or the
model instance exactly as the ancestor does. The ancestor also establishes
three intentional no-ops: the four-argument `draw_text`,
`draw_terrain_debug`, and `update_model_index_buffer`.

The shipped client retains no procedure or call-site expansion for this group.
Current callers of the grid, rotation, selection, depth, and 3D drawing APIs
are confined to editor source trees that are absent from the retail client
PDB; the remaining methods have no current caller. They stay
`STATE[UNMATCHABLE]` source priors rather than claims of exact v0.100b bytes.
The UI facade calls the separate ten-argument static `world::draw_text`, whose
54-byte target procedure is already exact; it does not exercise the recovered
four-argument no-op.

## Not observable in the client

Seven declarations remain placeholders because neither the shipped client nor
the ancestor supplies a body:

- `engine::world::{pick_lighting_luminance,
  set_picking_lighting_luminance_mode,generate_environment_probe,
  generate_sky_ao_map}`;
- `scene_renderer::{set_fixed_lod,get_fixed_lod,draw_present_impl}`.

Their names suggest plausible lower-level forwarding, but the corresponding
renderer entry points are themselves unimplemented in the surviving source.
Adding those guesses would not be target reconstruction. The PDB records the
four world methods as public, the fixed-LOD pair as public, and
`draw_present_impl` as private.

## PDB declaration structure

Before this audit, `scene_renderer` was already topology-identical. Two other
classes had one declaration-order defect each:

- retail records the instance `world::draw_text` before its static overload;
  restoring the ancestor's static-first source declaration reproduces that
  MSVC overload order;
- retail records `renderer_cook::renderer_resource` after the explicit default
  constructor (declaration position 4), not before it.

## Measured result

The full rebuild succeeds. `renderer_cook::destroy_resource` is 18 bytes on
both sides, byte-exact, and reports `STRUCTURE MATCH`. The 20 source-prior
methods remain absent from the base rich index; only the separate, already
exact ten-argument static `world::draw_text` is retained.

`scene_renderer` and `renderer_cook` are topology-identical. `engine::world`
has the same size and one identical 131-declaration semantic variant on both
sides; its only remaining topology report is physical record multiplicity
(two equal retail records versus five equal base records), not a declaration
or layout difference. The render strict-data gate is `OPEN=0`, render gains
one exact-max function, and no existing function loses `max`.
