# Agentic matching loop

How a module gets matched end to end: an **orchestrator** builds a queue of
unmatched functions and dispatches **workers** (a batch of functions each) in
parallel across sibling worktrees; each worker takes its batch as far as it
can, then stops.

**Starting context for a run:** [`MATCHING.md`](MATCHING.md) (how the
source must look), this file (the process), and
[`assembly_patterns.md`](assembly_patterns.md) (asm -> source mappings learned so
far). Read all three first.

**Unit of work:** one unit = one branch = one commit = one PR. A unit is a
BATCH of related functions by default - batching amortizes the worker's fixed
setup cost (shared docs, class decl, member offsets, anchor); sizes and
composition are in `.claude/agents/orchestrator.md` ("Batch several small
functions per dispatch"). Typical units: an *inlined cluster* (A inlined into
B, cannot be separated), or a group of *same-class functions*
(getters/setters, sibling state variants) that share scaffolding. Don't grab
unrelated nearby functions.

**PRs are stacked.** Each match branches off the previous match's branch (the
stack tip) and its PR targets that branch, not xray/feature. So every worker
inherits all prior matched source, anchors, and notes; `temp_include_all.cpp`
edits never conflict; and the human reviews the stack one PR at a time, in order.

**Above all, reproduce the target exactly - never "fix" bugs or odd logic** (see
MATCHING.md, rule #1). The target binary is ground truth; matching its bytes,
warts and all, beats any notion of "correct".

## Orchestrator and workers

Run the loop as two tiers so context stays clean:

- **Orchestrator** (long-lived): owns the queue (section 0). It prepares a
  sibling worktree per unit, dispatches a worker into it, and records a one-line
  result (`unit -> STATE% -> PR`) when the worker returns. It never holds the
  disassembly or diff text, so its context stays small across the whole module.
- **Worker** (one per unit): runs the per-function loop (sections 1-9) for its
  batch in its own fresh context, then returns just that one-line result. All the
  heavy context - target/base disassembly, `--view diff` dumps, the iteration
  history - lives and dies inside the worker, so it pollutes neither the
  orchestrator nor the next worker.

Dispatch is **parallel, up to N workers** (the run's cap, default 3) - each
worker runs in its OWN sibling worktree (`vostok_<N>`) with its own `binaries/`
and WINEPREFIX, so parallel rebuilds and `report.json`s never collide. (Dispatch
used to be sequential when all workers shared one build tree; per-worktree
isolation removed that constraint.) Workers must never share a file/TU - see
`.claude/agents/orchestrator.md` ("Concurrency") for the non-overlap rule.

In Claude Code this maps onto subagents: the orchestrator spawns each worker with
one `Agent` call - `subagent_type: matcher` (defined in
[`.claude/agents/matcher.md`](../../.claude/agents/matcher.md), preloaded with the
matching docs), `run_in_background: true` - so the worker runs in its own context
window and only its final one-line result returns to the orchestrator. The
orchestrator role itself is
[`.claude/agents/orchestrator.md`](../../.claude/agents/orchestrator.md) -
run it as the top-level agent, since subagents cannot reliably spawn subagents.

---

## 0. Build the queue (orchestrator)

Collect the `STUB` (and other unmatched) functions for the module with `rg`:

```
rg -n "STATE\[STUB\]" sources/vostok/<module>/sources
```

Also sweep [`review_todos.md`](review_todos.md)'s **Actionable** section for open
rows touching your module - identified review questions nobody acts on are wasted.
Order the queue however you like (small/leaf functions first is usually easiest).
Work the list top to bottom until every entry is `DONE` or deliberately parked
(`SKIPPED` / `BLOCKED` / `PARTIAL`) with a written reason.

## 1. Per-function loop (one worker)

The orchestrator hands the worker a single function; the worker does the rest:

1. **Get the target assembly** for it (section 2).
2. **Write a first approximation** of the body in its `.cpp`, following
   `MATCHING.md`. Reference it from `temp_include_all.cpp` so the linker keeps it
   (section 3). Pull in any missing types (section 4).
3. **`python3 scripts/rebuild.py`** - builds under Wine, then logs
   `Match: code X% / functions Y%` and refreshes the diff inputs. Read the new
   per-function number from `binaries/objdiff/report.json` and any
   regressions/improvements from `binaries/objdiff/report-changes.json`.
4. **Compare again and iterate.** The rebuild also refreshes the *base* rich
   index, so re-diff base vs target with `pdb_fetch --view diff` (section 2a) to
   read the exact diverging instructions, adjust the source, rebuild. Each pass
   should move the percentage or teach you something.
5. **Stop** when the function matches, or when you judge you can no longer make
   progress. You decide when to stop - do not spin.
6. **Record the outcome** (sections 5-7): update the `STATE` marker, write the
   stuck-reason and tried variants, NOTE any inlining, and flag any regression
   you caused.
7. **Commit** (section 9) - the orchestrator opens the PR. Move to the next
   function in the batch.

## 2. Getting the target assembly

Use **`pdb_fetch`** / **`pdb_rich_query`** (from vostok-pdb-parser, on PATH inside
`nix develop`) against the prebuilt indexes under `binaries/rich/`:

```
# discover / disambiguate overloads first (rva  file  signature)
pdb_rich_query --index binaries/rich/target/index.jsonl --function <name> --list

# the target listing for one function (--rva 0x... for an exact pick)
pdb_fetch --target-index binaries/rich/target/index.jsonl \
  --function <name> --view target
```

This is the reference ("rich") assembly you diff against: offset-prefixed
instructions, each statement annotated with its byte size `; <0xNN>`. Other views:
`structure-diff --condensed` (the two-sided target-vs-base statement diff - your FIRST
stop on a non-100% function: it localizes WHICH statement diverges and HOW, see 2a),
`structure` (one side's statement skeleton), `callees` (the function's call targets
resolved to signatures - match those first), `info` (PDB-recorded locals).

`binaries/rich/target` is built once at setup and never changes;
`binaries/rich/base` is refreshed by every `rebuild.py`, so you always have a fresh
base-vs-target pair (section 2a).

## 2a. The base-vs-target instruction diff

`report.json` (step 3 below) is the **scoreboard** - per-function match % and
regressions across a rebuild. To see *where* base and target diverge, ask
`pdb_fetch` for the operand-aware diff (objdiff-core backend), which interleaves
your base source onto the rows:

```
pdb_fetch --target-index binaries/rich/target/index.jsonl \
          --base-index   binaries/rich/base/index.jsonl \
          --function <name> --view diff \
          --objdiff-base-dir   binaries/objdiff/base \
          --objdiff-target-dir binaries/objdiff/target
```

Read the rows: `  ` equal, `~ base -> target` same slot/different instruction,
`-` base-only, `+` target-only. A *handful* of `~` rows that are only a register or
`[ebp-XX]` stack-slot difference is usually an LTCG/linker artifact, not a real
mismatch (see `assembly_patterns.md`) - match the body, don't chase those. But a
*storm* of `[ebp-XX]` slot renames (dozens at once) is usually NOT LTCG: it means
your locals landed in different stack slots because your block structure differs
from the target's - a missing brace scope, an `if`/loop the target has, statements
written flat where the target nests them. The carcass shows it as target `[n]`
block-opens with `<1>`-tagged block-scoped locals that your base structure lacks.
Block-scope the locals to match the target's braced blocks before writing it off as
LTCG. (This is exactly what masqueraded as "447 slot renames" on `process_leg`.)

The header `objdiff fuzzy match P%` is a target-byte-weighted fuzzy match (partial
credit per instruction, so a stack-slot-only `~` barely costs anything). It tracks
`report.json`'s `fuzzy_match_percent` closely but slightly conservatively (e.g.
89.6 vs 89.3 here) - it is *not* bit-identical, since `report.json` is generated by
a different objdiff version. So `report.json`/`STATE` stays the number of record;
use this view for *where* the diffs are.

## 3. Make it reachable (or the linker strips it)

The problem is not compilation - it is dead-code elimination. The delinker can
only compare functions that survive into the linked EXE; the linker drops any
function not reachable from `main`, so an unreferenced match never appears on the
base side and cannot be diffed.

`game_core` keeps matched code alive through
`sources/vostok/game_core/sources/temp_include_all.cpp`: an `IncludeAll` instance
is created in `game/sources/game_world.cpp` (reachable from `main`), and its
constructor calls one `use_<thing>( )` per matched class; each `use_*`
instantiates the class and calls its methods, referencing the functions so the
linker retains them. The same file also anchors the `network_core` and `logging`
functions being matched (e.g. `use_network_core_http_client`, `use_log`).

So when you finish a function, **reference it from `temp_include_all.cpp`**: add a
call in the relevant `use_*( )` block (or add a new `use_*` and call it from
`IncludeAll::IncludeAll( )`). Use `NULL` / dummy arguments - the call never runs,
it only has to exist so the symbol is kept.

Exception: a function already called by another anchored (reachable) function is
kept alive transitively - do not add it again.

## 4. Missing types

If the body references a class/struct that is **not declared anywhere** in base:
find it in the **target structure** (`binaries/structure/target`, the
PDB-generated headers/stubs) and declare it - forward-declare if a pointer
suffices, copy the full declaration if needed - **near where it is used**. The
carcass source-line numbers (`:'NN'`) tell you where the original file had the
declaration, so place it at the same relative spot.

## 5. Inlining (you cannot control it)

MSVC decides inlining; you cannot force or forbid it. When a call inlined in the
target but not in your base (or vice versa) you will see it as a missing/extra
`call` and, in the carcass, as a large `+delta` between two statement addresses.
Identify the block, leave a `claude@NOTE:` (or `claude@MATCH:` if you reshaped
the source to compensate), and do **not** burn the loop fighting inlining you
cannot steer - note it and set `STATE[..%|PARTIAL]`.

## 6. When stuck - what to write

At the **top of the function** (comment block above it):
- `claude@NOTE:` why it is stuck / your best current understanding.
- a short list (one line each) of the variants you tried.

Then update the `STATE` marker (`SKIPPED` / `PARTIAL` / `BLOCKED`, with the
reason and percentage). Full detail - every variant and its score - goes in the
commit message (section 7), not inline.

## 7. Where the rationale lives: the commit message (NO per-function `.md`)

Per-function `.md` trails and module `PROGRESS.md` ledgers were dropped
(2026-06-09): they proliferate, go stale the moment a later match touches the
function, and are pure context cost. Do NOT create
`docs/binary_matching/<module>/<function>.md`. What goes where instead:

- **The commit message** carries the run narrative: what you tried, each source
  variant and its resulting %, the final blocker. A reviewer replays your run
  from the commit/PR, not from a doc.
- **The source** keeps only the lean markers: the `STATE[..]` line and
  `claude@MATCH:`/`@NOTE:`/`@TODO:` tags (MATCHING.md "Comment hygiene"), plus
  the structure-verifier's `// STRUCTURE DIFF` embed on non-100% functions.
- **`docs/binary_matching/<module>/README.md`** holds module-wide notes only
  (gotchas, shared types, asm quirks that span functions) - never per-function
  history.
- **Reusable knowledge** is promoted: asm->source mappings to
  [`assembly_patterns.md`](assembly_patterns.md) (section 8), iteration-saving
  process tricks to [`loop_performance.md`](loop_performance.md).

## 8. Assembly patterns -> `assembly_patterns.md`

Whenever you recognize an asm shape mapping back to specific C++ (a ctor
prologue, an intrinsic, a constant-load idiom, a container call), **append it**
to [`assembly_patterns.md`](assembly_patterns.md). That file is shared context
for every future run; growing it is how the loop gets faster.

## 9. Commit & PR

One unit (the batch) per branch, commit, and PR. The worker's worktree is
already on the unit's branch off the current stack tip (the orchestrator
prepared it); the worker squashes to ONE commit whose message names the
module/functions, the final `STATE` percentages, and the run narrative
(section 7) - and does NOT push or open the PR: the orchestrator owns
branch/push/PR/stacking. Then the next unit.

---

**Stop condition for the whole run:** every function in the module is `DONE` or
parked (`SKIPPED`/`BLOCKED`/`PARTIAL`) with a written reason in the `STATE`
marker and commit message.

Missing tooling you wish you had goes in
[`unanswered_questions.md`](unanswered_questions.md).
