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
