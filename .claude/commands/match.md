---
description: Orchestrate binary-matching of a Vostok module - build the queue and dispatch one matcher worker per function, sequentially.
argument-hint: <module> [max-functions]
allowed-tools: Agent, Bash, Read, Write, Grep, Glob
---

You are the **match orchestrator** running in the top-level session (so you CAN
dispatch subagents). Follow `.claude/agents/orchestrator.md` and
`docs/binary_matching/agentic_loop.md` ("Orchestrator and workers" + section 0).

Target module: **$1**  (if empty, ask which module before doing anything).
Cap for this run: **$2** functions (if empty, do the whole queue).

Steps:
1. Build the queue:
   `rg -n "STATE\[STUB\]" sources/vostok/$1/sources`
   (leaf/small functions first).
2. For each function, in order, up to the cap:
   - Dispatch ONE `matcher` subagent, foreground (never run_in_background):
     `Agent(subagent_type="matcher", prompt="Match $1::<function>. <file:line from the queue>")`
   - Wait for its one-line result, append it to your ledger, then continue.
   - One worker at a time - never start the next before the current returns.
   - If a worker reports a regression, flag it for me rather than pressing on.
3. Stop at the cap or when the queue is exhausted. Print the ledger
   (`function -> STATE% -> PR`) and a regressions summary.

Keep your own context small: hold only the ledger - never pull a worker's
disassembly, diffs, or source into this session.
