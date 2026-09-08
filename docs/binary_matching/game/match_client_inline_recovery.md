<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Game match-client wrappers and queue ownership

PR 571 source-only review of all twelve inlines in the game-level
`survarium::match_client`, plus eight ownership/consumer anchors. Do not confuse
this wrapper with `vostok::network::match_client` or its network-core session.
Five literal markers are removed: four verified existing getters/forwarders and
one packet-deletion model. The private `on_connected` hook remains open.
No build or score refresh.

## Individual register: 20 declarations/consumer anchors

| # | Function | Disposition |
|---:|---|---|
| 1 | `are_there_any_packets_to_send` | Remove marker. Tick reads network_client+0x2e24 = wrapper+0x239c. Enqueue sets this flag and send_queued_packets clears it. This is not an underlying session's queue-size predicate. |
| 2 | `last_send_queed_packets_time_in_ms` | Remove marker; preserve retail spelling. Tick reads network_client+0x2e20 = wrapper+0x2398, adds 33 and compares to current time. Sender stores the supplied timestamp here. |
| 3 | `new_packet` | Existing enum-to-u8 forwarding retained. Connection-success consumer creates type 0x41 through the underlying client, then enqueues it. No new raw packet allocator or header construction is introduced at this layer. |
| 4 | `delete_packet` | Restore explicit model `m_client.delete_packet(packet)`. Underlying API uses the order-packet allocator and nulls the reference. No current outer deletion consumer was found; original wrapper expansion remains an open source-boundary question. |
| 5 | `set_on_packet_received` | Existing delegate retained. Game network-client construction binds its packet handler through this wrapper; the underlying client owns/stores the function object. No duplicate callback member. |
| 6 | `set_on_disconnect` | Existing delegate retained. Connection and close paths install/clear the underlying callback through this interface. Preserve that owner and its function-copy semantics. |
| 7 | `is_connected` | Existing delegate retained; retained tick calls underlying `network::match_client::is_connected` at +0x207 after obtaining the game wrapper. |
| 8 | `is_disconnected` | Existing delegate retained; tick similarly calls the underlying method at +0x194. Do not infer disconnected by negating connected: intermediate states exist below. |
| 9 | `get_stats` | Existing const-reference delegate retained. Current draw_stats uses this underlying statistics object; no copy, extra accumulator or session-pointer ownership is added. The outer source spelling is not independently separable from its inline expansion. |
| 10 | `last_receive_time_in_ms` | Existing delegate retained; tick calls the underlying reader at +0x265 and applies the 3000 ms timeout. Keep the timestamp owner below this wrapper. |
| 11 | `get_match_options` | Remove marker. Options live at wrapper+0xfc; network_client's virtual options accessor returns parent+0xb84 = +0xa88+0xfc. Match-info/profile and game UI consumers use this same mutable object. |
| 12 | `on_connected` | OPEN hook: four error enums suggest connect-result handling, possibly queue/options reset or notification. Current connect forwards the caller callback directly; this wrapper has no separate stored connect callback. No binding/consumer establishes this hook's operations, so no invented duplicate callback field or unconditional reset is added. |
| 13 | Constructor | Retained 0x5b7410 constructs underlying client with its orderer reference and null shipping emulator options, then orderer/options, timestamp zero and flag false. Preserve declared member order, including the reference passed before orderer construction. |
| 14 | Destructor | Retained 0x5b73e0 tail-jumps to underlying client destruction. Existing empty source body permits that generated member cleanup; no extra disconnect/reset is added. |
| 15 | `connect` | Retained 0x5b7470 creates packet 0x40, appends the four-byte session ID, and forwards host/port/time/packet/callback to the underlying client. No binding of row 12 appears in this live path. |
| 16 | `enqueue` | Retained 0x5b7400 calls underlying enqueue, then stores flag true at +0x239c. This existing public boundary handles the former private-field write. |
| 17 | `send_queued_packets` | Retained 0x5b7450 stores time at +0x2398, clears flag +0x239c, then calls underlying sender. Preserve this order rather than clearing after send. |
| 18 | `network_client::match_options` | Remove its marker and tentative comment. Exact-name target is `lea eax,[ecx+0xb84]; ret`; the existing two-layer reference forwarder returns that object. Other network-client placeholders remain outside this completed wrapper register. |
| 19 | `network_client::tick` queue consumers | At +0x2d8 reads flag; +0x2e1 reads last-send time; +0x2ee obtains the wrapper and sends. Existing public getters and sender are retained, not replaced by direct private reads. |
| 20 | `network_client::on_connected_to_match` enqueue expansion | At +0x79d selects embedded wrapper, creates packet 0x41, calls underlying enqueue and stores wrapper flag true. Current source already uses outer enqueue(new_packet(...)); remove stale invented friendship rather than exposing the flag. |

## Packet deletion is an ownership model, not a new observed leak fix

The lower `network::match_client::delete_packet` explicitly delegates to
`network_core::delete_udp_match_packet(*m_order_packets_allocator, packet)`.
That helper destroys the packet, deallocates it and sets the caller's pointer
to null. Response packets have a separate allocator/API. The outer wrapper
must not use plain delete, the response allocator, an enqueued packet's session
allocator, or a by-value pointer that loses the nulling effect.

No current source call or retained standalone procedure for the outer delete
wrapper was found. Its forwarding body is the bounded model supported by its
matching pointer-reference interface and underlying ownership, not a proven
runtime leak repair or original source expansion. Source/review TODOs retain
the missing consumer question.

## Callback and visibility boundary

`survarium::match_client::connect` passes the supplied callback directly into
`vostok::network::match_client::connect`. The latter owns its `m_on_connected`
function object privately; the outer wrapper has no such member. Do not fill
the old four-enum hook by reaching through that private member, adding another
field, or rebinding the live connection without evidence. Potential queue-state
changes remain hypotheses; the constructor/sender already establish their own
flag/timestamp behavior.

The removed `friend class network_client` was explicitly documented as an
invented aid for a direct queue-flag write. Current source has no remaining
external private write and uses the observed enqueue operation. Removing this
friend preserves all method access and exposes any future attempt to bypass
the actual queue boundary. It does not claim PDB absence alone proves that the
original developer never wrote a friendship.

Frozen topology finds one shared semantic variant of the 0x23a0 wrapper with
31 declarations; target has two equal records and base one. Do not confuse this
duplicate-record multiplicity with a layout/access mismatch. No method order,
field visibility, base access or data layout is changed in this batch.

Later validation: compile the restored delete forwarder and former-friend
consumers, then check queue/options accessors and callbacks in the batch report.
The remaining network-client packet handlers and connection predicate still
need their own individual audit; this register does not close that larger class.
