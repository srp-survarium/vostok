# Matching tools

Recipes for the structure campaign queue and the retail PDB declaration checks. The everyday loop (`vostok build`, ledger queries, `pdb inspect --view structure-diff`) is in [CLAUDE.md](../../CLAUDE.md).

The full-engine structural/byte campaign queue includes `game`, `game_core`,
`render`, named-local differences and unpaired frameless procedures:

```sh
python3 -m vostok ledger mismatch-queue --all-engine --write-queue
```

It writes `binaries/gen/structure_campaign_queue.md` and the uncapped machine-readable
`structure_campaign_queue.json` alongside it, using a completed build's report,
PDB evidence databases and ledger. Report/ledger-only compiler functions lacking a PDB
procedure record remain explicit coverage rows. Functions whose current-source `max` is 100% are listed
last; approximate statement classes, local-record differences and missing pairs
remain triage evidence requiring target inspection. Whole-PDB class variants,
enum and definition order are separate checks described in
[`divergence_queue.md`](docs/binary_matching/divergence_queue.md).

Class access, static/instance and virtual qualifiers, declaration order, and
candidate-source declarations are checked against retail PDB evidence:

```sh
python3 -m vostok pdb index source --database binaries/pdb/candidate/evidence.sqlite \
  --scope sources/vostok/network --scope sources/vostok/network_core
python3 -m vostok pdb compare source \
  --target binaries/pdb/target/evidence.sqlite \
  --candidate binaries/pdb/candidate/evidence.sqlite \
  --scope vostok::network --strict
```

The evidence-channel plan, proof boundaries, and extraction limits are in
[`pdb_evidence_engine.md`](docs/binary_matching/pdb_evidence_engine.md).

The check prints exact qualifier and declaration-order verdicts. PDB-unobservable
properties such as `mutable`, source expressions, and the physical home of an
unexpanded inline body stay explicit instead of being promoted to matches.

## Linker and PDB order probes

To investigate linker/PDB ordering without rebuilding game sources, run the
fixed-input VS2008 toy matrix with `python3 -m vostok tool pdb-order-probe`.
It records commands, artifacts, and complete `vostok-pdb topology --order --json`
comparisons under `binaries/gen/pdb-order-probes/`; see
[`pdb_topology.md`](pdb_topology.md#causal-order-probes).

## Joined statements

Two statements on one source line are never a retail shape to reproduce a line
record count with. `python3 -m vostok tool joined` lists them per module
(`--list` for every line, `--module`, `--blame`/`--since` for commit dates,
`--verdict` to look each line up in the PDB evidence: `retail-split` lines are
unwound first). Vendored subtrees and tool programs are skipped.

## One function's evidence

`vostok evidence` reads the last build's `binaries/pdb/*/evidence.sqlite` and the
ledger, without rebuilding. It gathers the ledger row, the PDB statement
partition (the `structure-diff` verdict), the relative-line geometry, the
`sema blocks` flow verdict and unmatched callees:

```sh
python3 -m vostok evidence 'weapon_core::tick'      # verdict line, then only what needs action
python3 -m vostok evidence <fn> --full              # matched rows, block diffs, callers too
python3 -m vostok evidence <fn> --json --only structure,flow
```

The first line is the verdict (`STRUCTURE MATCH, 2 SIZE residuals; cur 97.30,
max 98.10 (held)`). Exit status 0 means nothing needs action, 1 means something
does, and 2 means an ambiguous or unknown selector. A warning line flags evidence
built from an older revision of the source, whose line numbers no longer apply.

## Ledger history

```sh
python3 -m vostok ledger hist-gap --module render   # real drops: the commits that reached and lost the peak (--all: every hist > max row)
python3 -m vostok ledger last-exact <fn> --diff     # the last exact commit's source for <fn>, against HEAD
```

## Link layout and the whole image

`python3 -m vostok tool link-layout runs|owners` reads the section contributions
of both PDBs. It shows archive runs per section side by side, and the COMDAT
functions whose kept copy comes from a different compiland than retail's; see
[`link-layout.md`](link-layout.md).

`python3 -m vostok diff image` compares the linked EXE with retail region by
region (headers, Rich header, imports, resources, code and data bytes and order)
against the banked counts in `config/image_diff.tsv`; see
[`target-vs-base.md`](../target-vs-base.md). It runs on demand, not in the build.
