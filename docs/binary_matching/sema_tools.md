# `sema` - control-flow views over the base<->target pair

**STATUS: IMPLEMENTED (`scripts/sema.py`, 2026-08-01).**

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
stays a correct linear disassembly. Every `--diff` reports how many blocks it
contracted per side.

## Command surface

    python3 scripts/sema.py blocks   <fn>                 # target-side CFG, with bodies
    python3 scripts/sema.py blocks   <fn> --base          # base-side CFG
    python3 scripts/sema.py blocks   <fn> --lite          # one line per block
    python3 scripts/sema.py blocks   <fn> --diff          # THE MAIN VIEW
    python3 scripts/sema.py blocks   <fn> --diff --lite   # skeleton table only
    python3 scripts/sema.py branches <fn> [--base]        # ordered branch sequence
    python3 scripts/sema.py branches <fn> --diff          # branch sequence, diffed
    python3 scripts/sema.py dot      <fn> [--base|--diff] # graphviz
    python3 scripts/sema.py sweep --module render [--unit U] [--min-pct P] [--max N]

`<fn>` is a mangled name, a demangled substring, or a hex RVA/VA on either side.
Ambiguous substrings are listed, never guessed at.

`--diff` output, in order:

* a header naming both sides' file, RVA and byte size;
* `flow SAME | DIFFERS` plus block counts;
* on DIFFERS, the **first skeleton divergence** - the first block whose branch
  KIND+direction disagrees, computed without absolute indices so one inserted
  block does not cascade - and the base block's **source statement**;
* the aligned block table (`--lite`) or per-block instruction diffs, each
  divergent block tagged with the source statement it came from;
* `BASE-ONLY` / `TARGET-ONLY` blocks called out separately from `DIFFERS`;
* `ORDER-ONLY` when the two CFGs turn out to be isomorphic (see below).

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
| `COND-FLIP` | equal blocks AND branches, a mnemonic differs | inverted condition or signed/unsigned twin - a real source bug |
| `TOPOLOGY` | same mnemonics, different destination block | the shape an instruction diff cannot show |
| `BLOCK-SPLIT` | same branch sequence, different block count | usually an unreachable/padding artifact |

### Which classes are worth a matcher's time (measured, batch B7)

Batch B7 worked one full render/core sweep by hand and recorded the hit rate.
Work the classes in this order:

| class | opened | real source bugs | verdict |
|---|---:|---:|---|
| `BRANCH-COUNT` | 9 | **5** | **work this first** - a missing/extra branch is a missing/extra guard, and neither contraction nor alignment can manufacture one |
| `TOPOLOGY` | 6 | 0 | **skip** - it was almost all uncontracted padding; the class is now 12x smaller (see below), so the survivors deserve a look, but not before `BRANCH-COUNT` |
| `COND-FLIP` | 2 | 0 | both were `branches --diff` positional-pairing artifacts; the class no longer fires on them at all |

The five `BRANCH-COUNT` hits: `backend::flush_rt` 49.7 -> 100
(`render_dirty_targets::any()` was `|` where the target has `||`),
`res_effect::apply` 62.2 -> 100, `res_render_output::select_resolution`
71.4 -> 100, `constant_data_predicate` -> 100, `store_constant<T>` x4 - in a
subtree six earlier batches had already been over.

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

Whenever a `--diff` view comes out **clean** and `match.db` says the function is
**below 100%**, `sema` prints where the remaining signal is instead of leaving
the reader to conclude "regalloc, probably". This is the one behaviour worth
copying wholesale from the tool this is modelled on: the clean-view case is
exactly the case a human under-investigates.

## rc convention

    0   answered YES  (flow same / branches agree)
    1   answered NO   (flow or branch destinations differ)
    2   error         (no such function, only one side present, pdb_fetch missing)

`1` is an ANSWER, not a failure - do not treat a non-zero rc as a broken run.

## Known limits

* **One side only.** A `TARGET_ONLY` (nothing compiled yet) or `BASE_ONLY`
  function has nothing to diff; `blocks <fn>` still shows the side that exists.
* **Stale artifacts read as divergence.** `sema` reads
  `binaries/rich/{base,target}/index.jsonl`, which `rebuild.py` regenerates.
  A base index older than your source is the single most likely cause of a
  surprising verdict - `ls -la binaries/rich/base/index.jsonl` before believing
  one.
* **`branches --diff` pairs branches BY POSITION.** `blocks --diff [--lite]`
  aligns by CONTENT and is the view to take a verdict from. When the two sides
  have different block counts, `branches --diff` now says so and drops its
  per-row interpretation, but the rows are still shifted evidence, not a defect
  count. This is what produced batch B7's two phantom `COND-FLIP` rows
  (`create_texture`, `create_texture3d`): the mnemonics agree, the pairing did
  not. Those two now classify as `BLOCK-COUNT`.
* **Block alignment is content-based** (`difflib` over whole-block text). When
  both sides are heavily rewritten the pairing is a guess; the `flow` verdict
  and the first-skeleton-divergence line stay meaningful, the per-block bodies
  become advisory.
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
  bytes into plausible instructions (`mov dl,0CAh / jns ...`). `sema` drops
  trailing blocks that are unreachable from the entry and do not branch back
  into the kept prefix - which removes the phantom tail but keeps jump-table
  arms, since those DO jump forward to the merge block. `register_samplers`
  went from a bogus 17-vs-18-block verdict to a clean 13-vs-13 that isolates
  the one real difference (we tail-`jmp` the last call, the target `call`s and
  returns).
* **Degenerate fall-through blocks are NOT contracted before the isomorphism
  check** (found by batch B7). The "every post-branch instruction is a leader"
  rule manufactures single-instruction blocks with one predecessor, one
  successor and no terminator - a `nop`/`lea ecx,[ecx]` alignment pad, a
  spill reload, a re-materialised zero. If such a block sits at a DIFFERENT
  place on the two sides, the CFGs are still isomorphic after contracting
  single-pred/single-succ fall-through chains, but `sema` reports
  `flow DIFFERS`, points its first-skeleton-divergence line at the pad, and -
  because destinations are named by block INDEX - reports every later branch as
  retargeted. Three render/core rows were exactly this and cost real time:
  `effect_options_descriptor::operator[]` (271 bytes and 23 blocks on BOTH
  sides; the "missing early-out" at B2 is our base's B3, displaced by a 1-byte
  `nop`), `resource_manager::create_texture` (a `jmp`+pad vs a fall-through+pad),
  `res_texture_list::compare` x2 (a 1-instruction `mov edx,[esp+18h]` reload).
  Until this is fixed: when the first divergence is a block with no branch,
  contract it by hand before believing the verdict.
* **`branches --diff` aligns branches POSITIONALLY.** One extra or missing
  branch shifts every later pair, so a single real difference prints as a run of
  `POLARITY` / `TOPOLOGY` rows. That is how `sweep`'s two render/core
  `COND-FLIP` rows (`create_texture`, `create_texture3d`) were produced, and
  both are false alarms - the mnemonics agree once the extra pad block is
  removed. `blocks --diff [--lite]` uses content-based alignment and marks these
  `~=` ("same kind/shifted target"), so **prefer `blocks --diff` over
  `branches --diff` for the verdict** and use `branches` only to read a
  confirmed difference.

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
* **`match.db` is read-only and only supplies percentages** - the `< 100%` hint
  and `sweep`'s candidate list. Every CFG verdict comes from `binaries/rich`, so
  a DB from a different build changes *which* functions `sweep` lists and the
  `fuzzy` column, never the verdicts. `sema` never writes the DB.
