# Completion plan — from 70.5% exact to a full match

Standing at the time of writing (build #107): **9,118 / 12,932 exact (70.51%)**,
73.85% exact-max, 88.06% fuzzy. Anchors fully retired (`#line`: 0, pin cones:
detached at the `IncludeAll` root). One wiring hub (`on_project_loaded`)
reconstructed and its cone recovered.

The distance to "done" is four different kinds of work, so the plan is phased
by kind, not by module. Ordering rationale: wiring first (it un-strips held
maxima and un-perturbs the link for everyone else), provenance-certainty second
(cheap, mechanical, large certainty gain), the grind third, and the
convergence pass last (it is only meaningful once the rest has landed).

## Phase 1 — Wiring: game blocked rows → 0

Every blocked game row is a function whose only reference used to be a pin.
Its `max` is held; the missing piece is the real caller.

1. For each of the ~65 blocked game rows, `sema xref` the target to name the
   real caller. Group rows by caller — they cluster into a handful of hubs
   (proven: `on_project_loaded` alone re-tied the whole `game_world_ui` cone,
   blocked 71 → 65, recovered rows returned *above* their old scores).
   Known next hubs from the first sweep:
   - `profile_character::*_ready` family (held maxima 90.5 / 94.8 / 73.4) —
     caller is the lobby character-preview path (`lobby_menu_scene` cone).
   - `free_fly_camera::build_view_matrix` — caller is the camera tick.
   - `login_menu::fill_labels`, `lobby_menu::fill_inventory_labels` — menu
     setup paths.
   - `game_world_ui::set_using_info_message` (held 82.8) and the remaining
     game_world_ui leaves — callers in player/world tick paths.
2. Reconstruct each hub from target disasm the `on_project_loaded` way:
   full member-offset and vtable-slot verification, PDB locals as the
   local-variable ground truth. Never guess an operand a byte can contradict.
3. **Rule while wiring:** never edit the body of a still-stripped function —
   its `max` would reset against no measurable `cur`. Wire the caller first,
   let the row measure, then touch it.
4. When game blocked hits 0: delete the 31 dead `anchor_*.cpp` files, the
   `anchor.h` declarations, and the `IncludeAll` instantiation in
   `game_entry_point.cpp` (+ vcproj entries). The build must be
   anchor-free in the file tree, not just at the call graph.
5. Retire the zlib link-order pin the same way: find the engine's real
   `inflate`/`uncompress` caller in the target (resources decompression
   path), reconstruct it, then drop the pin from
   `survarium_pc_library_linkage.cpp` and verify zlib still resolves from
   zlibN.lib (1.2.3) and not Scaleform's bundled 1.2.7.

Exit criterion: `blkd = 0` for game; no anchor files; no pins except what a
byte-verified real reference replaces.

## Phase 2 — Bullet: certainty by PDB source hash

Bullet is dev-modified (confirmed), version-pinned by `BT_BULLET_VERSION 279`.
The certainty mechanism is the **PDB tooling**: the shipped `survarium.pdb`
records an MD5 checksum for every source file the original compiler consumed
(the DBI file-checksum records). A source file whose bytes hash to the
recorded MD5 is *exactly* the file the devs compiled — proof stronger than
any object-level score, and it certifies a whole module wholesale:

1. Surface the checksum records: extend pdb_parser/pdb_fetch with a per-file
   checksum listing for both PDBs (target: every `c:/survarium/sources/bullet/...`
   entry with its MD5; base: ours for cross-checking the normalization).
   Settle the normalization once — line endings, trailing whitespace, BOM —
   against a file we already know is stock.
2. Sweep `sources/bullet` against the target PDB's checksums. Three buckets:
   - **MD5 match** — the file is byte-certain; mark it done at file level,
     whatever the per-function byte score says (LTCG residuals on a
     hash-certain file are context, not source).
   - **MD5 mismatch, file also differs from upstream bullet-2.79** — a real
     dev modification; recover the exact dev text (guided by the disasm and
     the upstream base) until the MD5 lands.
   - **MD5 mismatch, file equals upstream** — the devs edited it and we
     wrongly assumed stock; same recovery loop.
   Fetch pristine upstream **bullet-2.79** as the triage baseline (same
   channel as the vorbis import; gcroot-pinned) — three-way file diffs
   (ours / upstream / PDB-hash verdict) localize every dev hunk fast.
3. The resulting delta list IS the dev-modification inventory
   (`docs/binary_matching/bullet_deltas.md`: file, hunk, byte evidence —
   e.g. the GSC `btIDebugDraw::drawObject` vtable insertion, the
   `objectQuerySingle` convexSweep branch set).
4. The known open rows (`objectQuerySingle` 57.7, `debugDrawObject` 45.5,
   `rayTestSingle` 97.1, `integrateTransforms` 98.9, `processCollision`
   99.8) stop being byte-grind targets: their files' MD5s either land — and
   the residual is provably compiler context — or the hash pinpoints the
   exact dev hunk still missing.

Exit criterion: every bullet source file MD5-matches the target PDB record.
That is 100% certainty for the whole module — and the same instrument then
generalizes: any module whose files hash-land is settled file-by-file
(vorbis, zlib, ogg first; then engine TUs as they mature).

## Phase 3 — The grind: open rows where source shape is wrong

Scope: ENGINE code only — game ~291, vostok 122, render tail 40, core 18,
vorbis 7 (-> Phase 2 hash), animation 3, ai 2. Work order: game → vostok →
render tail → core.

**stlport (113) and boost (244) are NOT ground.** They are vendored template
pools: every instantiation is compiled from an *engine* usage site, so their
divergences are instantiation context — inline decisions, fold
representatives, comparator shapes — owned by the engine TUs that
instantiate them. They move to Phase 4 (convergence) and are expected to
lift as engine callers land; the only direct interventions permitted there
are engine-header truths that gold attests globally (the
`operator<(buffer_string)` out-of-line decision was one — a vostok header,
not an stlport edit). Vendored template sources stay pristine.

The validated lever kit (memory: `matching-lever-mechanics.md`) — in
descending order of evidence strength:
1. **Target disasm reconstruction** — operands, member offsets, clamps,
   refetch-vs-cache (fill_character_data +19.2, on_project_loaded).
2. **PDB-locals cross-check** — existence/type/name of stack objects
   (initialize's `flash_value v` +1.3). Absence of a *scalar* proves
   nothing; absence of a *stack object* is real evidence.
3. **Per-site noinline forwarders** where the target keeps a fold-group
   ctor out of line (validated ×5; verify the call at each site first).
4. **Comparator out-of-line** decisions (operator< precedent) — only when
   gold attests out-of-line at every call site.
5. Statement fusions are BANNED (not dev style); line-splitting is a no-op;
   probes that regress get reverted same-build.

Everything that survives diagnosis as an LTCG/inline-context wall gets
parked with a one-line cause — parking honestly is output, not failure,
because Phase 4 is designed to lift exactly those.

## Phase 4 — Convergence: the final pass

The ~2,700 parked + held rows are mostly *context* walls: inline decisions,
ICF fold representatives, cold-block placement, section layout. They are
expected to land **together** as the surrounding state converges — the
anchor retirement previewed this (+52 exact from a link-shape change alone).

1. After each phase, full rebuild + re-derive; diff the README score block
   between checkpoints — it is the regression tracker.
2. Sweep `ledger list --headroom` after each checkpoint: `hist > max` rows
   that reappear are recoverable losses (anchor-era ghosts excepted).
3. Re-audit the parked pool with `pdb_divergence` at each 5%-exact
   milestone: walls fall passively as neighbors land; a parked row whose
   cause no longer holds gets reopened.
4. The endgame is a deliberate final pass: with every `max` at 100, one
   clean build should print cur == max across the roster. Rows that still
   dip there are genuine link-order/fold-order issues to settle last.

## Phase 5 — Caps and the definition of done

- **gfx (Scaleform)**: capped ~96.6% permanently — vendored *binary*, and
  the 55 structure divergences (AS2/AS3 layout, bundled zlib 1.2.7,
  RegisterClassA macro leak) are recorded via pdb_divergence. Done for gfx
  means: cap documented, no regressions against it. (Only a byte-matching
  GFx SDK rebuild could lift it; out of scope.)
- Vendored deps ARE matched and stay in scope: zlib 99.99, ogg 100,
  wildmagic 100, opcode 98.7, vorbis 98.4 (stock 1.3.3), stlport, boost.
- **Complete** means: every roster row is either `done` (max = 100) or
  carries a byte-evidenced cap entry (gfx, and any survivor of Phase 4
  with its cause); no anchors, no pins, no blocked rows; the README block
  is the proof.

## Standing rules (all phases)

- `max` is keyed to the body hash: probes are safe **iff** reverted on
  regression in the same cycle; a body edit while a row is stripped resets
  its max blind — never do it.
- One build per probe batch; measure before stacking a second change on
  the same function; full-log builds (`grep -cE "error C"`); the game
  module compiles as one cl job — one error silently staleness-poisons
  every game row.
- Comments lean, `sushi@TODO` for open questions, review_todos.md in the
  same commit.
