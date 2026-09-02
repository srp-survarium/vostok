# network_core - matching notes

- Namespace: `vostok::network_core`.
- Build: `/Od` (optimization off) + LTCG, `Master Gold` - see [../MATCHING.md](../MATCHING.md).
- Sources: `sources/vostok/network_core/sources/`.
- Reachability: network_core is retained through real network and game callers.
  The former `temp_include_all.cpp`/`anchor_network_core.cpp` reachability layer
  has been retired; `get_connection_info_from_string`, sequence subtraction, and
  `udp_match_packet::header_size` are all owned by the reconstructed call graph.
- Best reversed reference: `http_client.cpp`. Heavy Boost.Asio - expect the body
  to lean on asio / `boost::bind` shapes.

## Module-wide notes
Shared types, recurring gotchas, and asm quirks that span functions - add as you
find them.

### Legacy ancestry (body priors for the low-% units)
Much of this module is the 2012 `vostok::network` code relocated during the
rewrite. When matching the units below, read the legacy file FIRST - the bodies
often port nearly verbatim (proven: the network module's ported `send_order.h`
dtor/execute came out byte-identical):

| network_core unit (fuzzy) | legacy ancestor (`git show 885e1d4a4:temp/network_legacy/sources/<file>`; the tree is deleted) |
|---|---|
| `tcp_packet_socket_inline.h` (8%) | `packet_socket.h` + `packet_socket_inline.h` - same template, same methods (`send`, `start_receiving`, `on_packet_size_received<T>`, `on_packet_has_been_sent`, `delete_packet`) |
| `handler_allocator.h` (0%) | `handler_allocator.h` - both are the boost-asio allocation example, near-verbatim |
| `packet_reader_inline.h` (14%) | `../packet_reader.h` + `../packet_reader_inline.h` |
| `packet.h`/`packet_inline.h`/`base_packet.h` (0%) | `../packet.h` + `../packet_inline.h` |
| `sources/async_connector.cpp` (43%) | `client_impl.cpp` - its `resolve`/`on_resolved`/`connect(iterator)`/`on_connected` retry machinery was extracted into async_connector |
| `sources/tcp_packet_client.cpp` (85% residual) | `client_impl.{h,cpp}` - same surface (`is_connected`, `has_connection_established`, `close_connection`, `start_reading`, `on_error`) |

No PDB counterpart (dead for matching): the readers/writers web, `buffer_*_device`,
`signalling_reader`, `std_string`, `io_service` wrapper, `asio_*` glue,
`server*`/`client_session*` (server-side).

## Per-function logs
One `<function>.md` in this folder per function that needed real effort (see
[../agentic_loop.md](../agentic_loop.md) section 7). Live status is in the
`STATE[...]` markers in the sources; these files hold the deeper write-ups.
