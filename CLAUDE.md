# vostok - working agreements (read AGENTS.md too)

This repo binary-matches the Vostok Engine (Survarium v0.100b): C++ that,
compiled with the original MSVC 8.0 / VS2008 toolchain, produces byte-identical
objects to the shipped `survarium.exe` + `survarium.pdb`. `objdiff` compares our
"base" objects against the game's "target" objects. Everything (toolchain, Wine,
tools, assets) comes from the flake inside `nix develop`; never from sibling repos.

## The loop

    python3 -m vostok build          # ninja under Wine (~10 min, run it in the background),
                                     # then regenerate the base diff inputs and refresh the ledger
    python3 -m vostok derive refresh # re-derive the ledger from an existing report only - no rebuild

The objdiff config is `binaries/objdiff/objdiff.json`; each build writes
`binaries/objdiff/report.json` and `report-changes.json`. Header edits
recompile their dependents automatically.

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

    pdb_fetch --target-index binaries/rich/target/index.jsonl \
              --base-index binaries/rich/base/index.jsonl \
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

How to act: run the diff before writing a body (read the target's statements) and
after each build. `TRGT_ONLY` / `BASE_ONLY` rows are structure work - fix the shape
until they are gone. A `SIZE` residual that survives a correct shape is usually an
inline-vs-call or LTCG call-boundary difference: confirm it with
`python3 -m vostok sema blocks <fn> --diff --lite`, then park with the wall named
rather than bending the source to the bytes. The ledger's `cls` column
(`QUANTITY`, `SPLIT`, `SIZE`, `MATCH`) is only an approximation of this verdict;
`QUANTITY` and `SPLIT` rows are where to look first. Definitions and the deeper
PDB tools: `docs/binary_matching/ledger_design.md`, `docs/binary_matching/pdb_topology.md`.

## The tooling (`scripts/vostok/`)

One package, run as `python3 -m vostok ...` (`python3 -m vostok` maps the surface):
`core/` (paths, tsv, symbols, log, wine, fmt - blind helpers), `ledger/`, `derive/`
(report + rich indexes -> ledger), `sema/` (control-flow views), `build/`, `diff/`,
`tool/`, `shaders/`, `data/`, `tests/`. Every repo path is spelled once, in
`core/paths.py` - add new ones there. After editing anything under `scripts/`, run
`ruff check scripts/` and `python3 -m vostok.tests.test_match_db`. If you change
the build/diff flow, tool names or generated artifacts, update README.md too.

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
