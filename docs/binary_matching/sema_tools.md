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

Three readings, in descending order of importance:

1. **434 of 796 (54.5%) have a provably IDENTICAL control-flow graph.** Their
   residual is operands, register allocation or scheduling - not shape. No
   existing view could say that, so today the choice between "grind on the byte
   diff" and "the structure is wrong" is made by feel.
2. **84 false alarms.** objdiff flags a branch instruction as divergent on a
   function whose CFG `sema` proves identical. `device::destroy_d3d` (89%) is
   the clean case: the byte diff shows
   `~ 0x13: je short 00000037h -> je short 000000E8h`, two numbers from two
   address spaces, and the actual difference is a `push ecx`/`pop ecx` pair.
   A reader who chases the `je` is chasing nothing.
3. **The fully silent cell is real, just rare (2).** It is worth the tool on its
   own, because of *where* it lands.
   `effect_options_descriptor::operator[]` is **271 bytes on both sides, 23
   blocks on both sides, 15 branches on both sides**, scores 83.15%, and its
   comparison web is shaped differently - `sema branches --diff` names six
   branches that land on a different block. `pdb_fetch --view diff` flags 40
   rows for it and **not one of them is a branch**. Everything that view can
   show you is an operand; the actual defect is invisible. That is the function
   a matcher would spend a session on.

The practical consequence is the one that costs sessions: a matcher runs the
first-look command, sees a wall of `~` operand rows, and either grinds on
register spellings that can never converge (the shape is wrong), or writes the
residual off as "LTCG/regalloc" (the shape is right, but nothing proved it).
`sema` turns both of those guesses into a one-line fact.

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

`sweep` classifies a whole module in one pass:

| verdict | meaning | what to do |
|---|---|---|
| `IDENTICAL` | same blocks, same bodies | the residual is relocation/symbol identity - check the objdiff pairing, not the source |
| `FLOW-SAME` | same CFG, different block bodies | operands / regalloc / scheduling; **not** shape work |
| `ORDER-ONLY` | CFGs isomorphic, different block LAYOUT | one merged exit placed elsewhere - ONE fact, see below |
| `COND-FLIP` | a branch mnemonic differs | inverted condition or signed/unsigned twin - a real source bug |
| `TOPOLOGY` | same mnemonics, different destination block | the shape an instruction diff cannot show |
| `BRANCH-COUNT` | different number of branches | a guard we are missing, or an `if` one side folded |
| `BLOCK-SPLIT` | same branch sequence, different block count | usually an unreachable/padding artifact |

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
* **Block alignment is content-based** (`difflib` over whole-block text). When
  both sides are heavily rewritten the pairing is a guess; the `flow` verdict
  and the first-skeleton-divergence line stay meaningful, the per-block bodies
  become advisory.
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
  went from a bogus 17-vs-18-block verdict to a clean 14-vs-14 that isolates
  the one real difference (we tail-`jmp` the last call, the target `call`s and
  returns).
* **It says nothing about statements or locals.** Structure verdicts stay
  `pdb_fetch --view structure-diff`; `sema` is strictly about shape below the
  statement level.
* **`match.db` is read-only and only supplies percentages** - the `< 100%` hint
  and `sweep`'s candidate list. Every CFG verdict comes from `binaries/rich`, so
  a DB from a different build changes *which* functions `sweep` lists and the
  `fuzzy` column, never the verdicts. `sema` never writes the DB.
