# nvim plugin interface (agreed design, pre-implementation)

Source-buffer navigation into the match data: from the C++ you are editing to
the asm/structure/diff of the function (or single statement) under the cursor.
Interface agreed 2026-06-11 (sushi); expect revisions once it sees real use.

## Command

One entry point, two arguments, tab-completable:

    :Vostok {base|target|diff} {stmt|asm|structure}

|            | `stmt` (one statement)            | `asm` (function, rich)  | `structure`       |
|------------|-----------------------------------|-------------------------|-------------------|
| **base**   | asm of the statement at cursor    | rich asm                | statement table   |
| **target** | the PAIRED statement's asm; says "no paired statement" when unpaired | rich asm | statement table |
| **diff**   | asm diff windowed to the statement at cursor | asm diff     | structure diff    |

Buffer-local keymaps (C++ buffers + plugin views): `<leader>v` then a
two-letter chord - side, then view:

    bs/ba/bt   base   stmt/asm/structure     ("t" = sTructure)
    ts/ta/tt   target stmt/asm/structure
    ds/da/dt   diff   stmt/asm/structure

## Cursor resolution

Function at cursor, in order:

1. cursor word matches `0x[0-9a-f]+` -> that address picks the function (and
   statement); side disambiguation below.
2. otherwise buffer file + line -> function span in the BASE rich index
   (PDB line tables; works in carcass stubs, no clangd dependency). The
   target side resolves through the symbol pairing (fully-qualified name /
   known target RVA), so `:Vostok target asm` works with the cursor merely
   inside the base source.

Statement at cursor: the base statement whose source-line span covers the
cursor line (`--view structure`). `target stmt` maps through the
structure-diff pairing; unpaired rows report instead of guessing.

Address side disambiguation: inside plugin views the rendering column decides
(`b.va` is base, `t.va` is target). Both are VAs - pdb_fetch names its columns
for the space they are in, and its `; ` headers print `va=` and `rva=` side by
side. In source buffers, bare `0x...` in carcass comments (`<0x672482>|...`)
are target VAs by repo convention.

## Addresses are links

In EVERY plugin-rendered view (structure tables, diff tables, rich asm):

- `<CR>` (or `ga`) on any address -> that statement's asm, on the address's
  own side. Recurses (statement asm -> callee -> ...); small view stack for
  `<C-o>`-style back; `q` closes.
- `gd` on a paired diff-table row -> `diff stmt` for that pair.

## Rendering

- `stmt` views: floating window (a peek, transient).
- `asm` / `structure` / `diff` views: reusable scratch split, buffer names
  `vostok://<side>-<view>/<symbol>`, asm syntax highlighting, one buffer per
  view kind.
- Every view's header line: symbol, side, VA, current fuzzy % (report.json).

## Data layer

Every command is a `pdb_fetch` invocation over `binaries/rich/{base,target}`
(+ objdiff dirs for the operand-aware diff) - no plugin-side state, nothing to
go stale; target lookups key on the VA (the one thing that never moves).
Already available: `--view base|target|structure|diff|structure-diff`,
`--address 0x<va>` / `--offset` / statement-index selectors, `--rva`.

Known L0 gap (close before implementing): a MACHINE-READABLE statement
pairing (base addr <-> target addr). The structure-diff computes it
internally but only renders a table; the plugin needs it queryable.
