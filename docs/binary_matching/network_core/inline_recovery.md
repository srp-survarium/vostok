<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Network-core no-source audit

This is the exhaustive disposition of the 71 function bodies that PR 569 left
as `/* no source */` or as fake `return false`, `return 0`, or `return NULL`
implementations. Every one now has executable semantics. The table deliberately
separates source evidence from semantic inference: an inline declaration in the
PDB proves the seam and its access, but does not by itself prove the body.

Evidence labels used below:

- **retail expansion**: the operation is visible in a shipped client caller;
- **live sibling**: a retained engine path performs the same operation;
- **ancestor**: the parked `temp/network_legacy` implementation supplies the
  corresponding Vostok design;
- **ownership**: member types, access, inverse operations, or allocator ownership
  constrain the implementation;
- **server model**: the client PDB supplies the declaration and layout, while the
  body is reconstructed from the live connection layer and server invariants.

## Packet and allocator primitives (5)

| Declaration | Recovered behavior | Evidence |
|---|---|---|
| `packet<T>::clear` | reset `m_buffer_size` to zero | ownership; `clone` establishes zero as the empty state and append/reserve use the same size field |
| `udp_match_packet::helper::call_constructor` | placement-construct `udp_match_packet` in the supplied storage | live sibling; `new_udp_match_packet` performs the same placement construction |
| `udp_match_packets_allocator::increment` | interlocked increment of `m_reference_count` | retail expansion; intrusive-pointer policy performs the identical private-field update |
| `udp_match_packets_allocator::decrement` | interlocked decrement of `m_reference_count` | retail expansion; inverse of `increment` and identical intrusive-pointer policy update |
| `udp_network_flow_emulator_options::udp_network_flow_emulator_options` | null logging id and zero every numeric option | ownership; zero is the disabled/default state consumed by the emulator constructor |

## Async connector legacy seams (3)

These private helpers have no calls in the shipped client: the live
`async_connector` procedures open-code the work. Their placement in the `.cpp`
keeps the private seam without changing the class definition order.

| Declaration | Recovered behavior | Evidence |
|---|---|---|
| `async_connector::resolve` | allocate the resolver, create the query, and start `async_resolve` with the class handler allocator | ancestor plus live `connect`/`on_resolved` path |
| `async_connector::close_connection` | shutdown and close the socket through error-code overloads | ancestor plus live `reset`/connection teardown |
| `async_connector::on_error` | reset state and invoke the stored error callback | ancestor plus the error branches in the live connector procedures |

## UDP connection helpers (10)

| Declaration | Recovered behavior | Evidence |
|---|---|---|
| `udp_match_connection::has_disconnection_initiated` | state is no longer `connected` | state machine and live `disconnect` branches |
| `udp_match_connection::is_disconnecting` | state is `initiating_disconnection` or `confirming_disconnection` | state machine and live disconnect packet handling |
| `udp_match_connection::set_disconnected` | assign the `disconnected` state | ownership; inverse transition is explicit in live connection code |
| `udp_match_connection::new_packet` | allocate from `m_packets_allocator`, then call `construct_packet` | live sibling; `new_low_level_packet` uses the same allocator/constructor pair |
| `udp_match_connection::set_max_packet_wait_time_in_ms` | assign `m_max_packet_wait_time_in_ms` | ownership; live send/retry logic reads that field |
| `udp_match_connection::are_there_any_queued_packets` | test `m_packets_to_send.empty()` | live container ownership and server/client forwarding API |
| `udp_match_connection::last_send_time_in_ms` | return `m_last_send_time_in_ms` | ownership; live send path writes the field |
| `udp_match_connection::last_activity_time_in_ms` | return the maximum of last send and receive times | ancestor and the paired time-accessor family |
| `udp_match_connection::pending_operations_count` | return `m_pending_operations_count` | ownership; async handlers maintain the field |
| `udp_match_connection::disconnect_impl` | forward to `disconnect()` | ancestor seam; no client caller expands this retained private wrapper |

`disconnect_impl` is the weakest item in this group: the current shipped
`disconnect` procedure already structure-matches while open-coding its state
transition, so the client cannot establish whether an unused historical wrapper
called `disconnect` or carried an earlier implementation.

## UDP client convenience methods (2)

| Declaration | Recovered behavior | Evidence |
|---|---|---|
| `udp_match_client::handle_send` | report socket errors or zero-byte sends through `on_error` | live sibling; `udp_match_connection::handle_send` has the same Asio error contract |
| `udp_match_client::send` | `async_send_to` the packet buffer with `make_custom_alloc_handler` | live sibling; the connection send path uses the same socket, endpoint, bind, and handler allocator |

Neither helper has a client xref: actual match traffic is sent by
`udp_match_connection`. They remain legitimate convenience seams rather than
constant-return shams.

## Statistics helpers (11)

| Declaration | Recovered behavior | Evidence |
|---|---|---|
| `udp_match_items_stats::operator+=` | add `count` and `bytes` | ownership and the existing subtraction/comparison operators |
| `udp_match_items_stats::operator/=` | divide `count` and `bytes` | ownership and server averaging contract |
| `udp_match_items_stats::reset` | zero `count` and `bytes` | constructor and ownership |
| `udp_match_stream_stats::dump` | log packet, message, and data-byte totals | field ownership; exact server-only log spelling is not client-provable |
| `udp_match_stream_stats::operator+=` | add packets, messages, and data bytes | ownership and existing subtraction/comparison operators |
| `udp_match_stream_stats::operator/=` | divide packets, messages, and data bytes | ownership and server averaging contract |
| `udp_match_stream_stats::reset` | reset packet/message aggregates and data bytes | constructors and ownership |
| `udp_match_stats::dump` | dump all six streams and the two scalar counters | field ownership; exact server-only log spelling is not client-provable |
| `udp_match_stats::operator+=` | add streams/unacknowledged count and retain the greatest sequence difference | existing subtraction/comparison semantics and meaning of `max_` |
| `udp_match_stats::operator/=` | divide streams and unacknowledged count, retaining the peak maximum | server averaging semantics; a maximum is not an additive average |
| `udp_match_stats::reset` | reset every stream and both scalar counters | constructors and ownership |

## UDP server-side client session (16)

No procedure or inline expansion for `udp_match_client_session` exists in the
shipped client. These bodies are a coherent server model derived from the class
layout, the live connection API, and the parked server ancestor.

| Declaration | Recovered behavior | Evidence |
|---|---|---|
| `udp_match_client_session::udp_match_client_session` | construct the connection with server policy, retain endpoint, initialize destroy-list link, and mark the accepted peer connected | ancestor plus live client policy; a newly inserted server session must not remain in the connection's default `disconnected` state |
| `udp_match_client_session::~udp_match_client_session` | empty body; owned members perform teardown | ownership and RAII member destructors |
| `udp_match_client_session::on_packet_received` | delegate to the connection while binding this session ahead of message type/reader | server model; callback type fixes the adapter shape |
| `udp_match_client_session::enqueue` | forward to `m_connection.enqueue` | paired forwarding family |
| `udp_match_client_session::send_queued_packets` | forward current time to the connection | paired forwarding family |
| `udp_match_client_session::instant_disconnect` | connection-level initiator disconnect | server lifecycle and disconnect enum semantics |
| `udp_match_client_session::new_packet` | forward message type to the connection | paired forwarding family |
| `udp_match_client_session::delete_packet` | forward packet reference to the connection | paired forwarding family and allocator ownership |
| `udp_match_client_session::are_there_any_queued_packets` | forward connection queue query | paired forwarding family |
| `udp_match_client_session::unacknowledged_packets_count` | forward connection count | paired forwarding family |
| `udp_match_client_session::last_send_time_in_ms` | forward connection send time | paired forwarding family |
| `udp_match_client_session::last_receive_time_in_ms` | forward connection receive time | paired forwarding family |
| `udp_match_client_session::last_activity_time_in_ms` | forward connection activity time | paired forwarding family |
| `udp_match_client_session::is_disconnected` | forward connection state query | paired forwarding family |
| `udp_match_client_session::is_ready_to_be_destroyed` | require disconnected state and zero pending operations | async object-lifetime invariant and ancestor lifecycle |
| `udp_match_client_session::on_error` | disconnect as `disconnected_by_connection_lost` | error enum semantics and live client disconnect path |

The already-correct `get_stats` and `endpoint` accessors were controls, not
baseline placeholders, so they are not included in the count.

## UDP match server (24)

The retail PDB provides two server type variants. One declares `tick`; another
declares `remove_disconnected_clients` and exposes a differently placed
`send_queued_packets`. They must not be combined into a fabricated third class.
This reconstruction retains the variant already represented in source and
models disconnected-client removal inside `tick`.

| Declaration | Recovered behavior | Evidence |
|---|---|---|
| `udp_match_server::comparer::operator()(session, session)` | compare endpoints | intrusive-set key semantics and endpoint accessor |
| `udp_match_server::comparer::operator()(endpoint, session)` | heterogeneous endpoint comparison | `m_clients.find(endpoint, comparer())` contract |
| `udp_match_server::comparer::operator()(session, endpoint)` | reverse heterogeneous endpoint comparison | intrusive-set comparer contract |
| `udp_match_server::destroy_predicate::operator()` | retain non-ready sessions; delete ready sessions through the server virtual seam | destroy-list ownership and ancestor lifecycle |
| `udp_match_server::udp_match_server` | initialize socket/references plus time and accepting/reopen state | member ownership and ancestor |
| `udp_match_server::~udp_match_server` | stop I/O, detach clients, disconnect, and destroy both ownership queues | server lifecycle and ancestor |
| `udp_match_server::tick` | update time, reopen if needed, tick emulator, retire disconnected clients, reap deferred clients, send queues | alternate PDB method variant, ancestor, and live connection lifecycle |
| `udp_match_server::are_there_any_queued_packets` | return true if any session queue is nonempty | set ownership and session forwarding API |
| `udp_match_server::clients_count` | return intrusive-set size | ownership |
| `udp_match_server::get_stats` | accumulate session stats and average when nonempty | stats operators and server aggregation contract |
| `udp_match_server::set_on_packet_received` | assign callback | ownership and callback dispatch site |
| `udp_match_server::start_accepting` | open/bind UDP socket, mark accepting, start receive | ancestor plus live UDP client receive setup |
| `udp_match_server::stop_accepting` | clear accepting/reopen state, cancel and close socket | ancestor plus Asio lifetime symmetry |
| `udp_match_server::enqueue` | forward packet to the selected session | ownership and session API |
| `udp_match_server::delete_client` | erase, disconnect, defer destruction, null caller reference | intrusive ownership and pending-handler lifetime invariant |
| `udp_match_server::send_queued_packets` | forward current time to every session | session API and set ownership |
| `udp_match_server::new_client` | allocate a session through `network_core::g_allocator` and pass owned dependencies | project `VOSTOK_NEW_IMPL` policy and constructor ownership |
| `udp_match_server::delete_client_impl` | destroy through `network_core::g_allocator` | inverse allocator ownership and `VOSTOK_DELETE_IMPL` policy |
| `udp_match_server::process_incoming_packet` | find/create a session by endpoint, insert it, dispatch reader/callback | comparer contract, callback type, and ancestor |
| `udp_match_server::start_receiving` | issue allocator-backed `async_receive_from` into the server buffer | live UDP client receive pattern |
| `udp_match_server::unacknowledged_packets_count` | sum session counts | flow-emulator input contract |
| `udp_match_server::handle_receive` | handle cancellation/errors/zero bytes, emulate or parse, dispatch, and restart receive | live UDP client handler plus server ownership |
| `udp_match_server::on_error` | request socket reopen while accepting | reopen state ownership and ancestor recovery policy |
| `udp_match_server::try_reopen_socket` | preserve endpoint, close/open/bind, clear flag, restart receive | socket lifecycle and ancestor recovery policy |

## Result and proof boundary

The audit leaves zero `/* no source */`, `STATE[STUB]`, `STATE[REMOVED]`, or
fake constant-return bodies in `sources/vostok/network_core`. The older
`STATE[UNMATCHABLE]` comments on implemented TCP/UDP convenience seams, plus the
server-web marker, remain because they accurately describe absent client xrefs,
not missing semantics.

The 31 non-server-web primitive/connector/connection/client/stats bodies are
recoveries with varying local evidence; several retained legacy seams still lack
client xrefs, as their rows say. The 40 session/server bodies are managed semantic
reconstructions, not byte-match claims: their exact statement shape, logging
text, and server-variant source partition require the dedicated-server binary or
original source. No build was run for this batch, at the user's explicit request.
