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

## Module-to-100 campaigns

The ledger and `report.json` are the only authoritative progress snapshot. The
goal for the active non-vendor module is zero real PDB divergences,
target-faithful statement/local/declaration structure, and 100% hash-scoped
source MAX for every recoverable function. Establish PDB equality first, then
byte equality. Every irreducible remainder must have a concrete, queryable wall;
current fuzzy percentage or ordinary best-seen history is not MAX evidence.

Work the optimized call graph from owning roots toward leaves:

1. Audit the fresh retail/base PDBs before editing. Resolve access, declaration,
   definition, layout, enum, local-scope, const, and ownership divergences in a
   deliberate batch before paying for a full link.
2. Establish the module's owning roots and dispatch paths. Use `xref --callees`
   to identify only the descendants blocking each root, implement those, then
   immediately remeasure the root. Do not start with isolated helpers merely
   because the queue lists them first.
3. Drain each large caller cone while preserving execution order and target
   statement structure. A stubbed/null accessor or empty callee that collapses
   a caller is higher priority than a locally high-scoring leaf.
4. Apply cross-cutting structural levers in deliberate batches: target header
   location and `*_inline.h` ownership, access/CV/struct mangling, declaration
   and vtable order, layouts, globals, and shared-header fixes. Batch shared
   header changes so compiler-context effects are measured once, not piecemeal.
5. Audit residuals with `structure-diff`, named locals, `vostok sema`, and real
   call-site assembly. Work `QUANTITY`, `SPLIT`, wrong named locals, target-only bodies,
   and verified low-score shape mismatches. Park ordinary `SIZE`, frameless
   convention, ICF, or LTCG claims only after proving the exact non-steerable
   boundary.
6. When a remaining gap is caused by measurement rather than source, improve
   the tool instead of falsifying code. Preserve genuine function-scoped and
   highly-COMDAT evidence through the hash-scoped MAX mechanisms.

The campaign ledger is the linear Git history plus each commit's tracked ledger
snapshot.
For every complete TU or bounded caller cone: inspect target structure and
assembly, record each worked function exactly once with `vostok ledger tried`,
run the full `python3 -m vostok build -j6`, inspect `report-changes.json`, and
commit the source together with the regenerated README and ledger. Then run a
clean-HEAD no-op rebuild and amend its provenance metadata into that same commit.
Never borrow a later ledger snapshot. After each 10-15 TUs, perform a separate
structure-verification and stale-comment audit before continuing.

## Build and measurement

- Enter the Nix development environment; do not use sibling tool checkouts.
- Use `python3 -m vostok build` with no module argument for the authoritative
  build, relink, delink, report, README score, and ledger refresh.
- During iteration, compile only the directly affected libraries with
  `python3 -m vostok.build.ninja <library>...`. Fix compiler errors before the
  authoritative build; do not link or regenerate reports during this loop.
- A module-only build does not relink the executable and cannot establish a
  current match score.
- `python3 -m vostok derive refresh` only re-derives the database from the
  existing report. It does not compile changed source.
- Never hand-edit the generated match-score block in `README.md` or derived
  rows in `docs/binary_matching/match_state.tsv`.
- Every commit must represent a measured source state. Before creating or
  amending a commit, run a successful full `vostok build` and include the
  resulting `README.md` and `docs/binary_matching/match_state.tsv` changes in that
  same commit. This applies to source, comments, documentation, tooling, and
  structure-verification changes because line movement and LTCG can alter code.
- Inspect the small `binaries/objdiff/report-changes.json` for regressions.
  Slice the large `report.json` with `jq`; never print or load it wholesale.
- After editing `scripts/`, run `ruff check scripts/` and
  `python3 -m vostok.tests.test_match_db`.
- The tooling is the `scripts/vostok/` package (`core/ ledger/ derive/ sema/
  build/ diff/ tool/ tests/`); every command in these docs runs it as
  `python3 -m vostok ...` (the dev shell puts `scripts/` on `PYTHONPATH`).
  Put any new repo path in `vostok/core/paths.py`, which is the only module
  allowed to spell one. See CLAUDE.md, "Where the tooling lives".
- Treat historical fuzzy peaks as scheduling/ICF history only. The
  correctness-facing `max` ledger column is scoped to the function's own
  source-body hash and must never be backfilled from ordinary history.

## Matching invariants

- Reproduce the shipped target exactly, including bugs, dead code, unusual
  control flow, declaration order, access order, and const qualification.
- Faithful source structure outranks fuzzy percentage. A 100% byte score can
  still have the wrong initializer-list, braces, lexical blocks, locals, or
  statement partitioning.
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
  when necessary and when file ownership remains disjoint.
- Preserve definition order, even when access specifiers repeat or interleave.
- Use clangd helpers for source navigation and `pdb_fetch`/`pdb_rich_query` for
  binary evidence.

## Semantic evidence tools

- Start an optimized-function dossier with `python3 -m vostok sema rva <fn>`,
  `xref <fn> --callees`, and `strings <fn>`. Use `strings --find <text>` for
  reverse literal lookup and plain `xref <fn>` to find direct callers. Select
  `--base` only when investigating what the current build actually emitted.
- Diagnose control flow with `vostok sema blocks <fn> --diff --lite` first, then
  `blocks --diff` for instruction bodies and `branches --diff` only after the
  block view establishes a real divergence. Use `dot` for graph output and
  `sweep --module <m> [--unit <u>]` for a module queue. `BRANCH-COUNT` is the
  highest-yield class; `FLOW-SAME` is not source-shape work.
- The complete useful HoMM2/Gruntz sema family is covered, but its commands map
  to the strongest Vostok evidence owner rather than being duplicated:
  `pdb_fetch --view target|base|diff|structure|structure-diff|callees|info`
  owns disassembly and PDB statement/local evidence; `pdb_rich_query --list`
  owns symbol and RVA listing; `vostok tool clangd symbol|def|refs|hover` owns
  source-semantic navigation; and `vostok ledger report|list|queue` owns
  function/unit status, queues, attempt records, flags, and hash-scoped MAX state.
- Gruntz's `map`, `class`, and `vtable` commands compensate for a stripped
  binary. Vostok has the retail PDB: use `binaries/structure/target/headers` for
  class layout, inheritance, and virtual declaration order, target
  `pdb_rich_query --list`/`vostok ledger list` for the emitted function map, and
  `pdb_fetch --view target` at real virtual call sites for slot offsets. These
  are authoritative; do not port stripped-binary hierarchy or ownership guesses
  over the PDB evidence.
- Work optimized modules from owning roots toward leaves. Use `xref --callees`
  to descend only into blockers that keep a root folded, stripped, or shaped
  incorrectly; remeasure the root after bodying a callee. An isolated leaf is
  not useful LTCG evidence until its real caller cone exists.
- These commands read generated indexes. If their result conflicts with changed
  source, the base side is stale; run a successful full `vostok build` before
  trusting it. `vostok sema` return code 1 is an ANSWER, not a failure, but it
  does not mean "the flows differ": `blocks --diff` answers "same flow AND every
  aligned block identical", so it returns 1 for nearly every function below
  100%, including ones it printed `flow SAME` for. Take the flow verdict from
  the printed `flow SAME | DIFFERS` line. Return code 2 is the error code.
- `sema` trims trailing blocks it cannot reach from the entry, and a computed or
  tail `jmp` makes everything after it unreachable, so on some switch-shaped
  functions the trim removes real code and the verdict then covers a prefix
  only. The views print how many blocks they trimmed and flag the case where
  more was dropped than kept; when that flag appears, read the function with
  `pdb_fetch --view target|base` instead. See `sema_tools.md`, "Known limits".
- `pdb_divergence` excludes raw CodeView line-table entry-count differences by
  default. `--raw-line-table-counts` exposes them as `[line-table]` only for
  forensics; optimization attribution and source-line packing affect the count,
  so never change whitespace or combine statements to satisfy it. Verify real
  statement structure with `pdb_fetch --view structure-diff`, named locals, and
  the emitted assembly.

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
  status in source. They become stale; reports and the ledger own that state.
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
- Preserve every per-commit README/ledger snapshot. Land an approved linear
  stack by fast-forward, never squash it.
- Before committing a matching step, record every function actually worked
  exactly once with `vostok ledger tried` (a whole-TU mark is appropriate when
  the whole TU was reviewed). Put those marks and any flags in the same rebuilt
  ledger snapshot as the source change so the per-commit diff identifies the
  work.
- Do not push, merge, close PRs, prune worktrees, or delete branches unless the
  user or the active orchestration task authorizes it.

## Code review rules

- Review target faithfulness and source structure, not conventional refactoring
  preferences.
- Treat a high score over a `QUANTITY`, a wrong local set, or an ordering divergence as a
  correctness problem.
- Flag fabricated symbols, stale state comments, leftover carcasses, ungrounded
  constants, accidental logs, cross-unit inlining hacks, and generated
  README/ledger snapshots that do not correspond to their commit.
- Verify every proposed review fix with a full rebuild and report measured
  regressions, unresolved risks, and whether the stack is ready to fast-forward.
