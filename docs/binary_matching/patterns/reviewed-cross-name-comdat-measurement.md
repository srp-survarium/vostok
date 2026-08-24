# Measure an asymmetric ICF alias without replacing the canonical representative

tags: cpp:comdat cpp:template | topic:fold-icf topic:pairing topic:measurement

## Symptom

The retail PDB contains a target template identity, and the base rich PDB contains
the same source specialization at a shared RVA, but its delinked COFF group is
named after another ICF representative. Renaming the canonical base group to the
target identity makes the missing body measurable, but also steals the existing
representative's pairing and creates false regressions.

## Evidence threshold

Use this only for a reviewed pair whose retail and base PDB records establish the
logical source identity and ownership. Verify the target statement/local shape and
its real caller cone first. A similar instruction sequence or convenient shared
RVA is not enough, and this mechanism never justifies changing retail spelling or
source structure.

## Measurement

Record the target identity and base rich-PDB representative in
`measured_fold_aliases.tsv`. The supplementary cross-unit generator resolves the
base PDB name through the effective PDB-to-COFF symbol map, copies that base object
to a temporary directory, and applies `llvm-objcopy --redefine-sym` only to the
copy. Objdiff then compares the original target object with the disposable renamed
base object.

The canonical delinked object and `report.json` remain unchanged, so the existing
ICF representative keeps its pairing. A recovered fuzzy score is supplementary
evidence for the retail identity. If the structurally different folded body cannot
align, leave it unscored and park only on the concrete PDB/caller/convention wall;
do not promote absence of a score to a match.

Evidence: `vostok/intrusive_list_inline.h` singleton audit, 2026-08-24.
