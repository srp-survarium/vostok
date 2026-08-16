---
name: vostok-orchestrate-matching
description: Drive binary matching for a whole Vostok module by dispatching disjoint translation-unit workers, serializing their results into one linear stack, rebuilding every commit, preserving per-commit match databases, and periodically auditing structure. Use only when the user asks for module-scale or multi-unit matching and authorizes agent delegation.
---

# Orchestrate a matching stack

1. Read `AGENTS.md` and the orchestrator section of
   `docs/binary_matching/agentic_loop.md`. Maintain a visible ledger and plan.
2. Start from a clean, rebuilt stack tip. Use a separate clean worktree and
   `WINEPREFIX` for each worker.
3. Build a fresh queue from `match.db`. Dispatch genuine `QUANTITY`, `SPLIT`,
   `LOCALS`, target-only body, and verified low-score structural work. Do not
   waste workers on high-score `MATCH` or ordinary `SIZE` codegen walls.
4. Assign one complete TU per `$vostok-match-unit` worker; bundle only tiny
   related units. Keep every live worker's root and callee file sets disjoint.
5. Parallelize investigation, not history. Integrate each finished worker one
   at a time by cherry-picking onto the current tip. Never merge sibling
   branches or create a PR fan.
6. After each integration, run the full `python3 -m vostok build` and inspect
   regressions. Before committing, mark every function actually worked exactly
   once in `attempts` (use `vostok derive tried --unit <tu>` when the whole TU was
   worked), apply flags, then fold the generated README and `match.db` into that
   same source commit. A diff with no attempt increment for worked code is a
   bookkeeping defect. No commit may borrow a later database snapshot.
7. Push/open a PR only after its commit is measured. Base each PR on the unit
   immediately below it.
8. After roughly 10-15 integrated units, run one
   `$vostok-verify-structure` audit over the batch and land its rebuilt fix as a
   new stack commit.
9. Continue until work is compiled-done or parked with a queryable cause and
   next action. Use `source_maxima`, not ordinary `history.best_fuzzy_pct`, for
   MAX progress (`vostok derive max --module <m> --below 100`). Never carry MAX
   across an effective-source hash change. Island evidence must be measured by
   the normal report/index refresh before `record-max` annotates it; never type
   a score into the ledger. Audit attempts and flags before handoff.
10. Request or perform `$vostok-review-match` before landing. When authorized
    and ready, fast-forward the complete linear stack into the integration
    branch. Never squash; preserve every per-commit database snapshot.

Do not let workers push, merge, change PR bases, or edit shared worktrees.
