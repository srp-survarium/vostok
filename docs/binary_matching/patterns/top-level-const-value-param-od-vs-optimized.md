# Top-level `const` on a value parameter: byte-neutral under /Od, NOT under optimization

The pdb_divergence [fn-order] diff lists `const T arg` (target) vs `T arg` (base) as
unpaired only-tgt / only-base entries when the target declared a by-value parameter
`const`. Top-level const on a value parameter is **mangling-neutral** (the mangled name is
identical), so adding it is what makes those two entries pair.

## Where it is safe (byte-neutral)

In a **/Od** (unoptimized) translation unit, adding the `const` produces byte-identical
object code: the param still gets its own stack slot, the prologue/epilogue and every
instruction are unchanged. game_core is /Od (498+ byte-exact matches prove it), so
const-qualifying its value params is free and faithful. Apply to BOTH the header
declaration AND the definition (a divergent def or decl shows up alone otherwise).

Watch the **east-const** form: a def written `u32 const x` / `float const x` is ALREADY
top-level const (semantically `const u32 x`); do NOT prepend another `const` (you get the
illegal `const u32 const x`). When deciding "needs const?", treat any `const` token before
the param name (west or east) as already-const and skip it.

## Where it is NOT safe (optimized TU)

In an **optimized** TU (/O1/Ox/LTCG — animation, render, scaleform, and any module whose
target inlines aggressively) the `const` is a no-mutation guarantee the optimizer can act
on: it may keep the value in a register instead of spilling it, fold it differently, or
re-schedule — so adding `const` CAN change the emitted bytes and the match %.

Tell: after a byte-neutral-looking const sweep, an optimized TU regresses functions in the
same .cpp — *including a function you did not edit* (the whole TU recompiled and the
optimizer reshuffled). Observed: const-qualifying
`n_ary_tree_transition_tree_constructor`'s three `u32` ctor params dropped that ctor
57.8%->31.7% AND `n_ary_tree_cloner::initialize` (unedited, same TU) 97.1%->88.4%. That is
the optimizer, not fold churn, and it means the const made the optimized codegen diverge
from the target rather than match it -> revert the const in that TU.

## Rule of thumb

Apply the const-param pairing only to **/Od** modules (game_core, and verify others have no
regression in the edited functions after the rebuild). Skip / revert it in optimized TUs.
Reverting an unedited-sibling regression in an optimized TU is the FAITHFUL call here (the
const diverges from the target's optimized bytes), not a "% chasing" revert.

Reassignment guard: a candidate whose body **mutates** the param (e.g.
`weapon_core::set_target` reassigns `target`) cannot be const there — MSVC errors C3892
("cannot assign to a variable that is const"). Revert only that one param (def + decl),
keep the rest of the sweep.
