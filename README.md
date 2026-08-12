# vostok

Binary-matching decompilation of the **Vostok Engine** used by Survarium
(v0.100b). The goal is C++ sources that, compiled with the original toolchain
(MSVC 8.0 / VS2008 SP1), produce byte-identical objects to `survarium.exe`.

This README covers the **Nix / Linux** workflow, which builds the exact VS2008
toolchain under Wine for you - no Windows install required. For the original
manual Windows/VS2008 setup, see [docs/windows-setup.md](docs/windows-setup.md).

<!-- match-score:start -->
## Match status

_Auto-generated from `docs/binary_matching/match.db` - refreshed by `rebuild.py` at the end of every build; do not hand-edit. Diff this block across commits to spot regressions._

**Overall: 8,194 / 12,927 functions exact (63.39%) &middot; 8,202 / 12,927 functions exact-max (63.45%) &middot; 84.03% fuzzy &middot; 84.05% fuzzy-max.**

_All figures come from `match.db` over every target function (paired plus inlined/folded `target_only`). **Functions exact** and **Fuzzy** describe the current build. **Exact-max** and **Fuzzy-max** retain only observations from the same effective-source/compiler-context hash in `source_maxima`; ordinary `history.best_fuzzy_pct` observations are not promoted to MAX. Exact-max requires a byte-exact observation in the current source epoch. Byte-weighted code view: `scripts/match_score.py --max-code`._

| Module          | Units |       Functions exact |   Functions exact-max |  Fuzzy | Fuzzy-max |
| :-------------- | ----: | --------------------: | --------------------: | -----: | --------: |
| `render`        |   351 | 1,293 / 2,651 (48.8%) | 1,293 / 2,651 (48.8%) |  79.9% |     79.9% |
| `game`          |   141 |   635 / 1,498 (42.4%) |   636 / 1,498 (42.5%) |  65.2% |     65.2% |
| `core`          |   136 |   951 / 1,325 (71.8%) |   951 / 1,325 (71.8%) |  87.4% |     87.4% |
| `vostok`        |   112 |   775 / 1,252 (61.9%) |   781 / 1,252 (62.4%) |  81.9% |     82.2% |
| `game_core`     |   189 |   701 / 1,171 (59.9%) |   702 / 1,171 (59.9%) |  91.2% |     91.3% |
| `animation`     |   102 |     540 / 763 (70.8%) |     540 / 763 (70.8%) |  83.3% |     83.3% |
| `ai`            |   124 |     504 / 691 (72.9%) |     504 / 691 (72.9%) |  92.8% |     92.8% |
| `sound`         |    69 |     490 / 510 (96.1%) |     490 / 510 (96.1%) |  99.9% |     99.9% |
| `collision`     |    52 |     406 / 503 (80.7%) |     406 / 503 (80.7%) |  91.8% |     91.8% |
| `scaleform`     |    47 |     420 / 453 (92.7%) |     420 / 453 (92.7%) |  96.5% |     96.5% |
| `particle`      |    25 |     354 / 400 (88.5%) |     354 / 400 (88.5%) |  96.5% |     96.5% |
| `vfs`           |    71 |     191 / 390 (49.0%) |     191 / 390 (49.0%) |  88.9% |     88.9% |
| `ui`            |    27 |     210 / 227 (92.5%) |     210 / 227 (92.5%) |  97.3% |     97.3% |
| `physics`       |    14 |     134 / 197 (68.0%) |     134 / 197 (68.0%) |  88.5% |     88.5% |
| `fs`            |    25 |     101 / 165 (61.2%) |     101 / 165 (61.2%) |  87.2% |     87.2% |
| `engine`        |    22 |     137 / 162 (84.6%) |     137 / 162 (84.6%) |  92.3% |     92.3% |
| `network`       |    25 |      72 / 159 (45.3%) |      72 / 159 (45.3%) |  90.3% |     90.3% |
| `network_core`  |    22 |      47 / 127 (37.0%) |      47 / 127 (37.0%) |  88.5% |     88.5% |
| `debug`         |    16 |     106 / 122 (86.9%) |     106 / 122 (86.9%) |  98.6% |     98.6% |
| `logging`       |    10 |       43 / 72 (59.7%) |       43 / 72 (59.7%) |  94.6% |     94.6% |
| `input`         |     9 |       50 / 53 (94.3%) |       50 / 53 (94.3%) |  99.6% |     99.6% |
| `survarium`     |     5 |       20 / 22 (90.9%) |       20 / 22 (90.9%) |  97.6% |     97.6% |
| `ai_navigation` |     3 |      14 / 14 (100.0%) |      14 / 14 (100.0%) | 100.0% |    100.0% |

_Updated 2026-08-12 &middot; delinker `83bc6fc` (folded-symbol reconciliation)._
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

The first entry fetches/builds everything and runs `scripts/setup-toolchain.py`,
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
python3 scripts/rebuild.py
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
the code / function match percentages, and regenerates the match DB
(`docs/binary_matching/match.db`) from that fresh report at the end of the run.

Useful individual scripts (all run inside `nix develop`):

```sh
python3 scripts/ninja_build.py [target]              # build only (verbose, keep-going)
python3 scripts/generate_delink.py {base|target}     # COFF split for one side
python3 scripts/generate_structure.py {base|target}  # pdb-parser stubs for one side
```

### Source navigation (clangd)

`compile_commands.json` + `clangd-vfs.yaml` at the repo root (`vcproj2ninja
--target clangd`) give clangd a clang-cl view of every TU pinned to the MSVC
8.0 dialect (`_MSC_VER=1400`, i686, C++98), with a case-insensitive VFS overlay
because the sources spell includes in Wine's case-insensitive world. They are
generated by `setup-toolchain.py` on first `nix develop` entry, and the graph
regen renews them when they are missing or a TU/module is added or removed
(they are include-invariant, so ordinary rebuilds skip them); force with
`python3 scripts/regen_ninja.py --compdb` after a flags-only .vcproj edit.
clangd (in the devShell) navigates the whole engine through it - **clang is a
reader here**; its diagnostics are suppressed via `.clangd` and the Wine build
remains the only verdict on correctness.

Point any LSP editor at the repo root (the compdb is found automatically), or
query from the shell / an agent:

```sh
python3 scripts/clangd_query.py index                       # warm the background index once
python3 scripts/clangd_query.py symbol weapon_lexeme_pair   # fuzzy workspace symbols
python3 scripts/clangd_query.py def   <file> <line> [col]   # go to definition
python3 scripts/clangd_query.py refs  <file> <line> [col]   # all references
python3 scripts/clangd_query.py hover <file> <line> [col]   # type/expansion at point
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
fully rebuilt and carries the corresponding `README.md` and `match.db`; parallel
branches are never merged into a fan and the approved stack is never squashed.
After roughly 10-15 units, the batch receives a structure audit before review and
landing.

Warm worktrees should already contain `binaries/rich/target` and
`binaries/objdiff`. Full campaign rules live in
[`docs/binary_matching/agentic_loop.md`](docs/binary_matching/agentic_loop.md)
and [the orchestration skill](.agents/skills/vostok-orchestrate-matching/SKILL.md).

## Reviewing match % (no rebuild needed)

These read the **last build** - the committed `binaries/objdiff/report.json` and
`docs/binary_matching/match.db`. For *current* numbers after edits, run
`python3 scripts/rebuild.py` first (it regenerates `match.db` at the end of the
build); `python3 scripts/match_db.py refresh` only re-derives the DB from an
already-built `report.json` (it does NOT rebuild - run `rebuild.py` if sources moved).
A function is **DONE only when the compile says so** (`struct_class`/`fuzzy_pct`
below); the only hand-set status is a **PARK** (an `out_of_scope` flag with a
cause) - so a low % is "still open", never silently "done".

```sh
# headline + per-module table (overall fuzzy %, functions-exact) - READ-ONLY print;
# the README score block itself is refreshed by rebuild.py at the end of every build.
python3 scripts/match_score.py

# per-UNIT rollup for a module, sorted 100%->0% (a header inline shows its .h file,
# not a '(no unit)' lump): weighted_pct (size-weighted) + avg_pct (plain per-function
# mean), struct_match, out_of_scope (parked). --lite drops custom_conv/out_of_scope/suspicious.
python3 scripts/match_db.py report --module game_core --per-unit --lite

# ONE unit by name or substring (--module optional; lean view; refuses with
# paste-ready full names if the substring is ambiguous, e.g. medkit.cpp vs medkit.h)
python3 scripts/match_db.py report --unit medkit

# per-FUNCTION list for ONE unit, with a weighted%/avg% header. Columns: pct,
# best (best-ever % - best=100 with pct<100 is a TRANSIENT/regressed match), tries
# (matcher dispatches), cls, size, flag. Sorted 100%->0%; NULL pct = unpaired/open.
python3 scripts/match_db.py report --unit medkit --per-function

# any function by NAME substring, across files (e.g. all 'medkit::' members); same
# columns + a file column (the .cpp TU, or the header for an inline fn) and a
# count/weighted/avg header. --module optional.
python3 scripts/match_db.py report --function 'medkit::'

# (the fn column is the mangled-derived scope::name - no return type, template args, or
#  parameter list, so even a boost/asio handler reads cleanly. --verbose shows the
#  demangled signature instead; --json gives the full untruncated name. Works on diff too.)

# list EVERY function in a module/unit + its raw diff columns (fuzzy_pct, struct_class,
# t_stmts/b_stmts, sizes, target VA hint). The most direct per-function dump; filter by
# --class to pull just the steerable QUANTITY traps, or --presence for the unpaired set.
# Needs a FRESH DB - `list` bails on a stale report.json (run rebuild.py/refresh first),
# unlike report/sql which warn-and-proceed. --json gives full names; pipe wide output to `less -S`.
python3 scripts/match_db.py list --module game_core                  # all game_core functions
python3 scripts/match_db.py list --module game_core --class QUANTITY  # just the wrong-stmt-count traps
python3 scripts/match_db.py list --unit vostok/game_core/sources/weapon_core.cpp
python3 scripts/match_db.py list --module game_core --presence TARGET_ONLY  # unpaired (no base symbol yet)

# TRAP FINDER: high % but NON-MATCH structure (QUANTITY/SPLIT = bytes lined up over the
# WRONG statement shape). An APPROXIMATE screen - confirm each hit with the structure-
# verifier (`pdb_fetch --view structure-diff`); a 100%/QUANTITY is the classic false win.
python3 scripts/match_db.py sql "SELECT printf('%.1f',p.fuzzy_pct) pct, p.struct_class cls, substr(u.name,25) unit, s.demangled \
  FROM pairs p JOIN symbols s ON s.id=p.sym JOIN target_functions t ON t.sym=p.sym JOIN units u ON u.id=t.unit \
  WHERE t.module='game_core' AND p.fuzzy_pct>=80 AND p.struct_class IN ('QUANTITY','SPLIT') ORDER BY p.fuzzy_pct DESC"

# every PARKED function + why (the blocker, queryable - not buried in a PR body)
python3 scripts/match_db.py sql "SELECT substr(u.name,25) unit, s.demangled, f.cause \
  FROM flags f JOIN symbols s ON s.mangled=f.mangled JOIN target_functions tf ON tf.sym=s.id \
  JOIN units u ON u.id=tf.unit WHERE f.flag='OUT_OF_SCOPE' AND u.module='game_core'"

# function-level DIFF of the committed match.db across revisions (a regression tracker):
#   <hash>          compares that commit vs the working tree
#   <hash>..<hash>  compares two commits
# groups every function: regress / lost / new / improve / TOUCHED (retries up, % unchanged -
# i.e. a worked TU's 100% fns) / reclass. Columns include max (best-ever %) + tries (from->to).
# max printed as `X->Y` means the fn's SOURCE changed and best reset to the new % (a real
# re-work that lost ground) - vs a held max next to a dropped current %, which is just LTO.
python3 scripts/match_db.py diff <hash>..<hash> --module game_core   # --json for machine-readable

# bank the done set: stamp every fn that ever reached 100% with tries=1, so the queue
# (which ranks/excludes by best-ever %, not current %) drops them until real work is gone.
python3 scripts/match_db.py tried --done

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
