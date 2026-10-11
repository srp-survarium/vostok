# vostok - working agreements (AGENTS.md is this file)

This repo binary-matches the Vostok Engine (Survarium v0.100b): C++ that,
compiled with the original MSVC 8.0 / VS2008 toolchain, produces byte-identical
objects to the shipped `survarium.exe` + `survarium.pdb`. `objdiff` compares our
"base" objects against the game's "target" objects. Everything (toolchain, Wine,
tools, assets) comes from the flake inside `nix develop`; never from sibling repos.

## The loop

    python3 -m vostok build             # notification-driven in Codex; full Wine build + refresh
    python3 -m vostok build --code-only # iteration build: code lane + function ledger only
    python3 -m vostok derive refresh    # re-derive the ledger from an existing report only - no rebuild

`--code-only` runs ninja, PDB evidence, structure, the code COFF/report and the
function ledger, and skips the data lane (data manifests, data COFF/reports,
image-data ledger, module data audits) and the README - roughly 5 minutes of
every build. The function ledger reads only code-lane artifacts, so `cur`, `max`,
`hist` and `cls` are exactly what a full build would record. What it does not
see is the data side: a change that moves which datum a function references
shows up only in the next full build. Use it while iterating on a unit; the
build whose README and ledger you commit must be a full `vostok build`.

The objdiff config is `binaries/objdiff/objdiff.json`; each build writes
`binaries/objdiff/report.json` and `report-changes.json`. Header edits
recompile their dependents automatically.

Long builds are notification-driven, not polled. With `CODEX_THREAD_ID` set,
the canonical build command starts a host service and queues one completion
notification after compilation and all measurement refreshes. Request host
user-bus execution approval if sandboxed. Keep the build worktree and branch
unchanged until notification; then inspect the result and warnings once. A
successful launcher exit is not a successful build. Do not repeatedly check a
session, log, process, or service to watch progress. If notification launch fails,
resolve the delivery problem rather than falling back to polling. Use
`--foreground` only for CI or an actual supervising process that waits for exit;
outside Codex, use `--background --notify-thread THREAD` for agent-driven work.
This applies to every matcher, reviewer, skill and build caller.

### Notification-driven builds: keep the matching pipeline busy

Build latency is time to improve the next function/unit, not idle time. Pipeline
the work: reconstruct A -> launch build A -> reconstruct B while A builds ->
on A's completion, verify A and fix its residuals -> queue the next measured
build and continue the next independent unit. Do not abandon A when moving to B.

After launch, record the job ID and measured source state, give one brief
acknowledgment, and immediately pick useful work from the structure queue.
Inspect the next target's statements, assembly, locals and callees; reconstruct
and prepare its patch. Keep edits in a separate preparation worktree or patch
outside the active build inputs. Never change source, headers, branch, ledger
or report inputs being measured. Keep builds sharing output artifacts serialized.

On the matching completion notification, checkpoint the next-unit work and
return to the measured unit promptly. Check warnings, structure/locals, bytes
and global regressions; fix remaining mismatches and rebuild as needed. Keep
prepared work queued across those iterations. Integrate one unit at a time into
the linear stack, committing only source with its own full-build README and
ledger. Preparation is not measured completion.

Do not sleep, repeatedly poll sessions/logs/processes/services, or send waiting
updates. A launcher exit or automatic goal continuation is not build completion;
a continuation should advance independent queued work. Keep preparation turns
bounded: after a useful unit checkpoint, end the turn so queued completion
messages can be delivered. This is a delivery checkpoint, not an idle wait;
if the next turn has no completion event, continue useful queued work. Otherwise
yield only when no useful independent work remains or user input is required,
not merely because a build is running. Keep the full goal active while awaiting
results; waiting alone is not a blocker. This rule applies to every matcher,
reviewer, skill and build caller.

## The ledger (`config/match_state.tsv`)

The committed record: one row per target function. `vostok ledger` reads the
file directly, so every query works on a fresh clone without a build.

    python3 -m vostok ledger report --module render [--per-unit]   # rollup; per TU, worst first
    python3 -m vostok ledger queue  --module render                # one batch per TU, worst first
    python3 -m vostok ledger list   --module render --class QUANTITY,SPLIT
    python3 -m vostok ledger list   --module render --status parked|blocked
    python3 -m vostok ledger list   --headroom                     # hist > max: we had it better once
    python3 -m vostok ledger tried  <mangled> --note "what was attempted"
    python3 -m vostok ledger park   <mangled> --cause "why it stops here"   (`open` undoes it)

Three percentages per row:

- `cur` - this build. Noisy: under LTCG/ICF a function moves without its source moving.
- `max` - the peak proven for *this exact source body* (`hash`); resets only when
  the body changes. **Driving every `max` to 100 is the campaign.**
- `hist` - the all-time peak. Never resets.

`(held <max>)` next to a row means `cur` dipped below a still-proven `max`: compiler
noise, **do not chase it**. A real regression is a `max` that fell (the body changed).
`hist > max` (`--headroom`) means an earlier implementation scored better - that IS
worth working. `status` is `done` / `inprogress` / `blocked` (something missing,
incl. target-only) / `parked` (worked, could not raise it; `note` says why - keep it
short, it stops the next matcher re-deriving a dead end).

## Structure matching

The retail PDB records, for every function, how the source was partitioned into
statements: which line each statement came from and how many bytes it compiled to.
A match is real only when our source reproduces that partition - the same statements,
in the same order, each compiling to the same bytes. The same bytes over a different
partition is a false win: a 100% `cur` on a `QUANTITY` row. The parser's two-sided
diff is the verdict:

    python3 -m vostok pdb inspect \
              --target binaries/pdb/target/evidence.sqlite \
              --base binaries/pdb/base/evidence.sqlite \
              --function 'weapon_core::tick' --view structure-diff

It lists only the diverging statements, tagged, and ends with `STRUCTURE MATCH`
when there are none. Read it like this:

    target 12 statements / base 11
    #4   TRGT_ONLY   m_history.reserve( 64 );        retail has a statement we lack: add it
    #9   BASE_ONLY   result = compute( );             we split what retail wrote as one statement: merge
    #11  SIZE +5     return callback.hasHit( );       same statement, 5 more bytes: inline-vs-call, look at the asm

What moves the partition is source shape, not logic:

- a member initialised in the ctor's initializer list vs assigned in the body
- an early-return guard (`if (!x) return;`) vs wrapping the body in `if (x) { }`
- one expression as one statement vs split through a named temporary
- a ternary vs an if/else; braces that open a lexical block
- which `.cpp` a body lives in and its definition order (a moved body changes the record)

The one-shot first look gathers this verdict with the ledger row (cur/max/hist,
`tried`, note), the geometry check, the `sema blocks` flow verdict and unmatched
callees, in one process, and prints a one-line verdict followed only by what
needs action (`--full` for everything, `--json` for agents, `--only`/`--skip`):

    python3 -m vostok evidence 'weapon_core::tick'

How to act: run the diff before writing a body (read the target's statements) and
after each build. `TRGT_ONLY` / `BASE_ONLY` rows are structure work - fix the shape
until they are gone. A `SIZE` residual that survives a correct shape is usually an
inline-vs-call or LTCG call-boundary difference: confirm it with
`python3 -m vostok sema blocks <fn> --diff --lite`, then park with the wall named
rather than bending the source to the bytes. The ledger's `cls` column
(`QUANTITY`, `SPLIT`, `SIZE`, `MATCH`) is only an approximation of this verdict;
`QUANTITY` and `SPLIT` rows are where to look first. Definitions and the deeper
PDB tools: `docs/binary_matching/ledger_design.md`, `docs/binary_matching/pdb_topology.md`.

### The matching order: whitespace and statements first, then bytes

This is the primary way to match. A percentage only means something on top of a
retail partition, so never trade a structure MATCH for percentage.

1. **Statements and whitespace.** Reproduce retail's statement partition *and* its
   line geometry. Each record carries a line, so the relative lines (`t.ln`) show
   where blank lines, split braces, declarations and comments sit. A line with no
   record between two records is still source: a brace, a `case`/`default:` label,
   or a statement that compiled to zero bytes (e.g. `return format;` when the value
   is already in `eax`). Lay the source out until every record lands on retail's
   relative line. Jump tables and other data can confirm what a gap may hold.
2. **Statement by statement.** Once the partition matches, compare each statement's
   bytes (`SIZE` rows, `--view target|base`, `sema blocks --diff`): operand order,
   temporaries, by-value vs by-reference access, types.
3. **Correctness.** Check that the matched code computes what retail's does. When our
   LTCG makes the retail shape behave differently, keep the correct code and record
   the wall with `vostok ledger tried`.

Iterate with `vostok build --code-only`.

## Matching rules

- Reproduce the shipped target exactly: bugs, dead code, odd control flow,
  declaration and access order, const qualification. Read the target's structure
  and assembly before writing a body; never fabricate a function, type, override
  or symbol to gain percentage, and never change another unit to steer a caller's
  inlining. Match one translation unit at a time.
- Evidence beats taste: `vostok evidence <fn>` for the one-shot verdict,
  `vostok-pdb inspect --view target|base|structure-diff` for
  disassembly and PDB statements, `python3 -m vostok sema rva|xref|blocks --diff`
  for control flow (its exit code 1 is an answer, read the printed verdict),
  `binaries/structure/target/headers` for class layout and vtable order.
- Attribute a residual to LTCG only when it is proven to be argument passing at a
  call boundary; otherwise it is source shape, or a concrete wall to record.
- Every commit is a measured state: run the full `vostok build` and commit the
  refreshed README block and ledger with the change. Record worked functions with
  `vostok ledger tried`. Stacks stay linear; land by fast-forward, never squash.
- Source carries no derived status: no percentages or diff dumps in comments.
  Live markers are `STATE[STUB|INLINED|UNMATCHABLE]` and `sushi@TODO`; drop them
  and any carcass narration when their condition is resolved.

## The tooling (`scripts/vostok/`)

One package, run as `python3 -m vostok ...` (`python3 -m vostok` maps the surface):
`core/` (paths, tsv, symbols, log, wine, fmt - blind helpers), `ledger/`, `derive/`
(report + PDB evidence databases -> ledger), `sema/` (control-flow views), `build/`, `diff/`,
`tool/`, `shaders/`, `data/`, `tests/`. Every repo path is spelled once, in
`core/paths.py` - add new ones there. After editing anything under `scripts/`, run
`ruff check scripts/` and `python3 -m vostok.tests.test_match_db`. If you change
the build/diff flow, tool names or generated artifacts, update README.md too.

Every `python3 -m vostok ...` run appends one line to `binaries/vostok_usage.log`
(command, branch, duration, exit code, failure reason), pooled in the main
checkout; wrap any new entry point's `__main__` in `vostok.core.log.run(prog, main)`.
`python3 -m vostok tool usage [--failures|--slow|--unused]` reads it back: the
evidence for which verbs earn their keep and which keep failing under agents.

## Source conventions

- Comments stay lean: only the non-obvious *why* (a workaround, a quirk being
  matched, an ordering that matters). Deeper rationale goes to chat or a doc.
- A review comment that leaves an **open matching question** is captured twice in
  the same commit: a lean `// sushi@TODO:` at the anchor, and a row in
  `docs/binary_matching/review_todos.md`. Grep `sushi@TODO:` for the live set;
  clear both only when the question is resolved.
- New project-written files start with `// SPDX-License-Identifier: GPL-3.0-or-later`
  (`#` in Python/Nix). Files reproducing original engine sources (the GSC banner),
  vendored code and shader sources get no tag. In engine files the tag replaced one
  blank line so `__LINE__` geometry never moved; a new generated unit does the same
  or pins its functions with `#line`.

## The README score block

`vostok build` refreshes the `<!-- match-score -->` block from the ledger at the end
of every build. Never hand-edit it or ledger rows; on a merge conflict take either
side and rerun `vostok build` (or `vostok ledger readme --write-readme`).
