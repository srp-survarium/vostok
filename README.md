# vostok

Binary-matching decompilation of the **Vostok Engine** used by Survarium
(v0.100b). The goal is C++ sources that, compiled with the original toolchain
(MSVC 8.0 / VS2008 SP1), produce byte-identical objects to `survarium.exe`.

This README covers the **Nix / Linux** workflow, which builds the exact VS2008
toolchain under Wine for you - no Windows install required. For the original
manual Windows/VS2008 setup, see [docs/windows-setup.md](docs/windows-setup.md).

<!-- match-score:start -->
## Match status

_Auto-generated from `docs/binary_matching/match_state.tsv` (the committed matching ledger) - refreshed by `vostok build` at the end of every build; do not hand-edit. Diff this block across commits to spot regressions._

**Overall: 10,024 / 13,017 functions exact (77.01%) &middot; 10,668 / 13,017 functions exact-max (81.95%) &middot; 91.73% fuzzy &middot; 93.79% fuzzy-max.**

_All figures come from the ledger over every target function (paired plus inlined/folded target-only). **Functions exact** and **Fuzzy** describe the current build (`cur`). **Exact-max** and **Fuzzy-max** use `max`, the peak proven for the function's own source body (`hash`), which resets when that body changes; the all-time `hist` peak is never promoted into it, and a banked peak carrying no `hash` is not credited. Byte-weighted code view: `python3 -m vostok ledger readme --max-code`._

| Module          | Units |       Functions exact |   Functions exact-max | Fuzzy | Fuzzy-max |
| :-------------- | ----: | --------------------: | --------------------: | ----: | --------: |
| `render`        |   351 | 1,879 / 2,734 (68.7%) | 2,103 / 2,734 (76.9%) | 90.4% |     92.7% |
| `game`          |   141 |   926 / 1,528 (60.6%) | 1,064 / 1,528 (69.6%) | 79.5% |     83.5% |
| `core`          |   136 | 1,146 / 1,325 (86.5%) | 1,202 / 1,325 (90.7%) | 96.2% |     97.7% |
| `vostok`        |   112 | 1,052 / 1,250 (84.2%) | 1,072 / 1,250 (85.8%) | 95.4% |     96.3% |
| `game_core`     |   189 |   798 / 1,181 (67.6%) |   849 / 1,181 (71.9%) | 92.7% |     95.4% |
| `animation`     |   102 |     570 / 730 (78.1%) |     593 / 730 (81.2%) | 90.8% |     91.9% |
| `ai`            |   124 |     618 / 691 (89.4%) |     648 / 691 (93.8%) | 97.8% |     99.4% |
| `sound`         |    69 |     474 / 510 (92.9%) |     492 / 510 (96.5%) | 99.1% |     99.8% |
| `collision`     |    52 |     461 / 503 (91.7%) |     469 / 503 (93.2%) | 98.4% |     98.7% |
| `scaleform`     |    47 |     417 / 454 (91.9%) |     423 / 454 (93.2%) | 96.7% |     96.9% |
| `particle`      |    25 |     384 / 400 (96.0%) |     392 / 400 (98.0%) | 99.4% |     99.8% |
| `vfs`           |    71 |     283 / 390 (72.6%) |     285 / 390 (73.1%) | 96.1% |     96.8% |
| `ui`            |    27 |     217 / 227 (95.6%) |     218 / 227 (96.0%) | 99.1% |     99.5% |
| `physics`       |    14 |     131 / 198 (66.2%) |     152 / 198 (76.8%) | 87.3% |     92.4% |
| `fs`            |    25 |     133 / 165 (80.6%) |     136 / 165 (82.4%) | 97.1% |     97.6% |
| `engine`        |    22 |     148 / 162 (91.4%) |     153 / 162 (94.4%) | 97.8% |     98.3% |
| `network`       |    25 |      76 / 159 (47.8%) |      89 / 159 (56.0%) | 87.2% |     92.7% |
| `network_core`  |    22 |      58 / 127 (45.7%) |      64 / 127 (50.4%) | 93.2% |     94.3% |
| `debug`         |    16 |     117 / 122 (95.9%) |     118 / 122 (96.7%) | 98.6% |     98.9% |
| `logging`       |    10 |       52 / 72 (72.2%) |       59 / 72 (81.9%) | 97.0% |     99.4% |
| `input`         |     9 |       52 / 53 (98.1%) |       52 / 53 (98.1%) | 99.9% |     99.9% |
| `survarium`     |     5 |       19 / 22 (86.4%) |       21 / 22 (95.5%) | 96.0% |     98.1% |
| `ai_navigation` |     3 |       13 / 14 (92.9%) |      14 / 14 (100.0%) | 89.9% |    100.0% |

_Updated 2026-08-27 &middot; delinker `d7e9292` (folded-symbol reconciliation)._
<!-- match-score:end -->

## Requirements

- Linux (x86_64) with [Nix](https://nixos.org/download) and flakes enabled
  (`experimental-features = nix-command flakes`).

That's it - everything else (the VS2008 SP1 compiler, Windows/DirectX SDKs,
Wine, ninja, and the Rust tools) is provided by the flake.

## Quickstart

```sh
nix develop
```

> **Disk space:** the first `nix develop` realizes **~18 GiB** into the Nix store
> - the VS2008 toolchain, Wine, the Rust toolchain, the game install (incl. the
> ~1.5 GiB packed resources plus the ~1.6 GiB unpacked resource tree), and the
> third-party libs (~6 GiB). Later entries reuse it and are near-instant.

The first entry fetches/builds everything and runs `vostok tool toolchain`,
which:

- pins the toolchain, libs, and game binaries as gcroots under `binaries/nix-store/`,
- stages the prebuilt third-party `.lib`/`.dll` blobs into `binaries.prebuilt/`
  (gitignored; mirrors the shipped game's layout - not the `sources/` tree),
- initialises the Wine prefix and registry (PATH / INCLUDE / LIB),
- generates the ninja build graph from the `.sln` via `vcproj2ninja`, and
- generates the **target-side** diff inputs once from the original game
  (`binaries/objdiff/target`, `binaries/structure/target`).

Then, after editing sources, run the build/diff loop:

```sh
python3 -m vostok build
```

This first refreshes the ninja graph from the `.vcproj`s (write-if-changed: a no-op
regen touches nothing, so there are no spurious rebuilds), builds
`survarium - PC - DirectX 11` with ninja under Wine, then regenerates the
**base-side** diff inputs in parallel:

| | base (your build) | target (original game) |
|---|---|---|
| COFF objs (objdiff) | `binaries/objdiff/base` | `binaries/objdiff/target` |
| C++ stubs (pdb-parser) | `binaries/structure/base` | `binaries/structure/target` |

Open the result in [objdiff](https://github.com/encounter/objdiff) (config at
`binaries/objdiff/objdiff.json`) and match `base` against `target`. The rebuild
also writes an overall match summary to `binaries/objdiff/report.json`, logs
the code / function match percentages, and refreshes the matching ledger
(`docs/binary_matching/match_state.tsv`, via the regenerable
committed ledger) from that fresh report at the end of the run.

Useful individual steps (all run inside `nix develop`):

```sh
python3 -m vostok.build.ninja [target]                    # build only (verbose, keep-going)
python3 -m vostok.build.generate_delink {base|target}     # COFF split for one side
python3 -m vostok.build.generate_structure {base|target}  # pdb-parser stubs for one side
```

### Where the tooling lives

The code is a Python package at `scripts/vostok/`:

| package    | what it owns                                                         |
| :--------- | :------------------------------------------------------------------- |
| `core/`    | every repo path (`paths.py`), the tracked-table shape, symbol names   |
| `ledger/`  | the committed campaign record and the README score block              |
| `derive/`  | `report.json` + the rich indexes -> the committed ledger              |
| `sema/`    | control-flow views over one base<->target function pair               |
| `build/`   | the ninja graph, the delink/structure/rich generators, `rebuild`      |
| `diff/`    | target-vs-base source shape: layouts, declaration order, enums        |
| `tool/`    | clangd, the toolchain setup/release, prebuilt libs                    |
| `shaders/` | shipped shader blobs: coverage, dump, disasm, the fxc roundtrip proof ([docs/shaders.md](docs/shaders.md)) |

`scripts/` is the package root and the dev shell exports it on `PYTHONPATH`, so
every command above runs from anywhere in the tree. `python3 -m vostok` prints
the umbrella map; each package is also a direct entry:

```sh
python3 -m vostok                                    # a map of the whole surface
python3 -m vostok.ledger report --module render
python3 -m vostok.sema blocks <fn> --diff --lite
```

New repo paths go in `scripts/vostok/core/paths.py` - it is the only module
that spells one. After editing anything under `scripts/`, run `ruff check
scripts/` and `python3 -m vostok.tests.test_match_db`.

### Source navigation (clangd)

`compile_commands.json` + `clangd-vfs.yaml` at the repo root (`vcproj2ninja
--target clangd`) give clangd a clang-cl view of every TU pinned to the MSVC
8.0 dialect (`_MSC_VER=1400`, i686, C++98), with a case-insensitive VFS overlay
because the sources spell includes in Wine's case-insensitive world. They are
generated by `vostok tool toolchain` on first `nix develop` entry, and the graph
regen renews them when they are missing or a TU/module is added or removed
(they are include-invariant, so ordinary rebuilds skip them); force with
`python3 -m vostok.build.ninja_regen --compdb` after a flags-only .vcproj edit.
clangd (in the devShell) navigates the whole engine through it - **clang is a
reader here**; its diagnostics are suppressed via `.clangd` and the Wine build
remains the only verdict on correctness.

Point any LSP editor at the repo root (the compdb is found automatically), or
query from the shell / an agent:

```sh
python3 -m vostok tool clangd index                       # warm the background index once
python3 -m vostok tool clangd symbol weapon_lexeme_pair   # fuzzy workspace symbols
python3 -m vostok tool clangd def   <file> <line> [col]   # go to definition
python3 -m vostok tool clangd refs  <file> <line> [col]   # all references
python3 -m vostok tool clangd hover <file> <line> [col]   # type/expansion at point
```

A new `#include` changes neither generated file (include tracking lives in the
ninja graph), so the background index never reindexes more than the TUs whose
content actually changed.

## Game data

The whole game comes from one installer extraction, split into three outputs of
the `survarium` derivation. `nix develop` realizes and pins **all three** under
`binaries/nix-store/` by default - no extra step:

- `survarium-game` - `survarium.exe`, `survarium.pdb`, DLLs (also `SURVARIUM_BIN`)
- `survarium-resources` - `resources.db` + `resources/` (packed game data, ~1.5 GiB)
- `survarium-keys` - lobby/login server SSL certs + private keys

Plus a fourth, derived output pinned the same way:

- `survarium-resources-unpacked` - `resources.db` expanded into its file tree
  (~1.6 GiB, 12557 files) by [`vostok-resources-db`](https://github.com/srp-survarium/vostok-resources-db),
  for inspecting/diffing game assets directly.

To build any one standalone (e.g. outside the shell): `nix build .#survarium-resources`,
`.#survarium-resources-unpacked`, `.#survarium-keys`, `.#survarium-game`.

## Matching campaigns

Repository skills under [`.agents/skills/`](.agents/skills/) cover unit matching,
structure verification, stack review, and module-scale orchestration. Ask the agent
to use `$vostok-orchestrate-matching` for a whole module or `$vostok-match-unit`
for one translation unit.

A campaign prepares clean, warm sibling worktrees from the current integration
tip and dispatches disjoint translation units in parallel. Finished work is
integrated one commit at a time into a single advancing stack. Every commit is
fully rebuilt and carries the corresponding `README.md` and
`docs/binary_matching/match_state.tsv`; parallel
branches are never merged into a fan and the approved stack is never squashed.
After roughly 10-15 units, the batch receives a structure audit before review and
landing.

Warm worktrees should already contain `binaries/rich/target` and
`binaries/objdiff`. Full campaign rules live in
[`docs/binary_matching/agentic_loop.md`](docs/binary_matching/agentic_loop.md)
and [the orchestration skill](.agents/skills/vostok-orchestrate-matching/SKILL.md).

## Reviewing match % (no rebuild needed)

`docs/binary_matching/match_state.tsv` is **committed**, so every query below
answers on a fresh clone that has never compiled anything - no database, no
build. For *current* numbers after edits, run `python3 -m vostok build` first
(it re-derives the ledger at the end of the build);
`python3 -m vostok derive refresh` only re-derives from an already-built
`report.json` (it does NOT rebuild - run `vostok build` if sources moved).
A function is **DONE only when the compile says so** (`cls`/`max` below); the
only hand-set status is a **PARK** (with a cause) - so a low % is "still open",
never silently "done".

```sh
# headline + per-module table (overall fuzzy %, functions-exact) - READ-ONLY print;
# the README score block itself is refreshed by vostok build at the end of every build.
python3 -m vostok ledger readme

# per-UNIT rollup for a module, worst first. Columns: fns, done/open/park/blkd
# counts, `held` (cur < max: compiler noise, NOT a regression - do not chase it),
# `head` (hist > max: we had it better once, and that IS worth working), then the
# byte-weighted cur% and max%.
python3 -m vostok ledger report --module game_core --per-unit

# ONE unit by path substring (works on report and list alike)
python3 -m vostok ledger report --unit medkit

# every function in a module/unit: size, cur%, max%, cls, status, mangled name.
# --json gives full untruncated names; pipe wide output to `less -S`.
python3 -m vostok ledger list --module game_core
python3 -m vostok ledger list --unit vostok/game_core/sources/weapon_core.cpp
python3 -m vostok ledger list --module game_core --status blocked   # incl. target-only

# TRAP FINDER: high % over the WRONG statement shape. QUANTITY/SPLIT are the
# steerable structural classes; a 100%/QUANTITY is the classic false win. The `cls`
# column is an APPROXIMATION - confirm each hit with the structure-diff below.
python3 -m vostok ledger list --module game_core --class QUANTITY,SPLIT

# every PARKED function + why (the blocker, in the committed record - not buried in a PR body)
python3 -m vostok ledger list --module game_core --status parked

# we had it better once: hist > max, i.e. an earlier implementation scored higher
# and we lost it. The highest-value queue in the tree.
python3 -m vostok ledger list --module game_core --headroom

# one batch per TU, worst first - what an orchestrator hands a matcher
python3 -m vostok ledger queue --module game_core [--limit N] [--json]

# REGRESSION TRACKING is `git diff`: the ledger is text, one row per function, so a
# commit range shows exactly which functions moved and how. This is why the record is
# a .tsv and not a database - a SQLite file re-serialises its pages on every write, so
# it could neither be diffed nor merged.
git diff <hash> -- docs/binary_matching/match_state.tsv          # commit vs working tree
git diff <hash>..<hash> -- docs/binary_matching/match_state.tsv  # two commits
# Reading a row's numbers: `cur` is THIS build and is noisy (under LTCG/ICF a function
# moves without its source moving). `max` is the peak proven for this exact source body
# and resets only when that body changes - driving every max to 100 is the campaign.
# `hist` is the all-time peak and never resets.

# AUTHORITATIVE per-function structure diff (the `cls` column above is only an approximation).
# Prints each diverging statement tagged SIZE +/-N / BASE_ONLY / TRGT_ONLY, the target-vs-base
# statement counts, and `; STRUCTURE MATCH` when the shape is clean. --function is a substring of
# the signature; --view also takes target,base,structure,diff. This is how you confirm a
# QUANTITY is real (recoverable statements) vs a stale label (0/0 STRUCTURE MATCH) vs blocked.
pdb_fetch --target-index binaries/rich/target/index.jsonl --base-index binaries/rich/base/index.jsonl \
  --function 'weapon_core::tick' --view structure-diff

# Raw CodeView topology around one target function: exact procedure/frame/scope
# records, PDB line lengths, TPI neighbors, the owning class field-list entry,
# and physical/module-level symbol neighbors. Adjacency sections are explicitly
# labelled heuristic; explicit references and class bindings are authoritative.
pdb_topology --pdb "$SURVARIUM_BIN/survarium.pdb" \
  --module render_engine_world_pc_dx11 --function 'world::draw_scene'

# Compare the same raw evidence while ignoring PDB-local RVA/type-index churn.
pdb_topology --target-pdb "$SURVARIUM_BIN/survarium.pdb" \
  --base-pdb binaries/Win32/survarium-dx11-win32-gold.pdb \
  --module render_engine_world_pc_dx11 --function 'world::draw_scene'

# Compare every complete target class/struct/interface definition to base.
# Text prints differing/missing classes; --json preserves the complete model.
pdb_topology --target-pdb "$SURVARIUM_BIN/survarium.pdb" \
  --base-pdb binaries/Win32/survarium-dx11-win32-gold.pdb --classes

# Narrow a class audit without changing whole-scan semantics.
pdb_topology --target-pdb "$SURVARIUM_BIN/survarium.pdb" \
  --base-pdb binaries/Win32/survarium-dx11-win32-gold.pdb --classes \
  --class 'vostok::render::engine::world'
```

See [`pdb_topology.md`](docs/binary_matching/pdb_topology.md) for how to interpret
the record-order evidence without confusing linker/type deduplication with source
order.

**`cls` (structure class)** is the DB's *approximate* shape verdict (the
authoritative one is the structure-verifier's `pdb_fetch --view structure-diff`;
full defs in [`match_db_design.md`](docs/binary_matching/match_db_design.md)):

| `cls` | meaning |
|---|---|
| `MATCH` | same statement **count** *and* per-statement byte **sizes** - clean structure; only sub-statement noise left |
| `SIZE` | same count, >=1 statement differs in **bytes** - skeleton matches; residual = inline-vs-call / LTCG / reg-alloc |
| `SPLIT` | equal counts *and* total bytes, but alignment leaves paired target-only/base-only rows (line-attribution split) |
| `QUANTITY` | statement **counts differ** - real missing/extra source statements (the high-%-over-WRONG-structure trap) |
| `-` (NULL) | unpaired/open - no base match yet |

A high `fuzzy_pct` with `cls = QUANTITY`/`SPLIT` is the trap the structure-verifier
exists to catch: the bytes line up over the **wrong** statement shape.

## Docs

- [Matching guide](https://gist.github.com/sushi-shi/8bf16f82c3b1c65fd357d73ecfda909e) - how to actually match assembly.
- [docs/index.md](docs/index.md) - index of build and per-module matching notes.
- [docs/build/toolchain-build.md](docs/build/toolchain-build.md) - how the VS2008 toolchain is built under Wine (and why Wine must be staging).
- [docs/windows-setup.md](docs/windows-setup.md) - the legacy manual Windows/VS2008 workflow.
