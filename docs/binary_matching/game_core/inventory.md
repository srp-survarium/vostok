# inventory.cpp - structure verification (v2 verifier-matcher)

Branch: chain/game_core-sv-inventory (based on #185). One squashed commit.

## Summary of the recurring wall

Almost every residual in inventory.cpp is the same LTCG per-call-site
inline-vs-call decision over two trivial things:

1. `inventory::item_in_slot( slot )` - a trivial accessor
   `{ return m_slots[slot].item; }`. It EXISTS out-of-line in the TARGET
   (rva 0x7e5c0) but the TARGET INLINES it at these call sites (direct
   `lea [this + current*4 + 0x108]` = `&m_slots[current].item`). Our BASE
   instead emits an out-of-line `call survarium::inventory::item_in_slot`.
2. The `intrusive_ptr<inventory_item,...>` operators used on the slot item:
   - `operator unspecified_bool_type` / `operator T*() const` (the `if ( iitem )`
     / `if ( m_slots[i].item )` test) - TARGET calls out-of-line, BASE inlines.
   - `operator*` (the `iitem->...` deref) - TARGET calls out-of-line, BASE inlines.
   - `operator=(object_type*)` (in set_item) - TARGET INLINES (ctor+swap+dtor
     visible), BASE emits an out-of-line `call operator=`.
   - the copy ctor (in action / unload ammo's `inventory_item_ptr ammo = ...`) -
     TARGET calls out-of-line, BASE inlines the whole intrusive_ptr copy.

These are the documented "trivial-accessor / intrusive_ptr operator inline-vs-call"
LTCG class (assembly_patterns.md). The direction differs per call site and per
operator and is NOT steerable from the caller's source. Source spelling is already
correct (matched-author `sushi@MATCH` notes confirm).

## Steerable fixes APPLIED

- **set_holder / unset_holder**: the `for` loop body was braced
  `for (...) { if (...) ...; }`. The TARGET has a brace-less single-statement
  body, so the base emitted an EXTRA closing-brace `}` statement (a `+0x002`
  `jmp short` dead scope-exit) that the target lacked - a real QUANTITY diff
  (`ONLY base | }`). Dropping the braces removed the extra statement:
  set_holder now target 6 / base 6 (was 6/7), unset_holder target 7 / base 7
  (was 7/8). Both now quantity-diffs 0 (STRUCTURE MATCH). unset_holder's old
  `INCORRECT` tag was this same brace issue; re-tagged PARTIAL.
- **ctor `inventory::inventory()`**: now 100% after the full chain rebuild
  (was 91.03 in the stale handoff; the operand-aware diff was already 29/29 =
  100%, the residual was unresolved relocations that the rebuild closed).
  Re-tagged `100%|DONE`, embed stripped.

## Experiment that FAILED (reverted)

In setup_demo_profile, rewrote `item_in_slot( current )` as the direct member
access `m_slots[current].item` to try to force the inline and match the target's
direct `lea`. REBUILT WORSE: the weapon stmt grew from base 0x4c to 0x97 (the
direct access inlined even MORE of the resource_ptr construction). Reverted to
`item_in_slot( current )`. Conclusion: the original wrote `item_in_slot()` and the
inline-vs-call is genuine LTCG, not a member-access vs accessor spelling choice.

## Per-function verdicts (post final rebuild)

| function            | % (this chain) | verdict |
|---------------------|----------------|---------|
| inventory() ctor    | 100.0          | DONE (clean) |
| set_holder          | 71.08          | STRUCTURE MATCH; braces fixed; operator inline-vs-call residual |
| unset_holder        | 70.28          | STRUCTURE MATCH; braces fixed (was INCORRECT); operator inline-vs-call |
| set_item            | 69.33          | STRUCTURE MATCH; operator=/operator* inline-vs-call |
| action              | 58.21          | STRUCTURE MATCH (20/20); copy-ctor+operator inline-vs-call |
| setup_demo_profile  | 58.45          | STRUCTURE MATCH; item_in_slot+operator inline-vs-call |
| setup_from_profile  | 47.35          | STRUCTURE MATCH; item_in_slot+operator inline-vs-call (ONLY rows are alignment artifacts) |
| unload_to_profile   | 55.56          | STRUCTURE MATCH (43/43); item_in_slot+operator inline-vs-call |

The handoff brief's percents (36.41, 59.17, 65.84, ...) were STALE pre-chain
numbers; the table above is the current chain baseline after rebuild.
</content>
</invoke>
