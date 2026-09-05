<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Generated headers hardcode public base access

Resolved source question from PR 571: the six flagged classes have **private**
noncopyable inheritance in both actual PDBs. The current source is correct.
The generated target header's `public` spelling is a renderer defect, not a
retail visibility constraint or a topology-comparison blind spot for these rows.

## Raw records

Every row below starts its field list with `LF_BCLASS` (leaf 0x1400) followed
by attribute word **0x0001**. The low two `CV_access_t` bits are 1=private,
2=protected, 3=public. Thus the relevant entry prefix is `00 14 01 00`, not
`00 14 03 00`. All six base offsets are zero.

| Class (`survarium::`) | Target class / field-list TI | Frozen base class / field-list TI | Base |
|---|---|---|---|
| `client_player_history_item` | 0xaacf / 0xaace | 0x910b / 0x910a | Boost noncopyable |
| `profile_player_character` | 0x189a3 / 0x189a2 | 0x13ec7 / 0x13ec6 | Boost noncopyable |
| `bullet_manager::bullet_functor_mt_allocator` | 0xbb14 / 0xbb13 | 0x3e3e9 / 0x3e3e8 | Boost noncopyable |
| `animation_analyzer` | 0x3d04c / 0x3d04b | 0x49460 / 0x4945f | Core noncopyable |
| `ik_processor` | 0x6e81 / 0x6e80 | 0xa5ea / 0xa5e9 | Core noncopyable |
| `legs_ik_drawer` | 0x40478 / 0x40477 | 0x4bbfe / 0x4bbfd | Core noncopyable |

Target Boost/core base TIs are 0x13eb/0x12a5; frozen base equivalents are
0x216f/0x24a2. Their raw records name `boost::noncopyable_::noncopyable` and
`vostok::core::noncopyable`, respectively. This checks base identity as well
as attributes; the result is not inferred from an arbitrary byte search.

Class TIs were obtained from the focused topology JSON variant records.
A read-only MSF/TPI diagnostic independently followed each class's field-list
reference and checked both the leaf tag and access bytes. It validated the
whole directory consumption, TPI record bounds and record count:

- Retail: block size 1024, 2410 streams, directory 402104 bytes, directory-map
  block 98741; TPI range 0x1000..0x71652.
- Frozen candidate: 2163 streams; TPI range 0x1000..0x70a20.

LLVM 21.1.8's `llvm-pdbutil dump --types --type-index=...` rejects this retail
MSF with `Too many directory blocks`. That diagnostic is a reader limitation,
not evidence that the PDB is invalid. Retail needs 393 directory blocks and a
block-number map spanning more than one 1024-byte block. The bounded read used
that map and stream 2; it did not modify/repack the PDB or compile a tool.

## Pinpointed cause and source patch

The exact flake input is parser commit
`6262ce150b12729b865a7eca6d82ad563256ba20`. Its `src/gen_headers.rs`:

- `BaseClass` at line 251 retains type and offset, but discards access.
- The `TypeData::BaseClass` and `VirtualBaseClass` arms at lines 683/694 do
  not copy `data.attributes.access()`.
- The header renderer at line 1182 literally writes
  `write!(f, " {prefix} public {}", base.type_name)?;`.

In contrast, `src/bin/pdb_topology.rs` at lines 5535/5547 copies each base's
`attributes.access()` into its comparison entry. Its identical verdict for
these private/private pairs is correct.

`patches/vostok-pdb-parser/base-access.patch`, wired into the existing flake
package, preserves access on both base record kinds and prints the existing
access keyword. Unspecified access falls back to the enclosing class/struct
default instead of inventing public inheritance. No engine inheritance is
changed, no topology normalization is weakened, and no generated tree is
hand-edited to conceal the defect.

Validation so far: patch dry-run against the exact pinned source with
`--fuzz=0`; direct raw target/base evidence for all six rows; static inspection
of the existing `access_keyword`/`default_access` helpers and topology reader.
**No Rust/C++ build or generator execution.** The patched generator still
needs a later authorized build and focused output checks for private,
protected, public and unspecified bases. Existing generated headers remain
stale until then. This batch does not audit indirect virtual-base emission or
claim engine-wide access coverage from six examples.

The five review-table questions covering these six source classes are closed;
their obsolete source TODOs are removed. Other algorithm/ownership/source-form
questions in the same headers remain open.
