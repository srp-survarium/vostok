<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Generated special members

MSVC's PDB class records distinguish compiler-generated methods directly. The
method property bit `0x100` is rendered by `pdb_topology` as
`compiler-generated`; it is not inferred from disassembly, line zero, or a
missing procedure. This matters because an implicit constructor or destructor
may be emitted, folded, inlined, or absent while retaining the same type-record
property.

Use the two-sided class comparison as the verdict:

```sh
pdb_topology \
  --target-pdb "$SURVARIUM_BIN/survarium.pdb" \
  --base-pdb binaries/Win32/survarium-dx11-win32-gold.pdb \
  --classes --json
```

Inspect `method-qualifiers` differences where exactly one side contains
`compiler-generated`. Compare semantic variant sets, not a single convenient
record: duplicated PDB class records can describe different instantiations or
compilation contexts.

The source representation is ordinary C++03:

- if retail marks a special member compiler-generated, omit its declaration
  and definition;
- if retail marks it user-declared, reproduce the declaration, access, and
  declaration order, and add a body only when the target emits one;
- do not spell `= default` or introduce a matching macro;
- ignore scalar/vector deleting destructors as source declarations: they are
  compiler helpers, although their flags can expose a separate vtable issue.

The September 2026 whole-PDB audit found 107 retail-generated special-member
records represented by 65 explicit source declarations, plus two copy
constructors that retail declared but the reconstruction omitted. Removing the
explicit declarations also recovered one behavioral detail:
`render::effect_manager::effect_holder_struct` leaves its two pointer members
uninitialized in retail instead of explicitly zeroing them.

After rebuilding, all 28,933 compared classes agree on whether their special
members are compiler-generated. Both restored copy-constructor classes also
match access and declaration order. The remaining five `method-qualifiers`
differences are unrelated Bullet/Scaleform virtual-method or vtable flags; the
two destructor-helper rows among them agree on generatedness.
