<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Messaging-client inlines and account-list ownership

PR 571 source-only pass. All eight messaging-client inlines and the account
item's equality/default-construction/copy-construction/assignment/destruction
are individually covered: thirteen entries. Nine literal markers are removed,
not nine proven new algorithms. No compilation or score refresh.

## Individual register

| # | Function | Disposition |
|---:|---|---|
| 1 | `messaging_client::set_local_name` | Copy-only model into the existing 32-byte array using `strings::copy`. No surviving named caller was found. Original copy/length policy and whether this setter also notified chat remain open; do not route packet decoding through an invented extra copy. |
| 2 | `connection_info` | Existing mutable-reference getter verified through `network_client::on_http_result_ready`. After virtual messaging-client lookup, target writes need_resolve at +0x84 and increments error count at +0x80: connection record +8, fields +0x7c/+0x78. Keep reference, not a value copy. |
| 3 | `get_friend_list` | Existing const-reference getter verified in `lobby_menu::fill_friend_list`: target adds +0x140 to the returned messaging client, then walks the actual vector. |
| 4 | `get_ignore_list` | Existing const-reference getter verified in `fill_ignore_list`, client+0x150. No new filtering or copy. |
| 5 | `get_found_players_list` | Existing const-reference getter verified in `fill_found_players`, client+0x160. Preserve stored results and reference lifetime. |
| 6 | `connected` | Replace false placeholder with `m_connection_state == messaging::client_connected`. Ten live consumers compare client+0x88 to 3. Use the declared helper at all ten guards; operation is verified, original named inline boundary remains an inference. This is messaging sign-in state, not the lower-level TCP state. |
| 7 | `local_user_name` | Existing pointer getter returns the member array at +0x118. Restore it at outgoing UTF-16 conversion and sign-in chat notification; target passes exactly this address. Named boundary remains inferred; the writable packet-reader destination stays the array itself. |
| 8 | `local_player_team` | Existing enum getter verified through `chat_handler::set_mode`: compare returned messaging client+0x13c against zero/team_1, select channel 6 or 7. Producer `assign_match_channel_order` stores team at +0x13c. Preserve the early return for unchanged/invalid match ID. |
| 9 | `account_list_item::operator==(u32)` | Existing ID equality verified inside retained STL `__find` at RVA 0x97370. It compares the dword at item+0 with the requested ID and advances 0x34 bytes, with no online/name criterion. Public nonconst method and const value parameter are preserved. |
| 10 | Implicit `account_list_item` default constructor | Keep compiler-generated. Retail resize's temporary constructs the embedded fixed string with begin/end/capacity pointers and a NUL. No account_id/online default stores are observed there; do not invent zero initialization or an explicit empty constructor. Raw constructor method attributes are compiler-generated. |
| 11 | Implicit copy constructor | Keep generated member copy. `_Copy_Construct` at RVA 0x98160 copies ID and online, establishes destination-local fixed-string pointers, copies characters and terminates. A raw 0x34-byte memcpy would preserve pointers into the wrong object. |
| 12 | Implicit copy assignment | Keep generated member assignment. STL `__copy` at RVA 0x98060 preserves destination string storage, handles string self-assignment, copies ID/name/online and advances by 0x34. Raw method attributes also mark this generated. |
| 13 | Implicit destructor | Keep trivial cleanup: fixed string owns inline character storage, not a heap allocation. Messaging destruction at RVA 0x7561c0 releases the three vector allocations in reverse member order and then destroys the TCP client; there is no item-level name deletion. |

## Live state machine and callback evidence

The constructor at RVA 0x7566a0 stores state 1 (`client_disconnected`) at
+0x88 and registers the actual connected/disconnected/error callbacks.
`on_connected`, RVA 0x7565b0, stores state 2 (`client_connecting`), binds
`sign_in_on_packet_received`, then sends the existing authentication packet.
The sign-in callback at RVA 0x789b20 checks message 0xcb, copies the byte-length
name from the reader into +0x118, clears connection errors at +0x80, stores
state 3 and replaces the receive callback with `on_packet_received`.

The direct-call xref query for `sign_in_on_packet_received` returns no callers.
Its address-taking bind in `on_connected` proves this is still live; a missing
direct-call edge does not establish an unused callback.

These ten guards all test the same +0x88 state word against 3:

| Consumer | Target RVA | Guard offset |
|---|---:|---:|
| `on_message_typed` | 0x5b92f0 | +0x94 |
| `query_for_friend_list` | 0x5b9280 | +0x03 |
| `query_for_friends_status` | 0x5b9200 | +0x09 |
| `add_to_friend_list` | 0x5b9170 | +0x03 |
| `remove_from_friend_list` | 0x5b90e0 | +0x03 |
| `query_for_ignore_list` | 0x5b9070 | +0x03 |
| `add_to_ignore_list` | 0x5b8fe0 | +0x03 |
| `remove_from_ignore_list` | 0x5b8f50 | +0x03 |
| `find_players_by_name` | 0x5b8ea0 | +0x09 |
| `update_channel_subscriptions` | 0x789a60 | +0x03 |

The eight list/query consumers already match frozen target statement partitions
and extents. Their raw private-member tests are now expressed through the
recovered public predicate without changing early returns or packet formats.
The frozen executable already contains the correct tests: this is restoration
of the missing helper boundary, not a claim that live chat previously always
failed. `connected()` itself was an uncalled false stub in this source tree.

Do not broaden this predicate to `state != disconnected`, which would accept
connecting/disconnecting, or delegate to the TCP connection predicate, which
would accept an unauthenticated messaging session.

## Name, list and generated-method evidence

The outgoing conversion at `on_message_typed+0xa3` passes client+0x118 to
`mbstowcs_s`; sign-in +0x25c passes the same pointer to
`chat_handler::set_local_player_name`. The preceding packet read at sign-in
+0x111..0x12e writes the array directly and adds its NUL terminator. There is no
observed call selecting the new setter's CRT/engine-copy spelling or extra
notification policy. Its hypothesis therefore stays isolated from live reads.

Friend-status reading at RVA 0x756220+0x41 loads vector begin/end at
+0x140/+0x144 and invokes the actual account-ID `__find` instantiation.
`accept_message_from` also searches the ignore vector for account senders.
Keep these existing STL consumers, rather than replacing the equality predicate
with a generic name/online comparison.

Raw retail class TI 0x6af6 has field list 0x6af5. Its constructor overload list
0x6af2 contains method TIs 0x6af0 (one argument) and 0x6af1 (zero arguments),
both with attributes 0x0103 (public, compiler-generated). The assignment entry
has the same 0x0103 attributes; user equality has 0x0003. This distinguishes
implicit ownership operations from the actual written comparison method.

Both `messaging_client` and `account_list_item` have one complete semantic
variant on each side and topology reports no differences. That comparison
includes real access bits; keep private callbacks/data and private
`boost::noncopyable` inheritance. Remove the obsolete source comment claiming
the callback dump prints public. Generated header base-access rendering remains
the separately documented tooling issue, not a reason to change inheritance.

## Deferred checks and open questions

No build ran. `on_message_typed` still has an existing frozen 33-target versus
35-base statement discrepancy; `update_channel_subscriptions` has 6/6
statements but 0xb7/0xab bytes. Those are not newly measured regressions, and
this inline pass does not claim they are resolved or a linker wall.

Later validation must check all changed-header users and the ten guard/two
name-getter consumers. Source TODOs and the review ledger retain the setter's
copy/notification policy and the two original named-boundary questions.
The engine-wide reconstruction remains in progress.
