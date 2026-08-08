# Rich-index exact evidence fills an objdiff score gap

## Signal

A function is present by the same mangled name in both executable PDB indexes,
but objdiff reports `fuzzy_pct IS NULL` because its COMDATs were attributed to
different source trees or units. `pdb_fetch --view diff` nevertheless reports
100% and shows identical instructions.

## Proof and handling

The rich-index producer normalizes local branch labels and relocation operands
to symbolic instruction text. Equal function size plus an identical non-empty
ordered stream of `(offset, instruction length, instruction text)` is therefore
strict function-scoped exact evidence. It is safe to record 100%; it is not a
fuzzy approximation.

Never infer a score when either stream is empty or any tuple differs. Those
rows remain source work or a different measurement problem.

This recovered 48 Scaleform COMDAT/header functions whose direct bodies were
already exact while leaving its eight genuinely different NULL-score rows
uncredited.
