# network_core full-rebuild-from-canonical plan

GROUND TRUTH = `binaries/structure/target/headers/vostok/network_core/` (one file
per type: exact members, /* 0xNN */ offsets, STATIC_SIZE_ASSERT, access, signatures,
`/* no source */` inline bodies). NEVER guess - copy from there. The
`sources/.../*.cpp` carcasses provide the out-of-line FUNCTION BODY[VA] blocks.

Mechanical fixes only, applied to the canonical text:
- `/* INCLUDES */ class boost::...;` / `class vostok::...;` junk -> real #includes or drop.
- nested `struct X::Y {}` / `enum X::Y::state {}` -> declare inside the outer class.
- boost::intrusive member-hook/list OFFSET (e.g. `,8>` / `,28,`) -> member pointer
  `&T::member` (offset is the PDB encoding of that member).
- `__formal` params = genuinely unused in original -> keep a name + VOSTOK_UNREFERENCED_PARAMETER,
  with a comment that it is a real unused param (NOT a stub artifact).
- leave inline bodies as `{ /* no source */ }` - do NOT invent returns/impl; they
  compile because the inline headers are only included, methods uninstantiated.
- non-template out-of-line bodies (.cpp) carry the fresh addressed FUNCTION BODY[VA] carcass.
- long types -> member typedef per engine convention (udp_match_packet_list etc.).

## Per-file checklist (our header  <-  canonical)
- [ ] base_packet.h  <- base_packet.h
- [ ] packet.h / packet_inline.h  <- packet_vostok__network_core__{tcp_packet,udp_match_packet}_.h
- [ ] packet_reader.h / _inline.h  <- packet_reader.h
- [ ] sequence_number.h / _inline.h  <- sequence_number_unsigned {char,short}_.h
- [ ] tcp_packet.h / _inline.h  <- tcp_packet.h
- [ ] tcp_packet_socket.h / _inline.h  <- tcp_packet_socket_*.h
- [ ] tcp_packet_client.h  <- tcp_packet_client.h
- [ ] udp_match_packet.h (+helper)  <- udp_match_packet.h, udp_match_packet__helper.h, _1
- [ ] udp_match_stats.h  <- udp_match_stats.h, udp_match_items_stats.h, udp_match_stream_stats.h, _1
- [ ] udp_match_connection.h / _inline.h  <- udp_match_connection.h(+_1), __channel, __comparer, enums/state
- [ ] udp_match_client.h  <- udp_match_client.h, _1
- [ ] udp_match_server.h  <- udp_match_server.h, __comparer, __destroy_predicate, _1
- [ ] udp_network_flow_emulator.h  <- udp_network_flow_emulator.h, _options.h
- [ ] udp_match_packets_allocator.h  <- udp_match_packets_allocator.h
- [ ] udp_match_types.h (helpers)  <- udp_match_message_type_info, udp_match_packets_orderer,
        process_packet_predicate, move_to_list_predicate
- [ ] client_error_codes_enum.h  <- enums/client_error_codes_enum.h
- [ ] async_connector.h  <- async_connector.h (+ enums/async_connector__connection_state_enum)
- [ ] handler_allocator.h  <- handler_allocator.h
- [ ] http_client.h  <- http_client.h
