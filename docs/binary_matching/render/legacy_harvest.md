# render legacy-body harvest — campaign log

Bring the original render libraries' bodies (parked in `temp/render_legacy/`)
into the enabled carcass, top-down. This is the missed legacy-harvesting phase
of the render carcass rebuild: the structure queue is drained, but 1,826
generated stub bodies remain while 1,011 legacy files sit unported. Protocol
follows `docs/binary_matching/game/legacy_harvest.md`, adapted for render's
optimized (/O2 + LTCG) target.

## Baseline (audited 2026-07-31, HEAD 6931a537b)

Recovery bar = the pre-restructure score at the branch point `34639bbef`:

| checkpoint | render exact | render fuzzy | overall exact |
|---|---|---|---|
| pre-restructure (`34639bbef`) | 366 / 2,647 (13.8%) | 31.0% | 43.63% |
| carcass baseline (`6931a537b`) | 8 / 2,647 (0.3%) | 0.1% | 38.13% |

The harvest must first recover the 13.8% / 31.0% bar, then exceed it.

Carcass state at baseline:

- 351 units, all TUs enabled (0 `ExcludedFromBuild`), module compiles.
- 1,826 `STATE[STUB]` markers: engine 1,209 / core 467 / facade 145 / culling 5.
- 1,817 addressed `// FUNCTION BODY[0xVA]` carcass comments, 0 addressless.
- 71 `{ /* no source */ }` inline stubs.
- `temp/render_structure_queue/` EMPTY (drained; dispositions in
  `temp/render_structure_queue_triage.md`).
- `temp/render_legacy/` = 1,011 files: engine 410 / core 278 / base 120 /
  common 85 / facade 68 / dx9 46 / culling 1 / top-level 3
  (`triage_log.md`, `library_linkage.h`, `world.h`).

## Legacy classification (first pass, mechanical)

`temp/render_harvest/classify.py` → `temp/render_harvest/manifest.tsv`
(one row per legacy file; regenerate after each harvest batch). Buckets:

| bucket | files | meaning / disposition |
|---|---|---|
| BASENAME | 599 | same-stem counterpart in the new tree — port bodies, then delete |
| SYMBOL | 62 | defined type found under another new-tree file — port there |
| DEAD | 195 | defines classes with ZERO target-symbol evidence (old `blender_*` shader system, `du_*` debug prims, hw_wrapper era) — retire with triage note |
| DX9 | 46 | DX9 backend; target exe is DX11-only, match.db has 0 dx9 units — retire wholesale |
| UNMAPPED | 108 | data-only headers, defines/macros, free-function TUs — refine per file |

The `target_evidence=` column (defined classes appearing anywhere in the
shipped exe's demangled symbols) is the dead-vs-live discriminator; generic
tokens (`memory`, `std`, `point`) give false positives, so DEAD is decided on
zero evidence only, and evidence >0 means investigate, not port blindly.

## Port order (top-down; do NOT start with isolated helpers)

/O2 + LTCG batches helpers into compiler-state islands; a helper ported before
its owner exists measures noise. Order:

1. `facade/` — public entry points and owning objects: `render_facade_entry_point`,
   `render_facade_world`, `engine_renderer`, `game_renderer`, `scene_renderer`,
   `debug_renderer`, the channels/commands.
2. `engine/` world + owners: `render_engine_world_pc_dx11`, `scene`,
   renderer/stage owners, model cookers.
3. `core/dx11/` managers: `options`, `resource_manager`, `effect_manager`,
   backend/state objects.
4. Stages, models, effects, and only then their private helpers.

## Protocol (per function — same as game harvest)

1. Find the legacy ancestor (direct name first; renames via manifest mapping).
2. Port the body adapted to the CANONICAL carcass declaration (signature,
   members, module ownership). Keep the addressed `// FUNCTION BODY[0xVA]`
   comment verbatim. Leave `STATE[STUB]` markers untouched until the body is
   real; no `%` markers — matching is the next phase.
3. Cut the ported function from its `temp/render_legacy/` file; delete the
   file when empty. What remains in the tree IS the queue.
4. Batch = one subsystem; module compiles after every batch
   (`python3 scripts/rebuild.py render_*` per module, full `rebuild.py`
   before commit). Every commit carries the regenerated `match.db` + README
   block: track render exact / exact-max / fuzzy / fuzzy-max per checkpoint.
5. Legacy files that were ALREADY copied wholesale into the new tree:
   verify content, then delete from temp (record nothing — the port is the
   record). Files that DON'T map cleanly stay in temp; record disposition in
   `temp/render_legacy/triage_log.md` only when retiring (DEAD / DX9 /
   incompatible-interface).

## Completion gates (step 7)

- `temp/render_structure_queue/` empty (already true).
- `temp/render_legacy/` empty; every file ported, retired with a triage-log
  entry, or covered by a manifest bucket disposition (DX9 / DEAD wholesale).
- No unexplained `STATE[STUB]` bodies (each remaining stub is target_only /
  no-legacy-ancestor, i.e. matcher-phase work, not harvest work).
- Render builds; full game + render consumers link.

## Checkpoint log

| date | commit | batch | render exact | render fuzzy | stubs left | legacy files left |
|---|---|---|---|---|---|---|
| 2026-07-31 | 6931a537b | baseline | 8 (0.3%) | 0.1% | 1,826 | 1,011 |
