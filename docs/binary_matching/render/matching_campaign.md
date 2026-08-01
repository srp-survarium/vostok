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
| A4 | `lights_db.cpp` + `renderer.cpp` roots | 478 (18.1%) | 30.5% | 45.33% |
| B4 | compare chain (`shader_constant` -> `res_*_list::compare`) | 481 (18.2%) | 30.6% | 44.83%* |
| A5 | `register_samplers.cpp` + `renderer::render` (20 console switches) | 485 (18.3%) | 30.7% | 44.87%* |
| B5 | **inline-header split** (8 groups) + backend setters | 490 (18.5%) | 32.0% | 44.90%* |
| A6 | jitter modifier + `draw_debug` / `execute_stages` | 492 (18.6%) | 32.2% | 44.94%* |
| B6 | core inline-header splits + access/CV mangling sweep | 498 (18.8%) | 33.1% | 44.99%* |
| A7 | lane-A inline-header splits (facade 37 rows @100%) | 530 (20.0%) | 33.7% | 45.22%* |
| B7 | **sema-driven** shape work (5 real bugs) | 539 (20.4%) | 33.8% | 45.28%* |
| A8 | portal cone callees + access sweep | 541 (20.4%) | 34.1% | 45.29%* |
| B8 | sema BRANCH-COUNT queue + T5 `fixed_string` guard | 546 (20.6%) | 34.1% | **45.54%** |

\* From B4 onward the `overall exact` column is CURRENT, which dipped when a
from-scratch rebuild reshuffled ICF fold representatives across untouched
third-party code. `exact-MAX` - the campaign's actual metric - rose monotonically
throughout: 45.34 (A4) -> 45.36 (B4) -> 45.40 (A5) -> **45.43** (B5), with
fuzzy-max at **59.73%**. Nothing was lost; the dip is banked-wobble, not
regression.
| A5 | `register_samplers.cpp` + `renderer::render` (21 console switches) | - | - | - |
| A6 | `temporal_projection_matrix_modifier.cpp` + `renderer::draw_debug`/`execute_stages` | - | - | - |
| B6 | inline-header splits round 2 + access/CV mangling sweep | 498 (18.8%) | 33.1% | - |
| B7 | **first sema-driven batch** - 6 condition/predicate shape fixes | 507 (19.2%) | 33.2% | - |

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

## Batch B5 - the `*_inline.h` file-placement discovery (2026-08-01)

The largest single lever found so far in render is not code at all, it is FILE
PLACEMENT. objdiff units come from the delinked TARGET objects, one per source
file; pairing is `unit + mangled symbol`. The original tree split every
non-trivial inline body out of `<x>.h` into a sibling `<x>_inline.h`, and our
reconstructions kept them in the main header - so byte-identical bodies sat in
two different units and never paired. See
`patterns/inline-header-split-pairing.md` for the detection recipe (per-file
function-count diff of the two rich indexes, confirmed by mangled-name
intersection) and the mechanical fix.

B5 relocated eight header groups with no code change and took render/core from
zero paired rows in those units to 84 paired rows (state_cache 8/8 avg 95%,
effect_manager 44/114 avg 86%, backend 8, backend_handlers 11, utils 3/3,
shader_constant family 4/4, custom_config_value 3/4, `core/res_xs.h` 3/3).
Whole-EXE headline 36.32% -> 37.08% code, 53.02% -> 53.88% functions.

Still un-split and worth a follow-up batch: `dx11/res_effect.h` (20 target
rows), `dx11/res_xs_hw_impl.h` (13), `dx11/destroy_data_helper.h` (7, ours is at
`core/`), `dx11/effect_options_descriptor.h` (4), `dx11/shader_constant_buffer_inline.h`
(1), `dx11/effect_descriptor.h` (1), plus the 70 `effect_manager_inline.h` rows
whose base counterpart is still an in-class body.

**Two traps this batch hit, both worth remembering.**

1. `rebuild.py <module>` builds only that module's `.lib` and does NOT relink the
   EXE - the delink then re-measures the OLD binary and reports `+0.00` with
   `0 regressed, 0 improved`. Always rebuild with NO module argument when you
   want a score.
2. `nix develop /abs/worktree` does NOT reset `WINEPREFIX`; it inherits the
   orchestrator's. Two worktrees then share one `mspdbsrv` and the link dies with
   `fatal error LNK1318: Unexpected PDB error; RPC (23)`. Prefix every build with
   `WINEPREFIX=/abs/worktree/binaries/.wineprefix`.

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


## Shared-header tickets: land them TOGETHER, not one at a time

Batch B8 landed ticket T5 (a one-line self-assign guard). It was faithful and
net-positive on current measurement: **+39 improve / -11 regress repo-wide,
byte-exact 9839 -> 9854**, eight functions to 100% across fs/game/engine.

It also **reset banked maxima repo-wide**, because editing a shared core header
changes the effective-source fingerprint of every function that sees it.
Immediately after: exact-max fell 5,918 -> 5,881 and became identical to
current exact, i.e. every function's banked peak was discarded and re-seeded in
the new epoch. Nothing was lost to worse matching - the peaks were simply from
the old epoch - and max can only climb from there. But the headline exact-max
figure is NOT comparable across such a change.

**Consequence for T1 / T2 / T3** (`math_functions_inline.h`, `math_color.h`,
`threading_policies.h`): land them in ONE pass, not piecemeal. Each separate
landing pays the full reset again. The orchestrator should NOT authorise them
individually, as it did for T5.

B8 also corrected where T5 belonged. B7 placed the guard in
`buffer_string::operator=(pcstr)`; adding it there turned a 100% row UNPAIRED,
because the target's standalone COMDAT for that function is four instructions
with no guard. The guard lives in a wrapper we never declared -
`fixed_string<Size>::operator=(value_type const* const src)`, a by-value pcstr
overload (`QBD` = top-level pointer const). Pattern:
`patterns/inlined-guard-belongs-to-wrapper.md`.

## sema: what two batches of use established

- Empirical class yield: **BRANCH-COUNT productive** (B7 5/9, B8 3/14),
  **TOPOLOGY skip** (0/6; 11 of 12 rows were padding artifacts and dissolved
  once flow-free block contraction landed), **COND-FLIP** was 2 false positives,
  now 0 rows. **FLOW-SAME is not shape work.**
- `blocks --diff [--lite]` is the verdict view; `branches --diff` aligns
  positionally and its POLARITY/SIGNEDNESS rows are unreliable when block counts
  differ (it now warns).
- **Two limits.** (A8) sema says WHERE shapes differ, never WHY - pair it with
  `pdb_fetch --view structure-diff` / `--view target`. (B8) **it cannot
  distinguish "the target inlined our callee" from "the target has code we
  don't"** - both present as target-only blocks, and this cost B8 eleven of
  fourteen rows. Triage recipe: `patterns/branch-count-row-triage.md`.
- The tool's own first flagship claim was WRONG (a phantom missing early-out in
  `effect_options_descriptor::operator[]` that was a one-byte `nop` pad). Lesson
  recorded in `patterns/masked-diff-hides-branch-target.md`: *a shape tool must
  canonicalise before it compares; if the first divergence is a branchless
  block, suspect the tool.*

## Harness defect worth knowing (cost B8 three 600s timeouts)

`until ! pgrep -f rebuild.py; do sleep N; done` is **self-matching** - the
waiting shell's own command line contains `rebuild.py`, so the loop never exits.
Wait on a MARKER FILE instead (`echo $? > b.done`; `until [ -f b.done ]`). The
same self-match trap made an orchestrator `pkill -f mspdbsrv` kill its own
launcher. **Never `pkill -f rebuild.py`** on this box - it kills the
orchestrator's integration build.

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

## Batch A6 notes

- `temporal_projection_matrix_modifier`: `pop_jittering` 7.7 -> **100**, `push_jittering`
  0.4 -> **87.3**, ctor held at 100. Bodying the pair re-measured `renderer::render`
  57.0 -> **63.1** for free (the A5 blocker above is cleared) and `draw_debug` 0.2 -> 76.0.
- `push_jittering` is a **frustum round-trip**, not a matrix add: it rebuilds l/r/b/t from
  `get_near( ) * p.e11` / `* p.e00`, offsets all four by the jitter delta, then re-derives
  `p.e20 = (l+r)/(l-r)` and `p.e21 = (b+t)/(b-t)`. The algebra cancels to
  `2*jitter/window_size`, so the two ORIGINAL quirks are invisible at runtime and must be
  copied verbatim: the x extent uses `p.e11` (y uses `p.e00`, swapped), and `dx` groups as
  `(r-l) * (-1.f/w) * jitter[0]` while `dy` groups as `(t-b) * jitter[1] * (-1.f/h)`.
  Sample sets are `{ .25f, -.25f }` / `{ -.25f, .25f }` selected on
  `scene_view( )->get_render_frame_index( ) & 1`. The tail is `push_set_p( p )` (the
  `fixed_vector::push_back` + `set_p` pair inlines exactly).
- Two new patterns: `patterns/float-abs-bit-mask-and-epsilon-polarity.md` (the
  `and 0x7FFFFFFF`-through-a-stack-slot inline, and how `jbe`/`jb`/`jae` separates
  `abs(a-b) > eps` from `!is_similar` / `is_zero`) and
  `patterns/pointer-select-two-line-records-is-if-else.md`.
- **Locals are structure, and `const` is part of them.** The target records
  `float[2] jitter0`, `float[2] jitter1`, `float h` - all NON-const; writing them
  `float const` is a real divergence even though the bytes barely move. The
  `( float )m_window_size_x` sibling is NOT recorded because MSVC parks that x87->SSE temp
  in the dead incoming-`this` argument slot (`[esp+frame+4]`) - a 4-local target against a
  5-local-looking source is correct here.
- `renderer::execute_stages` 12 -> **41.5**, structurally complete except the event-query
  cone. Recovered: `s_render_stages[num_render_stages]` (the sibling of A5's
  `s_visibility_stage_stats`, 0x18/elem, walked in lockstep with `m_stages` via a
  `++stat` in the for-increment), and `stat->stg` is written BEFORE the null check.
- `renderer::draw_debug` 0 -> **76.0**. All 28 statistics writes are recoverable by
  arithmetic on the target headers (`statistics_value<T>::value` is at +0x98 of each
  member; `debug`/`general`/`visibility` group bases are 0x6fd0 / 0x6908 / 0x57f0).
  `renderer_context::get_rt( rt_present )` is the `+0x1cb8` load (index = (0x1cb8-0x98)/0xa0
  = 45). `draw_debug` reads `m_stages[..]->m_context` directly, so `stage` needs
  `friend class renderer;` (codegen-neutral).
- **Blocker (lane B / core, unchanged):** `event_query::issue`/`::wait` are empty, so the
  two profiling blocks of `execute_stages` (target lines 430/433/434 and 465/466) and of
  `renderer::render` DCE away. Lane A cannot fix this.
- **Blocker (lane A, next batch):** every remaining `draw_debug` TRGT_ONLY row is an empty
  stub callee DCE-ing its own guard - `draw_luminance_picker_info` (0xc89),
  `draw_stages_stats` (0x1670), `draw_frame_histogram` (0x247),
  `culling::portal_sector_system::render` (**0x7a, 7 statements - cheapest**) and
  `grass_world::render_debug` (0x6cf). Bodying `portal_sector_system::render` needs two more
  file-scope switches (`s_draw_draw_frustum_images_value`, `s_draw_portals_value`) plus
  `portal_sector_structure::get_sector_id` / `system_renderer::draw_aabb`.
- Non-steerable residual in `draw_debug`: the `render_target_ptr` temp's refcount ops. The
  target inlines them (`add [ecx],edx` / `add dword ptr [ecx],-1; jne`), we emit
  `push/call single_threading_policy::increment<unsigned int>/add esp,4`. That lives in
  `sources/vostok/threading_*.h` - an orchestrator header ticket, not lane A.

## Build gotcha (cost batch A5 three cycles)

`rebuild.py <module>` builds only that `.lib` - **it does not relink the EXE**, so
`report.json` stays at the previous epoch's numbers and `--view structure-diff` reads a
stale base. Always run `rebuild.py` with **no** module argument. Second trap: entering
`nix develop` again while a build runs kills that build's `mspdbsrv` (exit 144 / LNK1318);
never run a `pdb_fetch` in parallel with a rebuild in the same worktree.


## Batch B6 notes (render/core, inline-header splits round 2)

Continues B5. Detection and the three new variants are written up in
`patterns/inline-header-split-pairing.md`; the payoff table lives there too.
Render fuzzy 32.0% -> 32.9%, exact 490 -> 496, unmeasured render rows 208 -> 143.

**The splits are now DRY in `render/core`.** Every remaining target-vs-base file
mismatch in that subtree is one of:
- **Lane A (engine) owned:** `dx11/res_effect.h` also wants `material_effects`,
  `material_effects_instance`, `post_process_parameters` (4 rows, currently in
  `engine/sources/material_effects_instance.h`); `dx11/destroy_data_helper.h` wants
  `stage_forward`'s deleting dtor; `backend_handlers_inline.h` wants a
  `renderer_context_targets.h` row.
- **Include-cycle blocked:** `res_pass::res_pass` (see the pattern file).
- **ICF/neighbour artifacts:** `shader_constant_table::~shader_constant_table` filed
  under `res_geometry.h`.
- **Not splits at all:** the ~70 `effect_manager_inline.h` rows are
  `effect_manager::create_effect<effect_xxx>` instantiations our build never makes
  (engine effect types). B5's hand-off listed these as split work; they are not.

**Second lever found in B6: the access/CV-char mangling sweep**
(`patterns/access-cv-char-mangling-sweep.md`). One scripted pass over `render/core`
found 11 unpaired target functions whose base twin differed only in the
`@@<access><cv>E` mangling group, and fixing the declarations paired all of them
(`res_texture` dtor 100%, `set_hw_texture` 96.4%, `render_cc_*::fill_macro` 76.4%).
The sweep has not been run on `render/engine` or `render/facade` - do that first in B7,
it is minutes of work per module.

**Structure verdicts recorded this batch (do not re-open):**
- `res_xs_hw<T>::parse_resources` - STRUCTURE MATCH (16/16 statements, identical line
  deltas, 5/5 locals). The whole 53% residual is one inline-vs-call wall: the target
  out-lines the compiler-generated `texture_slot::operator=` (96 bytes, present in
  `res_xs_hw_impl.h`) where our build inlines it, so `textures[i] = tex_slot` is 0x14
  bytes there and 0x7f here. Not source-steerable.
- `res_effect::res_effect` and `binary_shader_source::binary_shader_source` -
  STRUCTURE MATCH, byte residual only.
- `shader_constant_buffer::set_memory` 18.4 -> 33.0: the target walks a POINTER pair to a
  precomputed end pointer (no index), clamps with `math::min( size, m_buffer_size - offset )`
  and has NO `offset < m_buffer_size` guard. Params are `u32 const offset` / `u32 const size`
  per the PDB. Residual is register/scheduling.

## Batch B7 notes (render/core, first sema-driven batch)

The queue came from `python3 scripts/sema.py sweep --unit render/core/ --min-pct 30`
(254 rows): `BRANCH-COUNT` 63, `TOPOLOGY` 15, `COND-FLIP` 2, `ORDER-ONLY` 1,
`FLOW-SAME` 168. Render fuzzy 33.1% -> 33.2%, render exact 498 -> 507.

**The classification held up in ONE direction only, and the direction was the
opposite of the dispatch's expectation.**

- `BRANCH-COUNT` was the productive class: of nine rows opened, **five were
  genuine source-shape bugs** and all five were fixed to 100% or near it. Work
  this class first.
- `TOPOLOGY` was not productive: of six rows opened, none was steerable. Its
  destination-by-block-INDEX comparison shifts whenever the two sides differ by
  one block, so a single displaced fall-through block prints as "every later
  branch retargeted".
- `COND-FLIP` - advertised as the highest-value class - was a **false positive
  in both rows** (`create_texture`, `create_texture3d`). `branches --diff`
  aligns branches positionally, so one extra pad block cascades into bogus
  `POLARITY` rows. Both limits are now written up in `sema_tools.md`.

Practical rule for the next batch: run `blocks --diff --lite` before believing
any verdict, and if the first skeleton divergence is a block with **no branch**
(a `nop`, an alignment `lea`, a spill reload), contract it by hand - the CFGs
are almost certainly isomorphic and the row is not shape work.

### Fixed

| function | before -> after | the actual defect |
|---|---|---|
| `backend::flush_rt` | 49.7 -> **100** | `render_dirty_targets::any()` is `\|` not `\|\|` (`patterns/bitwise-or-flag-aggregate.md`) |
| `res_effect::apply` | 62.2 -> **100** | `return select_technique(id) && apply_pass(pass_id);`, not two statements |
| `res_render_output::select_resolution` | 71.4 -> **100** | one statement: `if ( windowed ? GetClientRect(...) : GetWindowRect(...) )`; there is no `else { width=1024; height=768; }` arm |
| `constant_data_predicate` | -> **100** | one clause: `left.class_id < right.class_id` (target body is 19 bytes) |
| `res_effect::apply_pass` | 48.3 -> 67.8, **flow SAME 14/14** | the `ASSERT(id < ...)` is a real `if (id >= technique->m_passes.size()) return false;` guard; rest is T3 |
| `store_constant<T>` x4 | 50.5-54.8 -> 71.0-75.3 | predicate size was blocking `lower_bound` inlining; the scan loop has **no** `&& found->class_id == class_id` term (`patterns/trivial-predicate-unlocks-stl-inline.md`) |
| `texture_cook_wrapper::query_converted_texture` | 43.0 -> 62.9, **STRUCTURE MATCH** | no `assign_replace`: the path is constructed straight from `parent->get_requested_path()` |

`query_converted_texture` dropped the engine's only call to
`buffer_string::assign_replace`, so that function (100%, 44 bytes, real in the
target) is now unreachable and unpaired. Its `max` is banked; it comes back with
whichever target call site we have not reconstructed yet.

### Parked, with the cause proven

- `effect_options_descriptor::operator[]` (83.1) - 271 bytes, 23 blocks, 16/16
  statements, identical line deltas on both sides. The dispatch named it as a
  silent early-out miss; it is not. Residual is a CSE'd zero register. `claude@NOTE` in place.
- `res_texture_list::compare` x2 (55.8/62.3) - LTCG `this`-convention (`ret 8`
  vs `ret 4`) forces a spill+reload; CFGs isomorphic after contracting it. Third
  deferral, now with a note in the source so it stops coming back.
- `resource_manager::create_texture` / `create_texture3d` - LTCG convention plus
  `find_texture` inline-vs-call. `effect_constant_storage::is_equal`,
  `constants_handler<N>::assign`, `state_descriptor::color_write_enable` - LTCG
  argument passing (`color_write_enable` has its argument constant-propagated to 0).
- `backend::flush` (95.6) - the target cross-jumps the two `IASetVertexBuffers`
  epilogues into one tail block. Compiler decision, no source lever.
- `effect_compiler::end_technique` - STRUCTURE MATCH; residual is
  `vector::push_back` inline-vs-call.
- `render_cc_{bool,float,u32}::fill_macro`, `find_registered_sampler`,
  `register_sampler`, `res_texture::destroy_impl`, `post_process_parameters::operator=` -
  **all blocked on shared core headers**, see tickets T3 and the new **T5**
  (`buffer_string::operator=(pcstr)` is missing its `m_begin != s` self-assign
  guard - proven twice inside `render_cc_bool::fill_macro`).

### Suggested B8

1. Work the rest of the `BRANCH-COUNT` list (54 rows unopened), skipping
   `TOPOLOGY` entirely. High-block-deficit rows first: `effect_compiler::begin_pass`
   (29/77 - its line deltas already match, so the gap is downstream),
   `shader_binary_source_cook::create_resource` (25/68), `backend::~backend` (41/65),
   `textures_handler<1>::set_overwrite` (24/35), `options::register_console_commands` (117/241).
2. Land **T5** in a core pass - it is a one-line header change with an unusually
   clean proof and it uncaps the `fill_macro` trio plus every `= pcstr` in render.
3. Run B6's access/CV mangling sweep on `render/engine` and `render/facade`; it
   was never done and is minutes of work.
