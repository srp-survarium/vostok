<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Account-money and service-price inline records

PR 571 source-only follow-up covers all **three declared methods** in these
two records. Two empty packet helpers receive bodies; the existing money
constructor loses a stale marker. One live reader now calls the declared
helper. No compilation, score refresh or post-edit structure verdict.

## Individual register

| # | Function | Disposition and evidence |
|---:|---|---|
| 1 | `account_money::account_money()` | Keep the existing zero initializers for two u32 fields and one u8. Retail lobby construction and temporary assignment during profile clearing independently establish those stores. Do not clear the three padding bytes or redirect the skill byte to the separate leveling record. |
| 2 | `service_prices::serialize(tcp_packet&) const` | Restore an explicit inverse-wire model: append reroll, add-profile and rename-account prices as three u32 values. The actual receiving consumer verifies widths/order, not the original writer's append granularity, guards or call site. No artificial sender is added. |
| 3 | `service_prices::deserialize(packet_reader&)` | Restore three sequential u32 reads into the corresponding fields and use the helper in lobby_client::read_service_prices. All data operations are present in retail; the PDB declaration and caller line partition support this inline boundary without proving its exact original spelling. |

The following consumers and implicit record operations are supporting evidence,
not additional entries in this register. Lobby-client getters were already
counted in [the earlier audit](lobby_client_inline_recovery.md).

## Raw PDB interface, including declaration order

Retail `account_money` is complete TPI 0x6ae4, field list 0x6ae3, size 0xc.
Its explicit public constructor has attributes 0x0003 and type 0x6ae2;
public generic_money/premium_money/total_skill_points follow at offsets
0/4/8, with types u32/u32/u8. Preserve this order and the existing initializer
list. There are no separate copy/assignment/destructor records to manufacture.
Ordinary implicit record operations remain compiler-owned.

Retail `service_prices` is complete TPI 0x6aff, field list 0x6afe, size 0xc.
Its **three public u32 fields precede the two methods** in the raw field list.
The rendered target header moves methods above fields; that rendering is not
the declaration-order oracle. Keep the source's existing field-first order.
Both methods have public attributes 0x0003, with serialize type 0x6afb and
deserialize type 0x6afd. Preserve serialize's const qualification, mutable
reader reference and void returns. No constructor is recorded: do not add
zero initialization, ownership, padding fields or extra generated-method
declarations to this trivial record.

## A live inline reader hidden by open-coded assignments

`lobby_client::read_service_prices`, retail RVA 0x5b77d0, is 0x38 bytes.
At offsets +0x00, +0x12 and +0x23 it repeats the reader operation: load
packet_reader::m_pointer at +4, load one dword, advance/store that pointer by
four, then store into lobby offsets +0x860/+0x864/+0x868. The fields are
reroll_cost, add_profile_cost and rename_account_cost, in that order. At
+0x34 it returns true. There is no count prefix, condition, byte swap or bulk
copy in this retained receiving path.

Retail records only the return at source line 498 as an addressed caller
statement. The frozen candidate at RVA 0x5bc7b0 has the same 0x38-byte extent
but additionally records the second and third assignments at +0x12/+0x23.
The two-sided structure view therefore reports two BASE_ONLY statements.
Both record only the reader parameter as a named local.

This is positive caller evidence for the PDB-declared deserialize helper,
not an inference of non-use from its absent standalone symbol. Move the three
operations into that helper and call it once before returning true. Do not
collapse source lines or add force-inline attributes to obtain a partition.
The exact original named boundary and resulting candidate line projection
remain a source TODO pending the eventual batched build.

`network_client::on_lobby_packet_received` already dispatches this reader
when the received query type equals 0xa. `lobby_menu::fill_service_prices`, RVA
0x731e60, obtains the stored record through the client getter, loads the
first dword and presents it as root.set_reroll_cost. These consumers preserve
the record's owner and first-field meaning; they do not establish a new
outgoing service-price message. Current source references and the local
history beginning at 750537940 provide no earlier writer body. Clangd's
pre-edit deserialize references returned its declaration only, while the
retail caller visibly performs its operations: navigation alone cannot
classify an inline as unused.

The writer appends the three individual u32 fields rather than inventing a
message header or sending sizeof(service_prices) from an arbitrary address.
This is a model consistent with the observed wire, not independent evidence
of the unobserved writer's source. Its original consumer, guard and append
boundaries remain explicitly open.

## Money initialization, padding and the other skill byte

`lobby_client::lobby_client`, RVA 0x5b8a70, stores zero dwords to +0x828
and +0x82c at offsets +0xeb/+0xf1, then a zero byte to +0x830 at +0xf7,
before its first addressed body statement at +0xfd. This supports the
existing account_money initializer list, not body-level clearing or memset.
No corresponding +0x860/+0x864/+0x868 service-price initialization appears.

`lobby_client::clear_profile_info`, RVA 0x5b7a60, separately constructs a
12-byte stack temporary: zero dwords at +0x3c/+0x40 and a zero byte at +0x4a.
It then copies the first eight bytes and the final dword into the stored
account_money at +0x52/+0x5b. The final dword includes three uninitialized
padding bytes. Keep the existing temporary assignment and implicit copy;
do not replace it with whole-record zeroing. Other clear_profile_info
statement/allocator differences are not claimed resolved by this audit.

`read_account_money`, RVA 0x5b7930, reads the two monetary dwords into
+0x828/+0x82c, but its subsequent u8 goes into **+0x834**, the separately
owned player_leveling_info::total_skill_points. The old account_money byte
is at +0x830. The current source already preserves this distinction. Its
coincident name is not permission to change the destination or invent an
account_money deserialize method that the PDB does not declare.

## Deferred validation

Check all service_prices header users and the restored reader's statement
partition during the later batched build. Confirm the wire remains exactly
three u32 values and preserve the existing true result. No new build result,
match percentage or whole-lobby closure is asserted here.
