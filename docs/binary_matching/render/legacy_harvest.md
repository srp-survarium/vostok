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

## Waves (dispatch plan; one agent = disjoint file set, orchestrator builds+commits)

| wave | scope (stub counts at baseline) | status |
|---|---|---|
| 1 | facade: entry_point+world (me), channel+stage_types, scene_renderer (46), debug_renderer+commands (37), game/ui renderers+commands (~25) | DONE 55090cde2: ~97 fns, facade stubs 145->33, module green |
| 2 | engine roots: render_engine_world_pc_dx11 (115, +legacy render_engine_world_dx11.cpp merge), renderer (27), scene (65), renderer_context (17), system_renderer (18) | PORTED: world 66/115, scene 35/65, renderer_context 17/17, renderer 8/27, system_renderer 17/18; integration fixes: res_geometry+render_target placeholder->core redirects, struct-font/ui-world fwd-decls (mangling), technique enums x4, frac consolidated into help_math.h (target-attributed), SpeedTree SDK static libs linked per legacy pragmas (stand-in globals -> externs), forwarding ctor for singletons_on_preinitialize, decal friend-class device, draw_debug_terrain link stub |
| 3 | core/dx11 owners: resource_manager (80), options (26), custom_config (29), effect_manager (18+inline 114), backend+handlers (31), device (13), effect_compiler (15+9) | PART 1 DONE (last two workers; solo from here per sushi): resource_manager 68/82 (+create_geometry<size> wrapper, call_resource_destructor); res_* batch ~65 bodies + 5 generation-gap fns added (res_render_output) + carcass VA-swap fixed (render_target create overloads) + 5 header inline fills; integration: res_state friend, res_xs compare stub, operator< x3, compare_member_predicate exact-match fix, assert_on_fail qualifier. REMAINING for solo: options/custom_config/effect_manager/backend/device/effect_compiler |
| 4 | stages: visibility/gbuffer/lights/shadow/postprocess/LPV/distortion/rain... (~200) | |
| 5 | models+cookers: render_model_* / *_cooker / skeleton / static (~150) | |
| 6 | world systems: grass (35+12), speedtree (13), clouds (15), radiance_volume (26), lights_db, decals, particles, statistics (33) | |
| 7 | core res_* / state_* / shader_* helpers + remaining engine helpers | |
| 8 | sweep: culling, UNMAPPED/PORT rows, retire DEAD+DX9+editor with triage entries | |

Wave rule: a wave dispatches only after its owners' wave compiled green; agents
never build (orchestrator builds centrally, fixes trivial breaks, requeues bad
ports).

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
| 2026-07-31 | 55090cde2 | wave 1 (facade) | 8 (0.3%) | 0.1% (latent - callees stubbed) | 1,714 | 787 |
| 2026-07-31 | wave 2 | engine roots + SpeedTree SDK link | 129 (4.9%) | 6.3% | 1,567 | 563 |

| 2026-07-31 | bd12750c4 | wave 3 pt1 (core resource layer) | 168 (6.3%) | 7.5% | 1,393 | 510 |

Wave-2 side effect: overall repo exact 38.13% -> 41.45% (+428 fns), fuzzy
46.65% -> 50.88% - the restored create_world -> engine world -> cooks chain
un-DCE'd reachability cones in consumer modules too. Wave 3 pt1 adds
+39 render exact; overall 41.78%/51.28%.
