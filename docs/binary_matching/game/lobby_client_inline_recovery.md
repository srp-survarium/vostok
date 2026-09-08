<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Lobby-client inlines, inventory insertion and UI count widths

PR 571 source-only review of **all 32 inline methods** in `lobby_client.h`,
including the three unmarked callback setters. Twenty-nine literal markers
are removed. This count includes existing getters and explicit models, not
32 newly recovered algorithms. No build or score refresh.

## Individual register

| # | Function | Disposition and consumer evidence |
|---:|---|---|
| 1 | `set_on_packet_received` | Existing callback assignment verified in network-client construction: bind on_lobby_packet_received, assign parent+0x348 = lobby+0x130. Sign-in installs that stored callback into the TCP client. Do not invoke it from the setter or forward it prematurely. |
| 2 | `set_on_connected` | Existing assignment verified at parent+0x368 = lobby+0x150. It is invoked after successful sign-in, not directly upon transport connection. |
| 3 | `set_on_disconnected` | Existing assignment verified at parent+0x388 = lobby+0x170. Disconnect handlers test and invoke the stored callback after clearing the flag. Keep Boost value ownership. |
| 4 | `net_connected() const` | Existing byte getter at +0x194 verified in ping and external lobby consumers. Restore it in ping_server. Sign-in sets it true; disconnect clears it. Do not substitute raw TCP connectivity or m_status. |
| 5 | `status() const` | Keep existing enum value read at +0x190. The const status-formatting method reads/returns this field. A separately named no-argument const expansion is not identified; preserve its PDB interface alongside the mutable overload. |
| 6 | `status()` | Mutable enum reference verified through on_lobby_packet_received's store of in_match=3 at +0x190. This reference is a real write seam, not an accessor that can return by value. |
| 7 | `match_id() const` | Keep existing u32 field read at +0x198; const status formatting consumes it. Preserve the separately declared const overload even though ordinary mutable callers select the reference overload. |
| 8 | `match_id()` | Mutable reference verified by packet-derived write at +0x198 and subsequent messaging-channel assignment reads. |
| 9 | `team_id() const` | Keep existing enum field read at +0x1a0; matching producer/consumer field accesses and PDB declaration support it, without proving a separate named const invocation. |
| 10 | `team_id()` | Mutable enum reference verified by byte-read/enum-store at +0x1a0 and channel-order consumers. Do not change the return to u8 just because the wire value is one byte. |
| 11 | `match_order_id() const` | Keep existing +0x19c getter; restore it at the existing discard-order packet writer. The stored field is u32, including the -1 sentinel. Original named boundary remains inferred. |
| 12 | `profiles_count` | Existing u8 getter at +0x25c verified through profile enumeration and fill_profiles. It is not the fixed capacity three. |
| 13 | `profile(u8) const` | Existing const reference into +0x260 with 0x1b8 stride, verified through enumeration/fill_profiles. Preserve index-based access, not lookup by profile_id. |
| 14 | `player_skills_count` | Existing byte count at +0x79c, verified in fill_character_data and packet producer. |
| 15 | `player_skill(u8) const` | Existing const reference through pointer +0x798, two-byte stride. The two bytes are skill ID and points; not a dense dictionary indexed by skill ID. |
| 16 | `player_perks_count` | Existing byte count at +0x848, verified in the UI loop and packet producer. |
| 17 | `player_perk(u8) const` | Existing byte-indexed value read through +0x844. Do not manufacture a wider perk record. |
| 18 | `slot_restrictions_count` | Existing u32 getter at +0x850 is correct. Repair the UI caller that truncated it into a u8 temporary; can_move_item also consumes the full field. |
| 19 | `slot_restriction(u32) const` | Existing const reference through +0x84c, two-byte slot/category record, verified by both permission checks and UI serialization. |
| 20 | `item_compatibilities_count` | Existing u32 getter at +0x858 is correct. Repair the second UI caller's u8 count narrowing; check_compatibility uses the full field. |
| 21 | `get_items_compatibility(u32) const` | Existing const reference through +0x854, four-byte pair of u16 IDs. check_compatibility tests both orientations; the getter itself does not reorder/deduplicate them. |
| 22 | `inventory_item_instances` | Existing mutable vector reference at +0x788 verified by shop updates and inventory UI. Preserve container ownership and mutation access. |
| 23 | `add_inventory_item(inventory_item_instance)` | Restore push_back of the value argument and use it in the shop handler's absent-item branch. PDB by-value type and retail's separate 16-byte copy provide strong source-seam evidence. Duplicate stack increment and UI/account-money refresh remain in the caller. |
| 24 | `connection_info` | Existing mutable reference at +0x24. HTTP failure stores need_resolve at lobby+0xa0 and increments error count at +0x9c. Preserve writes to the actual record. |
| 25 | `get_account_money` | Existing const record reference at +0x828. UI reads generic/premium money at +0x828/+0x82c. No synthetic aggregation or copy. |
| 26 | `get_player_leveling` | Existing const record reference at +0x834. UI reads total_skill_points as a byte, then experience fields at +0x838/+0x83c/+0x840. Preserve mixed field widths. |
| 27 | `get_service_prices` | Existing const record reference at +0x860, verified by fill_service_prices and the three-u32 reader. |
| 28 | `skills_tree_config` | Existing mutable resource-pointer reference at +0x85c. fill_skills_tree loads the stored resource and its root; keep the declared reference, not a by-value owning copy or raw-pointer return. |
| 29 | `get_player_faction_reputation(u8)` | Replace zero placeholder with a first matching faction_id lookup over the received reputation array, returning u16 points widened to u32; missing key returns zero in this model. No consumer establishes original duplicate/miss policy. Do not treat faction ID as an array index or reinterpret points as a reputation level. |
| 30 | `get_player_reputation(u8)` | Existing const reference through +0x7a0 with four-byte stride. UI reads faction byte +0 and points word +2. Distinct from the faction-key lookup model. |
| 31 | `get_player_reputations_count` | Existing byte count at +0x7a4, verified in packet allocation and UI loop. |
| 32 | `get_player_name() const` | Keep fixed-string reference at +0x230. Enumeration copies profile zero's name into that stored string, but no named getter consumer was found. Original consumer remains open; don't alias account_nickname_, return a temporary or infer editor/server ownership. |

## By-value insertion is visible in the retained consumer

`network_client::process_shop_action`, target RVA 0x5b69f0, decodes the
operation byte, u16 dictionary ID, u32 item ID and u32 condition/stack value.
It initializes amount_in_inventory to zero, walks the stored inventory, and
adds to the matching stack at +0xc3. Keep that duplicate path outside the new
insertion helper.

The missing-item path at +0x73 copies both eight-byte halves of `new_item`
from stack+0x10/+0x18 into stack+0x20/+0x28 before looking up the lobby client.
The overflow insertion at +0xc7 passes this second item object. The fast path
can copy from the original storage, but the separate value copy is still
present. The independently fetched frozen candidate has only one item object
and pushes directly into the vector. Restoring the PDB-declared value-parameter
helper explains this copy without inventing a local or changing its parameter
to const reference.

Both frozen sides record only packet and new_item as named locals. Retail has
14 statements / 0xff bytes versus candidate 11 / 0xfb. Its begin/end loads
have separate statements, followed by an explicit scalar walk; current source
uses std::find. The old claim that this was certainly the same source and an
uncontrollable STL/LTCG wall is removed. Search/loop source partition remains a
specific TODO, not solved by the insertion restoration or by adding invented
named iterators. No post-edit byte result is claimed.

## Two genuine narrowed-count discrepancies

The UI handlers at target RVAs 0x732c70 (compatibility) and 0x732a40
(restrictions) both compare the **u32 stored count**, at offsets +0x858 and
+0x850 respectively. Their initial compare at +0x50 and loop-back compare at
+0x1a2 access the dword directly. The frozen candidate instead loads one byte
at +0x43 into a cached u8 count. The source's count temporaries cause that
truncation; the getter return types were already correct.

Remove the two narrowed temporaries and call the u32 getters in the loop
conditions. Keep `u8 i`: retail PDB records that exact type, and +0x193..0x19e
loads the byte counter, increments AL, zero-extends it and stores the byte back.
With an unchanged count at least 256, this counter would wrap; do not silently
widen it or clamp the count while matching. No claim is made that production
server data reaches that case.

Target names four locals in each handler (array, property, u8 i, item), while
the frozen base records three. Both sides nevertheless have fourteen raw
statements. Aggregate statement equality did not exonerate the narrowed count.

The target also captures the element pointer at +0x60/+0x6c, before the
CreateObject call at +0x8c. Original reference/construction boundaries and the
remaining member-call partitions are still open: do not invent a named alias
solely to force this hoist. These are recorded at both UI anchors and in the
review ledger; the count correction does not claim whole-handler equality.

## Callback ownership, access and bounded models

Network-client construction at RVA 0x6f5000 binds its three lobby handlers and
assigns the stored callback objects at +0x348/+0x368/+0x388. These are the
embedded lobby client's +0x130/+0x150/+0x170 members. Successful sign-in at
0x5b8120 passes the stored receive callback to the lower-level client, sets
+0x194 true and invokes the connected callback. Transport connection first
installs sign-in handling instead; none of these setters should immediately
send, call the handler, or replace this authentication sequence.

Topology reports one complete semantic variant on each side for lobby_client,
with no differences. Preserve its private core::noncopyable base, private
callback/data members, public account_nickname_ and all overload/CV/width
distinctions. Remove the obsolete comment claiming the callback dump says
public; the generated base-access rendering issue is tracked separately.

Reputation decoding at RVA 0x5b85f0 reads a u8 count, allocates four bytes per
record and copies the raw records. `on_player_reputations_arrived` consumes the
faction byte and u16 points independently, then derives a level from dictionary
thresholds. This supports a keyed points lookup, but it does not prove the new
helper's first-match/zero fallback or any original caller. Its source TODO
retains those alternatives; the existing UI path is not replaced with it.

## Deferred validation

No compilation or derived-score update. Later checks must cover all header
users, especially by-value insertion, both full-width count loops, ping and
discard-order helper seams. Keep packet formats, duplicate-item policy and
stored reference lifetimes unchanged. The per-function register accounts for
every lobby inline; the explicit source-model and consumer-partition questions
remain open, and the full engine goal is not complete.
