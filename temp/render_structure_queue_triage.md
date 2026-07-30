# Render structure queue

Reset from `binaries/structure/target/` on 2026-07-29 for a clean-room rebuild.

## Contents

- `headers/vostok/render/` contains the complete synthetic, namespace-keyed
  type structure.
- `sources/vostok/render/` contains the complete path-preserved generated
  structure for files with at least one function. Despite the directory name,
  entries may represent either headers or source files.

## Drain rules

- Rebuild from the top-level owners downward; do not begin with small helpers
  that the optimized LTO build may batch into a different compiler-state island.
- Move an entry into the live render tree only when its owner is being rebuilt.
- Use the original library for implementation evidence and surrounding engine
  modules for local style, naming, ownership, and include-path evidence.
- Record any merged, redirected, or intentionally omitted queue entry below.

## Dispositions

`queue path | disposition | evidence`

- `headers/vostok/render/{statistics*,*_statistics_group,cpu_gpu_timer_scope,timer_scope}.h`
  | merged into `sources/vostok/render/engine/sources/statistics.h`
  | these records form one subsystem; the `double` and `int` records share one
  target-compatible `statistics_value<T>` primary template.
- `sources/vostok/render/engine/sources/statistics_inline.h`
  | merged into `sources/vostok/render/engine/sources/statistics.h`
  | the addressed instance bodies belong to the canonical template definition.
- `headers/vostok/render/render_particle_emitter_instance_{1,2}.h`
  | merged into `sources/vostok/render/engine/sources/render_particle_emitter_instance.h`
  | all three generated records describe the same 0x480 class.
- `headers/vostok/render/{particle_sprite_vertex,subuv_particle_sprite_vertex,particle_beamtrail_vertex}.h`
  | merged into `sources/vostok/render/engine/sources/render_particle_emitter_instance.cpp`
  | the vertex records are source-local implementation types in the original library.
- `headers/vostok/render/{vertex_buffer,index_buffer}.h`
  | moved to canonical `sources/vostok/render/core/` headers
  | the emitter owns complete `uninitialized_reference` instances of these DX11 core types;
  their function sources remain queued under their actual core owners.
- `headers/vostok/render/effect_blur_{0,3,5,7,9,13,17,21,25}_.h` and
  `sources/vostok/render/engine/sources/effect_blur.h`
  | merged into `sources/vostok/render/engine/sources/effect_blur.h`
  | the target compiland emits one shared `effect_blur<N>` primary template for
  sample counts 3 through 25; the synthetic `<0>` record is the same class shape.
  The target debug symbols preserve values but not the declared integral parameter
  type, so `u32` follows the engine's local size/count template convention.
- `headers/vostok/render/effect_complex_post_process_blend_{0,0,0;0,0,1;1,0,0;1,0,1;1,1,0;1,1,1}_.h`
  and `sources/vostok/render/engine/sources/effect_complex_post_process_blend.h`
  | merged into `sources/vostok/render/engine/sources/effect_complex_post_process_blend.h`
  | the target mangled names prove one primary template with three boolean
  parameters; the six generated records are emitted specializations, not classes.
- `headers/vostok/render/{obb,plane_spot,point,sphere,spot}_light_effect_*.h`
  and their path-preserved engine headers
  | merged into five canonical primary-template headers under
  `sources/vostok/render/engine/sources/`
  | target symbols and the original library agree that these are boolean
  shadow-mode templates; `point_light_effect` has two parameters and the other
  four have one. The target emits eleven specializations in total.
