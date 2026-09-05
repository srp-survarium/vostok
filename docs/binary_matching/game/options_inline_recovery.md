<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Options inline and consumer recovery

Source-only continuation above `e62bc422a` in PR 571. **63 additional entries**
cover all 14 in-class bodies in this family, generated operations and the
retained consumers/ownership anchors listed below. No build or new score claim.
Addresses are retail RVAs unless explicitly labeled frozen base.

Two behavioral corrections are established independently of guessed helper
boundaries: options input priority is **5**, not zero; monitor changes refresh
the **resolution item 1**, not the monitor's own item 0. The null-returning item
lookup is replaced with the observed two-stage lookup. Its general-index
contract, the waiting predicate's original named boundary, and two explicit
destructor models remain open questions, indexed in `review_todos.md`.

## Individual register

Number ranges enumerate distinct generated methods, not additional recovered
bodies. Noncopyable bases remain private; generated copy/assignment records do
not establish that those operations are usable.

| # | Function | Evidence and disposition |
|---:|---|---|
| 1 | `game_options::input_handler` | Existing unmarked `return *this` agrees with constructor-installed target slot `0x9b350`; independently selected frozen base is also `mov eax,ecx; ret`. Keep this introducing virtual, not an input-interface override. |
| 2 | `input_priority` | Installed target slot `0x4c80` returns 5; frozen base slot `0xbc470` returns zero. Correct the literal. Activation registers this handler with the priority-ordered input dispatcher. |
| 3 | `is_active` | Existing `m_is_active` getter verified at `game::tick 0x5d81e0 +0xdc`: byte at game `+0x850`, options `+0x34`. Remove stale marker, retain nonconst signature. |
| 4 | `get_options_item(tab,item)` | Replace null with `m_options[tab]->option_by_id(item)`. `refill_item_data +0x9f` reads options tab 2 then item 1 and calls its virtual `fill_data`. General guards and original named boundary remain modeled. Include the complete tab type. |
| 5 | Private `is_waiting_for_bind_key` | Replace false with the observed `m_waiting_for_bind_action != kLASTACTION` predicate; propagate at keyboard, mouse-key and mouse-motion consumers. Sentinel is `0x40`, not unbound-action `0x41`. Original named boundary remains inferred. |
| 6–7 | Implicit copy constructor; assignment | Generated public nonvirtual records; keep implicit through private noncopyable base. Do not introduce copying of owned tabs or rebinding of the game reference. |
| 8 | Explicit constructor | `0x6f84a0` installs two vptrs, initializes both movie handles, game reference, mouse position, waiting sentinel and conflict vector. Preserve fields not written by the retained constructor; do not add initialization as cleanup. |
| 9 | Explicit virtual destructor | `0x6f83c0`: delete four tabs, release conflict vector/movie handles, destroy flash external-handler implementation. Preserve existing generated member cleanup, not a custom duplicate release. |
| 10 | `on_keyboard_action` | `0x6f8c60 +7` tests sentinel, processes down events and calls `finish_binding`; substitute only the predicate seam. Keep UI fallback and consumed-event result. |
| 11 | `on_mouse_key_action` | `0x6f8ba0 +0xc` tests the same sentinel. Retained end-binding operations have their own caller source lines; keep that sequence rather than assuming a call to `finish_binding`. Only predicate seam changes. |
| 12 | `on_mouse_move` | `0x6f84e0 +3` tests sentinel before movement/GUI forwarding. Use the negated waiting helper; preserve coordinates and event-result behavior. |
| 13 | `refill_item_data` | `0x6f5700`: six statements, one `flash_value[3]` local. Observed specialization uses video tab 2/resolution item 1; existing source now obtains a real item through the recovered lookup. No extra local or null fallback. |
| 14 | `finish_binding` | `0x6f8800` ends UI key capture, shows cursor and resets waiting action to `0x40`. This writer establishes predicate meaning, not just a name-based guess. |
| 15 | `activate` | `0x6f8b40`: active guard, parent/movie setup, active store, handler registration, menu-button population. Keep existing lifecycle and registration seam. |
| 16 | `deactivate` | `0x6f8b00`: active guard, movie hiding, parent clear, active clear, handler removal. No new reset of waiting/conflict state. |
| 17 | `fill_settings_data` | `0x6f6310 +0x791` reads item type at `+0xc` before selector-specific data handling. Confirms the item-type getter; not a claim that the complete UI builder is matched. |
| 18 | `options_tab::option_by_id` | Existing unmarked direct private-array getter. Monitor refresh reads item 1, quality handling item 8; no bounds check added. |
| 19 | `type` | Parent tab `+8` read in item-base revert `0x5ba3c0`; preserve enum result. |
| 20 | `get_game` | Parent tab `+0xc` read in monitor refresh `0x5bae90`; retain borrowed `game&`. |
| 21 | `get_movie` | Parent tab `+0x10` yields a pointer to the borrowed handle in item-base revert; retain reference return, not an extra owning-handle copy. |
| 22–23 | Implicit copy constructor; assignment | Generated records through private noncopyable base; no explicit bodies. |
| 24 | Explicit constructor | `0x5bb240` owns its option-pointer array and allocated option objects; borrows game and movie handle. Existing four tab inventories remain, including monitor 0, resolution 1 and quality 8 in video. |
| 25 | Explicit nonvirtual destructor | Retained loop destroys through `options_item_base*`, frees each outer allocation, then pointer array. It does not invoke derived monitor/resolution destructors; preserve this significant behavior. |
| 26 | `apply` | Existing per-item apply, video-specific render-options completion, save-user-config and reinitialization remain. No new ownership or ordering policy. |
| 27 | `revert` | Existing per-item virtual revert loop remains; no extra initialize pass. |
| 28 | `initialize_data` | Existing item initialize/value fill and UI array publication remain; its private-array accesses are owner-internal and do not require invented friendship. |
| 29 | `options_item_base::option_type` | Getter of protected enum at `+0xc`; verified in settings-data consumer. Remove stale marker. |
| 30–32 | Implicit copy constructor; assignment; destructor | All generated public nonvirtual. In particular, polymorphic base does **not** have a virtual destructor. Keep compiler-generated cleanup of `flash_function_handler`. |
| 33 | Explicit constructor | Retained base handler construction, type/parent/id stores, console-command lookup and missing-command diagnostic. No invented command ownership or fallback. |
| 34 | `revert` | `0x5ba3c0`: parent type/id, virtual value fill, flag and movie invocation verify the tab getter seams and borrowed-handle shape. |
| 35–37 | `options_item_int` implicit copy; assignment; destructor | Generated nonvirtual operations. Do not add generic deletion of `m_values`: ordinary selectors borrow static value tables. Derived allocated-table ownership is separate. |
| 38–40 | `options_item_float` implicit copy; assignment; destructor | Generated nonvirtual operations; preserve protected step/current/source floats at `+0x18/+0x1c/+0x20`. |
| 41–43 | `options_item_bool` implicit copy; assignment; destructor | Generated nonvirtual operations; preserve protected current/source bytes at `+0x18/+0x19`. |
| 44 | `options_gamma_selector` explicit inline constructor | Tab constructor `+0x34c` constructs slider type 2, command `r_gamma_correction_factor`, item 6 and step `0.01f` (`0x3c23d70a`). Existing initializer is real; remove stale marker. |
| 45–47 | Gamma implicit copy; assignment; destructor | Generated nonvirtual operations, no added data ownership. |
| 48 | `options_graphics_quality_selector::value` | `options_item_int::call 0x5ba1f0 +0x1e` finds quality item 8 and reads its byte `+0x1d`. Preserve `u8`, remove stale marker. |
| 49–51 | Quality implicit copy; assignment; destructor | Generated nonvirtual operations, no custom static-table cleanup. |
| 52 | `options_monitor_index_selector` explicit inline destructor | Raw PDB says explicit/nonvirtual, unlike the generated int-base destructor. Model guarded deletion of its owned pointer array; original guard and typed destruction consumer remain unverified. |
| 53–54 | Monitor implicit copy constructor; assignment | Generated operations; do not introduce deep-copy policies. |
| 55 | Explicit monitor constructor | Base command `r_monitor_index`, item 0; allocates pointer array and fills pointers into six embedded cached strings. Supports ownership model, not proof of the destructor's exact body. |
| 56 | `refill_resolutions_data` | `0x5bae90`: obtain item 1, refill its resolutions, refresh options UI. Correct second refresh argument from this monitor's item 0 to resolution item 1. |
| 57 | Monitor `call` | `0x5baf00`: int-base value handling followed by the same resolution refresh and item-data call. Existing helper seam retained. |
| 58 | Monitor `revert` | Restore source byte to current, invoke base revert, refill resolutions, tail-jump to item-data refresh. Direct-call-only xrefs miss that tail edge. |
| 59 | `options_resolution_selector` explicit inline destructor | Raw PDB says explicit/nonvirtual. Model guarded pointer-array deletion using the cleanup already observed in `fill_resolutions`; original typed destruction and guard remain open. |
| 60–61 | Resolution implicit copy constructor; assignment | Generated records; no fabricated copying or ownership transfer. |
| 62 | Explicit resolution constructor | Base command `r_resolution`, item 1, null initial values/count; fill from current monitor. Embedded cache owns the strings, dynamic allocation owns only their pointer array. |
| 63 | `fill_resolutions` | `0x5ba9e0`: preserve previous selected string/index, delete old pointer array, rebuild/filter modes and cache pointers. Existing cleanup supplies the destructor model; do not free embedded strings separately. |

## PDB/access and vtable evidence

Raw retail complete class/field-list pairs are:

| Class | Class / field list | Size | Destructor attributes |
|---|---|---:|---|
| `game_options` | `0xb720/0xb71f`, equal second variant `0xf4f9/0xf4f8` | `0x50` | `0x007`, explicit virtual |
| `options_tab` | `0xb6b9/0xb6b8` | `0x14` | `0x003`, explicit nonvirtual |
| `options_item_base` | `0xb6a1/0xb6a0` | `0x18` | `0x103`, generated nonvirtual |
| `options_item_int` | `0xb6fd/0xb6fc` | `0x20` | `0x103` |
| `options_item_float` | `0xb6dc/0xb6db` | `0x24` | `0x103` |
| `options_item_bool` | `0x16890/0x1688f` | `0x1c` | `0x103` |
| `options_gamma_selector` | `0xb6cc/0xb6cb` | `0x24` | `0x103` |
| `options_graphics_quality_selector` | `0xb6ed/0xb6ec` | `0x20` | `0x103` |
| `options_monitor_index_selector` | `0x168ba/0x168b9` | `0x128` | `0x003`, method type `0x16877` |
| `options_resolution_selector` | `0x168ae/0x168ad` | `0x5820` | `0x003`, method type `0x16850` |

Game-options fields and tab fields are private; item-base and int/float/bool
fields are protected; selector string caches are private. Selector inheritance
is public, noncopyable inheritance private. `is_waiting_for_bind_key` is private;
the item/tab getters are public. No visibility widening or new friendship.

The two complete game-options records have one semantic variant, versus one
complete frozen-base record. This is **not** whole-record-multiplicity equality;
remove obsolete header narration suggesting a second source-class union.

Retail constructor installs input vtable RVA `0x95b554`, whose priority slot
`+0x18` points to `0x4c80` (`mov eax,5; ret`). Frozen base installs `0x94e6e4`,
same slot pointing to `0xbc470` (`xor eax,eax; ret`). The extra `input_handler`
slot is `+0x20`; its raw method attribute is `0x013`, introducing virtual.
Use constructor-installed tables rather than arbitrarily choosing an ICF alias.

## Lookup, sentinel and ownership constraints

`refill_item_data +0x9f` loads `[options+0x20]` (tab 2), then the tab's pointer
array and `[array+4]` (item 1), then calls `fill_data` at vtable `+8`. This
establishes the corrected resolution target even though LTCG removes the call
arguments. It does not establish the unobserved general-index guard policy.
Do not mistake the lack of a standalone getter for absence of its operation.

The constructor and `finish_binding` store `kLASTACTION` (`0x40`) at `+0x38`.
All three input consumers test that field. Restore the declared private
predicate at those sites, while retaining the original-boundary question.
In particular, mouse-key end-binding operations retain local caller line
records distinct from `finish_binding`'s lines; no forced helper substitution.

Monitor and resolution constructors allocate arrays of pointers to their own
embedded strings. The resolution refill already frees its old array: the
retained code subtracts the eight-byte allocation cookie and frees that block.
Use the existing `VOSTOK_DELETE_ARRAY_IMPL` allocator path for both destructor
models. The helper does not accept null safely by itself; the modeled guard is
explicit, **not** claimed to be an observed original destructor guard.

The shipped `options_tab` destructor destroys items through a nonvirtual base
destructor, releases the flash-handler implementation, and frees outer item
storage. It does **not** free these derived pointer arrays. Preserve that path;
do not invent virtual destructors or dispatch to hide a potential retail leak.
The raw explicit-destructor flag proves source authorship, not its body or an
observed typed invocation. Both reconstructed bodies remain ownership models.

## Checkpoint

Twelve literal markers removed, four new TODO anchors indexed. Nine source
files changed; no layout/member/access reordering, invented data, SDK edits,
compilation, tool rebuild or ledger/README refresh. Static checks do not prove
post-edit byte or statement equality; retain this batch for the eventual
combined build and caller/structure comparison.
