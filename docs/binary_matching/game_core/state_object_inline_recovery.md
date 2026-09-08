<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# State and object base inline review

Scope: all 39 inlines in `weapon_core_base_state` (9),
`player_logic_base_state` (9), `inventory_item` (16), `interactive_object` (3)
and `game_world_object` (2). Abstract declarations with no body remain abstract;
they are not missing implementations. Nine literal no-source markers are
classified, and two unmarked empty serialization hooks are reconstructed.

Frozen raw PDB comparisons show identical semantic class variants for all five
classes. The two state classes have duplicate-equal-record multiplicity 2:1;
the three object classes are fully identical in that comparison. No access,
layout, interface or virtual-order changes are made. No build was run.

## The four firing-state consumers

Both ordinary and aimed fire-state `initialize`/`finalize` methods contain an
assert-shaped dummy use followed by a write through the pointer at +0x12c.
The target slices are ordinary initialize statement 4, ordinary finalize
statement 3, aimed initialize statement 1, and aimed finalize statement 5.
The values are respectively true, false, true, false.

Restore protected `set_is_firing(bool)` as that assertion plus indirect write,
and replace all four open-coded pairs with calls. The pointer-not-null assert
expression is an explicit `UNKNOWN_EXPRESSION_T` inference; its shipping
condition is discarded. Do not omit the assert-shaped operation, add a null
guard, or turn the write into assignment of the pointer itself. Existing
`set_is_firing_ptr` remains the distinct pointer-binding setter.

Remove stale comments claiming base `execute` is an empty body: it resets the
playback state, and retail executes the two corresponding zero stores. The
separately fetched base `finalize` calls that same playback reset operation.

## Unmarked inventory hooks were not empty

`serialize_game_world_object_header` was only `{ /* rva ... */ }`. Its target
body instead copies the entire packet (0x12c bytes) and object (0x110 bytes), in
right-to-left argument order, and calls the common no-op helper. The matching
source idiom is a direct
`vostok::detail::unreferenced_parameter_helper(object, packet)` variadic call.
The default `deserialize_game_world_object` likewise pushes the two words of
the reader (0x8 bytes) and calls that helper. Their independently selected
candidate bodies were bare `ret 8` and `ret 4` respectively.

Restore those by-value argument operations, with complete type headers. Do not
pass `&object`/`&packet`/`&reader`: that would copy pointers rather than the
observed objects. Do not use the guarded plural macro: it adds a condition
which is absent here and can remove the body entirely. This is the existing
[direct unused-parameter idiom](../patterns/unreferenced-params-eater.md), not a
new serialization format.

Follow-up raw-PE inspection resolved the apparent clipped epilogues: the code
at RVA 0x869f0 ends after stack cleanup at +0x36 and the code at 0x9b250 ends at
+0x13, both immediately followed by `cc` padding. There are **no** saved-register
restores or return instructions hidden beyond the reported spans. The source
now follows each helper call with the existing unreachable macro, a shipping
`__assume(0)` representation. Normal return was not supported. Original macro
spelling and MSVC variadic-copy codegen remain verification questions; the raw
extent itself is no longer an unresolved question. See the trap-family register.

These virtuals have real usage paths: `base_player::deserialize_game_world_object`
dispatches through an inventory item, and `booby_trap_core::serialize` dispatches
the header hook through its owner. Empty base behavior need not imply unused
virtual declarations or empty derived overrides.

## A shared trap address is not an original-body oracle

`player_logic_base_state::get_attachment_transform` has the same one-byte
`int3` record at RVA 0x112e20 already observed for `empty_hands::deserialize`.
There are **29** entries at that RVA in the frozen target index:

```
pdb_rich_query --index binaries/rich/target/index.jsonl --rva 0x112e20 --list
```

They include void visitors, bool/matrix-returning object methods, resource
quality methods and `Scaleform::SysAllocMalloc::Free`. The pinned SDK's Windows
`Free` body actually calls `_aligned_free`; the current animation visitor
uses `NODEFAULT()`. Thus sharing that projected address does not by itself
establish one common original source body. Follow the actual vtable entries,
linker/section contribution and caller reachability before replacing source
with a trap, a safe fallback, or a claim of unreachability.

The attachment-transform placeholder is now explicitly `STATE[STUB]` with a
source/index TODO. Its existing default-matrix return is **not** accepted as
recovered. The prior build-survival explanation did not establish semantics.
This pass does not close the earlier empty-hands extent question either.

## Every weapon-core base-state inline

| Function | Disposition |
|---|---|
| `is_ready_to_be_deactivated` | Retain readiness flag getter. |
| `get_body_part_mask_for_user` | Retain body-part mask getter. |
| `has_animation_ended` | Retain animation-ended flag getter; implicit inline despite no keyword. |
| `initialize` | Retail framed return only; empty default confirmed. |
| `finalize` | Retail calls playback reset; existing out-of-class inline retained. |
| `execute` | Retail zeroes the two playback fields at +0x120/+0x124; retain reset. |
| `is_ready_for_transition` | Retail returns true; retain default. |
| `set_is_firing_ptr` | Retain pointer binding at +0x12c. |
| `set_is_firing` | Restore asserted indirect write and propagate all four consumers. |

## Every player-logic base-state inline

| Function | Disposition |
|---|---|
| `get_attachment_transform` | Open shared-trap/vtable/extent question above; placeholder explicitly marked. |
| `serialize` | Retail `ret 4`; empty default confirmed. |
| `deserialize` | Retail `ret 4`; empty default confirmed. |
| `id` | Retain const state-id field getter. |
| `is_ready_to_be_deactivated` | Retain readiness flag getter. |
| `is_ready_for_transition` | Retail `mov al,1; ret`; retain true default. |
| `is_weapon_visible` | Field-ownership model returns `m_is_weapon_weapon_visible` at +0x24; preserve retail's doubled spelling. |
| `is_smoothing_needed` | Field-ownership model returns +0x25 flag. |
| `is_physics_transform_allowed` | Field-ownership model returns +0x26 flag. |

The three flag getters have no independently retained procedure or consuming
source use found in the engine-wide name scan. Their constructor initializes
each corresponding field. That supports the models, but does not prove their
original assertions or classify them as editor/server-only. Keep that question
open instead of adding invented uses to make them appear in the binary.

## Every inventory-item inline

| Function | Disposition |
|---|---|
| `set_inventory` | Retain inventory pointer and slot stores. |
| `get_inventory` | Retain reference dereference; no invented null fallback. |
| `profile_slot_id` | Retain slot getter. |
| `get_action_behaviuor` | Retain const-reference return and original spelling. |
| `action` | Retail `ret 4`; retain singular parameter-use macro. Remove incorrect claim that implementation belongs to a different class's header. |
| `remove` | Retail `ret`; empty default confirmed. |
| `holder_assigned` | Retail `ret`; empty default confirmed. |
| `holder_removed` | Retail `ret`; empty default confirmed. |
| `set_amount` | Retain u16 amount store. |
| `amount` | Retain u16 amount getter. |
| `set_dict_id` | Retain dictionary-id store. |
| `get_dict_id` | Retain nonconst dictionary-id getter; do not silently const-qualify it. |
| `cast_weapon_core` nonconst | Retail null return, RVA 0x327c0; valid base behavior. |
| `cast_weapon_core` const | Same null-return RVA; keep distinct const interface. |
| `serialize_game_world_object_header` | Restore observed object/packet value copies into direct variadic helper. |
| `deserialize_game_world_object` | Restore observed reader value copy into direct variadic helper. |

## Every interactive/world-object inline

| Function | Disposition |
|---|---|
| `interactive_object::assign_game_ui` | Retail `ret 4`; empty default retained. |
| `interactive_object::cast_weapon_core` nonconst | Retail null return at 0x327c0; retained. |
| `interactive_object::cast_weapon_core` const | Same target RVA; retained. |
| `game_world_object::owner` nonconst | Retail framed null return at 0x10c0d0; retained. |
| `game_world_object::owner` const | Same target RVA; retained. |

Verification so far is source/evidence review and patch checks only. Deferred
build checks must preserve the firing consumers' statement boundaries and
confirm MSVC's direct variadic-copy/unreachable operations. This register accounts for all
39 methods without treating the remaining model/extent questions as closed.
