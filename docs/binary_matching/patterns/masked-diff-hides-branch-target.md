# A byte diff that flags no `j*` row is NOT proof the control flow matches

tags: cpp:if cpp:loop cpp:compare | asm:jcc asm:jmp | topic:tooling topic:condition-shape topic:structure-shape
symptoms: `--view diff` full of `~` operand rows and no branch row; `~ je short 00000037h -> je short 000000E8h` with two meaningless addresses; a function stuck in the 80s that will not move on operand edits; a CFG diff whose first divergence is a block with NO branch (that one is the tool, not the source)
confidence: 9/10

## Symptom

`pdb_fetch --view diff` shows a wall of `~` rows on registers and displacements and
**not one flagged branch instruction**, so the natural conclusion is "control flow is
fine, this is regalloc". Then no amount of operand work moves the score.

Or the opposite: the byte diff DOES flag a branch, as two absolute addresses from two
different address spaces, and the reader chases a divergence that does not exist.

## Cause

`--view diff` is objdiff-core. It compares a branch destination by the destination's
index in the **aligned** instruction vector (`arg_eq` / `ObjInsArg::BranchDest`), which is
the right idea and makes a branch row read *Equal* most of the time - and `pdb_fetch`
renders an Equal row with the base text only. Two consequences:

* when the two sides are shaped differently, the Patience alignment over the opcode
  sequence is locally plausible and globally wrong, and a genuine retarget can be
  absorbed into that misalignment with no branch row flagged at all;
* when an instruction is inserted or removed anywhere, a branch row can be flagged even
  though both sides jump to the corresponding block.

Neither direction is legible, because the rendered operand is an address, not a block.

## Fix - ask the CFG, not the bytes

```sh
python3 scripts/sema.py blocks   <fn> --diff --lite   # THE VERDICT: flow SAME/DIFFERS + where
python3 scripts/sema.py branches <fn> --diff          # then read the difference branch by branch
```

Naming destinations by block index makes a uniform displacement shift compare EQUAL and a
real retarget compare DIFFERENT. Run this **before** reading an instruction diff: an
instruction diff of two differently-shaped functions is noise.

## Evidence (render, 2026-08-01)

Sweeping all 796 paired non-100% render functions that have a branch: a majority have a
provably IDENTICAL CFG (so their residual is genuinely not shape work), and **84 carry a
flagged branch row that the CFG proves is a false alarm**. In the other direction,
`effect_options_descriptor::operator[]` (271 bytes, 83.15%) has **40 flagged rows and not
one of them a branch**. Whether `--view diff` flags a branch carries no information about
the flow, either way - which is the whole reason to ask the CFG instead.

(That function was first written up as a *silent control-flow divergence*. It is not: its
CFG matches, and the early `sema` was miscounting blocks - see the two traps below. The
`--view diff` silence is real; the conclusion drawn from it was not.)

## Trap 1 - a shape tool must CANONICALISE before it compares

A `nop`, a `lea ecx,[ecx]` pad, a spill reload or a re-materialised zero sitting right
after a `jcc` starts its own basic block: one predecessor, one successor, no branch. It
carries no control flow, but if destinations are named by block INDEX and one side has it,
**every later name shifts** and the comparison prints a cascade of "retargeted branches".
`effect_options_descriptor::operator[]` was exactly this - a one-byte `nop` - and it read
as six retargets. `sema` now contracts such blocks first (`contract()` in
`scripts/sema.py`): the render/core `TOPOLOGY` class fell from 12 rows to 1, eight of them
becoming `FLOW-SAME`. **If a first-skeleton-divergence points at a block with no branch,
suspect the tool, not the source.**

Corollary for reading `sema` output: `blocks --diff [--lite]` aligns by CONTENT and is the
verdict view; `branches --diff` pairs branches by POSITION, so unequal block counts turn
one real difference into a run of bogus `POLARITY` rows. Two render/core `COND-FLIP`
findings (`create_texture`, `create_texture3d`) were entirely this.

## Trap 2 - `ORDER-ONLY`

`jcc X / fall Y` and `jcc' Y / fall X` are the same edge pair. Moving ONE block in the
linear order therefore flips the polarity of every branch that reaches it and retargets
every branch that jumped past it - so one layout change reads as several independent
condition bugs. `sema` canonicalises each block's exit and tests the two CFGs for
isomorphism; when one exists it reports `ORDER-ONLY` and names the moved blocks.

`compare( shader_constant const&, shader_constant const& )` - 100 bytes and 15 blocks on
both sides - reports three retargeted branches and two "inverted conditions" positionally,
and `ORDER-ONLY: base B4->target B6, base B5->target B4, base B6->target B5` structurally.
Both sides merge the two `return 1;` into one block; ours keeps the earlier copy, the
target a later one. **Nothing about the conditions is wrong** - "fixing" the polarity here
would have made the source less faithful, not more.

## Related
- `three-way-compare-family.md` - the `>`-spelling rule that DOES fix a genuine
  operand-order inversion in the same family of functions; use `ORDER-ONLY` to tell the
  two apart.
- `docs/binary_matching/sema_tools.md` - the tool, its rc convention and its limits.
