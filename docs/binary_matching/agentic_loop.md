# Agentic matching loop

How a module gets matched end to end: an **orchestrator** builds a queue of
unmatched functions and dispatches one **worker** per function, sequentially;
each worker takes its function as far as it can, then stops.

**Starting context for a run:** [`MATCHING.md`](MATCHING.md) (how the
source must look), this file (the process), and
[`assembly_patterns.md`](assembly_patterns.md) (asm -> source mappings learned so
far). Read all three first.

**Unit of work:** one function = one branch = one commit = one PR. Exception: if
function A is inlined into B and cannot be separated, match the cluster together
in a single commit. Each PR is for a human to review before the next starts.

A concrete dry run of the whole loop on one function is in
[`agentic_loop_example.md`](agentic_loop_example.md).

## Orchestrator and workers

Run the loop as two tiers so context stays clean:

- **Orchestrator** (long-lived): owns the queue (section 0). For each entry it
  dispatches one worker, waits for it to finish, and records a one-line result
  (`function -> STATE% -> PR`). It never holds the disassembly or diff text, so
  its context stays small across the whole module.
- **Worker** (one per function): runs the per-function loop (sections 1-9) in its
  own fresh context, then returns just that one-line result. All the heavy
  context - target/base disassembly, `--view diff` dumps, the iteration history -
  lives and dies inside the worker, so it pollutes neither the orchestrator nor
  the next worker.

Dispatch is **sequential, one worker at a time** - not parallel. Each function's
`rebuild.py` rewrites the shared base side (`binaries/rich/base`, `report.json`),
and PRs are reviewed in order, so concurrent workers would race the build and the
scoreboard.

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

Collect the `STUB` (and other unmatched) functions for the module with `rg`:

```
rg -n "STATE\[STUB\]" sources/vostok/<module>/sources
```

Order them however you like (small/leaf functions first is usually easiest).
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
7. **Commit and open the PR** (section 8). Move to the next function.

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
`structure` (statement skeleton only - the cheap signal to compare before writing
code), `callees` (the function's call targets resolved to signatures - match those
first), `info` (PDB-recorded locals).

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
`-` base-only, `+` target-only. A `~` that is only a register or `[ebp-XX]`
stack-slot difference is usually an LTCG/linker artifact, not a real mismatch (see
`assembly_patterns.md`) - match the body, don't chase those.

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
reason and percentage). Full detail - pasted asm, every variant and its score -
goes in the per-function log (section 7), not inline.

## 7. Per-function log: `docs/binary_matching/<module>/<function>.md`

Each module has its own folder under `docs/binary_matching/` so its material does
not mix with the general docs: `README.md` holds module-wide notes (gotchas,
shared types, asm quirks for the whole module), and one `<function>.md` per
function you spent real effort on. Suggested shape for a function log:

```
# <module>::<function>

Target asm: <pdb-parser output, or the key fragment>

## Attempts
1. <one-line description>  ->  <match %>  ->  <what differed / why it failed>
2. ...

## Outcome
STATE[..]: <final status>. Blocker: <...>. Regressions caused: <unit/fn, or none>.
Inlining: <blocks that inlined differently>.
```

## 8. Assembly patterns -> `assembly_patterns.md`

Whenever you recognize an asm shape mapping back to specific C++ (a ctor
prologue, an intrinsic, a constant-load idiom, a container call), **append it**
to [`assembly_patterns.md`](assembly_patterns.md). That file is shared context
for every future run; growing it is how the loop gets faster.

## 9. Commit & PR

One function (or one inlined-together cluster) per branch, commit, and PR.
Branch off the main matching branch; commit message names the module/function
and final `STATE` percentage; open the PR with `gh` for review. Then start the
next function.

---

**Stop condition for the whole run:** every function in the module is `DONE` or
parked (`SKIPPED`/`BLOCKED`/`PARTIAL`) with a written reason and a per-function
log where it mattered.

Missing tooling you wish you had goes in
[`unanswered_questions.md`](unanswered_questions.md).
