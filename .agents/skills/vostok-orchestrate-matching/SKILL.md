---
name: vostok-orchestrate-matching
description: Drive binary matching for a whole Vostok module by dispatching disjoint translation-unit workers, serializing their results into one linear stack, rebuilding every commit, preserving the per-commit ledger, and periodically auditing structure. Use only when the user asks for module-scale or multi-unit matching and authorizes agent delegation.
---

# Orchestrate a matching stack

1. Read `AGENTS.md` and the orchestrator section of
   `docs/binary_matching/agentic_loop.md`. Maintain a visible ledger and plan.
2. Start from a clean, rebuilt stack tip. Use a separate clean worktree and
   `WINEPREFIX` for each worker.
3. Build a fresh queue from the ledger. Dispatch genuine `QUANTITY`, `SPLIT`,
   wrong-named-locals, target-only body, and verified low-score structural work.
   (The ledger's four classes are MATCH / SIZE / QUANTITY / SPLIT; a wrong local
   set has no class of its own - `pdb_fetch --view structure` is what shows it.) Do not
   waste workers on high-score `MATCH` or ordinary `SIZE` codegen walls.
4. Assign one complete TU per `$vostok-match-unit` worker; bundle only tiny
   related units. Keep every live worker's root and callee file sets disjoint.
5. Parallelize investigation, not history. Integrate each finished worker one
   at a time by cherry-picking onto the current tip. Never merge sibling
   branches or create a PR fan.
6. After each integration, run the full `python3 -m vostok build` and inspect
   regressions. Before committing, mark every function actually worked exactly
   once (`vostok ledger tried <mangled>...` for the TU's functions), apply parks,
   then fold the generated README and ledger into that
   same source commit. A diff with no attempt increment for worked code is a
   bookkeeping defect. No commit may borrow a later ledger snapshot.
7. Push/open a PR only after its commit is measured. Base each PR on the unit
   immediately below it.
8. After roughly 10-15 integrated units, run one
   `$vostok-verify-structure` audit over the batch and land its rebuilt fix as a
   new stack commit.
9. Continue until work is compiled-done or parked with a queryable cause and
   next action. For MAX progress use the `max` column (`vostok ledger list
   --module <m>`). Never carry MAX across an effective-source hash change; never
   type a score into the ledger. Audit attempts and parks before handoff.
10. Request or perform `$vostok-review-match` before landing. When authorized
    and ready, fast-forward the complete linear stack into the integration
    branch. Never squash; preserve every per-commit ledger snapshot.

Do not let workers push, merge, change PR bases, or edit shared worktrees.
