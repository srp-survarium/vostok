# Empty-`{}` callee stub collapses the caller's body — match the call tree BOTTOM-UP

## Symptom

You body a caller faithfully (the statement shape matches the target's PDB line
numbers exactly), it PAIRS, but the structure-diff shows your base collapsing N
target statements into 1, with the others tagged `TRGT_ONLY`. The surviving base
statement is usually a loop header or the one call whose callee is NOT a stub.
Fuzzy lands low (15-25%) even though the source is correct.

Example (game_core animation, `n_ary_tree_comparer::add_weight_synchronization_group`):
target 4 stmts / base 1 stmt; the `m_equal=false`, the `weight_driving` ternary,
and the `add_animation(*i, weight_driving)` body all become `TRGT_ONLY`. Only the
`for` loop header survives.

## Cause

A callee invoked from the bodied caller is still an EMPTY stub (`{ }` — a bare
`ret`). The optimizer inlines the empty body to NOTHING, so:
- the `call <callee>` instruction disappears (no out-of-line target to call), and
- the caller statements that exist only to set up that call's arguments
  (`m_equal=false`, the arg ternary, the `*i` deref) get folded/CSE'd away with it.

The caller's TRUE shape and byte-image cannot be reproduced until the callee is a
real, non-empty, out-of-line function. This makes the dependency BOTTOM-UP: in a
call tree of mutually-stubbed members (a visitor / merge algorithm), body the
LEAVES first (the large helpers that never inline), THEN the mid-tree callers
expand and match.

## Secondary trap: bodying a mid-tree function STEALS a sibling's objdiff pairing

When you body a mid-tree function whose callees are still stubs, it becomes a new
real symbol that calls the same helpers a well-matched sibling calls. objdiff's
fuzzy symbol-matcher can then re-assign the sibling's base symbol to your new one,
dropping the sibling from e.g. 37% to `unpaired` — a NET regression for a ~15%
gain. (Seen: bodying `change_weight_synchronization_group` un-paired `merge_trees`;
reverting it to a parked stub restored `merge_trees` to 37%.)

## Fix / decision rule

- Body the LEAF helpers first (large enough to stay out-of-line), rescore, THEN
  the callers.
- A caller body is a WIN to keep only if it (a) is a confident decode anchored to
  the PDB line numbers, AND (b) does not steal a higher-% sibling's pairing.
  Confirm with `--view structure-diff` on the caller AND a rescore of its siblings.
- If a trial body collapses (all `TRGT_ONLY`) and you cannot verify its structure,
  PARK it with a `claude@NOTE` (record the decoded shape + the callees to body
  first) rather than bank an unverifiable low-% body.

## Tags

cpp:inline cpp:member | asm:call | topic:inline-vs-call topic:reachability topic:scoring-artifact
