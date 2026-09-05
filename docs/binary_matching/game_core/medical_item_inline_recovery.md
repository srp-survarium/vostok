<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Medkit, oxygen tank and lifebone inline recovery

Scope: all 13 medkit, 13 oxygen-tank and 12 lifebone explicit/implicit inline
declarations: **38 total**, including unmarked empty/unreachable overrides.
No C++ build, tool build or score refresh. Target and frozen candidate bodies
were fetched independently to avoid the two-sided alias-selection defect.
Raw class topology is identical for all three classes; preserve their access,
virtual order, fields and the lifebone writer's distinct one-argument signature.

## Their virtual bodies exist

The old comments claimed activation/selection had no distinct symbols and were
unpairable empty defaults. Each class actually has retained header procedures.
Several share code, but the target constructor-installed vtables point at the
same bodies too:

- Medkit primary vtable VA `0x978514`, from constructor RVA `0x750270 + 0x14`.
- Oxygen-tank primary vtable VA `0x97868c`, from constructor `0x6ea530 + 0x14`.
- Lifebone primary vtable VA `0x9787a4`, from constructor `0x57fc10 + 0x3f`.

The PE immediates and table entries were read directly, not inferred from the
first ICF symbol selected for a function address.

| Primary-table slot | Method | Retail RVA in all three |
|---:|---|---|
| 7 | `activate` | `0xbc450` |
| 8 | `deactivate` | `0x1a800` |
| 9 | `transform` | `0xd2070` |
| 10 | `tick` | `0x1a800` |
| 11 | `is_ready_to_be_deactivated` | `0x1a800` |
| 12 | `selected_animations` | `0xbcd50` |
| 13 / 14 | model-added / model-removed | `0x1a800` |
| 15 | `update_bones_matrices` | `0xbc810` |
| 16 | `is_sprinting` | `0x1a800` |

The raw bytes at `0x1a800` contain only frame setup, this spill and an ordinary
return. **They never assign AL/EAX.** The independently selected candidate bool
procedures added false for both medkit and oxygen hooks, and true for both
lifebone hooks. Remove all six unsupported constants. As in the ammunition
register, `UNREACHABLE_CODE()` is the shipping `__assume(0)` source-form model;
it is not a claim that the original methods returned a valid bool or that all
possible callers are unreachable. Original macro/assert spelling remains open.

Do not substitute arbitrary register reads, assembly, or a manufactured return
value to preserve undefined results. The model must be checked in the deferred
compiler pass. The neighboring void functions can keep empty bodies: unlike
the bools or matrix result, they promise no returned value.

## Missing argument-use operations

`activate` at `0xbc450` does more than return:

1. Emits the shipping dummy assertion-use sequence (zero byte, address, call).
2. Pushes the **reference pointers** for engine and user, calls the argument
   helper directly, cleans eight bytes and returns normally with `ret 8`.

Restore `ASSERT(UNKNOWN_EXPRESSION)` and the existing direct argument helper
with `&user, &engine` in all three. This follows the previously inspected
ammunition sibling; do not pass either noncopyable/incomplete object by value
or replace the direct call with a conditional unused-argument macro. Exact
assert expression remains unknown. Frozen candidate activation omitted both
operations; medkit even carried its argument macro only as a comment.

`selected_animations` at `0xbcd50` similarly emits the dummy use, then pushes
the bool and both words of `mutable_buffer`, calls the helper, cleans 12 bytes
and returns with `ret 12`. It does **not** construct the expression result.
Restore the dummy use and direct by-value buffer/bool helper, retaining an
explicit unreachable-result model. Unlike the inventory-header/trap hooks
documented earlier, the raw PE here really has a normal epilogue before padding.
Do not generalize their non-returning fallthrough onto these functions.

`update_bones_matrices` is different again: at `0xbc810` the conditional
`identity(false)` argument-eater branch survives. It copies the skeleton
pointer value, two matrices by value, three scalar/pointer values and the
noncopyable animation player by address (0x94 stack bytes). Independently
fetched target and candidate have these operations already. Preserve the
existing plural unused-parameter macro and shorten only stale narration.

## Where the actual item work lives

All three constructors pass `use_silent = 1` into the inventory base. Retail
`inventory::action` routes that behavior to virtual slot 23 (`+0x5c`) and the
item's `action(bool)`, rather than installing it as an active hand object.
The active-item switch has a different branch calling `set_new_active_item`.
This explains the ordinary consumer separation, not universal unreachability.

Oxygen action toggles `set_active`; the latter registers/unregisters its
`active_tick` callback and damage protectors. Its retained tick at `0x6ea0d0`
reduces remaining time and uses the declared `empty()` in statement 4: zero
comparison at item +0x120, followed by deactivation. Medkit schedules its own
`active_tick` (`0x74fa50`) to handle delay, healing, stamina regeneration and
expiry. Statement 22 tests activity time at +0x13c through `empty()`.
Keep these two existing accessors; do not move scheduled work into the
parameterless interactive `tick` just because its name suggests a busy method.
Clangd also resolves oxygen's expiration call to the protected inline owner.

Lifebone's actual work is in `action`, `activate_impl`, holder notifications
and passive protection registration. Its interactive `activate` is not a
replacement for `activate_impl`; that name similarity does not justify merging
their responsibilities.

## Serialization has two distinct lifebone entries

Medkit and oxygen slot 18 (`0xbcd90`) forwards packet/client offset to
`inventory_item::serialize`, and slot 19 (`0xbcdb0`) forwards the reader to
the base. Existing source is correct; no extra item-state fields added.

Lifebone slot 18 instead points at the inherited two-argument writer
`0x590840`. Its explicitly declared **one-argument** writer is a separate slot
31 at `0xd2070`, the ordinary framed `ret 4` body. Its reader override, slot
19, also points at `0xd2070` and consumes no bytes. Remove their stale markers
but keep these empty bodies and signatures. Adding a client-offset parameter
or base delegation to the one-argument writer would change the PDB API and
conflate distinct virtual functions. This is not evidence that the class is
unconstructed: its concrete constructor, action and vtable are retained.

## Every declaration

| # | Class / method | Disposition |
|---:|---|---|
| 1 | `medkit::empty` | Existing activity-time zero test verified in active_tick; remove derived assembly comment. |
| 2 | `medkit::activate` | Restore assertion-shaped dummy use and direct pointer argument helper; assertion source remains open. |
| 3 | `medkit::deactivate` | Verified framed empty void body retained. |
| 4 | `medkit::transform` | Existing unreachable-result model retained; no matrix output written by retail. |
| 5 | `medkit::tick` | Verified framed empty void body; remove stale marker. Scheduled active_tick owns real timed work. |
| 6 | `medkit::is_ready_to_be_deactivated` | Remove unsupported false return; unresolved unreachable-source model. |
| 7 | `medkit::selected_animations` | Restore dummy/argument-use operations; assertion and result source form open. |
| 8 | `medkit::on_player_model_added` | Verified framed empty void body; remove marker. |
| 9 | `medkit::on_player_model_removed` | Same, distinct declaration accounted for. |
| 10 | `medkit::update_bones_matrices` | Existing conditional argument-eater expansion retained; no real animation output inferred. |
| 11 | `medkit::serialize(packet&,u32)` | Existing base delegation verified. |
| 12 | `medkit::deserialize` | Existing base delegation verified. |
| 13 | `medkit::is_sprinting` | Remove unsupported false return; unresolved unreachable-source model. |
| 14 | `oxygen_tank::empty` | Existing remaining-time zero test verified in active_tick. |
| 15 | `oxygen_tank::activate` | Restore assertion-shaped dummy use and direct pointer argument helper; assertion source remains open. |
| 16 | `oxygen_tank::deactivate` | Verified framed empty void body retained. |
| 17 | `oxygen_tank::transform` | Existing unreachable-result model retained; no matrix output written by retail. |
| 18 | `oxygen_tank::tick` | Verified framed empty void body; remove marker, preserve separately scheduled active_tick. |
| 19 | `oxygen_tank::is_ready_to_be_deactivated` | Remove unsupported false return; unresolved unreachable-source model. |
| 20 | `oxygen_tank::selected_animations` | Restore dummy/argument-use operations; assertion and result source form open. |
| 21 | `oxygen_tank::on_player_model_added` | Verified framed empty void body; remove marker. |
| 22 | `oxygen_tank::on_player_model_removed` | Same, distinct declaration accounted for. |
| 23 | `oxygen_tank::update_bones_matrices` | Existing conditional argument-eater expansion independently checked on both sides. |
| 24 | `oxygen_tank::serialize(packet&,u32)` | Existing base delegation verified. |
| 25 | `oxygen_tank::deserialize` | Existing base delegation verified. |
| 26 | `oxygen_tank::is_sprinting` | Remove unsupported false return; unresolved unreachable-source model. |
| 27 | `artefact_lifebone_core::activate` | Restore assertion-shaped dummy use and direct pointer argument helper; not activate_impl. |
| 28 | `artefact_lifebone_core::deactivate` | Verified framed empty void body retained. |
| 29 | `artefact_lifebone_core::transform` | Existing unreachable-result model retained; no matrix output written by retail. |
| 30 | `artefact_lifebone_core::tick` | Verified framed empty void body; remove marker. |
| 31 | `artefact_lifebone_core::is_ready_to_be_deactivated` | Remove unsupported true return; unresolved unreachable-source model. |
| 32 | `artefact_lifebone_core::selected_animations` | Restore dummy/argument-use operations; assertion and result source form open. |
| 33 | `artefact_lifebone_core::on_player_model_added` | Verified framed empty void body; remove marker. |
| 34 | `artefact_lifebone_core::on_player_model_removed` | Same, distinct declaration accounted for. |
| 35 | `artefact_lifebone_core::update_bones_matrices` | Existing conditional argument-eater expansion retained; noncopyable player reference passed by address. |
| 36 | `artefact_lifebone_core::serialize(packet&)` | Verified empty one-argument extra virtual; not the inherited two-argument writer. |
| 37 | `artefact_lifebone_core::deserialize` | Verified empty reader override; no base delegation or payload invented. |
| 38 | `artefact_lifebone_core::is_sprinting` | Remove unsupported true return; unresolved unreachable-source model. |

Fifteen literal markers and two nonstandard medkit bool-placeholder comments
are classified. The nine source TODOs keep original assertions and unreachable
contracts open, with existing transform uncertainty covered by the same result-
contract review. Source-only checks cover helper argument types and unchanged
layout/access. Deferred compilation must verify the new argument-use shapes
and six removed return assignments; no score improvement is claimed here.
