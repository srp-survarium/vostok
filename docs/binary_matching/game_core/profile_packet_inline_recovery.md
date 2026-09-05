<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Profile, item, slot and booster inlines

PR 571 source-only reconstruction. No build, new score, or fresh C++ execution
claim. All target/base observations below refer to existing PDBs and images.

## Individual register: 11 declarations

| # | Function | Disposition and evidence |
|---:|---|---|
| 1 | `player_profile::player_profile` | Existing body retained. Target RVA `0x97700`: zero account/profile IDs, construct 11 boosters and 19 slots, team=3, is_local=false, first name byte=0, then zero all 88 booster bytes. Preserve the apparently redundant zero after the member constructors. |
| 2 | `player_profile::serialize` | Restore a sparse inverse-reader model in `game_net_defines.h`, next to its existing reader and shared slot-mode table. Byte team/bool/name, u16 booster mask, selected byte-ID/float pairs, then tagged slot records to packet end. Original sender and selection/assertion policy remain open. |
| 3 | `player_profile::deserialize` | Existing body retained. RVA `0x98670` verifies byte team, normalized local bool, length-prefixed name, u16 mask, 11 booster positions, zeroing absent boosters, then repeated byte-slot/item records until EOF. No account/profile IDs are read; absent slots are not cleared. |
| 4 | `inventory_item_instance::inventory_item_instance` | Existing field-zero initializer list retained. Its expansion through `profile_slot` construction at `0x97760` stores three dwords and one word. Do not replace with a 16-byte memset: two tail-padding bytes are not initialized. |
| 5 | `inventory_item_instance::serialize` | Restore dict_id u16, id u32, optional condition/stack narrowed to u16, optional inventory amount u32. Mode tests mirror the verified reader; original writer/range assertion policy remains open. |
| 6 | `inventory_item_instance::deserialize` | Correct its two mode predicates to explicit accepted-value disjunctions, as retained in the profile caller. Preserve reader order and untouched fields for disabled modes. No blanket clearing or invented invalid-mode exception. |
| 7 | Free `operator==(inventory_item_instance const&, u32)` | Existing id-only comparison retained. `network_client::process_shop_action`, RVA `0x5b69f0`, compares each element's `+8` against the received ID in its find loop. No member operator added. |
| 8 | `profile_slot::serialize` | Restore the item-writer forwarder with the same packet/mode. The corresponding reader seam is present in the retained profile consumer; original writer boundary remains a model. |
| 9 | `profile_slot::deserialize` | Existing item-reader forwarder retained. The profile caller inlines through both layers and writes dict_id/id/selected payload at the recorded slot offsets. |
| 10 | `profile_slot` implicit constructor | Keep generated. RVA `0x97760` contains the item member's four zero stores; do not invent a second explicit constructor or extra padding stores. |
| 11 | `skill_booster::skill_booster` | Existing id=0/value=0 body retained. RVA `0x97670` writes a byte at 0 and zero float at +4. It leaves internal padding alone; the enclosing profile subsequently zeroes its whole booster array. |

Frozen `pdb_topology --classes --show-identical` reports identical semantic
records for all four classes, with public data/methods. `player_profile` has
two equal records on each side, not two competing layouts. Existing generated
copies/trivial destruction remain compiler-owned; no new named special members
are fabricated.

## The actual reader mismatch

In `player_profile::deserialize`, the target and frozen base each have ten
statements. The last statement is 0x6e bytes in retail versus 0x64 in base.
Inspecting both one-sided expansions resolves the cause:

- Retail at `+0xfd`: accept condition for mode 2 **or** 0.
- Retail at `+0x117`: accept amount for mode 2 **or** 1.
- Frozen base: accept condition for anything except 1, and amount for anything
  except 0.

Use `mode == serialize_both_values || mode == serialize_just_...` for each
field. All three declared modes have the same payload as before, but an
out-of-range mode now leaves both optional fields and reader cursor untouched,
matching the target branches. This is a demonstrated source/branch mismatch,
not a claim that a shipped caller has actually supplied such an invalid mode.
No optimizer workaround, shared packet change or build was needed to identify it.

The slot-mode array is also verified from raw retail data: the indexed load
at `0x98670 + 0xd6` names VA `0x9c2290`, RVA `0x9b2290`. Its 19 dwords are
`0,0,0,0,0,0,0,0,2,2,0,2,2,2,2,2,2,2,2`, exactly the existing source table.
Keep one table owner for the paired profile bodies.

## Writer model and remaining alternatives

The sparse booster mask selects nonzero IDs. This is a **source model**,
supported by zero-default/absent entries and `boosters_enum` IDs 1 through 11;
`get_booster_value` scans those IDs and returns the matching value, otherwise
zero. It does not prove the original writer selected by ID rather than value,
serialized all entries, or asserted additional invariants.

Likewise the model sends slots with nonzero item ID, in ascending index order.
The retained inventory cook and lobby UI use that ID as the occupied-slot
test, but those are not original packet writers. Sending all slots or choosing
a different sparse policy can also be compatible with the reader. This writer
models a fresh profile snapshot; since the reader does not clear absent slots,
it must not be advertised as an arbitrary update/removal protocol on an already
populated profile. A missing sparse slot cannot by itself clear an old item.

No account/profile IDs, structure padding or extra EOF/count marker are sent.
Only the name's length-prefixed bytes are serialized. The raw array is cast
to `pcstr`, selecting the actual string overload. The new profile body lives
beside the retained reader in `game_net_defines.h`; its class header has the
inline declaration, not a second definition/table.

Condition/stack is stored in a u32 member but this particular wire format
uses u16. The writer explicitly narrows it; whether the original writer
asserted its range, clamped it, or simply narrowed remains open. Do not change
the PDB member type to u16 to fit one protocol.

## Similar-looking consumers that must stay distinct

`network_client::process_shop_action` reads a u16 dictionary ID, a u32 item ID,
and a **u32** condition/stack update, then sets amount to zero. Its target
uses a four-byte third read and adds that value to the found item. Do not
propagate the profile-slot reader into this differently formatted message.

`lobby_client::read_enumerate_inventory_info` reads a count, resizes the vector,
then reads `count * sizeof(inventory_item_instance)` raw bytes. That is a third
protocol, including the 16-byte record layout, not a stream of the compact
profile records. It remains separate too.

Model-only Python checks exercised all 2,048 booster masks, empty/single/all
slot streams, dictionary/instance IDs above 255, explicit 32-to-16-bit
condition narrowing, the 8/10/12-byte valid item modes, and six-byte headers
for invalid modes. They check format arithmetic and the model, **not** compiled
C++ behavior, original sender bytes, or fresh statement matching. `git diff
--check` passed. The three original sending consumers/policies remain tracked
with source TODOs and review-table entries.
