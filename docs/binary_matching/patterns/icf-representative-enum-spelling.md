# ICF representative and optional `enum` spelling can hide an exact template body

## Signal

A target-only template specialization can have an equal-size base body in the
same source unit, but the base PDB names a different specialization from the
same ICF group. The two demangled signatures may differ only because one PDB
prints the optional elaborated-type keyword `enum`. Otherwise-identical
instruction streams can also print different callees at an assertion or other
folded relocation.

## Proof and handling

Treat the optional `enum` keyword as PDB display noise only for pairing. Then
require the same source owner, equal body size, equal non-empty instruction
geometry, and equality of every instruction after resolving differing
`call`/`jmp` operands through the rich PDB alias sets. The candidate must be
unique. A base RVA already consumed by the ICF representative may support the
additional target identity only when a full-signature base alias at that RVA
has the same source owner.

This recovered exact `buffer_vector<pair<...>>::erase` and `push_back` bodies
whose target and base selected different ICF representatives. It also preserves
the representative's report identity instead of trading one paired row for a
new blocker.

Do not use this rule for a real type mismatch, an instruction or size mismatch,
an ambiguous alias set, or a body absent from the base. In particular, do not
add a reachability anchor merely to force the target spelling to become the
base representative.
