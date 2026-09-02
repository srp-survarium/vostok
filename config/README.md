# config/ — tracked machine-readable evidence and policy

This directory follows the Gruntz convention: TSV ledgers, retail censuses,
reviewed overrides, and ratchets live here. Explanatory prose belongs in
`docs/`; regenerable evidence belongs in `binaries/gen/`.

## Root matching tables

- `match_state.tsv` is the generated per-function campaign ledger.
- `exact_fold_aliases.tsv`, `measured_fold_aliases.tsv`,
  `folded_symbol_overrides.tsv`, and `module_ownership_overrides.tsv` are
  reviewed function-pairing inputs.

## retail/

- `data.tsv` is the admitted retail `.rdata`/`.data` start-and-kind census.
  The shipped PDB seeds it, but normal builds do not rewrite it.
- `data_symbols.tsv` contains only retail referents without an exact or
  extent-backed PDB owner. Its columns are exactly `rva`, `name`, `size`,
  `type`, and `comment`. Every field is mandatory and every row is manually
  reviewed; a new unresolved referent is an authoritative-build failure.
- `pdb_data_extents.tsv` supplies reviewed sizes only for symbols whose start
  is already named by the retail PDB but whose CodeView record has no usable
  extent. It does not duplicate PDB names or introduce data symbols.
- `reloc_referents.tsv` uses Gruntz's symbol-plus-addend schema for exceptional
  references just outside a PDB-owned extent, such as one-past end pointers.

## cleanliness/

- `data-integrity-ratchet.tsv` holds maximum allowed projection debt. It remains
  the Gruntz-style ceiling table for the consumer-owned projection. The same
  `vostok data check --gate` invocation independently requires zero open
  function datum-use rows across every module in the current match ledger.
