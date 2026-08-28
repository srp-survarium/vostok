# Image data matching

Function objdiff does not establish equality for globals, constants, pointer
tables, or loader-zero BSS. The data lane measures those independently and
never changes `binaries/objdiff/report.json` or the function ledger.

## Evidence and flow

The pinned Survarium-compatible delinker has an export-only
`--write-data-index` mode. It reads data definitions directly from each PDB and
records exact symbol-name bytes, compiland/archive ownership, RVA, linkage,
CodeView type index, and a type-derived extent. It exits before normal COFF
emission, so enabling the export cannot move function pairings.

Run the immutable retail census once, then refresh base after a successful
build:

```sh
python3 -m vostok data init-target
python3 -m vostok data refresh
python3 -m vostok data report
```

`refresh` performs the following pipeline:

1. Pair external definitions by PDB public identity and local definitions by
   normalized owning module, compiland, and local PDB identity.
2. Read `.rdata` and `.data` from the linked images. Reads beyond raw `.data`
   use loader-zero bytes, so BSS is measured instead of silently omitted.
3. Normalize PE `HIGHLOW` pointer cells before hashing bytes, then compare the
   relocation topology and resolve each pointer to a PDB data/function identity
   plus addend. Equal zeroed bytes with a different pointer target are `RELOCS`,
   not exact.
4. Build code-to-data access maps from `.text` relocation sites and retail/base
   disassembly, joined to PDB function owners.
5. Regenerate the tracked `data_state.tsv` and the ignored detailed reports
   under `binaries/data/`.

The direct linked-image comparison is authoritative. Candidate-shaped COFF
sections remain a secondary objdiff workbench for reviewed definitions; they
must never replace the image census or feed the function score.

## Metrics

- **Gross coverage** is the address-union of valid PDB type extents divided by
  the complete retail virtual size of `.rdata` plus `.data`.
- **Reconstructable coverage** uses the same numerator and removes only ranges
  explicitly reviewed in `data_exclusions.tsv` from the denominator. Unknown
  ranges remain eligible.
- **Fidelity** is the address-union of `EXACT` definitions divided by all
  paired, compared target extents. `EXACT` requires both normalized bytes and
  resolved relocation signatures to agree.

Use `vostok data coverage` to show the largest uncovered retail ranges. Use
`symbol`, `relocs`, and `access` with a substring, symbol, or RVA to move from a
ledger row to its pointer cells and real callers.

`vostok data check` validates every evidence file against hashes captured by the
report and checks that the standalone coverage report and tracked ledger are
exact projections of it. `--gate` reads floors from `data_gate.tsv`; it
intentionally fails while that table is empty. Arm the gate only after the
whole-image census is calibrated against zlib, then render.

The calibration invariant is live: all PDB-typed zlib definitions must pair and
be `EXACT`, or even shadow-mode `data check` fails. The initial census proves
19/19, so pointer normalization and loader-zero handling have a known-clean
library oracle before render residuals are classified as source work.
