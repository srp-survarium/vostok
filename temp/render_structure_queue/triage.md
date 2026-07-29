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

