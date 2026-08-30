# `sema` - control-flow views over the base<->target pair

**STATUS: IMPLEMENTED (`scripts/vostok/sema/`, 2026-08-01; run it as
`python3 -m vostok sema` or `python3 -m vostok.sema`).**

`sema` is the layer vostok did not have: a **basic-block / branch** comparison of
our compiled function against the original one. It sits below `pdb_fetch --view
structure-diff` (source statements) and beside `pdb_fetch --view diff`
(instructions), and answers a question neither of them asks:

> Does our function have the same SHAPE - the same blocks, the same branch
> destinations - as the target's?

## Why - the blind spot, with evidence

`pdb_fetch --view diff` is an operand- and relocation-aware instruction diff
(objdiff-core 2.5.0). It is excellent at what it does, and it is **structurally
unable to show you a control-flow difference as a control-flow difference**:

1. Branch destinations print as raw addresses from two different address spaces.
   A retarget renders as
   `~ 0x2b: je short 0000029Eh    -> je short 000003C1h`
   - two numbers a reader cannot compare, cannot tell apart from a harmless
   upstream size drift, and cannot map to a block.
2. objdiff-core normalizes those addresses internally (`arg_eq`,
   `ObjInsArg::BranchDest`, `diff/code.rs:278`) by comparing
   `branch_to.ins_idx` - the destination's index in the ALIGNED instruction
   vector. That is the right idea, and it means a branch row is often marked
   **Equal**. `pdb_fetch` renders Equal rows with the base text only. So a
   correct-looking `je` row proves nothing about where either side jumps.
3. When the two sides are shaped differently, objdiff's Patience alignment over
   the opcode sequence produces an alignment that is locally plausible and
   globally wrong; every later row then reads as a `~`. The output is long,
   every line looks like a defect, and none of it says "the shape is wrong -
   stop reading instructions".

### Measured, 2026-08-01, on `campaign/render-carcass-rebuild`

Every paired render function below 100% that has at least one branch on either
side - **796 functions** - cross-tabulated by (a) does `sema` see a control-flow
difference, (b) does `pdb_fetch --view diff` flag any row whose instruction is a
branch:

| | objdiff flags a branch row | objdiff flags NO branch row |
|---|---:|---:|
| **sema: flow DIFFERS** | 360 | **2** |
| **sema: flow SAME** | **84** | 350 |

(Numbers from the run that first justified the tool. The `flow DIFFERS` row is
now smaller: the contraction rule below moved a chunk of it to `flow SAME`.)

Two readings, in descending order of importance:

1. **A majority have a provably IDENTICAL control-flow graph.** Their residual is
   operands, register allocation or scheduling - not shape. No existing view
   could say that, so the choice between "grind on the byte diff" and "the
   structure is wrong" was being made by feel.
2. **objdiff's branch rows are not usable as a control-flow signal in either
   direction.** 84 times it flags a branch on a function whose CFG is identical -
   `device::destroy_d3d` (89%) shows
   `~ 0x13: je short 00000037h -> je short 000000E8h`, two numbers from two
   address spaces, and the actual difference is a `push ecx`/`pop ecx` pair. And
   it is capable of flagging nothing at all: for
   `effect_options_descriptor::operator[]` (271 bytes, 83.15%) it flags **40 rows,
   not one of them a branch**. Whether a branch row appears carries no information
   about the flow.

The practical consequence is the one that costs sessions: a matcher runs the
first-look command, sees a wall of `~` operand rows, and either grinds on
register spellings that can never converge (the shape is wrong), or writes the
residual off as "LTCG/regalloc" (the shape is right, but nothing proved it).
`sema` turns both of those guesses into a one-line fact.

> `effect_options_descriptor::operator[]` was originally written up here as a
> *silent control-flow divergence*. It is not - see **Cost of getting this wrong**
> at the end of this file. Its CFG matches; the tool was miscounting blocks. The
> `--view diff` silence is real, the conclusion drawn from it was not.

## What it does

Both sides come from the same delinker and the same disassembler
(`pdb_fetch --view target` / `--view base` over `binaries/rich/{target,base}`),
so - unlike the two-disassembler setups this idea comes from - **almost no
instruction normalization is needed**. Exactly one thing is masked: the branch
operand. Everything else that differs textually is a real byte difference.

The CFG is built per side and every branch destination is renamed to a **block
index of that side**. That is the whole trick: `jcc B4 | fall B2` means the same
thing in both address spaces, so

- a uniform displacement shift (everything moved because an earlier statement
  grew) compares **EQUAL**, and
- a genuine retarget (a branch that lands on a different block) compares
  **DIFFERENT**.

Leaders are branch destinations **plus every post-branch instruction**. The
second rule is not optional: a jump table's dispatch arms carry no label, and
without it they get absorbed into the preceding block, erasing its terminator
and inventing a flow difference that is not there.

### Flow-free blocks are contracted first

That same leader rule manufactures blocks that carry no control flow at all. The
instruction after a `jcc` always starts a block - even when it is an alignment
`nop`, a `lea ecx,[ecx]` pad, a spill reload or a re-materialised zero. Such a
block has one predecessor (reached by that fall-through), one successor and no
branch: it contributes nothing to the shape, but because destinations are named
by INDEX, one appearing on only one side shifts every later name and the whole
comparison reads as a cascade of retargeted branches.

`contract()` splices them out before anything else looks at the graph. A block
goes only when **its exit is a bare `fall`**, **exactly one block reaches it and
reaches it by fall-through**, and merging would not leave a terminator with two
edges to one block. The fall-through requirement preserves real structure: an
`if`/`else` arm entered by the TAKEN edge of its guard is never contracted. And
because only branch-free blocks are ever removed, contraction cannot change the
branch COUNT - it can never hide a missing guard. The elided instructions are
prepended to the successor, where they physically sit, so the block listing
stays a correct linear disassembly. Every view reports how many blocks it
contracted per side - on stdout, except `dot`, which writes its notes to stderr
so the graphviz stream stays pipeable.

## Command surface

The navigation front of the HoMM2/Gruntz sema family is available against
Vostok's PDB-rich indexes as well as the CFG views:

    python3 -m vostok sema rva      <fn>             # address/source/match dossier
    python3 -m vostok sema xref     <fn> --callees   # direct callees (root-first work)
    python3 -m vostok sema xref     <fn>             # direct callers
    python3 -m vostok sema strings  <fn>             # referenced MSVC literals
    python3 -m vostok sema strings  --find <text>    # reverse literal lookup
    python3 -m vostok sema blocks   <fn>                 # target-side CFG, with bodies
    python3 -m vostok sema blocks   <fn> --base          # base-side CFG
    python3 -m vostok sema blocks   <fn> --lite          # one line per block
    python3 -m vostok sema blocks   <fn> --diff          # THE MAIN VIEW
    python3 -m vostok sema blocks   <fn> --diff --lite   # skeleton table only
    python3 -m vostok sema branches <fn> [--base]        # ordered branch sequence
    python3 -m vostok sema branches <fn> --diff          # branch sequence, diffed
    python3 -m vostok sema dot      <fn> [--base|--diff] # graphviz
    python3 -m vostok sema sweep --module render [--unit U] [--min-pct P] [--max N]

`rva`, `xref`, and `strings` were ported as native rich-index readers rather
than copying either project's PE/COFF assumptions. Direct calls and literal
symbols are taken from the same symbolized instruction records used by the CFG
view. MSVC literal names expose a readable prefix; long literals remain
compiler-truncated, so the command prints both the decoded hint and raw symbol.

Flags, exactly:

* `--base` picks the side for a one-sided view (`rva` has none - it always shows
  both). `blocks`/`branches`/`dot` ignore it under `--diff`, which shows both.
* `--lite` exists on `blocks` only. It used to be accepted and ignored by
  `branches` and `dot`; they now reject it.
* `xref --raw` prints one row per call site instead of one per callee/caller,
  in address order. Without it, callees are grouped by name with `xN` counts,
  so the offset column is the FIRST call site, not a sorted address list.
* `xref --callees` lists indirect calls under their register operand (`eax x8`).
  Those are call sites, not a function named `eax`; `pdb_fetch --view callees`
  omits them, which is why its count is lower.
* `rva` prints a `match` block only for a function the committed ledger carries,
  which is exactly the TARGET functions. A `BASE_ONLY` symbol gets its `base`
  record and nothing else - the absence of the block is the only signal that it
  is base-only.
* An empty result is an empty result: `xref` with no callers, `strings` with no
  literals and `strings --find` with no hits print their header and stop, with
  rc 0.

Other sema-family capabilities already have stronger Vostok-native owners:

| HoMM2/Gruntz view | Vostok owner |
|---|---|
| `disasm` / rich source lines | `pdb_fetch --view target|base|diff|structure|structure-diff` |
| `match` | `vostok ledger report --unit <tu>` or `--module <m> --per-unit` |
| `symbol`, `def`, `refs`, `hover` | `vostok tool clangd` with the same operation name |
| symbol/function map | `pdb_rich_query --list` and `vostok ledger list` |
| class hierarchy and layout | generated `binaries/structure/target/headers` |
| vtable order and slot use | target structure headers plus `pdb_fetch --view target` at a real vcall |

Gruntz needs separate `map`, `class`, and `vtable` inference because its retail
binary is stripped. Vostok's retail PDB already records the declarations and
ownership those commands try to infer. Treat the PDB-derived structure and real
vcall assembly as authoritative instead of importing stripped-binary guesses.
Keeping these commands as companions also avoids a second implementation of the
PDB and match-database logic.

`<fn>` is a mangled name, a demangled substring, or a hex RVA/VA on either side.
Ambiguous substrings are listed, never guessed at.

A bare hex is FOUR questions - target RVA, target VA, base RVA, base VA - and
sema has no side flag for an address. Every reading that hits a real function is
listed and the command stops; pick one by its mangled name, or ask `pdb_fetch`,
which takes `--target-index`/`--base-index` with `--rva`/`--va` and so has no
tie. About 3,000 of the ~42,000 addresses in the two indexes are ties.

Several index records at ONE address are an **ICF fold group**, not an ambiguity
(1042 target RVAs carry 2..8 names). They collapse to one function, with a
stderr line naming the fold and the symbol being read.

### Addresses: always labelled

Two address spaces are in play and they differ by the 0x10000 image base. Paste
an RVA where a VA belongs and you land 64 KB early, inside a different function,
with nothing to complain about it - so every absolute address printed anywhere in
the toolchain says which space it is in:

    rva=      what the rich indexes and the ledger STORE; `pdb_fetch --rva`
    va=       rva + image_base - what IDA shows and what carcass comments quote;
              `pdb_fetch --va`
    +0xNN     a function-RELATIVE offset (xref call sites, block starts, the
              `@1f` in a block header). Never an address.

`sema rva` prints BOTH forms of each side's address, so nothing has to be worked
out by hand:

    target  rva=0x6243e0  va=0x6343e0  size=0x80  stmts=11

`pdb_fetch` labels its own headers the same way and names its structure-diff
columns `t.va`/`b.va`; its flags (`--rva`, `--va`, `--address`, `--offset`) each
say which kind they take. A stale `nix develop` shell can still hold a
`pdb_fetch` that prints bare addresses - the numbers are the same either way.

The hex-ambiguity listing prints both forms too, because the READING that hit
(`as rva` / `as va`) is not the number the record sits at - it used to print the
RVA under an `va` label, which is the exact confusion the listing exists to
prevent:

    $ python3 -m vostok sema rva 0x6243e0
      target as rva: rva=0x6243e0 va=0x6343e0  vostok::render::get_format_block_size
          unsigned int vostok::render::get_format_block_size(DXGI_FORMAT)
      base as va: rva=0x6143e0 va=0x6243e0  ??0vfs_iterator@vfs@vostok@@QAE@ABV012@@Z
          vostok::vfs::vfs_iterator::vfs_iterator(vostok::vfs::vfs_iterator const&)
    sema: '0x6243e0' reads as 2 different functions (listed above): ...

`--diff` output, in order:

* a header naming both sides' file, RVA and byte size;
* what the graph LOST before comparison: blocks contracted (shape-preserving)
  and trailing blocks trimmed (**not** shape-preserving - see the `trim_tail`
  limit below), with a `!!` line when the trim dropped more than it kept;
* `flow SAME | DIFFERS` plus block counts;
* on DIFFERS, the **first skeleton divergence** - the first block whose branch
  KIND+direction disagrees, computed without absolute indices so one inserted
  block does not cascade - and the base block's **source statement**;
* the aligned block table (`--lite`) or per-block instruction diffs, each
  divergent block tagged with the source statement it came from;
* `BASE-ONLY` / `TARGET-ONLY` blocks called out separately from `DIFFERS`;
* `ORDER-ONLY` when the two CFGs turn out to be isomorphic (see below).

Markers in a block listing (`blocks <fn>`, with or without `--lite`):

    B7  @1ae   2i  [jcc B3^ | fall B8]   LOOP        (test..)
    B14 @455   7i  [ret]                 <== tail    (mov..)

`^` on a destination means a BACK edge (the destination's index is <= this
block's), and `LOOP` restates that for the line. `<== tail` marks a `ret` block
that several terminators name - the merged exit. Both are reading aids; no
verdict depends on either.

`branches --diff` adds the per-branch classification: `TOPOLOGY` (same mnemonic,
different destination block), `POLARITY` (inverted condition), `SIGNEDNESS` (a
signed/unsigned twin - nearly always a real source type bug), `OTHER`.

> **Take the verdict from `blocks --diff [--lite]`, not from `branches --diff`.**
> The block views align by CONTENT; `branches --diff` pairs branches by POSITION,
> which is only meaningful when both sides have the same number of blocks. When
> they do not it now says so, prints no per-row interpretation, and points back
> here - but the rows are still evidence, not a defect count. `branches --diff`
> is for READING a difference `blocks --diff` has already established.

`sweep` classifies a whole module in one pass:

| verdict | meaning | what to do |
|---|---|---|
| `IDENTICAL` | same blocks, same bodies | the residual is relocation/symbol identity - check the objdiff pairing, not the source |
| `FLOW-SAME` | same CFG, different block bodies | operands / regalloc / scheduling; **not** shape work |
| `ORDER-ONLY` | CFGs isomorphic, different block LAYOUT | one merged exit placed elsewhere - ONE fact, see below |
| `BRANCH-COUNT` | different number of branches | a guard we are missing, or an `if` one side folded |
| `BLOCK-COUNT` | equal branches, unequal blocks | an extra block contraction could not remove; positional branch pairing is meaningless here |
| `COND-FLIP` | equal blocks AND branches, a mnemonic differs | possibly an inverted condition or signed/unsigned twin - but the mnemonic lists are compared BY POSITION, so confirm with `blocks --diff --lite` before believing it (see below) |
| `TOPOLOGY` | same mnemonics, different destination block | the shape an instruction diff cannot show |
| `BLOCK-SPLIT` | same branch sequence, different block count | usually an unreachable/padding artifact |
| `TRIMMED` | the trailing trim dropped more blocks than it kept, so the graph is a PREFIX | **not a flow verdict** - read the function with `pdb_fetch --view target|base`; see the `trim_tail` limit |

### Which classes are worth a matcher's time (measured, batches B7 + B8)

Two full render/core sweeps have been worked by hand with the hit rate recorded.
Work the classes in this order:

| class | opened | real source bugs | verdict |
|---|---:|---:|---|
| `BRANCH-COUNT` | 23 | **8** | **work this first** - a missing/extra branch is a missing/extra guard, and neither contraction nor alignment can manufacture one |
| `TOPOLOGY` | 7 | 0 | **skip** - almost all uncontracted padding; the one survivor after contraction (`backend::flush`) turned out to be cross-jumping, see below |
| `COND-FLIP` | 2 | 0 | both were `branches --diff` positional-pairing artifacts; the class no longer fires on them at all |

**The ranking held on a second batch, but the yield falls as a subtree matures.**
B7 hit 5 of 9 on its first pass; B8 hit 3 of 14 on what was left. The 11 misses
were not tool errors - they were **core-header inline-vs-call walls**, where the
target inlines a callee (`intrusive_ptr::operator=`,
`single_threading_policy::increment/decrement`, `~textures_handler<N>`,
`detail::strcmp_s` behind `operator==`) that our base still calls, or the reverse
(the target *calls* a `math::max(u32,u32)` overload we do not declare, so we
inline where it does not). Those give large block deficits -
`effect_compiler::begin_pass` is 29 vs 77 - with **zero** caller-side work to do.
Read the BASE block first: if it ends in a `call` to a known small callee and the
target's extra blocks are that callee's body expanded in place, close the row.
Full recipe in `patterns/branch-count-row-triage.md`.

`BLOCK-COUNT` is the natural next queue once `BRANCH-COUNT` thins out.

B7's five `BRANCH-COUNT` hits: `backend::flush_rt` 49.7 -> 100
(`render_dirty_targets::any()` was `|` where the target has `||`),
`res_effect::apply` 62.2 -> 100, `res_render_output::select_resolution`
71.4 -> 100, `constant_data_predicate` -> 100, `store_constant<T>` x4 - in a
subtree six earlier batches had already been over.

B8's three: `res_render_output::present` (a missing
`DXGI_ERROR_DEVICE_REMOVED/_DEVICE_RESET/_DRIVER_INTERNAL_ERROR` ->
`device::on_device_removed()` arm), `shader_constant_buffer::update` (the target
uses `UpdateSubresource` under a `backend::ref().disabled_shader_constansts_set`
guard, not `Map`/`CopyMemory`/`Unmap` - which also un-walls
`constants_handler<0..2>::update_buffers`, since the real body is small enough to
inline and ours was not), and
`texture_options_binary_cooker::on_binary_config_loaded` (a stack
`fs_new::virtual_path_string` plus
`replace("resources/", "resources.sources/")`).

**The one surviving `TOPOLOGY` row is explained and closed.** `backend::flush` is
63 blocks / 35 branches on both sides with a single retargeted `jmp`; the target
**cross-jumped** the two `IASetVertexBuffers` call sites onto one shared
four-instruction indirect-call suffix and the base emitted both copies. A pure
optimizer decision - the arms even push different argument counts. Do not reopen
it.

`SIGNEDNESS` has no sample yet, but a signed/unsigned twin is a type bug by
construction, so treat it like `BRANCH-COUNT` when one appears.

### What the contraction fix did to those classes

Same 252 render/core functions, same artifacts, contraction off vs on:

| verdict | before | after |
|---|---:|---:|
| `FLOW-SAME` (not shape work) | 162 | **170** |
| `BRANCH-COUNT` | 75 | 75 |
| `BLOCK-COUNT` | - | 5 |
| `TOPOLOGY` | **12** | **1** |
| `COND-FLIP` | 2 | 0 |
| `ORDER-ONLY` | 1 | 1 |

Eleven of the twelve `TOPOLOGY` rows were padding. Eight became `FLOW-SAME` -
`effect_options_descriptor::operator[]`, `constants_handler<0..2>::assign`,
`effect_manager::~effect_manager`, `res_texture_list::compare` x2,
`effect_constant_storage::is_equal` - i.e. eight functions a matcher would have
opened looking for a control-flow bug that was not there. Three more, plus both
`COND-FLIP` rows, moved to the honest `BLOCK-COUNT` label. `BRANCH-COUNT` is
unchanged at 75, which is the invariant the contraction rule guarantees.

## `ORDER-ONLY` - the verdict that collapses five defects into one

`jcc X / fall Y` and `jcc' Y / fall X` are the same edge pair written two ways,
so moving ONE block in the linear order flips the polarity of every branch that
reaches it AND retargets every branch that jumped past it. Positionally, that
one move reads as several independent defects.

`sema` therefore canonicalises each block's exit (pick one of the inverse pair,
swap the successors to match) and searches for a bijection between the two CFGs.
If one exists, the graphs are **isomorphic** and the only difference is layout -
reported as `ORDER-ONLY`, with the moved blocks named.

Worked example, `compare(shader_constant const&, shader_constant const&)` - 100
bytes on both sides, 15 blocks on both sides. `branches --diff` alone reports
**three retargeted branches and two inverted conditions**; the isomorphism check
reports the truth:

    ORDER-ONLY: ... moved: base B4->target B6, base B5->target B4, base B6->target B5

Both sides merge the two `return 1;` statements into one block. Ours keeps the
FIRST copy (right after the first `>` test); the target keeps a later position
(after the second `>` test). Nothing about the conditions is wrong. Chasing the
"inverted" `jbe`/`ja` here would have produced a source change that is less
faithful than what is already written.

## The hint

Whenever a `--diff` view comes out **clean** and the ledger says the function is
**below 100%**, `sema` prints where the remaining signal is instead of leaving
the reader to conclude "regalloc, probably". This is the one behaviour worth
copying wholesale from the tool this is modelled on: the clean-view case is
exactly the case a human under-investigates.

## rc convention

    0   answered YES
    1   answered NO
    2   error         (no such function, an ambiguous selector, only one side
                       present, pdb_fetch missing)

`1` is an ANSWER, not a failure - do not treat a non-zero rc as a broken run.
**What the answer is about differs per verb, so do not read rc 1 as "the flow
differs":**

| verb | rc 0 | rc 1 |
|---|---|---|
| `blocks --diff [--lite]` | same flow AND every aligned block byte-identical | flow differs **or any aligned block body differs** |
| `branches --diff` | branch counts, mnemonics and destination blocks all agree | any of those differ |
| `rva`, `xref`, `strings`, `dot`, `sweep` | always 0 | never |

`blocks --diff` therefore returns 1 for essentially every function below 100%,
including ones it has just printed `flow SAME` for:

    $ python3 -m vostok sema blocks '?create_d3d@device@render@vostok@@AAEXXZ' --diff --lite
    [block diff: base 48 blocks vs target 48 blocks; flow SAME]
    ...
    [this function is 99.02%, not 100, yet base and target have the SAME
     control-flow graph and the SAME branch destinations...]
    $ echo $?
    1

Take the flow verdict from the printed `flow SAME | DIFFERS` line, not from rc.
`branches --diff` on that same function returns 0.

## Known limits

* **One side only.** A `TARGET_ONLY` (nothing compiled yet) or `BASE_ONLY`
  function has nothing to diff; `blocks <fn>` still shows the side that exists.
* **Stale artifacts read as divergence.** `sema` reads
  `binaries/rich/{base,target}/index.jsonl`, which `vostok build` regenerates.
  A base index older than your source is the single most likely cause of a
  surprising verdict - `ls -la binaries/rich/base/index.jsonl` before believing
  one.
* **`branches --diff` pairs branches BY POSITION.** `blocks --diff [--lite]`
  aligns by CONTENT and is the view to take a verdict from. When the two sides
  have different block counts, `branches --diff` now says so and drops its
  per-row interpretation, but the rows are still shifted evidence, not a defect
  count. This is what produced batch B7's two phantom `COND-FLIP` rows
  (`create_texture`, `create_texture3d`): the mnemonics agree, the pairing did
  not. (Re-measured 2026-08-16: both have since been reworked and now classify
  as `BRANCH-COUNT`, and the 2d one is spelled `create_texture2d`.)
* **`sweep`'s `COND-FLIP` is computed positionally too**, from
  `[mnemonics of base branches] != [mnemonics of target branches]` - the same
  index-wise pairing this file warns about for `branches --diff`. Equal branch
  TOTALS do not mean the branches correspond: `stage_lights::stage_lights` is
  193-vs-193 blocks and 123-vs-123 branches and classifies `COND-FLIP`, but
  `blocks --diff --lite` shows the real fact - base B81 is `fall B82` where the
  target is `jcc B83 | fall B82`, i.e. a guard in a different place, not an
  inverted condition. Confirm every `COND-FLIP` with `blocks --diff --lite`
  before reading it as a source bug; the measured hit rate for the class is
  0 of 2.
* **Block alignment is content-based** (`difflib` over whole-block text). When
  both sides are heavily rewritten the pairing is a guess; the `flow` verdict
  and the first-skeleton-divergence line stay meaningful, the per-block bodies
  become advisory. It also degenerates on a function built from REPEATED
  near-identical groups, where difflib emits one long `replace` opcode and the
  rows inside it are paired index-wise. `device::on_device_removed` (base 67
  blocks, target 66, six near-identical guard groups offset by one) prints ~50
  consecutive `!!` rows; only the header lines are evidence -
  `[skeleton diverges at B9: base [jcc B66 | fall B10] vs target [jmp B60]]`
  and `[first true skeleton divergence at B9]` are correct, the rows under them
  are not a defect list.
* **Contraction merges into the successor, so a back-edge into a contracted
  block's successor now points one block earlier** in the listing. The edge set
  is unchanged and both sides contract by the same rule, so no verdict depends
  on it; only the arrow's printed landing offset is loose.
* **Jump tables are opaque.** A computed `jmp` yields `jmp <ext>`: the tool
  knows a branch happened, not where it went. Two switch statements with
  different case counts therefore compare on their range checks and arm blocks,
  not on the table itself.
* **A switch's jump TABLE sits in `.text` right after the function** and is
  covered by the delinker's symbol size, so the disassembler decodes the table
  bytes into plausible instructions (`mov dl,0CAh / jns ...`). `trim_tail` pops
  TRAILING blocks that are unreachable from the entry and do not branch BACKWARD
  into the kept prefix. `register_samplers` goes from a bogus 18-vs-17-block
  verdict to 14-vs-14 (13-vs-13 after contraction), isolating the one real
  difference (we tail-`jmp` the last call, the target `call`s and returns).
* **`trim_tail` can eat the whole function, and then the verdict is about a
  prefix.** Its edge model gives a computed or tail `jmp` no successors, so any
  such jump makes every later block unreachable and the pop runs back to the
  entry block. `renderer::recreate_stage` - 2344 bytes, 98 statements, a 24-arm
  stage dispatcher whose target disassembly holds 765 instructions, 25 branches
  and 48 `ret`s - reduces to ONE block, and the diff prints
  `base 1 blocks vs target 1 blocks; flow SAME`. `sema branches` agrees: "1
  branch(es), 0 ret(s), 1 block(s)". Over the 792 paired non-100% render
  functions, 22 lose blocks to the trim and 3 collapse a >4-block side to <=2
  (`model_factory::create_render_surface` 30->1 / 23->1,
  `speedtree_tree::set_material_effects` 22->1 / 19->1,
  `mesh_type_to_vertex_input_type` 12->1 / 10->1); one more,
  `stage_lights::render_speedtree_lighting`, loses base 81->1 against target
  87->72 and so invents 71 TARGET-ONLY blocks. **Every view now says how many
  trailing blocks it trimmed per side and flags the case where it dropped more
  than it kept, and the `< 100%` hint is suppressed there** - but the underlying
  edge model is still wrong. When you see that `!!` line, read the function with
  `pdb_fetch --view target|base`, not with sema.

  The candidate fix, measured on two functions only: seed reachability with the
  disassembler's LABEL TARGETS as well as the entry block. Jump-table DATA
  carries no label definition, so the phantom tail `trim_tail` exists for is
  still trimmed. That gives `recreate_stage` 73 blocks on both sides (from 1)
  and `register_samplers` 16-vs-16 (from 13-vs-13 - more blocks, still equal, so
  the one real difference stays isolated). It needs a whole-module before/after
  sweep before it goes in, because it moves the block count of every
  switch-shaped function.
* **`branches --diff` aligns branches POSITIONALLY.** One extra or missing
  branch shifts every later pair, so a single real difference prints as a run of
  `POLARITY` / `TOPOLOGY` rows. That is how `sweep`'s two render/core
  `COND-FLIP` rows (`create_texture`, `create_texture3d`) were produced, and
  both are false alarms - the mnemonics agree once the extra pad block is
  removed. `blocks --diff [--lite]` uses content-based alignment and marks these
  `~=` ("same kind/shifted target"), so **prefer `blocks --diff` over
  `branches --diff` for the verdict** and use `branches` only to read a
  confirmed difference.
* **`sweep`'s candidate set is `paired AND fuzzy_pct < 100`.** Pairs objdiff
  could not score (`fuzzy_pct IS NULL`) are not candidates - 560 globally, 134
  in render - and are counted in the footer rather than being listed.
* **`blocks --diff` non-`--lite` calls one-sided blocks out separately;
  `--lite` shows them as a `-` row marked `!!`.**

## Cost of getting this wrong - the contraction bug (found by batch B7, fixed)

Between the first version of this tool and the contraction rule above, `sema`
did not remove flow-free blocks, and the write-up in this file used
`effect_options_descriptor::operator[]` as its flagship example of a control-flow
divergence hidden from the byte diff. **That reading was wrong.** The function is
271 bytes with 16-of-16 statements and identical line deltas on both sides; the
"missing early-out at B2" was our own next block, displaced by a one-byte `nop`
pad. The real residual is a CSE'd zero: the target keeps 0 in `edx` and spells
its null tests `cmp reg,edx` where we emit `test reg,reg`.

The blind spot the tool exists to close is still real (see the evidence table
above - `pdb_fetch --view diff` flags 40 rows for that function and not one is a
branch). What was wrong was the direction of the finding. Two lessons, both now
built in:

1. **A shape tool must canonicalise before it compares.** Naming destinations by
   index is only sound once the node set is canonical; otherwise the tool
   converts one byte of padding into a dozen "retargeted branches" and reads as
   authoritative while doing it.
2. **When a first-skeleton-divergence points at a block with no branch, suspect
   the tool.** That is now impossible by construction, but it remains the right
   instinct for whatever the next canonicalisation gap turns out to be.
* **It says nothing about statements or locals.** Structure verdicts stay
  `pdb_fetch --view structure-diff`; `sema` is strictly about shape below the
  statement level.
* **`sema` does not read `binaries/match.db`.** It never did anything the two
  rich indexes and the committed ledger cannot answer, and reading a derived
  cache meant `sema` could not answer at all on a tree that had never run a
  derivation. The split now is: `binaries/rich/{target,base}/index.jsonl` owns
  every BUILD fact (address, size, statements, owning file, both spellings of
  the name, and - through `vostok.sema.pairing` - which base function a target
  function IS); `config/match_state.tsv` owns the CAMPAIGN's
  memory (`cur`/`max`, structure class, attempts, status, park note, module and
  TU ownership). `sweep` takes its scope and percentages from the ledger and its
  RVA pair from the pairing; `rva` reads both; every CFG verdict comes from
  `binaries/rich` as before.
* **The pairing is recomputed, not cached.** `vostok.sema.pairing` runs the same
  passes as `vostok.derive.pairing` over the same helpers and reproduces all
  18,791 pairs at the same two RVAs (measured 2026-08-16: 0 disagreements, 0
  missing, 0 extra). ~600 of them exist only because a spelling gap is
  reconciled - the retail PDB writes `vostok::render::`dynamic initializer for
  's_cc''` where ours writes `` `dynamic initializer for 'vostok::render::s_cc'' ``
  - which is why naming one of those by its mangled name used to need the cache.
  It costs one pass over each index (~3 s) and is LAZY: `rva`, `blocks`,
  `branches` and `dot` resolve by name and never build it; `sweep` and
  `diff tu-order` always do. The two implementations differ only in where the
  "objdiff scored this" evidence comes from - `report.json` in `derive`, the
  ledger here - and are meant to converge on one, called with that evidence as
  an argument. Until they do, a change to a pass belongs in both.
* **The ledger stores four decimals.** `sweep` prints three, so a percentage can
  land 0.001 away from the raw `report.json` figure (175 of 18,791 rows). No row
  ever crosses the `< 100` line, so the candidate list is unaffected.
* **`rva`'s `stmts=` is not `pdb_fetch --view structure`'s count.** `rva`
  prints `len(record.statements)` from the rich index (and the same number under
  `statements=t:b`); `pdb_fetch` counts BODY statements
  and drops the opening and closing brace records, so it reports two fewer.
  `stage_postprocess::execute` is `stmts=201` / `statements=201:200` in sema
  and `199 statements` / `198` in `pdb_fetch`. Neither is wrong; they count
  different things.
