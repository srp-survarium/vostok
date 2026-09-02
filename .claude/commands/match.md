---
description: Orchestrate binary-matching of a Vostok module - build the queue, dispatch matcher workers in batches across N parallel sibling worktrees on a stacked-PR chain, then structure-verifiers to verify and fix each unit.
argument-hint: <module> [parallel-workers]
allowed-tools: Agent, Bash, Read, Write, Grep, Glob
---

You are the **match orchestrator** running in the top-level session (so you CAN
dispatch subagents). Follow `.claude/agents/orchestrator.md` and
`docs/binary_matching/agentic_loop.md` ("Orchestrator and workers" + section 0).

Target module: **$1**  (if empty, ask which module before doing anything).
Parallel workers: **$2** (if empty, default 3) - this caps CONCURRENCY only; the run
processes the WHOLE queue regardless.

Full rules: `.claude/agents/orchestrator.md`. In short:
1. **Build the queue:** `python3 -m vostok ledger queue --module $1` (one batch per TU,
   worst first). Order leaf/small-first. (`vostok build` keeps the README
   score block current on its own - no manual refresh step.)
2. **Stacked PRs:** track the **stack tip** (the latest match branch, or the current tip
   of `xray-2.0-prog-v0.100b` to root a fresh stack). Matchers branch off the TOP so
   percentages compound; you review the stack BOTTOM-UP and merge one PR at a time.
3. **Dispatch up to $2 matchers in PARALLEL** (`run_in_background: true`), each in a sibling
   `vostok_<N>` worktree YOU prepared (`git reset --hard <tip>`, `vostok.build.ninja_regen`,
   `git checkout -b match/$1-<unit>`) - isolated `binaries/`/`WINEPREFIX`, so builds don't
   race. Batch per unit (the worker pays setup once): **6-9 small multi-line fns**, **up to
   ~12 one-liners**, **fewer when hard**; a related cluster; pick NON-OVERLAPPING units
   (never two live workers on the same file/TU). The worker just matches + makes ONE commit:
   `Agent(subagent_type="matcher", prompt="Work in vostok_<N> (on branch match/$1-<unit>, indexes warm). Match $1::<batch>. <file:line/rva each>. Commit ONE commit; do not branch/push/PR.")`
4. **On each return (per unit):** push the branch + `gh pr create` (minimal body: fns + %s),
   then dispatch the `structure-verifier` and the `reviewer` onto the SAME branch (the PR
   becomes match + verify, + a review commit only if it fixed source). Same-wave siblings:
   resolve their `.vcproj` conflicts when stacking. The unit's branch is
   the new tip. If a worker reports a regression, queue a follow-up or flag it for me.
5. **Stop** when the queue is exhausted/parked (with reasons). Refresh + commit the README
   score, then print the ledger (`fn -> STATE% -> PR`) + a regressions summary.

Keep your context small: hold only the ledger - never pull a worker's disassembly, diffs,
or source into this session.
