<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# In-repository PDB evidence engine

The implementation lives in `tools/vostok-pdb`. Python owns build sequencing,
the committed matching ledger, and the existing CFG views; Rust owns PDB/PE
extraction, source declarations, and their comparisons. The editor invokes the
same Rust executable. Generated SQLite databases are disposable snapshots,
not another committed ledger.

## Evidence and comparison plan

| Question | Evidence and implemented interface | Proof boundary |
|---|---|---|
| Where was emitted code attributed? | `index pdb`, `compare pdb`, `inspect`: procedure file, each statement's file and relative line geometry | A missing emitted body does not prove `inline`, nor recover its original header |
| Is source still a placeholder? | `index source`, `compare source`: literal marker inventory in all scoped source files | PDB line tables do not contain original expressions; helper behavior still needs assembly/caller evidence |
| Are access and member qualifiers correct? | `compare source`: access, static/instance, virtual and method const; `topology --classes`: complete type variants and member attributes | A qualifier projection match is not a complete class match |
| Are fields and bases correct? | `topology --classes`: field types, offsets, bases, size and nested declarations; source gate checks parsed access/static/offset facts | Duplicate incompatible complete class records remain separate |
| Is source mutability observable? | Candidate AST retains mutable/pure-virtual syntax; explicit `UNOBSERVABLE` findings | Retail CodeView does not reliably recover these source spellings |
| Is order correct? | Relative source declaration order; `divergence` checks definition order using both line attribution and procedure records; topology retains raw order | Linker order and physical stream order do not justify arbitrary source reordering |
| Are statements and locals correct? | `inspect --view structure-diff`, `compare pdb`, local name/type/scope payloads and existing ledger/CFG consumers | Optimized debug records are an emitted-code projection, not full original source |
| What else was retained? | `topology --order`: MSF, named streams, DBI, TPI/IPI, globals/publics, C13 checksums/lines/inlinees, optional debug streams and raw fallbacks | See the channel inventory in `pdb_comparison_audit.md`; unknown raw payload is not decoded semantics |
| Is the comparison current? | PDB/EXE SHA-256, compile-command hash, scoped source snapshot; strict comparison rejects stale inputs | Shared headers outside source scopes and compiler-environment changes require explicit re-indexing |

Every new comparison channel must identify its source record, preserve duplicate
identities, define its unsupported/absent behavior, and test a real false-match
case before joining a strict gate. A successful build, byte score, source gate,
and class-topology comparison answer different questions.

## Implemented safeguards

`MATCH`, `MISMATCH`, `TARGET_ONLY`, `BASE_ONLY`, `AMBIGUOUS`, `UNOBSERVABLE`,
`UNSUPPORTED`, and `STALE_INPUT` are distinct verdicts. Strict comparison fails
on deterministic differences, unsupported inputs, and stale snapshots.
Ambiguous function candidates are not relabeled as base-only procedures.
Signed relative line positions preserve backwards line-table transitions.
Scoped parse errors, including those in `.cpp` files, make source coverage
incomplete. The clang-cl adapter preserves `/clang:` forwarding and translates
Wine paths in the case-insensitive include overlay for the Linux libclang host.
Querying a function by an ambiguous name requires disambiguation.

## Remaining extraction limits

The retained-procedure index does not yet count every symbol-record skip from
`pdb2`; its coverage explicitly reports this unsupported channel. The raw
topology interface remains available for examining records outside that index.
Malformed Scaleform modules can be skipped by carcass extraction and are named
in build warnings. The source parser is an explicit libclang declaration pass
using the repository's `arguments`-form compilation database; it is not an
MSVC compiler replacement and does not certify unparsed or optimized-away code.

The former standalone source-checksum, compiler-summary, and PE-summary
frontends are not shipped under old binary names. Their historical reports
remain documented; raw checksum/compiler/PDB topology evidence is available
through the unified tool. Those historical source-hashing/PE summaries should
not be mistaken for newly reproduced gates.

## Network application

The 67 literal marker occurrences comprised 64 helper bodies and three umbrella
comments. Their per-item semantic record is `network_core/inline_recovery.md`.
The field and declaration decisions, ambiguity boundaries, and full-build
verification are recorded in `network_core/pdb_evidence_audit.md`.
