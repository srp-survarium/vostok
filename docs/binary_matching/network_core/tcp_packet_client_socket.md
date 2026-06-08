# tcp_packet_client + tcp_packet_socket (TCP send/receive path)

The out-of-line members of `tcp_packet_client` (`tcp_packet_client.cpp`) plus the
`inline`-but-out-of-lined members of `tcp_packet_socket<tcp::socket>`
(`tcp_packet_socket_inline.h`). Under /Od MSVC does NOT inline `inline` functions,
so each socket method emits its own out-of-line body and the client forwards via a
`call`.

## Anchor
Added `use_network_core_tcp_packet_client()` to
`game_core/sources/temp_include_all.cpp` (mirrors `use_network_core_http_client`):
constructs `tcp_packet_client c(io_service)`, calls `connect/disconnect/send/io_service`,
and is invoked from `IncludeAll::IncludeAll()`. Added
`#include <vostok/network_core/tcp_packet_client.h>`. This instantiates the
`tcp_packet_socket<tcp::socket>` template so its members resolve in the base index.

## Infrastructure completed to build the async path
- `custom_alloc_handler.h`: added `make_custom_alloc_handler()` + the asio glue
  (`operator()` arity-1/2, `asio_handler_allocate/deallocate/invoke` friends). The
  STUB class had no callable, so `async_read`/`write` would not compile.
- `tcp_packet_socket::delete_packet()` (was STUB) -> `VOSTOK_DELETE_IMPL( m_packet_allocator, packet )`.

## Per-function results (objdiff fuzzy %, or direct byte-match where unit-pairing zeroes the report)
- `on_connected`, `connect`, `on_error`, `disconnect`  -> 100% DONE
- `start_reading` 99.70, `send` (client) 99.69  -> DONE (residual = inline-boundary of forwarded socket call)
- `start_receiving` 99.88, `stop_receiving` 99.81  -> DONE
- `new_packet` 95.5 (report 0 = unit pairing; only diff is the assert-stub delinker naming) -> DONE
- ctor 70, dtor 91  -> PARTIAL (body byte-correct; boost::bind/function reps fold under LTCG)
- `close_connection` 66  -> PARTIAL. ASSERT(empty_stub) at top recovered (+ frame align).
  Residual: `m_socket.close(ec)` out-lines to `basic_socket::close` in target but
  over-inlines to the win_iocp service close in base (inline-boundary, not source-steerable).
- `on_packet_has_been_sent` 52, `on_packet_received` 40, `on_packet_size_received<u8/u16>` PARTIAL.
  Control flow matched: the asio `operation_aborted` (error 995/0x3E3) early-return, the
  `bytes_transferred != packet->allocated_size()` size check, the header re-read recursion
  (on_packet_size_received<u8> reads 1 byte then issues a u16 read), and the new_packet/resize/
  async_read continuation. Residual is the LOG_ERROR blocks: the `__FILE__` baked into every
  log call is `C:\survarium\sources\...` in target vs our `Z:\home\...` build path -> a
  permanent byte diff on logging-dominated functions (same wall as http_client's LOG residuals).

## Key asm->source findings
- The asio completion binds use `boost::asio::placeholders::error` /
  `::bytes_transferred`, NOT boost's global `_1`/`_2`. The target reads the placeholder
  from a global var (`mov eax,[placeholder]; movzx byte[eax]`); `_1`/`_2` instead emit
  `byte[_1]` literals. Using asio placeholders took start_receiving 94 -> 99.88.
- LOG strings use CRLF (`\r\n`), e.g. `"unable to read from socket\r\n"`,
  `"error during reading from socket: %s\r\n"` (string content is rdata, doesn't move the
  instruction-equal count, but documents intent).
- Read-error size check is `packet->allocated_size()` (tcp_packet's m_allocated_size),
  not `buffer_size()`.
- `tcp_packet_socket::send` target does `cloned->clear(); cloned->append(packet.buffer(),
  packet.buffer_size())`, but `base_packet::buffer() const` is PRIVATE in the PDB-matched
  header, so the socket (a non-member) cannot call it on a `const&`. Used `clone(packet)`
  instead and took the hit rather than widen access (another unit's concern).
