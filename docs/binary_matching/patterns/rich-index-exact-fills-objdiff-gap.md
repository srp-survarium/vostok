# Rich-index exact evidence fills an objdiff score gap

## Signal

A function may be present by the same mangled name in both executable PDB
indexes while objdiff reports `fuzzy_pct IS NULL` because its COMDATs were
attributed to different source trees or units. A second form occurs when ICF
chooses a different canonical mangled name for the folded RVA on each side,
leaving the real target identity in `target_only`. In both cases the rich PDB
aliases can still identify the same demangled signature and source owner.

## Proof and handling

The rich-index producer normalizes local branch labels and relocation operands
to symbolic instruction text. Equal function size plus an identical non-empty
ordered stream of `(offset, instruction length, instruction text)` is therefore
strict function-scoped exact evidence. A stream is also exact when every
differing tuple has the same offset, length, and `call` or `jmp` opcode, and the
two printed operands resolve uniquely on their respective sides to called RVAs
whose PDB alias sets overlap. This proves that ICF selected different display
names for the same relocation identity; ambiguous operands or disjoint alias
sets are refused.

For a cross-name fold, additionally require an exact demangled signature, the
same source file, a unique unused base RVA, and preserve the PDB statement
classifier independently. It is safe to record 100%; it is not a fuzzy
approximation and does not turn a `QUANTITY` result into `MATCH`.

Never infer a score when either stream is empty, instruction geometry differs,
a non-control-transfer instruction differs, or the PDB cannot uniquely prove a
control-transfer alias. Those rows remain source work or a different
measurement problem.

The same-name pass originally recovered 48 Scaleform COMDAT/header functions.
The strict cross-name pass later recovered four more Scaleform identities whose
direct bodies were already exact while leaving genuinely different or absent
bodies uncredited. Alias-resolved control transfers later exposed another 101
exact callers hidden only by different ICF representative names.
