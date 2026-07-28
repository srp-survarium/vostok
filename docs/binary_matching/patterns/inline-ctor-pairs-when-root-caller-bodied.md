# An `inline` ctor pairs only once a non-inlining out-of-line caller is bodied

cpp:ctor cpp:inline cpp:reachability | asm:call | topic:inline-vs-call topic:reachability topic:un-dce

## Symptom

A header-declared `inline` constructor (or any `inline` member) shows `unpaired` in
`report.json` even though its body in the `*_inline.h` is already fully and correctly
written. `pdb_rich_query --index base/index.jsonl --function <ctor>` returns nothing -
the base emits no standalone COMDAT for it at all. The TARGET index DOES list it as a
standalone symbol (e.g. `n_ary_tree_animation_node::n_ary_tree_animation_node` at its own
RVA, single-statement structure = just the prologue + member-init stores).

## Cause

MSVC emits an `inline` function as an out-of-line COMDAT only when some call site declines
inlining (body too large, or called from enough sites). If every would-be caller is still a
`return NULL;` STUB, the ctor has ZERO real call sites, so it is never emitted and never pairs
- it is not an inline-vs-call wall, it is a *reachability* gap. The fix is not to touch the
ctor; the ctor is right. The fix is to BODY THE ROOT CALLER that constructs it.

## Fix

Body the caller(s) that do `new ( p ) T( ... )` with the real argument list (recover the
arg order from the target asm's push sequence and the class layout). Once a bodied, anchored
caller (here `new_animation`, reached via the `change_animation` anchor) issues an out-of-line
`call T::T`, MSVC emits the ctor COMDAT and it pairs - even if the CALLER itself only reaches a
partial % (its surrounding logic need not be byte-perfect for the ctor to pair). Measured:
bodying `new_animation` (0.27%->50%) paired BOTH `n_ary_tree_animation_node` ctors
(unpaired -> 41.6% / 43.6%) in one rebuild; the `animation_interval` ctor in the same clone
loop stayed `unpaired` because its body is tiny enough that MSVC inlined it at the only call
site (genuine inline-vs-call, a separate wall).

This is the INVERSE of root-first-descend: instead of bodying a stub callee to un-DCE a caller,
you body a stub *root caller* to force a correct-but-never-emitted inline *callee* out-of-line.

## Related

- `inline-ctor-body-from-expansions.md` - recovering the ctor BODY (this is about its REACHABILITY).
- `empty-callee-collapses-caller-body.md` - the bottom-up direction.
- `tcp-packet-ctor-out-of-line.md` - the opposite case (target CALLs, base inlines a small ctor).
