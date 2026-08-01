# A byte diff that flags no `j*` row is NOT proof the control flow matches

tags: cpp:if cpp:loop cpp:compare | asm:jcc asm:jmp | topic:tooling topic:condition-shape topic:structure-shape
symptoms: `--view diff` full of `~` operand rows and no branch row; equal byte size and equal statement count on both sides; `~ je short 00000037h -> je short 000000E8h` with two meaningless addresses; a function stuck in the 80s that will not move on operand edits
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
python3 scripts/sema.py blocks   <fn> --diff --lite   # flow SAME/DIFFERS + where it splits
python3 scripts/sema.py branches <fn> --diff          # each destination named by BLOCK INDEX
```

Naming destinations by block index makes a uniform displacement shift compare EQUAL and a
real retarget compare DIFFERENT. Run this **before** reading an instruction diff: an
instruction diff of two differently-shaped functions is noise.

## Evidence (render, 2026-08-01)

`effect_options_descriptor::operator[]` - **271 bytes on both sides, 23 blocks on both
sides, 15 branches on both sides**, 83.15%. `--view diff` flags 40 rows, **zero of them
branches**. `sema branches --diff` names six branches landing on a different block:

```
  #4   B6   @30     jne    target lands on B10, we land on B11
  #5   B7   @36     je     target lands on B9,  we land on B10
  #7   B9   @42     jne    target lands on B5,  we land on B6
  #8   B10  @4c     jmp    target lands on B11, we land on B12
```

Sweeping all 796 paired non-100% render functions that have a branch: 434 (54.5%) have a
provably IDENTICAL CFG (so their residual is genuinely not shape work), 84 carry a flagged
branch row that the CFG proves is a false alarm, and 2 are fully silent as above.

## The trap on the other side - `ORDER-ONLY`

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
