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
