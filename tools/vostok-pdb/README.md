<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# vostok-pdb

`vostok-pdb` is Vostok's single Rust evidence engine for retail/candidate PDBs,
their paired PE images, and scoped candidate C++ source. The normal build writes
uncommitted SQLite databases at `binaries/pdb/{target,base}/evidence.sqlite`;
candidate source is indexed only when explicitly requested.

```text
vostok-pdb index pdb --pdb FILE --exe FILE --database DB --side target
vostok-pdb index source --compile-commands compile_commands.json \
  --database binaries/pdb/candidate/evidence.sqlite --scope sources/vostok/network_core
vostok-pdb inspect --target TARGET_DB --base BASE_DB \
  --function udp_match_connection::tick --view structure-diff
vostok-pdb compare pdb --target TARGET_DB --base BASE_DB \
  [--function NAME] [--file SOURCE-SUBSTRING] [--strict]
vostok-pdb compare source --target TARGET_DB --candidate CANDIDATE_DB [--strict]
vostok-pdb coverage --database DB
vostok-pdb generate carcass --pdb FILE --output DIR \
  --engine-path 'c:\survarium\sources'
vostok-pdb topology --target-pdb TARGET_PDB --base-pdb BASE_PDB --classes --json
vostok-pdb divergence --target-pdb TARGET_PDB --base-pdb BASE_PDB \
  --target-engine-path 'c:\survarium\sources' \
  --base-engine-path 'c:\survarium\sources'
```

Every fact carries an origin (`OBSERVED`, `CORRELATED`, `INFERRED`, or
`REVIEWED`). Comparison verdicts are `MATCH`, `MISMATCH`, `TARGET_ONLY`,
`BASE_ONLY`, `AMBIGUOUS`, `UNOBSERVABLE`, `UNSUPPORTED`, or `STALE_INPUT`.

Inspection prefers an exact full signature among shared decorated aliases
before source-file preference, but retains true duplicate ambiguity. A missing
paired function is reported separately from an omitted `--base` argument.

`inspect --view base --json` selects the paired candidate record and rejects
missing or ambiguous candidates. JSON `diff`/`structure-diff` views are not
implemented: use the text views or `compare pdb --json`. Earlier versions
incorrectly returned the target record for all JSON views; target-vs-base
instruction/local comparisons made that way must be rerun.

PDB comparison first narrows shared decorated representatives by full signature.
If the decorated representative is absent, an exact full-signature candidate
can still be compared. Such a pairing retains a correlated `identity:MISMATCH`
with both representative names; source-location differences remain independent.
It does not prove byte equality or canonical ownership of an ICF group.
Inferences and reviewed hypotheses never become `MATCH`.

Carcass generation preserves recorded base-class access and virtual inheritance.
It suppresses unobserved copy constructors/assignments only when CodeView marks
them compiler-generated; a missing standalone body alone is not sufficient.
It does not infer `explicit` from a constructor's argument count. Generated
declarations remain reconstruction inputs, not proof of unobservable syntax.
Enum rendering preserves signed minima and the full unsigned 64-bit range.
Focused type-order JSON includes both sides' raw module-reference lists, even
when their comparison has no differences; these references do not establish
ownership of a particular same-name complete class variant.

Comparisons validate the indexed PDB/EXE hashes. Source comparison also checks
the compile-command hash and scoped source-file snapshot (including new and
deleted files); `STALE_INPUT` fails `--strict` and requires re-indexing. This
snapshot does not yet track headers outside the requested scopes. Re-index
source after changing shared dependencies or the compiler environment.
`inspect` is a snapshot query; it rejects ambiguous name selections and accepts
an exact mangled identity or `--rva` to select a procedure.

`compare pdb` covers procedure presence, source location, statement partition,
relative line geometry, and recorded local names/types/scopes. Its scoped views
retain candidate-only procedures, and an empty target selector is not a clean
match. `topology --classes` is the type-stream view for duplicate-preserving
class variants, field offsets/access, method access/static/virtual/const status,
and relative declaration order. Candidate-source comparison treats facts that
CodeView cannot express reliably (notably mutable and pure-virtual source
syntax) as `UNOBSERVABLE`, never as a guessed match.
`divergence` supplies the broader normalized enum, class, source-definition
order, constant, and out-of-line-presence views; `--headers` and `--sources`
restrict its channels, while the default runs both.

The PDB parser was imported from
`srp-survarium/vostok-pdb-parser@6262ce150b12729b865a7eca6d82ad563256ba20`
and includes the former Vostok focused-type-order patch. It now preserves each
line record's file, retains all complete type variants and their type indices,
and does not infer `inline` merely because an emitted body is absent.
