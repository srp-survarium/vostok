# Address-taken member-pointer anchor emits a header COMDAT in the RIGHT unit
tags: cpp:inline cpp:member cpp:template | asm:call asm:mov | topic:anchoring topic:inline-vs-call topic:fold-icf
symptoms: header getter/setter standalone in target index only, plain anchor call gets inlined by /GL, function absent from base index
confidence: 8/10
variants: inline-vs-call-trivial-getter.md, single-tu-anchor-inlines-helper.md, carcass anchors (temp_include_all)

A header-inline member the target keeps standalone is inlined by our /GL at every
call site - a plain anchor CALL emits nothing. Taking its ADDRESS into a member
pointer (escaped via `example_callback`) forces a standard-convention out-of-line
COMDAT, and its line records attribute it to the defining HEADER, so the delinker
pairs it with the target's header pseudo-unit:

```cpp
survarium::inventory_holder& ( survarium::inventory::*holder_fn )( )
    = &survarium::inventory::holder;
example_callback( reinterpret_cast< pcstr >( &holder_fn ) );
```

Outcome depends on the TARGET copy's shape:
- target kept the plain /Od thiscall shape -> near/exact match
  (weapon_user_animations_selector::set_animations: 100%, only the ICF canonical
  name of the called resource_ptr/intrusive_ptr operator= differs).
- target kept an LTCG custom-convention copy (this in eax/esi, no frame) -> pairs
  but ~0% bytes (inventory get_active_slot/holder/item_in_slot, material_pair
  accessors). Still preferable to TARGET_ONLY: right unit, structure visible.
- the emitted /Od body may ICF-fold with a same-shape accessor from another
  header and vanish into THAT unit (material_pair::decal1 -> single_sound.h twin).

Two gotchas discovered alongside:
- report.json cannot distinguish paired-at-0.0% from unpaired: protobuf omits
  zero fields, so `fuzzy_match_percent // "unpaired"` lies for 0% pairs. Confirm
  with `objdiff-cli diff -1 <target.obj> -2 <base.obj> -o - --format json <sym>`
  and look for `target_symbol`.
- a worktree's inherited report.json may predate the branch state: rebuild a clean
  baseline before attributing "regressions" to your edits (flapping 100->0 thunks
  / boost binders / empty_stub entries are zero-sum ICF-lottery churn on every
  relink, not regressions).

Evidence: game_core tiny-units batch (inventory.h, material_pair.h,
weapon_user_animations_selector.h), 2026-06-12.
