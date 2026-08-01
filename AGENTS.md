# Vostok agent guide

This repository reconstructs the Vostok Engine used by Survarium v0.100b.
The goal is source that MSVC 8.0 / VS2008 SP1 compiles into object code matching
the shipped executable and PDB. Target disassembly, PDB structure, and compiled
comparison results are authoritative; modern C++ taste and inferred game
semantics are not.

## Read before working

- Read `CLAUDE.md` for repository-wide workflow and tooling agreements.
- For binary matching, read `docs/binary_matching/MATCHING.md`,
  `docs/binary_matching/agentic_loop.md`, and
  `docs/binary_matching/patterns/INDEX.md`.
- Read only the module notes and individual pattern files relevant to the task.
- Use the repository skills under `.agents/skills/` for unit matching,
  structure verification, match review, and module orchestration.

## Build and measurement

- Enter the Nix development environment; do not use sibling tool checkouts.
- Use `python3 scripts/rebuild.py` with no module argument for the authoritative
  build, relink, delink, report, README score, and `match.db` refresh.
- During render clean-room reconstruction, compile only the three retail render
  libraries with `python3 scripts/ninja_build.py render_facade
  render_core_pc_dx11 render_engine_pc_dx11`. Fix compiler errors until all
  three libraries build; do not link or regenerate reports during this loop.
- A module-only build does not relink the executable and cannot establish a
  current match score.
- `python3 scripts/match_db.py refresh` only re-derives the database from the
  existing report. It does not compile changed source.
- Never hand-edit the generated match-score block in `README.md` or derived
  rows in `docs/binary_matching/match.db`.
- Every commit must represent a measured source state. Before creating or
  amending a commit, run a successful full `rebuild.py` and include the
  resulting `README.md` and `docs/binary_matching/match.db` changes in that
  same commit. This applies to source, comments, documentation, tooling, and
  structure-verification changes because line movement and LTCG can alter code.
- Inspect the small `binaries/objdiff/report-changes.json` for regressions.
  Slice the large `report.json` with `jq`; never print or load it wholesale.
- After editing `scripts/`, run `ruff check scripts/`.
- Treat `history.best_fuzzy_pct` as scheduling/ICF history only. Correctness-facing
  MAX comes from `source_maxima`, is scoped to an effective source/compiler-context
  hash, and must never be backfilled from ordinary best-seen history.

## Matching invariants

- Reproduce the shipped target exactly, including bugs, dead code, unusual
  control flow, declaration order, access order, and const qualification.
- Faithful source structure outranks fuzzy percentage. A 100% byte score can
  still have the wrong initializer-list, braces, lexical blocks, locals, or
  statement count.
- Read the target structure and assembly before writing a body. Build before
  comparing the new base side.
- Preserve the target's named local set in non-optimized units. In optimized
  units, account for PDB information removed by optimization while still
  steering the visible structure.
- Do not fabricate functions, accessors, types, overrides, or symbols to gain
  percentage. Do not change another unit merely to influence a caller's
  inlining.
- Only attribute an irreducible residual to LTCG when it is proven to be
  argument passing at a call boundary. Investigate other differences as source
  shape or record a concrete blocker.
- Match one complete translation unit at a time. Descend into required callees
  outside `render` when necessary and when file ownership remains disjoint.
  Match `render` last.
- Preserve definition order, even when access specifiers repeat or interleave.
- Use clangd helpers for source navigation and `pdb_fetch`/`pdb_rich_query` for
  binary evidence.

## Semantic evidence tools

- Start an optimized-function dossier with `python3 scripts/sema.py rva <fn>`,
  `xref <fn> --callees`, and `strings <fn>`. Use `strings --find <text>` for
  reverse literal lookup and plain `xref <fn>` to find direct callers. Select
  `--base` only when investigating what the current build actually emitted.
- Diagnose control flow with `sema.py blocks <fn> --diff --lite` first, then
  `blocks --diff` for instruction bodies and `branches --diff` only after the
  block view establishes a real divergence. Use `dot` for graph output and
  `sweep --module <m> [--unit <u>]` for a module queue. `BRANCH-COUNT` is the
  highest-yield class; `FLOW-SAME` is not source-shape work.
- The complete useful HoMM2/Gruntz sema family is covered, but its commands map
  to the strongest Vostok evidence owner rather than being duplicated:
  `pdb_fetch --view target|base|diff|structure|structure-diff|callees|info`
  owns disassembly and PDB statement/local evidence; `pdb_rich_query --list`
  owns symbol and RVA listing; `clangd_query.py symbol|def|refs|hover` owns
  source-semantic navigation; and `match_db.py report|list|queue|max` owns
  function/unit status, queues, attempts, flags, and hash-scoped MAX state.
- Gruntz's `map`, `class`, and `vtable` commands compensate for a stripped
  binary. Vostok has the retail PDB: use `binaries/structure/target/headers` for
  class layout, inheritance, and virtual declaration order, target
  `pdb_rich_query --list`/`match_db.py list` for the emitted function map, and
  `pdb_fetch --view target` at real virtual call sites for slot offsets. These
  are authoritative; do not port stripped-binary hierarchy or ownership guesses
  over the PDB evidence.
- Work optimized modules from owning roots toward leaves. Use `xref --callees`
  to descend only into blockers that keep a root folded, stripped, or shaped
  incorrectly; remeasure the root after bodying a callee. An isolated leaf is
  not useful LTCG evidence until its real caller cone exists.
- These commands read generated indexes. If their result conflicts with changed
  source, the base side is stale; run a successful full `rebuild.py` before
  trusting it. `sema.py` return code 1 means the compared flows differ, not that
  the tool failed.

## Source comments and state

- Keep comments lean and explain only non-obvious reasons or matching
  constraints.
- Remove carcass dumps and historical narration after implementing a function.
  Do not leave comments saying code was formerly removed, missing, stubbed, or
  restored when that history no longer explains the current source.
- Remove `STATE[STUB]` when the body is genuinely reconstructed. Preserve live
  `STATE[STUB|INLINED|UNMATCHABLE]`, `sushi@TODO`, grounded matching-wall notes,
  and required evidence until their condition is resolved.
- Do not put percentages, structure-diff dumps, verdicts, or other derived
  status in source. They become stale; reports and `match.db` own that state.
- Strip logs, traces, commented debug code, and diagnostics that the target
  does not emit.
- When review raises an unresolved matching question, add a lean
  `// sushi@TODO:` at the source location and a corresponding row in
  `docs/binary_matching/review_todos.md`. Remove both only when resolved.
- Put detailed experiments and percentage history in commit or PR text. Add a
  reusable assembly-to-source lesson to `patterns/` and its index.

## Git and worktrees

- Preserve unrelated user changes and local-only branches.
- Keep concurrent workers in separate clean worktrees with separate
  `WINEPREFIX` values.
- Keep matching stacks linear. Integrate parallel worker results one at a time
  onto the advancing tip; never merge a fan.
- Preserve every per-commit README/database snapshot. Land an approved linear
  stack by fast-forward, never squash it.
- Before committing a matching step, record every function actually worked
  exactly once in `attempts` (a whole-TU mark is appropriate when the whole TU
  was reviewed). Put those marks and any flags in the same rebuilt database
  snapshot as the source change so the per-commit diff identifies the work.
- Do not push, merge, close PRs, prune worktrees, or delete branches unless the
  user or the active orchestration task authorizes it.

## Code review rules

- Review target faithfulness and source structure, not conventional refactoring
  preferences.
- Treat a high score over a `QUANTITY`, `LOCALS`, or ordering divergence as a
  correctness problem.
- Flag fabricated symbols, stale state comments, leftover carcasses, ungrounded
  constants, accidental logs, cross-unit inlining hacks, and generated
  README/database snapshots that do not correspond to their commit.
- Verify every proposed review fix with a full rebuild and report measured
  regressions, unresolved risks, and whether the stack is ready to fast-forward.
