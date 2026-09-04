# Data reconstruction and matching

Function bytes alone do not prove that a function uses the right global,
constant, table element, initializer pointer, or loader-zero storage. Vostok
therefore has a separate data lane modelled after Gruntz. It does not alter the
normal code ledger.

## Ownership under LTCG

The linked image and PDB do not retain trustworthy compiler-object ownership
for every datum. Guessing one original owner would make the comparison depend
on an unrecoverable detail. The operational ownership rule is instead:

> Every function comparison unit owns a private comparison copy of every datum
> that its functions address, plus the transitive data reached through those
> data initializers.

A header constant or folded COMDAT may therefore be emitted into several
synthetic units. This is intentional and target-faithful for comparison: MSVC
can emit the same COMDAT in several TUs and the linker folds it, while LTCG can
move or clone the use. The unit is the existing objdiff function unit, not an
invented source declaration. No `DATA` macro is required.

Only allocations with a complete PDB type extent enter objdiff. External and
qualified local PDB identities pair directly. MSVC's ordinal-only `$S<n>`
locals, and local allocations whose LTCG owner moved, pair only when their
complete code-consumer fingerprints agree; equal initializer bytes alone are
never sufficient. Repeated identities are compared as physical-allocation
multisets, so folded aliases do not collapse distinct linked copies. An
unpaired allocation remains in the generated closure and blocker tables; it is
not emitted on one side, because that would shift later section ordinals and
poison unrelated rows. Local PDB allocations receive stable synthetic external
names derived from their retail RVA so the same allocation can be cloned into
several comparison objects. Public mangled names are preserved verbatim.

## Artifacts and flow

Tracked evidence and policy use the same names and locations as Gruntz:

- `config/retail/data.tsv` is the admitted retail start/kind census.
- `config/retail/data_symbols.tsv` contains only referents without an exact or
  extent-backed retail PDB owner. PDB-owned symbols never receive duplicate
  rows here. Its schema is `rva, name, size, type, comment`. `auto:` and
  `derived:` rows are review candidates, not accepted labels: every row must be
  inspected and given a review comment before the strict census can pass.
- `config/retail/pdb_data_extents.tsv` records reviewed sizes for exact PDB
  symbol starts whose CodeView records omit a usable extent. It closes their
  interiors without copying PDB names into the non-PDB registry.
- `config/retail/reloc_referents.tsv` is the Gruntz-style reviewed exception
  table for exact symbol-plus-addend references which containment cannot infer
  (one-past, one-before, or a field cursor outside the nominal PDB extent).
- `config/cleanliness/data-integrity-ratchet.tsv` holds maximum permitted debt.
- `binaries/gen/delink_data_manifest.tsv` and
  `binaries/gen/delink_data_section_manifest.tsv` are the target projection.
- `binaries/gen/base/` contains the necessary base-side counterparts; Vostok
  needs these because there are no pre-LTCG base objects to compare directly.
- `binaries/gen/data_access_map.tsv`, `data_coverage_gaps.tsv`,
  `data_consumer_closure.tsv`, `data_function_state.tsv`, and
  `data_manifest_blockers.tsv` are generated query evidence.
- `binaries/gen/missing_data_candidates.tsv` is the delinker's raw set of
  referents it could not resolve through retail PDB symbols or procedure/string
  extents. `missing_data_xrefs.tsv` removes targets contained by complete PDB
  data extents and assigns the remainder to reviewed non-PDB rows.
- `binaries/gen/missing_data_report.json` records the exact set subtraction and
  its input hashes.
- `binaries/gen/data_strict_report.json` is the independent strict-referent
  objdiff report.
- `binaries/gen/render_reloc_audit.tsv` contains one row for every physical
  render-function relocation into `.rdata`, `.data`, or loader-zero BSS on
  either image. It pairs exact function-relative sites first and then unique
  decoded access-sequence sites; unmatched sites remain explicit.
- `binaries/gen/render_extentless_data.tsv` deduplicates retail PDB starts with
  no trustworthy complete size. Each row records xref/access evidence,
  observed and neighbouring boundaries, candidate-address votes, normalized
  hashes, and retail/candidate byte-pattern previews.
- `binaries/gen/render_function_data.tsv` compares the deduplicated whole-datum
  set reached by every paired function. References to different fields or
  elements of one complete allocation therefore enrol that allocation once;
  the strict-referent objdiff report separately preserves exact symbol-relative
  addends at each instruction. Compiler-owned constants pair by their
  complete normalized bytes even when LTCG changes their PDB pool owner or
  moves them between `.data` and `.rdata`; named allocations without such a
  byte identity retain their PDB identity. Missing
  uses and allocations missing from either linked image are separate columns.
  A second set of columns expands exact encoded E8 calls and E9/EB tail jumps
  from each paired root. It records both cone sizes, remaining cone differences,
  and the shortest target/base path to every direct referent that moved across
  an inline or overlapping-PDB-owner boundary.
- `binaries/gen/render_reloc_report.json` hashes those inputs and tables. The
  human-readable problem list is tracked in
  `docs/data_matching/render_data_problems.md`. The machine TSVs remain
  exhaustive; settled extentless comparisons are omitted from the Markdown so
  it contains differences and unresolved evidence rather than matched debt.
- `python3 -m vostok data module-relocs MODULE` applies the same exhaustive
  audit to any ledger module. Its generated files use the module name in place
  of `render`; its readable dossier is
  `docs/data_matching/MODULE_data_problems.md`. `render-relocs` remains
  the compatibility spelling used by the authoritative build.
- `binaries/gen/data_module_reloc_report.json` aggregates every module present
  in the current function ledger. Its module set is derived rather than
  allowlisted, and each per-module report hash is pinned. The authoritative
  build refreshes this complete denominator after regenerating the ledger.
- `binaries/gen/data_function_open.tsv` is the corresponding complete work
  queue. It contains every function datum-use row whose resolution is `OPEN`;
  the zero gate requires it to contain only its header.
- `config/codex_wall_reviews.tsv` records manually proved residuals using
  Gruntz's `open`, `bounded`, and `exact` statuses and wall-class vocabulary.
  Vostok additionally binds every terminal review to both the function's
  source-body hash and its exact datum-difference hash, so a source, referent,
  or call-cone change automatically makes the review stale and reopens the row.
- `docs/data_matching/datum_use_audit.md` is the closeout report for the first
  complete all-module datum-use audit. It separates source corrections from
  compiler/linker walls and records what the zero gate does and does not prove.
- `binaries/data-objdiff/` is the complete parallel comparison project. The
  ordinary `binaries/objdiff/` project keeps its measured legacy delinker and
  objdiff CLI and never consumes data manifests.

The authoritative build runs this sequence after linking:

1. Export target and base PDB data indexes with the pinned delinker. Separately,
   enumerate all relevant retail relocations and write only `all referents -
   PDB-owned referents` to `data_symbols.tsv`; unresolved rows remain explicit
   Gruntz-style fences until reviewed: `DAT_<va>` for proven library/linker
   contributions and fail-closed `UNPROVISIONED_<va>` when any engine/game or
   unattributed contribution references the target.
   Self-describing MSVC90 EH/SEH records supply review candidates with exact
   counts and extents. Each referenced private `.text` address remains its own
   unresolved row: the containing PDB section contribution is only a sizing
   bound and ownership clue, never a substitute for reviewing the individual
   code entry. A permanent overlap check forbids a reviewed anonymous code
   extent from covering any PDB procedure.
2. Enumerate absolute `.text` data references from PE `HIGHLOW` relocation
   sites, decode their access direction/width, and attach each to its PDB
   function and comparison unit.
3. Seed each unit with its directly addressed typed allocations. Walk
   relocations inside those allocations to close data-to-data initializer
   edges.
4. Write identical paired target/base manifest sequences. Each allocation is
   a separate COMDAT section, so one mismatch does not make a whole TU's data
   indivisible.
5. Delink target and base into `binaries/data-objdiff/` using the dedicated
   data-topology delinker and patched data objdiff CLI. Its default report is
   the data denominator and loose-referent baseline; it cannot feed the code
   ledger.
6. Generate a second data-project report with `functionRelocDiffs=all`. This compares a
   function's relocation target identity and symbol-relative addend as well as
   its instructions; neither data-project report feeds the normal function
   score or hash-scoped MAX ledger.
7. Refresh the linked-image audit and all query tables, then hash their inputs
   into `data_image_report.json`.

Use the commands directly when investigating the lane:

```sh
python3 -m vostok data init-target
python3 -m vostok data missing
python3 -m vostok data missing-next
python3 -m vostok data missing --check
python3 -m vostok data missing-symbol PATTERN
python3 -m vostok data missing-report
python3 -m vostok data project
python3 -m vostok data refresh
python3 -m vostok data render-relocs
python3 -m vostok data render-relocs --check
python3 -m vostok data render-relocs PATTERN
python3 -m vostok data module-relocs MODULE
python3 -m vostok data module-relocs MODULE --check
python3 -m vostok data module-relocs MODULE PATTERN
python3 -m vostok data module-relocs all
python3 -m vostok data review PATTERN --status bounded --class inline --evidence TEXT
python3 -m vostok data review --list
python3 -m vostok data report
python3 -m vostok data function PATTERN
python3 -m vostok data access PATTERN
python3 -m vostok data relocs PATTERN
python3 -m vostok data symbol PATTERN
python3 -m vostok data coverage
python3 -m vostok data check
python3 -m vostok data check --gate
python3 -m vostok data check --all-zero
```

`missing-next` prints the retail bytes, PDB neighbours, owners, callers, and
xrefs for the first row that lacks a complete manual label. After editing that
row, rerun `missing --no-export` and repeat until the queue is empty. A new
unresolved non-PDB referent is a hard build failure. `project` updates manifests
without delinking. `refresh` updates the complete audit against an
already-generated normal and strict report. A normal `python3 -m vostok build`
owns the complete ordered flow. `render-relocs` is the direct render check: it
does not require a datum to be enrolled in objdiff and therefore exposes every
physical relocation, including candidate-only sites and extentless retail PDB
data. With no pattern it regenerates the audit and Markdown report; a pattern
inspects generated rows without rebuilding.

## What each result proves

- **Consumer reachability** is the unique union of trusted retail extents
  reached by functions or their initializer closure, divided by the complete
  virtual size of retail `.rdata` plus `.data`.
- **Consumer pairing** is the unique reachable subset for which the base PDB
  has the same identity and complete extent.
- **Projected objdiff match** counts comparison copies. Its denominator can be
  much larger than the unique retail extent because one datum is deliberately
  cloned into every consuming unit. Never divide this value by retail image
  bytes.
- **Strict referent debt** is the loss of data-project exact functions/code
  bytes when `functionRelocDiffs=all` is applied to that project. This is the direct answer to
  “does the function use the same named datum and addend?”
- **Linked-image exactness** independently compares final bytes. Pointer cells
  are normalized before hashing and then compared by resolved PDB identity and
  addend, so equal address-shaped bytes cannot hide a wrong referent.
- **Gross PDB coverage** is only the union of valid PDB type extents. It is a
  census-quality measurement, not a source match.

The access map also records width evidence. An access that leaves its PDB
extent becomes a manifest blocker instead of being silently truncated. Indexed
table counts and pointer-derived accesses without an absolute relocation remain
explicit blind spots; the strict report and linked-image relocation closure are
the backstops, not grounds for guessing an extent.

The direct render audit closes the absolute-relocation side of that blind spot.
For a complete retail extent it compares normalized bytes, relocation-cell
layout, and resolved relocation targets over the complete allocation. For an
extentless start it uses every observed xref and reports a bounded evidence
window ending at the next known start, plus the start of the next relocation
cell inside data. These windows are diagnostic rather than silently promoted
to reviewed extents. Its function table is a source-review sieve, not a second
code score: `USE_DIFF` says the linked bodies reach different datum sets, while
`*_DEFINITION_MISSING` says a referenced allocation has no stable
identity/content counterpart anywhere on the other image. A row is not a
source edit until target assembly or PDB evidence identifies the responsible
literal, member, linkage, table entry, or named datum.

`vostok data check` validates schemas and content hashes, requires every
non-PDB datum to have all five reviewed fields, and requires the zlib control
set to remain exact. It also requires a fresh direct relocation report for
every module in the function ledger. `--gate` additionally reads the projection
maxima in `config/cleanliness/data-integrity-ratchet.tsv` and requires the
aggregate function datum-use count with `resolution=OPEN` to be exactly zero.
Raw relocation-set differences proved byte-exact by the current build or by
the hash-scoped MAX for the current source body remain visible as
`CURRENT_EXACT` or `HASH_MAX_EXACT`, but do not count as open debt.
`CALL_CONE_EXACT` likewise preserves the raw direct difference while proving
that the paired roots reach identical complete datum sets through decoded
internal calls. A display-name match is never used as an edge: destinations
come from the executable instruction bytes and must land on a rich-index
function start. Any cone referent difference remains `OPEN`.

`--all-zero` is the deliberately uncompromising completion oracle. In addition
to the normal gate, it requires every linked-image allocation to be `EXACT`,
all consumer copies and bytes to pair, every projected objdiff byte to match,
every function datum-use row to be exact, and strict-referent debt to be zero.
It is expected to fail while any source difference, unpaired allocation, or
proved compiler/linker placement wall remains; unlike the ratchet gate, it does
not accept reviewed exceptions.

`REVIEWED_WALL` is the only manual terminal result. Record it only after the
target PDB, assembly, complete allocation bytes, and source establish that the
remaining datum-set difference is a compiler, linker, or external build-input
boundary rather than a source correction. The review command captures the
current source and diff hashes; it cannot permanently excuse a changing row.

The deferred PDB-extent review and shifting-candidate identity-transfer work is
specified in
[`data_extent_handoff.md`](../data_matching/data_extent_handoff.md).
