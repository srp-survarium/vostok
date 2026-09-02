# Agentic matching loop

How a module gets matched end to end: an **orchestrator** builds a queue of
TUs with open functions and dispatches one **worker** per TU; each worker
takes its TU's functions as far as it can, then stops.

**Starting context for a run:** [`MATCHING.md`](MATCHING.md) (how the
source must look), this file (the process), and
[`patterns/INDEX.md`](patterns/INDEX.md) (one line per known asm -> source
pattern; cheap skim). Read all three first; pull individual pattern files on
demand via the search protocol in [`assembly_patterns.md`](assembly_patterns.md)
(grep the INDEX by `cpp:`/`asm:`/`topic:` tag or symptom token, read only hits).

**Unit of work: one TU** = one branch = one commit = one PR - a worker owns ALL
of a translation unit's open functions (sushi, 2026-06-12; supersedes
per-function and cross-TU small-function batching, which caused churn). Matched
in their real TU, small helpers share their callers' inlining/LTCG environment
and pair the way the target did. Tiny 1-3-function header units may be bundled
into one dispatch, each still handled whole; an inlined cluster spanning TUs is
the one reason to pull in an outside function.

**PRs are stacked.** Each match branches off the previous match's branch (the
stack tip) and its PR targets that branch, not xray/feature. So every worker
inherits all prior matched source, anchors, and notes; `temp_include_all.cpp`
edits never conflict; and the human reviews the stack one PR at a time, in order.

**Above all, reproduce the target exactly - never "fix" bugs or odd logic** (see
MATCHING.md, rule #1). The target binary is ground truth; matching its bytes,
warts and all, beats any notion of "correct".

## Orchestrator and workers

Run the loop as two tiers so context stays clean:

- **Orchestrator** (long-lived): owns the queue (section 0). For each TU it
  dispatches one worker, collects its one-line result
  (`unit -> per-fn %s -> PR`), and records flags. It never holds the
  disassembly or diff text, so its context stays small across the whole module.
- **Worker** (one per TU): runs the per-function loop (sections 1-9) for each
  open function in its TU, in its own fresh context, then returns just that
  one-line result. All the heavy context - target/base disassembly, `--view
  diff` dumps, the iteration history - lives and dies inside the worker, so it
  pollutes neither the orchestrator nor the next worker.

Within one build tree dispatch is **sequential** (each `vostok build` rewrites
the shared base side - `binaries/rich/base`, `report.json`); the orchestrator
parallelizes across SIBLING WORKTREES instead (own `binaries/`, own Wine
prefix), up to its worker cap - see `.claude/agents/orchestrator.md`.

In Claude Code this maps onto subagents: the orchestrator spawns each worker with
one `Agent` call - `subagent_type: matcher` (defined in
[`.claude/agents/matcher.md`](../../.claude/agents/matcher.md), preloaded with the
matching docs), foreground, not `run_in_background` - so the worker runs in its own
context window and only its final one-line result returns to the orchestrator.
Spawn the next worker only after the current one returns. The orchestrator role
itself is [`.claude/agents/orchestrator.md`](../../.claude/agents/orchestrator.md) -
run it as the top-level agent, since subagents cannot reliably spawn subagents.

---

## 0. Build the queue (orchestrator)

The ledger (`config/match_state.tsv`, design in `ledger_design.md`)
owns queue building:

```
python3 -m vostok build            # canonical build; re-derives the ledger at the end
python3 -m vostok ledger report --module <m> [--per-unit]
python3 -m vostok ledger queue  --module <m> [--limit N] [--json]
```

`queue` emits ONE batch per TU - all of the TU's open functions together,
smallest TU first - automatically skipping done functions (100% + struct
MATCH), out-of-scope ones (paired once, vanished/regressed without a source
touch - external inlining), and parks. `vostok ledger list --status blocked` finds the unpaired
sets; `rg "STATE\[STUB\]" sources/vostok/<module>` still works for an in-source
view. Work the batches until `report` shows every function done or parked
(a `SKIP` flag with a written cause).

The orchestrator is the match DB's SINGLE WRITER: `vostok build` regenerates the
DB at the end of every build (or `refresh` re-derives it regen-only), it records
`flag`s (from worker result lines), and commits the DB at run milestones -
workers never edit it.

## 1. Per-function loop (one worker, per function of its TU)

The orchestrator hands the worker a TU + its open-function list; for each
function the worker does the rest:

1. **Get the target assembly** for it (section 2).
2. **Write a first approximation** of the body in its `.cpp`, following
   `MATCHING.md`. Reference it from `temp_include_all.cpp` so the linker keeps it
   (section 3). Pull in any missing types (section 4).
3. **`python3 -m vostok build`** - builds under Wine, then logs
   `Match: code X% / functions Y%`, refreshes the diff inputs, and regenerates
   the ledger from the fresh `report.json`. Read the new per-function number from
   `binaries/objdiff/report.json` and any regressions/improvements from
   `binaries/objdiff/report-changes.json`.
4. **Compare again and iterate.** The rebuild also refreshes the *base* rich
   index, so re-diff base vs target with `pdb_fetch --view diff` (section 2a) to
   read the exact diverging instructions, adjust the source, rebuild. Each pass
   should move the percentage or teach you something.
5. **Stop** when the function matches, or when you judge you can no longer make
   progress. You decide when to stop - do not spin.
6. **Record the outcome** (sections 5-7): on a real match drop the function's
   `// STATE[STUB]` flag and delete the carcass; if parked, leave the
   `claude@NOTE:` and name the cause in your result line (the orchestrator
   records it as a match-DB flag). Put the stuck-reason and tried variants in
   the commit message, NOTE any inlining, and flag any regression you caused.
7. **Commit and open the PR** (section 8). Move to the next function.

## 2. Getting the target assembly

Use **`pdb_fetch`** / **`pdb_rich_query`** (from vostok-pdb-parser, on PATH inside
`nix develop`) against the prebuilt indexes under `binaries/rich/`:

```
# discover / disambiguate overloads first (rva  file  signature)
pdb_rich_query --index binaries/rich/target/index.jsonl --function <name> --list

# 1. the statement STRUCTURE - the shape to reproduce (one row per source
#    statement: address, offset, <byte size>, 'srcline', [n] block-opens):
pdb_fetch --target-index binaries/rich/target/index.jsonl \
  --function <name> [--rva 0x...] --view structure

# 2. the RICH ASM for the whole function - offset-prefixed instructions,
#    each statement headed by `[0xNN]:` with its byte size:
pdb_fetch --target-index binaries/rich/target/index.jsonl \
  --function <name> --view target

# 3. the asm of ONE statement when the function is dense - slice by the
#    structure's `address` column (an absolute VA: it also SELECTS the function,
#    so no --function needed), by function-relative offset, or by 1-based index:
pdb_fetch --target-index binaries/rich/target/index.jsonl --view target --address 0x<va>
pdb_fetch --target-index ... --function <name> --view target --offset 0x<offst>
pdb_fetch --target-index ... --function <name> --view target --index <N>
```

Work in that order: read the structure first (it is the skeleton your source must
reproduce - statement count, order, sizes, block-opens), then the rich asm to write
the body, then per-statement slices to drill into any statement that is dense or
diverging - a slice keeps context tight where the full listing would drown you.
Other views: `structure-diff --condensed` (the two-sided target-vs-base statement
diff - your FIRST stop on a non-100% function: it localizes WHICH statement diverges
and HOW, see 2a), `callees` (the function's call targets resolved to signatures -
match those first), `info` (PDB-recorded locals). The same `--address`/`--offset`/
`--index` slicing works on the base side (`--base-index ... --view base`) once your
code builds.

`binaries/rich/target` is built once at setup and never changes;
`binaries/rich/base` is refreshed by every `vostok build`, so you always have a fresh
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
mismatch (grep `patterns/INDEX.md` for `topic:convention`) - match the body, don't chase those. But a
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
a different objdiff version. So `report.json` stays the number of record;
use this view for *where* the diffs are.

## 3. Make it reachable (or the linker strips it)

The problem is not compilation - it is dead-code elimination. The delinker can
only compare functions that survive into the linked EXE; the linker drops any
function not reachable from `main`, so an unreferenced match never appears on the
base side and cannot be diffed.

Matched code is kept alive through
`sources/vostok/game/sources/temp_include_all.cpp` (game's, since the game
carcass rebuild; previously under game_core): an `IncludeAll` instance
is created in `game/sources/game_entry_point.cpp::create_world` (reachable
from `main` via game_module_proxy), and its
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
cannot steer - name the cause in your result line (it becomes the function's
match-DB `NOTE` flag).

## 6. When stuck - what to write

A failed/parked match leaves its context where the NEXT matcher will look (the
same two-layer convention other decomp projects use - a non-matching note at the
function + an external tracker):
- a terse **`claude@NOTE:` above the function** - why it is stuck, what you tried,
  a couple of lines max. Facts about the attempt, never a % or a diff dump.
- a **match-DB `SKIP` flag** whose cause is the same conclusion plus the concrete
  next step (named in your result line; the orchestrator records it - the live %
  stays in `report.json`).

Full detail - every variant and its score, the diverging statements - goes in the
COMMIT/PR MESSAGE (section 7), not inline.

## 7. Run narrative: the commit/PR message (NO per-function `.md`)

We do NOT keep per-function `.md` logs. Module-wide notes (gotchas, shared types,
asm quirks for the whole module) still live in
`docs/binary_matching/<module>/README.md`; everything per-function splits into:

- **the match DB** - derived status (%s, structure class, pairing history) plus
  the hand-written `SKIP`/`NOTE` flag causes;
- **the COMMIT/PR MESSAGE** - the narrative a reviewer needs to replay the run:
  the key commands, each source variant you tried with its resulting % /
  structure-diff outcome, the final residual and why. Shape:

  ```
  <module>: match <function> (NN% TAG)

  structure: target N stmts / base N stmts (MATCH | residual: <...>)
  variants: <one line each - what changed, what it scored>
  blockers/regressions: <... or none>
  ```

- any reusable asm -> source mapping is promoted to `patterns/` (section 8).

## 8. Assembly patterns -> `patterns/`

Whenever you recognize an asm shape mapping back to specific C++ (a ctor
prologue, an intrinsic, a constant-load idiom, a container call), add it as a
**new file** under [`patterns/`](patterns/) plus **one line** in
[`patterns/INDEX.md`](patterns/INDEX.md), same commit (schema + tag vocabulary
in [`assembly_patterns.md`](assembly_patterns.md)); merge into an existing
pattern file when it is the same lesson. That knowledge base is shared context
for every future run; growing it is how the loop gets faster.

## 9. Commit & PR

One function (or one inlined-together cluster) per branch, commit, and PR.
Branch off the main matching branch; commit message names the module/function
and final percentage; open the PR with `gh` for review. Then start the
next function.

---

**Stop condition for the whole run:** every function in the module is `DONE` or
parked (a `SKIP` flag) with its cause recorded in the match DB (narrative
in the commit messages).

Missing tooling you wish you had goes in
[`unanswered_questions.md`](unanswered_questions.md).
