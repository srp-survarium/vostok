# Duplicate PDB type records are competing variants, not automatic phantoms

- confidence: 9
- cpp tags: cpp:member cpp:struct cpp:template
- asm tags: asm:mov asm:cmp
- topic tags: topic:tooling topic:structure-shape topic:scoring-artifact

## Symptom

`pdb_divergence --headers-only` can report one `[member]` or `[size]` row for a
qualified name even though the same PDB contains several complete records with
that name and different layouts. For example, the network cluster has records
that disagree about `sequence_number<u8>` versus `sequence_number<u16>` and
about related containing-class sizes.

The historical comparator normalized the merged type stream to one record per
name. Any verdict derived from that one selected record overstates the evidence.

## What the PDB actually establishes

A merged/LTCG PDB may contain:

- repeated records with exactly the same semantic shape;
- several different semantic variants under the same qualified name;
- different variant sets or record multiplicities in retail and candidate.

None of those facts identifies one globally canonical definition. Different
variants may reflect per-compiland definitions, incremental-link history,
CodeView merging, or a real ODR/layout inconsistency in the shipped program.
Calling an unmatched record `stale` or `phantom` requires evidence beyond its
position or name in the type stream.

Use the raw variant-set query first:

```sh
pdb_topology \
  --target-pdb "$SURVARIUM_BIN/survarium.pdb" \
  --base-pdb binaries/Win32/survarium-dx11-win32-gold.pdb \
  --classes --class 'vostok::network_core::udp_match_connection'
```

The statuses are deliberately narrow:

- `identical`: equal semantic variant sets and record multiplicities;
- `record-multiplicity`: equal shapes, unequal equal-record counts;
- `variant-overlap`: at least one shared shape plus unmatched variants;
- `different`: no shared shape for that name.

PDB-local type indexes are preserved as provenance. They are allocation
positions, not cross-PDB identities.

## How to resolve a consumer

The assembly of a real consuming function can bind that function to one layout:

- `mov [eax], cx`, a word load/store, or offsets spaced by two bytes establishes
  a 16-bit member for that access path;
- `mov [eax], cl`, a byte load/store, or offsets spaced by one byte establishes a
  byte member for that access path;
- loop bounds and element strides can establish the array extent used by a
  constructor or allocator;
- the function's CodeView procedure/local types and its owning field-list binding
  can strengthen the association.

This proves which shape the inspected code uses. It does **not** prove that every
other same-named record is fictitious or that no other shipped compiland uses it.
Record-to-compiland and record-to-consumer provenance must be shown before a
whole-name divergence is closed.

For candidate-side history, use a fresh full link before classifying anything.
An old incremental PDB can retain records that no current object emits. A byte
match in `report.json` proves the current object code for a function, but does not
by itself explain every type record in the linked PDB.

## Reclassified network evidence

The existing `udp_match_connection::update_acknowledgements` disassembly supports
the `<u16>` shape for that function: it uses 16-bit stores at two-byte-spaced
offsets. The inspected `match_client_impl` constructor supports an 8192-element
arena for its access path. Those are useful consumer-level facts.

They are not sufficient to label the disagreeing retail records, or all related
network rows, as phantoms. Keep the rows as variant/provenance questions until a
fresh PDB audit shows the exact target/candidate variant sets and binds the
relevant records to compilands or emitted consumers.

## Counter-example

`particle_system_lod` had a genuine field reorder and the emitted accesses agreed
with the reordered layout. That was actionable. The lesson is not that
single-record rows are real and duplicate-record rows are fake; it is that the
type-stream result and the code using the type must be made consistent before a
source change or a parked classification is justified.
