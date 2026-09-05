# Network inline-body audit

This audit distinguishes three cases that the old `STATE[REMOVED]` label had
collapsed together. A declaration present in the retail PDB is not evidence that
the original body was removed. It may have been expanded into a caller, or it may
simply be uninstantiated in the shipped client executable.

An inline recovery is not complete merely because equivalent field logic was
written in the caller. The caller must retain the declared helper seam: the
source boundary can affect statement topology and MSVC's inlining/codegen even
when the final operation is semantically identical. Each classification below
therefore comes from the target caller expansion as well as the symbol census.

## Recovered from retail code

| Declaration | Retail evidence | Recovered source seam |
|---|---|---|
| `udp_match_packets_allocator::destroy` | `intrusive_ptr<udp_match_packets_allocator>::~intrusive_ptr` at RVA `0x7dc10` decrements the count at `+0x10`, then passes the object at `+0x0c` to allocator vslot `+0x18` | `VOSTOK_DELETE_IMPL(m_allocator, allocator)` |
| `tcp_packet_socket::set_on_packet_received` | `network::tcp_packet_client::create_client` at RVA `0x74c050` installs the callback at core-client offset `+0x48`, the start of `m_packet_socket`, rather than the outer callback at `+0x940` | assign the socket's `m_on_packet_received` |
| `tcp_packet_socket::set_on_error` | the core `tcp_packet_client` constructor at RVA `0x77cc70` installs its error bridge at overall offset `+0x68` (`m_packet_socket + 0x20`), not outer `m_on_error` at `+0x960` | assign the socket's `m_on_error` |
| `tcp_packet_client::set_on_packet_received` | the same `create_client` expansion reaches the nested socket callback | forward to `m_packet_socket.set_on_packet_received` |
| `tcp_packet_client::{set_on_connected,set_on_disconnected,set_on_error}` | the remaining `create_client` expansions reach outer offsets `+0x900`, `+0x920`, and `+0x960` | direct member assignments |

The TCP shapes are independently corroborated by the parked 2012 ancestor at
`git show 885e1d4a4:temp/network_legacy/sources/{client_impl,packet_socket}.h`.
That ancestor also supplies the callback typedef names recorded by the retail
PDB.

Already-recovered inline bodies remain marked `STATE[INLINED]` at their source
anchors: the network HTTP setters/getter, the UDP client callback setters and
state/time forwards, and the fixed-packet allocator wiring.

## UDP connection client-expansion sweep

The shipped client constrains these `udp_match_connection` helpers through
actual callers. Their calls must not be replaced with equivalent access to the
underlying fields or containers.

| Declaration | Retail client expansion |
|---|---|
| `is_connected` | `udp_match_client::enqueue` and `network::match_client::is_connected` compare `m_state` with `connected` |
| `is_disconnected` | `udp_match_client::{process_incoming_packet,handle_receive,send_queued_packets}` and the network wrapper compare `m_state` with `disconnected` |
| `delete_packet` | `udp_match_client::enqueue` passes `m_packets_allocator` to `delete_udp_match_packet` |
| `unacknowledged_packets_count` | `udp_match_client::handle_receive` obtains the intrusive-list count for the flow emulator |
| `get_stats` | the network match-client sampling paths retain the stats accessor chain |
| `last_receive_time_in_ms` | `network::match_client::last_receive_time_in_ms` performs the nested volatile load |

The following connection declarations have neither a standalone procedure nor
an expansion in any shipped client caller:

- `has_disconnection_initiated`, `is_disconnecting`, `set_disconnected`,
  `set_max_packet_wait_time_in_ms`, `are_there_any_queued_packets`,
  `last_send_time_in_ms`, `last_activity_time_in_ms`,
  `pending_operations_count`, and `disconnect_impl`;
- `new_packet`, whose only current source consumer is an un-emitted
  `udp_match_client` convenience wrapper.

Their current no-source bodies are not reconstructions. Original source or
another binary that consumes the interfaces is required to recover them.

## Reconstructed seams without a retail expansion

The retail PDB declares these interfaces, and the surviving ancestor or the
paired lower-level interface determines their delegation, but no shipped caller
expands them. They therefore remain `STATE[UNMATCHABLE]` even though the source
seam is restored:

- `tcp_packet_client::{is_connected,has_connection_established}` forward to
  `async_connector`, matching the legacy client split.
- `udp_match_client::{new_packet,delete_packet,are_there_any_queued_packets,
  last_send_time_in_ms,last_activity_time_in_ms}` delegate to the same-named
  `udp_match_connection` interface. Its `construct_packet` forwards to the
  connection's static implementation.

## No client-target body exists

These declarations are present in type information, but the shipped client has
neither a standalone procedure nor an inlined use from which a body can be
recovered:

- the dedicated-server web: `udp_match_server` and
  `udp_match_client_session`;
- `udp_match_stats` aggregate mutators and dump methods;
- the unused `async_connector::{resolve,close_connection,on_error}` legacy seams;
- `packet<T>::clear`, `udp_match_packet::helper::call_constructor`, and the
  `udp_network_flow_emulator_options` default constructor;
- `udp_match_packets_allocator::{increment,decrement}`. The actual shipped
  intrusive-pointer policy updates `m_reference_count` directly through its
  friendship instead of calling these hooks.

Their empty bodies are placeholders only. `STATE[UNMATCHABLE]` means original
source or another consuming target (principally the dedicated server for the
server/session web) is required; it does not mean the original developers wrote
an empty function.

## PDB declaration structure

The retail/base class comparison found one method-access mismatch in
`network_core`: `udp_match_client::construct_packet` was private locally but is
public in retail. It is restored immediately after `new_packet`, its retail
declaration position. The TCP callback aliases, `async_connector` aliases, and
UDP client aliases are likewise restored from retail nested-type records. No
other `network_core` method-access mismatch was reported at this audit baseline.

After the rebuild, `async_connector`, `tcp_packet_client`, and the instantiated
TCP socket specialization are topology-identical to retail. The UDP client has
an exact overlapping `0xb28` retail variant; retail also contains a separate
`0xb20` variant from another compilation context. Both `network` and
`network_core` pass the strict data-relocation gate with zero open rows.
