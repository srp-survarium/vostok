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
| A5 | `register_samplers.cpp` + `renderer::render` (21 console switches) | - | - | - |

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

## Build hygiene for the ORCHESTRATOR (learned the hard way at batch B4)

One integration cost four rebuild cycles through a chain of self-inflicted
failures. Each fix was too narrow and caused the next one:

1. **Two `rebuild.py` runs at once in the same worktree -> C2471 "cannot update
   program database" across every TU.** The rule was already known
   (serialize integration rebuilds) but not *checked*. **Always
   `pgrep -f scripts/rebuild.py` before starting one**, and confirm the log file
   exists a few seconds after launching - a detached launch that silently failed
   looks exactly like one that is running.
2. **`pkill -f mspdbsrv` -> C1090 "PDB API call failed, error code 23".** Killing
   the PDB server mid-write corrupts every intermediate `vc90.pdb`. Also note the
   pattern matched the killing command's OWN command line and killed the
   launcher, so the "retry" never started. Never pattern-kill with a string that
   appears in your own command.
3. **Deleting `vc90.pdb` alone -> C2859 "not the file the compiler used to create
   this precompiled header" x209.** `.pch` and `.pdb` are a MATCHED PAIR.

4. **The real root cause, found last instead of first: ninja defaults to `-j26`
   on this box.** Up to 26 `cl.exe` instances contend on ONE per-project
   `vc90.pdb` through mspdbsrv under Wine. That survives a small incremental
   build (the first seven batches were fine) but NOT a from-scratch rebuild -
   and it concentrates in the largest module (`scaleform`). Retrying at the same
   parallelism does not converge: the error count went 522 -> 112 -> 105 and
   stalled. **`python3 scripts/rebuild.py -j6` built green, 0 errors.**
   `rebuild.py` forwards unknown flags straight to ninja.

**The correct recovery, if PDB state is ever suspect:** stop all builds, then
`find binaries/Win32/intermediates \( -name '*.pch' -o -name '*.pdb' -o -name
'*.idb' \) -delete` and do ONE full rebuild **with `-j6`**. Clearing a subset is
worse than clearing nothing.

**Meta-lesson: diagnose before acting at full force.** The initial mistake (two
concurrent rebuilds) was cheap. Every "fix" after it was a guess applied
destructively - killing the PDB server mid-write, clearing half a matched pair,
then re-running an unchanged command three times. Checking what the build was
actually doing (`rebuild.py --help` -> ninja's job count) would have found it in
one step.

**Never `git commit --amend` a DB snapshot after a failed build.** Doing so
stamps a stale `match.db`/README onto the batch commit, and the tell is subtle:
the score line reads *identical to the previous batch*. Verify the build exited
0 AND that `git diff --stat docs/binary_matching/match.db` is non-empty before
amending.

## Batch A5 notes

- `register_samplers` 61.0 -> 82.9, **45/45 statements** (structure count match, 0x6a0 vs
  0x698 bytes). The unreconstructed head was `math::clamp_r( options::ref( ).current
  .m_max_anisotropic, 0u, 4u )` followed by a `switch` on the SAME member mapping 0..4 to
  0/2/4/8/16 - the clamp result survives as the `default:` value. The `+0xb8` options member
  is `current.m_max_anisotropic` (`options::current` at +0x0c, `optinos_table
  ::m_max_anisotropic` at +0xac). Also recovered: the trailing
  `resource_manager::ref( ).bind_samplers_to_shaders( )`, and four wrong address modes
  (`s_base_hud`/`s_position` are CLAMP, `s_nofilter`/`s_linear` are WRAP).
- `renderer::render` 22.3 -> 57.0. Twenty-one file-scope console switches recovered from
  `renderer.cpp`'s `dynamic initializer` list - see the batch recipe appended to
  `patterns/entry-guard-is-file-scope-debug-switch.md`. `s_ui_enabled_console_command` is
  declared at **global scope**, ahead of `namespace vostok`.
- Reproduce-exactly note: the target sets `view_mode = unlit_view_mode` for BOTH
  `s_unlit_value` and `s_distortion_value` (a copy-paste bug - `mov eax, 2` is reused).
- **Blocker (lane B / core):** `render::event_query::issue` and `::wait` are empty stubs in
  `render/core/sources/event_query.cpp`, so the `s_do_stages_profiling` and
  `s_use_gpu_sync_value` blocks DCE away in `renderer::render`.
- **Blocker (lane A, next batch):** `temporal_projection_matrix_modifier::push_jittering` /
  `::pop_jittering` are stubs, so the whole jitter cone DCEs - and with it `view_mode`
  (lines 868-873), `scene::process_streaming` (904-905), the TAA condition (909) and the
  jitterer ctor/push/pop (911-912, 1210). `pop_jittering` is 0x26 bytes
  (`if ( m_need_modify ) { pop the projection stack at renderer_context+0x3880; m_jittered
  = false; }`); `push_jittering` is ~0x180 bytes of halton/2x jitter-sample selection.

## Build gotcha (cost batch A5 three cycles)

`rebuild.py <module>` builds only that `.lib` - **it does not relink the EXE**, so
`report.json` stays at the previous epoch's numbers and `--view structure-diff` reads a
stale base. Always run `rebuild.py` with **no** module argument. Second trap: entering
`nix develop` again while a build runs kills that build's `mspdbsrv` (exit 144 / LNK1318);
never run a `pdb_fetch` in parallel with a rebuild in the same worktree.

