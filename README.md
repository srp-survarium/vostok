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

**Overall: 54.07% fuzzy (+2.32%) &middot; 4,696 / 12,915 functions exact (36.36%, +329 best-ever).**

_All figures from `match.db`, over ALL target functions (paired + inlined/folded `target_only`). `Functions exact` / `Fuzzy` = current; the `max` columns are best-ever per function (`history.best_fuzzy_pct`, ICF-churn-immune): `Functions exact max` shows `(+N)` functions that were exact before they folded away, `Fuzzy max` shows `+Δ%` regained. Byte-weighted code view: `scripts/match_score.py --max-code`._

| Module          | Units |     Functions exact | Functions exact max | Fuzzy | Fuzzy max |
| :-------------- | ----: | ------------------: | ------------------: | ----: | --------: |
| `render`        |   351 | 360 / 2,647 (13.6%) |           382 (+22) | 30.4% |     +2.1% |
| `game`          |   141 | 302 / 1,498 (20.2%) |            306 (+4) | 40.0% |     +0.4% |
| `core`          |   136 | 645 / 1,320 (48.9%) |           673 (+28) | 76.8% |     +2.7% |
| `vostok`        |   112 | 493 / 1,252 (39.4%) |           558 (+65) | 53.6% |     +5.5% |
| `game_core`     |   189 | 483 / 1,171 (41.2%) |          587 (+104) | 76.2% |     +8.3% |
| `animation`     |   102 |   185 / 763 (24.2%) |           196 (+11) | 20.0% |     +0.8% |
| `ai`            |   124 |   405 / 690 (58.7%) |           444 (+39) | 86.2% |     +1.1% |
| `sound`         |    69 |   205 / 509 (40.3%) |            208 (+3) | 50.1% |     +0.1% |
| `collision`     |    52 |   409 / 503 (81.3%) |            412 (+3) | 95.4% |     +0.2% |
| `scaleform`     |    47 |   191 / 453 (42.2%) |           201 (+10) | 53.1% |     +1.2% |
| `particle`      |    25 |   213 / 400 (53.2%) |            217 (+4) | 78.1% |     +1.0% |
| `vfs`           |    71 |   162 / 390 (41.5%) |            168 (+6) | 84.8% |     +1.7% |
| `ui`            |    27 |   145 / 227 (63.9%) |            145 (+0) | 77.6% |     +3.6% |
| `physics`       |    14 |    83 / 197 (42.1%) |             90 (+7) | 67.6% |     +2.9% |
| `fs`            |    25 |    65 / 165 (39.4%) |             68 (+3) | 79.3% |     +3.3% |
| `engine`        |    22 |    53 / 162 (32.7%) |             53 (+0) | 82.7% |     +0.4% |
| `network`       |    25 |    67 / 158 (42.4%) |             74 (+7) | 84.9% |     +0.9% |
| `network_core`  |    22 |    35 / 127 (27.6%) |            46 (+11) | 83.2% |     +4.1% |
| `debug`         |    16 |   111 / 122 (91.0%) |            111 (+0) | 96.8% |     +0.0% |
| `logging`       |    10 |     34 / 72 (47.2%) |             34 (+0) | 90.4% |     +0.5% |
| `input`         |     9 |     26 / 53 (49.1%) |             28 (+2) | 67.1% |     +1.5% |
| `survarium`     |     5 |     13 / 22 (59.1%) |             13 (+0) | 83.8% |     +0.0% |
| `ai_navigation` |     3 |     11 / 14 (78.6%) |             11 (+0) | 93.8% |     +0.0% |

_Updated 2026-06-24 &middot; delinker `5118e2a` (folded-symbol reconciliation)._
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

## Matching (the orchestrator)

Binary-matching is driven by an **orchestrator** that dispatches `matcher` +
`structure-verifier` (+ a tiny `reviewer`) workers and stacks their PRs onto
`feature/agentic-matching-loop-2` for review. **Start it from a fresh top-level Claude Code
session** - it spawns worker subagents, so it cannot run as a nested subagent - with the
`/match` slash command:

```sh
/match network_core        # whole queue, default 3 workers in parallel
/match game_core 2         # whole queue, but only 2 workers in parallel
```

Per unit it then: prepares a sibling worktree `vostok_<N>` off the stack tip, dispatches a
`matcher` (fanning out up to the worker cap, default 3), integrates each finished matcher's
commit into a single linear stack, and **builds before opening that unit's PR** so each PR
carries a current `match.db`. Matchers spawn off the **top** of the stack so percentages
compound; their PRs form **one linear chain** (each based on the unit below it, never fanned
into a shared base). After every 10-15 matchers it runs one `structure-verifier` over the
batch. You review the stack **bottom-up**; when approved it lands into the integration branch
by a single fast-forward (all commits preserved).
Prereqs: worktrees `vostok_1..3` clean + warm (`binaries/rich/target` + `binaries/objdiff`
present). Full rules: [`.claude/agents/orchestrator.md`](.claude/agents/orchestrator.md).

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
```

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
