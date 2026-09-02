# Raw PDB topology evidence

`pdb_parser` already uses the `pdb2` Rust crate, but its normal carcass and rich
index intentionally flatten most CodeView records. `pdb_topology` owns the raw
CodeView comparisons that need record identity or sequence: one-procedure
topology, complete class variant sets, and the observable MSF/PDB stream layout.
Inspect one procedure with:

```sh
pdb_topology --pdb "$SURVARIUM_BIN/survarium.pdb" \
  --module render_engine_world_pc_dx11 \
  --function 'world::draw_scene' --context 6
```

Add `--json` to receive the same owned evidence model for `jq`, batch audits, or
future match-database ingestion.

For matching, compare the retail and reconstructed PDBs directly:

```sh
pdb_topology \
  --target-pdb "$SURVARIUM_BIN/survarium.pdb" \
  --base-pdb binaries/Win32/survarium-dx11-win32-gold.pdb \
  --module render_engine_world_pc_dx11 \
  --function 'world::draw_scene' --context 6
```

Diff output uses conventional `- base` / `+ target` rows. Procedures are paired
by decoded signature and object name. Volatile PDB-local addresses, symbol
positions, and type indexes are excluded from equality: a type index is an
allocation position within one PDB and has no identity in another PDB. Decoded
types, declaration positions, frame shape, line byte lengths, flags, storage,
and record order remain comparable. Every evidence section is printed, with
`no semantic differences` when it agrees. `--json` returns the paired procedures
and structured per-section differences.

## Whole-PDB class comparison

`--classes` walks every complete `LF_CLASS`, `LF_STRUCTURE`, and `LF_INTERFACE`
definition in the target TPI stream and compares it with the same qualified class
in base:

```sh
pdb_topology \
  --target-pdb "$SURVARIUM_BIN/survarium.pdb" \
  --base-pdb binaries/Win32/survarium-dx11-win32-gold.pdb \
  --classes
```

The text view prints every differing or base-missing target class, plus identical
names that still carry multiple semantic variants or repeated equal records. Add
`--show-identical` to print the remaining clean classes too, or `--json` to retain
all target classes as a queryable model regardless of status. `--class
'qualified::name'` restricts a diagnostic run to one case-insensitive qualified
name; the unfiltered command remains the authoritative sweep.

The comparison resolves PDB-local type indexes before equality and reports:

- class kind, size, and CodeView properties;
- direct and virtual inheritance, access, offsets, and virtual-base layout;
- instance/static field names, types including CV qualifiers, visibility,
  offsets, attributes, presence, and declaration order;
- method names, return/argument types, member-function CV qualifiers, calling
  convention/constructor flags, visibility, virtual/pure/introducing/sealed
  attributes, vtable slots, presence, and declaration order;
- nested type names, resolved types, visibility, presence, and order; anonymous
  enums also include their enumerator names and values when the field list is
  readable.

Overloads are paired first by name and resolved signature, then by name and
argument count. A remaining target declaration may pair with an otherwise
identical base declaration to surface a likely member or function rename instead
of a misleading add/remove pair. Class names themselves are paired only by their
normalized qualified name: guessing that unrelated one-sided classes are renames
would not be authoritative.

The type stream records argument *types* and qualifiers for all declared methods,
but does not retain source parameter names for declaration-only methods. Parameter
name comparison is therefore not claimed. Malformed/unrenderable type records are
kept as `<unresolved-type>` rather than aborting the sweep, and the summary counts
those declarations.

Complete records are grouped by normalized qualified name and then by their
entire semantic shape. The report preserves every PDB-local type index and uses
four explicit outcomes instead of selecting a supposedly canonical record:

- `identical`: both PDBs contain the same semantic variant set and the same
  record multiplicity for every variant;
- `record-multiplicity`: the semantic variant sets agree, but one PDB contains
  more equal records;
- `variant-overlap`: at least one semantic variant agrees, while either PDB also
  contains unmatched variants;
- `different`: the same name exists on both sides but no semantic variant agrees.
  The detailed field diff uses the closest disjoint pair only as a diagnostic and
  identifies both type-index sets; it is not called canonical.

This distinction matters for merged/LTCG PDBs. A same-named second record is
evidence of a variant set, not by itself proof that either record is stale, a
phantom, or the record used by a particular emitted function.

## Whole-PDB order comparison

`--order` inventories every present MSF stream and compares the physical,
semantic, hash, and address-derived sequences separately:

```sh
pdb_topology \
  --target-pdb "$SURVARIUM_BIN/survarium.pdb" \
  --base-pdb binaries/Win32/survarium-dx11-win32-gold.pdb \
  --order --limit 100
```

The coverage header states exactly which layers were decoded. They include:

- MSF superblock, free-page map, directory/map pages, stream slots, page lists,
  allocation runs, fragmentation, identified roles, and unidentified streams;
- PDB Info named streams and `/names` strings, metadata, feature codes, live,
  deleted, and hash-bucket order;
- DBI substream layout, module/object and library groups, source-file lists,
  section contributions/maps/headers, EC strings and hashes, and optional debug
  stream presence;
- all raw TPI/IPI record kinds, named complete records, complete enum value
  order, per-record hashes, index checkpoints, and adjustment buckets;
- all raw and recognized global/module symbol records, GSI/PSI hashes and
  buckets, public address/thunk/section maps;
- C13 subsections and their local strings, checksums, line programs, frame data,
  inlinees, cross-scope maps, and ordered raw fallback for unknown payloads;
- decoded legacy FPO and frame-data records, plus inventory of absent OMAP,
  fixup, xdata/pdata, token/RID, and other optional streams.

If a present stream has no stable role, it is still listed in the unidentified
inventory. If a supported optional stream is absent, the coverage report says
so instead of implying that it was compared. `--limit 0` is useful for printing
all channel summaries without individual rows; `--json` remains uncapped.

Only a key that occurs exactly once in both sequences participates in a semantic
order claim. Duplicate keys with equal counts are `excluded-nonunique`; unequal
counts are a `multiplicity` difference. One-sided records and changed
descriptions are separate. A `moved` row means that the shared unique record
participates in at least one pairwise inversion; an insertion before it cannot
manufacture a move. Raw kind streams and address/ordinal-only formats are
explicitly labeled weaker because they cannot always provide such an identity.

Each comparison also reports exact inversion count/rate, longest ordered
subsequence, retained and reversed adjacency, longest contiguous run,
increasing runs, and rank displacement. A moved-participant count can make one
block rotation look nearly total; the locality metrics preserve that distinction.

Most whole-PDB streams are physical, linker-, hash-, type-index-, or
address-derived evidence. LTCG, COMDAT selection, archive extraction, type
merging, and linker processing can reorder them. Anonymous TPI records cannot
be authoritatively paired across independently allocated streams and are not
given invented identities. Source definition order remains owned by
`pdb_divergence`; class and complete-enum declaration order and
function-internal structure are the high-confidence `--classes`, enum-scope,
and `--function` channels. The current measured coverage and confidence limits
are recorded in `pdb_comparison_audit.md`.

## Causal order probes

Use the repository's fixed-input VS2008 probes before interpreting a whole-game
order difference as a source or build lever:

```sh
python3 -m vostok tool pdb-order-probe
python3 -m vostok tool pdb-order-probe --list
python3 -m vostok tool pdb-order-probe --case archive-member-order
```

The harness compiles and links only tiny `/Z7` fixtures without the CRT. It
keeps source and output paths fixed, changes one input per pair, and stores the
exact commands, linker logs, file hashes, copied PDB/EXE artifacts, full
`pdb_topology --order --json` reports, and channel summaries under
`binaries/gen/pdb-order-probes/`. The directory is generated and gitignored;
each invocation replaces it, so preserve a result externally if it must outlive
the next run.

The 21-case matrix separates direct object order, archive-member order,
unresolved-root demand, and library order both with and without `/GL`/`/LTCG`.
It also covers LTCG compilation chronology, direct-header order, PCH-internal
order and composition, headers and declarations after a PCH, redundant includes
on either side of the PCH boundary, type-contributor and within-TU type-use
order, linker `/ORDER`, clean relinks, and replayed incremental relinks. A result
establishes behavior for the small VS2008 fixture only. It is a causal model to
test against the real link, not a license to reorder production inputs without
target DBI/TPI/C13, section, and code evidence. The current conclusions and
their open whole-game applications are tracked in
[`pdb_order_causal_attribution.md`](../todos/pdb_order_causal_attribution.md).

For a large real PDB, a focused query avoids loading TPI, global symbols, and
physical stream topology when one module/library population is under test. It
still decodes the DBI sequence and the matching modules' local C13 source-file,
checksum, line, and subsection sequences:

```sh
pdb_topology \
  --target-pdb "$SURVARIUM_BIN/survarium.pdb" \
  --base-pdb binaries/Win32/survarium-dx11-win32-gold.pdb \
  --order --module vostok_vfs --json
```

The case-insensitive substring is matched against both module and object/library
names. The report retains the raw filtered target and base DBI sequences plus
the usual unique-key inversion metrics and module-local C13 comparisons. Use
the unfiltered `--order` command when TPI, public/frame, hash, or MSF channels
are required.

The output is divided by evidentiary strength:

- **Explicit procedure evidence (high confidence):** procedure type/index,
  prologue and epilogue bounds, flags, exact frame/padding/saved-register sizes,
  parameters and locals with type/storage, lexical blocks, constants, frame
  cookies, and indirect-call signatures (`S_CALLSITEINFO`).
- **Line-program geometry (high confidence):** the PDB's own byte length for each
  source line record. This preserves information that was previously replaced by
  a span-to-the-next-record calculation.
- **Class field-list binding (high confidence):** joins the procedure type back to
  the owning class's `LF_FIELDLIST`/`LF_METHODLIST`, showing the exact declaration
  entry and its neighboring members/methods. Identical same-class signatures can
  legitimately yield more than one candidate, and are printed as such.
- **TPI neighborhood (heuristic):** adjacent type indexes often expose a family of
  argument lists and member-function types emitted together. The linker merges
  and deduplicates TPI, so adjacency is a clue, never proof of lexical order.
- **Physical/top-level symbol neighborhoods (heuristic):** record adjacency can
  reveal nearby generated helpers, statics, initializers, and UDT records. LTCG,
  COMDAT emission, and linker processing can reorder them; confirm any inference
  with line records, class fields, assembly, or a real call site.

## Retail PDB audit

An inventory of the shipped PDB found 2,396 compilands and 47,792 procedures.
Previously flattened but matching-relevant records include 48,300 frame records,
24,963 indirect-call signature records, 440 frame-cookie records, and 433,853
module-scope UDT records. All 390,834 line records carry an explicit byte length.
This VS2008 PDB has no inline-site/IPI records and no line-column, multiline-range,
or expression-kind variants, so those newer CodeView channels cannot be recovered
from this artifact.

For `world::draw_scene`, the tool additionally confirms a `0x44` frame with four
bytes of saved registers and binds the procedure to field #35 of
`vostok::render::engine::world`, between `setup_rotation_control_modes` and
`pick_lighting_luminance`. That binding is direct CodeView evidence; the nearby
TPI records are supporting context only.

Comparing the current base PDB explains why the same method is field #31 there.
The target field list starts with the `boost::noncopyable` base, three nested
typedef records (`colored_vertices_type`, `colored_indices_type`, and
`ui_vertices_type`), then `m_renderer`, then the methods. The base has the base
record, lacks those three nested typedefs, and records `m_renderer` after the
methods, matching its current source placement in `render/engine/world_pc.h`.
This is actionable declaration-order/type-alias evidence that the generated
header and disassembly views did not expose on their own.

The direct diff summarizes the function-level consequences without type-index or
RVA noise:

```diff
[explicit procedure evidence — high confidence]
- ... len=0x5d body=0xa..0x56
+ ... len=0x61 body=0xa..0x5a

[line-program geometry — high confidence]
- bytes=0x8 cols=None-None kind=Statement
+ bytes=0xc cols=None-None kind=Statement

[class field-list binding — high confidence]
- > field#31 method draw_scene access=public ...
+ > field#35 method draw_scene access=public ...
```
