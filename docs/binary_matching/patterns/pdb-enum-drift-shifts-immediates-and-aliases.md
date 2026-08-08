# PDB enum drift shifts immediates and can alias generated switch cases

## Signal

Several otherwise identical optimized functions differ only in enum-derived
immediates.  A typical cluster has every mask and range base shifted by the
same power of two, while control flow, size, and statement structure already
match.

## Check

Compare the active source enum with the target PDB enum dump before editing
the function bodies.  Imported SDK headers may contain a later enumerator
inserted in the middle of a bit-field enum, or a later generated shader corpus
with wider base ranges.  The resulting caller assembly is faithfully compiled
from the wrong constants.

Restore the target enum as one shared-header batch.  If an imported generated
switch then reports duplicate case values, enumerate the collisions.  Target
PDB aliases can make later SDK permutation names indistinguishable; remove
only the later generated case arms that collide, and keep the target enum
values globally consistent.

## Scaleform example

Survarium's `PrimitiveFillFlags` PDB enum has no `FF_Invert` or
`FF_BlendMask`, so `FF_AlphaWrite` through `FF_LeaveOpen` are half the values
in the imported later SDK.  Restoring the PDB values fixed the mask immediates
in both `StaticShaderForFill` overloads.  The target D3D1x `ShaderType` enum
similarly uses smaller base ranges; restoring it exposed duplicate `*CxformAc*`
case labels only in a base-only helper from the unavailable later generated
shader corpus.

## Rule

Do not patch the immediates in each caller.  Treat a repeated constant shift as
shared enum/version evidence, verify it against the target PDB, and rebuild all
consumers together.
