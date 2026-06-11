# ICF fold survivor attributed to a SIBLING header's unit: unit-level None over a byte-identical body
tags: cpp:member | topic:fold-icf topic:scoring-artifact
symptoms: unit pairing None, target symbol's line table names the sibling header, --view diff by NAME pairs clean
confidence: 8/10
variants: boost-bind-icf-sibling-fold.md, none-constant-pool-names.md, unit-own-symbol-vs-fold-rep.md

When two classes carry identical method bodies, /OPT:ICF keeps ONE copy whose unit/line
attribution is the SIBLING header, so the symbol never appears in this header's target
unit and objdiff reports `None` for the unit pairing. Verify by NAME instead:
`pdb_fetch --view diff --function <class>::method` pairs the base symbol against the
fold survivor. Write the real body in its own header; never leave `{}` because "no code
is attributed here".

```asm
; target ?execute@string_response@... rva 0x49490, line table = string_order.h:62-68
; (string_response::execute's real body lives in string_response.h - identical twins fold)
```
Evidence: string_response::execute diffs with ZERO divergent rows (frame 0x388 included) despite the unit-level None.
