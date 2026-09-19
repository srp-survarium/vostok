<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Network-core no-source audit

At commit `f44ef8df4`, an exact `rg` over `network` and `network_core` found 67
literal `/* no source */` occurrences: 64 function-body annotations and three
umbrella comments on the UDP connection, session, and server classes. This is
the per-function disposition of that inventory, together with the fake
`return false`, `return 0`, or `return NULL` bodies in the same seams. Every
function now has executable semantics and every umbrella comment has been
replaced by a proof-boundary comment. The tables deliberately separate source
evidence from semantic inference: an inline declaration in the PDB proves the
seam and its access, but does not by itself prove the body.

Evidence labels used below:

- **retail expansion**: the operation is visible in a shipped client caller;
- **live sibling**: a retained engine path performs the same operation;
- **ancestor**: the parked `temp/network_legacy` implementation supplies the
  corresponding Vostok design;
- **ownership**: member types, access, inverse operations, or allocator ownership
  constrain the implementation;
- **server model**: the client PDB supplies the declaration and layout, while the
  body is reconstructed from the live connection layer and server invariants.
- **source-file table**: the target PDB's compilation-unit files and visible
  procedure line geometry constrain where an uninstantiated inline body lived.

## Marker-only annotations (7)

These functions already performed all of their work in an initializer list or
single store, or have a deliberately empty RAII body. Removing their marker does
not invent additional behavior.

| Declaration | Existing behavior | Evidence |
|---|---|---|
| `custom_alloc_handler::custom_alloc_handler` | bind the allocator pointer and copy the handler in the initializer list | emitted constructor is identical on both sides |
| `sequence_number::sequence_number(sequence_number const&)` | copy `m_number` in the initializer list | ownership and target declaration |
| `sequence_number::sequence_number(sequence_number_type)` | initialize `m_number` from the argument | ownership and target declaration |
| `tcp_packet::tcp_packet` | retain allocator and initialize allocated size to zero | ownership; destructor/reallocation use both fields |
| `tcp_packet_socket::tcp_packet_socket` | bind allocator/socket and zero the header buffer | ownership and live socket operations |
| `udp_match_packet::~udp_match_packet` | empty body; base/aggregate members own teardown | no standalone target symbol; inlined destruction is trivial |
| `udp_match_server::destroy_predicate::destroy_predicate` | bind the server reference | target PDB declaration, private noncopyable base, and reference ownership |

The predicate destructor is intentionally implicit. The target PDB marks it
compiler-generated; spelling an empty destructor produces a distinct qualifier
and declaration-order record even though both bodies do no runtime work.

The predicate copy constructor is also intentionally implicit. Both the retail
and current base type streams contain the same compiler-generated public copy
constructor at declaration order 1 and the explicit server-reference constructor
at order 2. The private `boost::noncopyable` base makes an attempted copy
unusable; it does not justify inventing an explicit derived copy constructor.
The active `remove_if` instantiation accepts the temporary without copying it.

## Packet and allocator primitives (6)

| Declaration | Recovered behavior | Evidence |
|---|---|---|
| `packet<T>::clear` | reset `m_buffer_size` to zero | ownership; `clone` establishes zero as the empty state and append/reserve use the same size field |
| `udp_match_packet::helper::call_constructor` | placement-construct `udp_match_packet` in the supplied storage | live sibling; `new_udp_match_packet` performs the same placement construction |
| `udp_match_packets_allocator::destroy` | destroy the allocator object through the allocator that owns it | retail expansion; inverse of allocation and project `VOSTOK_DELETE_IMPL` ownership policy |
| `udp_match_packets_allocator::increment` | interlocked increment of `m_reference_count` | retail expansion; intrusive-pointer policy performs the identical private-field update |
| `udp_match_packets_allocator::decrement` | interlocked decrement of `m_reference_count` | retail expansion; inverse of `increment` and identical intrusive-pointer policy update |
| `udp_network_flow_emulator_options::udp_network_flow_emulator_options` | null logging id and zero every numeric option | ownership; zero is the disabled/default state consumed by the emulator constructor |

## Async connector legacy seams (3)

These private helpers have no calls in the shipped client: the live
`async_connector` procedures open-code the work. The target object names
`async_connector.h` and `async_connector.cpp`, but CodeView does not identify the
physical source file of an unexpanded inline body. The recovered definitions
therefore live in `async_connector_inline.h`, following the module's established
header/`_inline.h` convention. This source home is a reviewed inference, not a
PDB `MATCH`; the evidence database reports source-body and unexpanded-inline
location as `UNOBSERVABLE`.

| Declaration | Source home | Recovered behavior | Evidence |
|---|---|---|---|
| `async_connector::resolve` | `async_connector_inline.h` | allocate the resolver, create an IPv4 query, and start `async_resolve` | ancestor and live `connect`/`on_resolved` path; IPv4 protocol proven in live `connect`, consistency applied to this unexpanded helper; source home reviewed inference |
| `async_connector::close_connection` | `async_connector_inline.h` | shutdown and close the socket | ancestor and live `reset`/connection teardown; source home reviewed inference |
| `async_connector::on_error` | `async_connector_inline.h` | reset state and invoke the stored error callback | ancestor and live connector error branches; source home reviewed inference |

## TCP marker and sham bodies (5)

| Declaration | Recovered behavior | Evidence |
|---|---|---|
| `tcp_packet_client::is_connected` | forward to `m_async_connector.is_connected()` | ancestor and retained connector state API; no shipped caller expands this seam |
| `tcp_packet_client::has_connection_established` | forward to `m_async_connector.has_connection_established()` | ancestor and retained connector state API; no shipped caller expands this seam |
| `tcp_packet_client::set_on_connected` | assign `m_on_connected` | retail expansion in the wrapper client constructor |
| `tcp_packet_client::set_on_disconnected` | assign `m_on_disconnected` | retail expansion in the wrapper client constructor |
| `tcp_packet_client::set_on_error` | assign `m_on_error` | retail expansion in the wrapper client constructor |

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

## UDP client convenience methods (8)

| Declaration | Recovered behavior | Evidence |
|---|---|---|
| `udp_match_client::new_packet` | allocate and construct through `m_connection` | paired connection API and allocator ownership |
| `udp_match_client::delete_packet` | forward the packet reference to `m_connection.delete_packet` | paired connection API and allocator ownership |
| `udp_match_client::are_there_any_queued_packets` | forward the connection queue query | paired connection API |
| `udp_match_client::last_send_time_in_ms` | forward the connection send timestamp | paired connection API and timestamp ownership |
| `udp_match_client::last_activity_time_in_ms` | forward the connection activity timestamp | paired connection API and timestamp ownership |
| `udp_match_client::handle_send` | report socket errors or zero-byte sends through `on_error` | live sibling; `udp_match_connection::handle_send` has the same Asio error contract |
| `udp_match_client::send` | `async_send_to` the packet buffer with `make_custom_alloc_handler` | live sibling; the connection send path uses the same socket, endpoint, bind, and handler allocator |
| `udp_match_client::construct_packet` | forward to `udp_match_connection::construct_packet` | target PDB fixes this as a public static inline seam; the connection implementation is the retained packet-construction authority |

The send pair has no client xref: actual match traffic is sent by
`udp_match_connection`. The forwarding accessors remain legitimate convenience
seams rather than constant-return shams.

## Network match-client allocator stubs (3)

These did not carry a literal no-source marker, but the same sweep found their
`STATE[STUB]`/constant bodies and includes them in the semantic closure.

| Declaration | Recovered behavior | Evidence |
|---|---|---|
| `network::match_client::delete_packet` | destroy an order packet through `m_order_packets_allocator` | inverse of `new_packet`; allocator ownership |
| `network::match_client::new_response_packet` | allocate and placement-construct through `m_response_packets_allocator` | allocator symmetry and target-equivalent direct allocation path |
| `network::match_client::delete_response_packet` | destroy through `m_response_packets_allocator` | inverse factory and `receive_udp_response` teardown |

`on_packet_received` deliberately retains the target's open-coded call to
`new_udp_match_packet`. Routing that site through `new_response_packet` caused a
measured data-reference regression, so the recovered private helper remains a
semantic convenience rather than a forced call boundary. Public order-packet
deletion remains the inverse API consumed by callers that own packets returned by
`new_packet`.

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
fake constant-return bodies in `sources/vostok/network` and
`sources/vostok/network_core`. The older
`STATE[UNMATCHABLE]` comments on implemented TCP/UDP convenience seams, plus the
server-web marker, remain because they accurately describe absent client xrefs,
not missing semantics.

The non-server-web primitive/connector/connection/client/stats bodies are
recoveries with varying local evidence; several retained legacy seams still lack
client xrefs, as their rows say. The session/server bodies are managed semantic
reconstructions, not byte-match claims: their exact statement shape, logging
text, and server-variant source partition require the dedicated-server binary or
original source. The normal client build proves compilation and guards emitted
client consumers, but it cannot turn absent server code into a byte-match claim.

## Field semantics retained by the recovered helpers

The bodies are intentionally small, but they are not arbitrary accessors. Their
field choices are constrained by the surrounding ownership and state machines:

| Field family | Semantic invariant |
|---|---|
| `packet<T>::m_buffer_size` | zero is the sole empty-packet state; `clear`, `clone`, `resize`, and `append` all maintain it |
| allocator `m_allocator` / `m_reference_count` | the stored allocator destroys the object whose atomic reference count reaches zero |
| connection packet queues | `m_packets_to_send` is the queued-work predicate; outgoing and unacknowledged queues remain separate lifetime states |
| connection send/receive timestamps | send and receive are independently written; activity is their maximum, not a new stored clock |
| `m_pending_operations_count` | deferred session destruction is legal only after the connection is disconnected and this count reaches zero |
| `m_state` fields | connection and wrapper queries expose named state-machine values rather than cached booleans |
| callback fields | setters retain the exact callback object later invoked by the corresponding receive, disconnect, or error path |
| endpoint fields | client and server-session accessors expose the endpoint owned by their socket/connection role |
| server `m_clients` / `m_clients_to_destroy` | active sessions remain indexed by endpoint; disconnected sessions move to deferred destruction until async work drains |
| server reopen flags | `m_is_accepting` expresses requested service state; `m_should_reopen_socket` records recoverable socket failure independently |
| statistics fields | counters and bytes add/divide; `max_sequence_difference` remains a peak and therefore is never averaged |
