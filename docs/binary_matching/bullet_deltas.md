# Bullet source deltas

The target PDB records 221 Bullet source files.  Source identity is checked with
`pdb_diff --all-files --source-dir sources`, which accepts either the stored LF
bytes or their lossless CRLF conversion.  The pristine comparison tree is the
pinned official Bullet 2.79 commit
`ce62d7615ecb1faa876d11172c418244c7246b5c` exposed as
`$BULLET_2_79_SOURCE`.

The initial sweep found 192 exact files and 29 mismatches.  Each row below is
added only after the checkout lands on the target PDB MD5.  The remaining
mismatches are target-specific recovery work; a function score cannot override
the file-level byte evidence.

| file | target MD5 | recovered hunk and evidence |
|---|---|---|
| `BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp` | `f229827edf6833f9aee1b329f0e71ef1` | Restored pristine 2.79: removed the local `BT_USE_SSE` compensator and restored the upstream final newline. |
| `LinearMath/btScalar.h` | `0a6585b3ab94a861314376e9de21a214` | Restored pristine 2.79: the MSVC 2005+ branch defines `BT_USE_SSE`; the prior commented definition was not target source. |
| `LinearMath/btVector3.h` | `c694a4b7c8bb947f0834a701a76f8489` | Restored pristine 2.79 whitespace only; executable declarations and expressions were unchanged. |

Current checksum state: 195 exact, 26 mismatching.
