# PDB source checksums

The target and base PDBs carry CodeView file-checksum records for every source
file represented in a compiland's line program.  The repository's pinned
`pdb_diff` exposes two views:

```bash
# Stable TSV: algorithm, digest, engine-relative path.
pdb_diff --target-pdb binaries/nix-store/survarium-game/survarium.pdb \
  --target-engine-path c:/survarium/sources \
  --all-files --list-checksums

# Compare every recorded source and header against the checkout.
pdb_diff --target-pdb binaries/nix-store/survarium-game/survarium.pdb \
  --target-engine-path c:/survarium/sources \
  --all-files --source-dir sources

# Compare the target and current-build PDB checksum tables directly.
pdb_diff --target-pdb binaries/nix-store/survarium-game/survarium.pdb \
  --target-engine-path c:/survarium/sources \
  --base-pdb binaries/Win32/survarium-dx11-win32-gold.pdb \
  --base-engine-path z:/home/sheep/projects/survarium/vostok/sources \
  --all-files
```

Without `--all-files`, `pdb_diff` retains its historical behavior of selecting
the first C/C++ implementation file represented by each compiland.  The full
mode walks the DBI file-checksum subsection directly, so included headers are
not inferred from functions or source paths.

## Line-ending normalization

The target build checksummed CRLF bytes, while this checkout and the base build
use LF.  `pdb_diff --source-dir` hashes the file as stored first, then retries a
lossless LF-to-CRLF conversion.  A stock Bullet file proves the normalization:

| input | MD5 for `btAxisSweep3.cpp` |
|---|---|
| target PDB | `c0856dc75541391fe69d3370d32fa87d` |
| checkout/base PDB (LF) | `f67d91430bab77fc0136732a7226c403` |
| checkout converted to CRLF | `c0856dc75541391fe69d3370d32fa87d` |

No BOM, whitespace, or final-newline normalization is performed.  A `MATCH`
therefore proves equality of the target compiler's input bytes after only the
repository-wide LF/CRLF storage conversion.

The initial full Bullet sweep contains 221 target checksum records: 192 match
and 29 differ.  That 29-file set is the source-recovery worklist; function
scores on the 192 byte-certain files are compiler or link context, not evidence
for editing those files.

## Pristine Bullet baseline

The development shell pins official Bullet commit
`ce62d7615ecb1faa876d11172c418244c7246b5c`, the first upstream revision marked
2.79.  Its `src` directory is available as `$BULLET_2_79_SOURCE` and retained by
the `binaries/nix-store/bullet-2.79-source` indirect GC root.

This is a byte-evidenced baseline, not a version-number inference.  After the
same LF-to-CRLF conversion, the upstream `btAxisSweep3.cpp`, `btDbvt.cpp`,
`btDbvt.h`, `btRigidBody.cpp`, and `btTransform.h` MD5s all equal their target
PDB records.  The remaining 29 files must therefore be classified against this
pinned tree as either pristine-source restoration or a target-specific delta.
