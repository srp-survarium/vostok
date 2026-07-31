# Render matching campaign (root-first, two Opus lanes)

Goal (sushi 2026-07-31): drive render to **100% max**. Match **from roots, not
leaves** - under LTO/O2 a leaf carries no context, so a leaf match measures
nothing useful until its callers exist. Two Opus matcher lanes run in parallel
on disjoint file scopes; the orchestrator integrates each finished batch onto a
single linear stack, rebuilds, and commits a measured `match.db` snapshot per
batch.

Branch: `campaign/render-matching`, stacked on `xray-2.0-prog-v0.100b` at
`37eb3fbf6` (the closeout of the three-lane cherry-pick integration).

## Lane split (never overlapping)

| lane | worktree | scope |
|---|---|---|
| A | `vostok_1` | `sources/vostok/render/engine/**` + `sources/vostok/render/facade/**` |
| B | `vostok_4` | `sources/vostok/render/core/**` |

`sources/vostok/*.h` (core headers outside render) belong to NEITHER lane -
they are orchestrator tickets, see `core_header_tickets.md`.

## Batch log

| batch | unit(s) | render exact | render fuzzy | overall exact |
|---|---|---|---|---|
| - | campaign start | 396 (15.0%) | 24.6% | 43.94% |
| B1 | `resource_manager.cpp` + `CHECK_RESULT`/`make_d3d11_error_string` | 408 (15.4%) | 25.3% | 44.48% |
| A1 | `render_engine_world_pc_dx11.cpp` (world root) | 420 (15.9%) | 27.0% | 44.59% |
| B2 | `backend.cpp` + `backend_handlers.cpp` + `effect_manager.cpp` | 427 (16.1%) | 27.3% | 44.65% |
| B3 | `effect_compiler.cpp` + `shader_constant_table.cpp` + `res_declaration.cpp` | 437 (16.5%) | 28.5% | 44.72% |
| A2 | `scene.cpp` (scene root) + struct-vs-class sweep | 446 (16.8%) | 28.7% | 44.82% |
| A3 | `scene_renderer.cpp` (facade root) + facade anchor | 473 (17.9%) | 29.9% | 45.16% |

Health at A2 (`match_db.py diff 37eb3fbf6..HEAD --module render`): 167 IMPROVE,
73 NEW, 298 TOUCHED, 16 REGRESS, 3 LOST. Thirteen of the sixteen regressions
carry a HELD `max` (LTO wobble - correctly left alone per the standing rule);
the other three are small max resets from faithful structure re-work
(`effect_compiler::end_pass`/`end_technique`, `shader_constant_table::parse`).

## Techniques this campaign established

1. **Descend into stub callees, then re-measure the root.** A `return 0` stub
   callee makes LTCG constant-fold the caller. `make_d3d11_error_string` was
   such a stub, so the message argument folded to `0` at EVERY `CHECK_RESULT`
   site in render; bodying it re-measured dozens of callers at once. The
   `compare(shader_constant const&,...)` stub is the same shape (batch B4).
2. **struct-vs-class mangling sweep (A2's biggest single win).** Types we
   declare `class` that the target defines `struct` mangle `V` instead of `U`,
   so the signature never pairs - even with a byte-identical body
   (`scene::set_sky_material` was fully correct and scoring zero). Tell: a
   byte-identical-but-unpaired function. Sweep:
   `grep -c "U<type>@render@vostok@@"` against the target index, C4099 as the
   pre-filter. A2 fixed 10 types.
3. **File-scope debug switches are recoverable, not noise.** An unexplained
   entry guard over a whole body is usually a `cc_bool` / `cc_u32` /
   `command_line::key` static. Recover the name from
   `dynamic initializer for 's_x'` + the `.data` pointers below `m_type`.
   Recovered so far: `s_no_level`, `s_no_effects_initialize`,
   `s_no_effect_result`, `s_one_texture`, `s_max_triagles_per_dip`,
   `s_debug_enabled_ds_clearing_value`, `s_debug_enabled_rt_clearing_value`,
   `s_use_poral_culling_cc`. Patterns:
   `patterns/command-line-key-is-set-inlined.md`,
   `patterns/entry-guard-is-file-scope-debug-switch.md`.
4. **Definition ORDER is structure.** Restore source definition order to target
   order (PDB line numbers prove it) - done in `shader_constant_table.cpp`,
   `res_declaration.cpp`.
5. **A bodied function can still score zero: check `/OPT:REF` before blaming the
   body.** Batch A3 set out to body a stubbed facade cone and found the bodies
   were already there - `reload_shaders` was complete and still unpaired,
   because nothing referenced it and the linker stripped it. The fix was an
   address-take anchor (`facade/sources/anchor_render_facade.cpp`), which paired
   **260** functions in one batch. Two dispatch lines in
   `game_core/sources/anchor.{h,cpp}` were required as well: a self-dispatching
   static initializer cannot work, because an `.obj` that resolves no undefined
   symbol is never extracted from a static lib
   (`patterns/static-lib-anchor-needs-external-dispatch.md`).

   **Anchors are a reachability hack, not original source.** Keep a pin ONLY
   while removing it un-pairs the function; expect anchored methods to cap at
   79-95% because the pin forces a stock `__thiscall` entry where the target's
   real cross-module callers let LTCG pick a stack-slot convention. Do not try
   to beat that cap by editing the anchored function - retire the pin instead,
   once a real caller exists, and verify the function stays paired.

## Standing rules for dispatched matchers

- **Never reverse correct code.** The metric is MAX %, which banks each
  function's peak per source epoch. CURRENT % below banked max with UNCHANGED
  source is wobble from other batches landing - leave it, it recovers as the
  call graph fills in. Only re-touch when the function's own source epoch
  changed (e.g. a shared header it sees was edited).
- **Don't chase unmeasured functions** (`fuzzy_pct IS NULL`) - ticket T4.
- **Absolute paths for every build.** The harness resets cwd between calls, so
  `cd X && nix develop --command ...` can execute in a SIBLING worktree and
  clobber its artifacts (this happened once in batch A2 - no sources were
  modified, but `binaries/`, `match.db` and `README.md` of the sibling were
  regenerated). Use
  `nix develop /abs/worktree --command bash -c 'cd /abs/worktree && python3 /abs/worktree/scripts/rebuild.py <module>'`.
- Structure before %; named locals ARE structure; keep faithful even if % falls;
  no cross-unit out-lining; R_ASSERT guards verbatim; `STATE[STUB]` removed only
  on a real port.
