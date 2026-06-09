---
description: Orchestrate binary-matching of a Vostok module - build the queue, dispatch matcher workers in batches on a stacked-PR chain, then structure-verifiers to verify and fix each unit.
argument-hint: <module> [max-functions]
allowed-tools: Agent, Bash, Read, Write, Grep, Glob
---

You are the **match orchestrator** running in the top-level session (so you CAN
dispatch subagents). Follow `.claude/agents/orchestrator.md` and
`docs/binary_matching/agentic_loop.md` ("Orchestrator and workers" + section 0).

Target module: **$1**  (if empty, ask which module before doing anything).
Cap for this run: **$2** functions (if empty, do the whole queue).

Steps:
1. **Build the queue:** `rg -n "STATE\[STUB\]" sources/vostok/$1/sources` (plus any
   `PARTIAL`/`SKIPPED` you were asked to retry). Order leaf/small-first. Refresh the
   README score block now as a baseline (`python3 scripts/match_score.py --write-readme`).
2. **Stacked PRs - every unit stacks on the previous one.** Track the current **stack
   tip** (start: the latest match branch, or `feature/...` to root a fresh stack). Before
   each dispatch `git checkout <tip>` so the worker branches off it, and name `<tip>` as
   the PR base in the prompt; the returned branch is the new tip. Workers thus inherit each
   other's source, anchors, and notes automatically (no forward-porting), and
   `temp_include_all.cpp` edits never conflict. To land/refresh a stacked PR on an advanced
   base, **cherry-pick its OWN commits** onto a fresh checkout of the base - never `git
   merge` the base in (it drags in every inherited file and mangles
   `temp_include_all.cpp`).
3. **Dispatch matchers in BATCHES**, foreground (never `run_in_background`), one at a time
   up to the cap ($2):
   `Agent(subagent_type="matcher", prompt="Match $1::<batch of functions>. <file:line/rva each>. Branch off <tip>, PR --base <tip>.")`
   Batching lowers token cost - the worker pays the shared setup (docs, class decl, member
   offsets, anchor, context) ONCE per unit: **3-4 small multi-line functions** per unit,
   **up to ~10 one-liners**, **fewer (down to 1) the harder they are**. Prefer a related
   cluster (same class, or sibling same-shape classes). Tell the worker to mark any hard
   member `INPROGRESS` rather than spin. Wait for its one-line result, append to the ledger,
   advance the tip. Never start the next before the current returns (workers share the base
   build / `report.json`, so parallel runs race). If a worker reports a regression, decide:
   queue a follow-up fix or flag it for me - don't silently move on.
4. **Audit each finished unit:** dispatch a `structure-verifier` over the unit's functions.
   It embeds the condensed `--view structure-diff` + a `// VERDICT:`, downgrades any
   mislabeled `DONE` (a high % over the wrong shape), AND - its Phase 2 - becomes the matcher
   and FIXES the divergences it found, rebuilding and re-diffing until the structure matches
   or only an LTCG/argument residual remains. (Optionally a `reviewer` for target/base
   confusion, lean-comments, %-accuracy, wrongly-banked LTCG.) Only queue a separate
   follow-up matcher if the verifier flags something outside its scope.
5. **Stop** at the cap or when the queue is exhausted/parked (with reasons). Refresh +
   commit the README score block, then print the ledger (`function -> STATE% -> PR`) and a
   regressions summary.

Keep your own context small: hold only the ledger - never pull a worker's disassembly,
diffs, or source into this session. Full detail lives in `.claude/agents/orchestrator.md`.
