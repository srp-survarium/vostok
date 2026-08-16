# vostok - Claude working notes (Read AGENTS.md too).

This repo binary-matches the Vostok Engine (Survarium v0.100b): we write C++
that, compiled with the original toolchain (MSVC 8.0 / VS2008), produces
byte-identical object files to the shipped `survarium.exe` + `survarium.pdb`.
Matching is verified with `objdiff`, comparing our compiled "base" objects
against the original game's "target" objects.

These are the repo-local working agreements, auto-loaded when working in `vostok/`.

## Comments

Comments are still wanted - just keep them lean. Don't restate what the code
plainly shows; reserve a comment for non-obvious *why* (a workaround, a quirk
being matched, an ordering that matters). When deeper rationale or context would
bloat the code, surface it in chat or record it in `CLAUDE-WORK.md` instead of a
long inline comment.

## Review TODOs (`sushi@TODO:`)

When a PR review leaves an inline comment that is an **open matching question**
(something unresolved, suspect, or to investigate later), capture it in BOTH places
in the same commit that touches that PR:

1. **In source**, at the spot the comment is anchored, as a `// sushi@TODO:` line
   (paraphrase the reviewer's point; keep it lean).
2. **In `docs/binary_matching/review_todos.md`**, as a new table row (Status /
   Function / File:line / PR / TODO) so a later sweep agent can find it even after
   squash-merges erase the branch.

Grep `sushi@TODO:` for the live set; `review_todos.md` is the curated index. Tick a
row to `done` (and drop the marker) only when the matching question is actually
resolved.

## Tools and assets come from Nix, not sibling repos

Inside `nix develop` the flake provides everything:

- binaries on PATH: `vostok-delinker`, `pdb_parser`, `pdb_fetch`,
  `pdb_divergence` (the base-vs-target structure-divergence verifier),
  `objdiff`, `objdiff-cli`, `wine`, plus `vcproj2ninja.exe` / `ninja.exe`
  run under Wine.
- assets via env vars: `VOSTOK_LIBS_DIR`, `SURVARIUM_BIN`, `MSVC_DIR`, ...
- large fetched packages pinned as gcroots under `binaries/nix-store/`.

The default shell is **lean**: it realizes the toolchain, libs, and the small
`SURVARIUM_BIN` (the exe/pdb matching needs) but **not** the heavy game
resources (~1.5 GiB packed + ~1.6 GiB unpacked). Those are opt-in via
`nix develop .#with-resources`, which adds `VOSTOK_RESOURCES_DIR` /
`VOSTOK_RESOURCES_UNPACKED` and pins them as gcroots. Binary matching never
touches the resources, so the default shell is all you need; reach for
`.#with-resources` only when inspecting/diffing game assets.

## Base PDB paths are Wine paths

The base build runs MSVC under Wine, so `survarium-dx11-win32-gold.pdb` records
source paths on the Z: drive (Wine maps `/` to `Z:`), lowercased and
backslash-separated, e.g. `z:\home\you\proj\vostok\sources\vostok\...`.
pdb-parser and the delinker strip an engine-path prefix off these paths; pass
the Wine-rendered prefix (see `_wine_path()` in
`scripts/vostok/build/generate_structure.py` and `.../generate_delink.py`), not
the native `/home/...` path. The original game PDB instead uses
`c:/survarium/sources`.

## Where the tooling lives (`scripts/vostok/`)

All the Python tooling is one package - there are no loose `scripts/*.py` any
more (what else lives in `scripts/` is non-Python: `ida/`, `vs/`, `samples/`,
the `*_tus.txt` lists, `create-toolchain-release.nix`). Every command in these
docs runs it as `python3 -m vostok ...`, and the code is here:

    core/     paths.py (EVERY repo path, once), tsv.py (the tracked-table
              shape), symbols.py (objdiff symbol normalization)
    ledger/   store.py (the committed record + the cur/max/hist policy),
              cli.py (the verbs), readme.py (the README block), queue.py
    derive/   report.json + the rich indexes -> binaries/match.db: aliases.py,
              modules.py, classify.py, maxima.py, roster.py, and the verbs
    sema/     one module per view: rva, xref, strings, blocks, branches, dot,
              sweep (plus index/disasm/cfg underneath)
    build/    ninja_regen, ninja, generate_{delink,structure,rich}, rebuild
    diff/     layout, order, tu_order, enums
    tool/     clangd, toolchain, libs, sizes, breakpoints
    tests/    the tooling's own unit tests

**Add a new repo path to `core/paths.py`, never to the module that uses it.**
Nine scripts used to re-derive the repo root and eight hard-coded artifact
locations; moving `match.db` then meant editing four copies, one was missed,
and the miss silently produced an empty database that corrupted the README
score block.

`scripts/` is the package root and the dev shell puts it on `PYTHONPATH`, so
`python3 -m vostok` (a map of the whole surface), `python3 -m vostok.ledger
report --module render`, `python3 -m vostok.sema blocks <fn> --diff` and so on
work from anywhere in the tree. After editing anything under `scripts/`, run
`ruff check scripts/` and `python3 -m vostok.tests.test_match_db`.

## Build / diff loop

    nix develop                  # first entry builds the toolchain, sets up Wine
                                 # + ninja, and generates the target side once
    python3 -m vostok build      # ninja build under Wine, then regenerate
                                 # binaries/structure/base + binaries/objdiff/base,
                                 # then refresh the matching ledger

A full engine build under Wine takes ~10 minutes; run it in the background (it is no
longer the loop bottleneck - agent token cost is). The
objdiff config is `binaries/objdiff/objdiff.json`; a match report is
`objdiff-cli report generate -p binaries/objdiff`. Every base delink regenerates
`binaries/objdiff/report.json` (and `report-changes.json`); `vostok build` then
refreshes the ledger from that report at the end of its run. `vostok derive
refresh` is the regen-only path (re-derive from an already-built report;
run `vostok build` first if sources moved) - it does NOT rebuild.

## The matching ledger (`vostok ledger`)

`docs/binary_matching/match_state.tsv` is the committed record of the campaign:
one text row per target function. Query and update it with `vostok ledger` -
it reads the file directly, so it needs no database and no build.

    python3 -m vostok ledger report --module render      # byte-weighted rollup
    python3 -m vostok ledger report --per-unit --module render
    python3 -m vostok ledger queue --module render       # one batch per TU, worst first
    python3 -m vostok ledger list --module render --class QUANTITY,SPLIT
    python3 -m vostok ledger list --headroom             # hist > max: we had it better once
    python3 -m vostok ledger tried <mangled> --note "what was attempted"
    python3 -m vostok ledger park <mangled> --cause "why it stops here"
    python3 -m vostok ledger open <mangled>              # undo a park

Each row carries three percentages, and the difference between them is the whole
point:

- **`cur`** - this build. Noisy: under LTCG/ICF a function moves without its
  source moving.
- **`max`** - the peak proven for *this exact source body* (`hash`). It resets
  only when that body changes, because new source must prove itself.
  **Driving every `max` to 100 is the campaign goal**; a final pass then lands
  them together.
- **`hist`** - the all-time peak. Never resets.

### `(held)` means do not chase it

When a build reports a function lower than its `max`, `vostok ledger` prints
**`(held <max>)`** next to it. That is not a regression: the peak is still
proven for this exact source, and the dip is compiler noise - a fold
representative changed, or an inline decision flipped in a sibling TU. **Do not
try to "fix" a held row.** `report` counts them in the `held` column.

A real regression is a row whose `max` fell, which only happens when the source
body changed. `hist > max` (the `head` column, `list --headroom`) means an
earlier implementation scored better and we lost it - that IS worth working.

`status` is `done` (max >= 100) / `inprogress` / `blocked` (something missing,
including target-only) / `parked` (worked, could not raise it - `note` says why).
Keep `note` short; it is what stops the next matcher re-deriving a dead end.

## Match score (README regression tracker)

`vostok ledger readme` rolls `binaries/match.db` up into the overall exact/fuzzy
figures plus a per-module table, in the `<!-- match-score:start -->` block at the
top of README.md. **`vostok build` refreshes that block at the end of every build**
(right after the `match.db` regen it reads), so it stays current on its own - you
do not run `vostok ledger readme` by hand. The numbers are the DB's own roster
over every target function (the source carries no status markers; per-function
status lives in the ledger - see above), so the README is an honest, no-run
regression tracker - diff the block across commits. If the block ever conflicts on
a merge/cherry-pick, don't hand-resolve it: take either side and rerun `vostok build`
(or, if the artifacts are already built, `vostok ledger readme --write-readme`) to
reconcile it deterministically - same as the ledger.

Both the ledger and the README block are **text**, so a conflict is a normal
text conflict and `git diff` shows exactly which functions moved. That is why
the old committed `match.db` is gone: SQLite could not be diffed or merged, and
re-serialised its pages on every write, so each commit stored a fresh ~4 MB blob
(~3.5 GB of history). `binaries/match.db` remains only as a regenerable cache.

### Header edits trigger rebuilds (vcproj2ninja tracks `#include`s)

The generated ninja graph lists every header a `.cpp` transitively `#include`s as
an implicit input, so **editing a header recompiles the dependent TUs
automatically** (the precompiled-header chain included).

vcproj2ninja resolves includes through the project's `/I` dirs (it evaluates
`#if`/`#ifdef`, erring toward over-inclusion so a real dependency is never missed).
Only project headers are tracked; system/CRT headers from `%INCLUDE%` are
intentionally not (they don't change).

`vostok build` also regenerates the graph itself on every run (write-if-changed,
no mtime churn on no-ops), so NEW `#include`s, un-excluded TUs, and .vcproj
edits are picked up without any manual `vostok.build.ninja_regen` step.

## Keep the README current

README.md documents this Nix/Linux workflow and goes stale easily. Whenever you
change something fundamental - the build/diff flow, script names or locations,
devShell tools, generated artifacts, or paths - check README.md and update it in
the same change.
