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

## Per-function results (structure-verifier re-adjudicated; objdiff fuzzy %)

Note: `tcp_packet_client` and `tcp_packet_socket` share their names with the old
`vostok::network::tcp_packet_client` / `vostok::network::packet_socket` symbols, so the
ctor/dtor/send/size_received diffs had to be pinned by the **network_core target rva**
(the by-name lookup resolves the wrong namespace's first match).

- `on_connected`, `connect`, `on_error`, `disconnect`  -> 100% DONE
- `start_reading` 99.70%, `send` (client) 99.69%  -> **DONE, STRUCTURE MATCH** (1/1 stmt,
  0 qty / 0 size; residual is the sub-statement inline boundary of the forwarded socket call).
- `start_receiving` 99.88%, `new_packet` 95.5% (report 0 = unit pairing)  -> **DONE,
  STRUCTURE MATCH** (1/1 stmt, 0 qty / 0 size).
- `stop_receiving` 99.81%  -> **DONE, STRUCTURE MATCH** (2/2 stmts, 0 qty, 1 SIZE =
  basic_socket::cancel(ec) inline-boundary).
- ctor 70.24%  -> **PARTIAL, STRUCTURE MATCH** (shape ok). 1/1 stmt, 0 qty, 1 SIZE on the
  m_on_error bind: target inlines the m_packet_socket member ctor + boost::function assign
  epilogue regalloc; base emits one ctor call. No source lever.
- dtor 90.98%  -> **PARTIAL, STRUCTURE MISMATCH (quantity)**. target 3 / base 2 stmts: the
  1 extra target row is a 2-byte branch (target lowers the guard `jne .disconnect; jmp .end`,
  base a single `je .end`). has_connection_established test + disconnect align; if-branch
  codegen layout, not a source-shape miss.
- `close_connection` 66.30%  -> **PARTIAL, STRUCTURE MATCH** (shape ok). 5/5 stmts align,
  0 qty, 1 SIZE on `m_socket.close(ec)`: target out-lines to basic_socket::close, base
  over-inlines to the win_iocp service close (boost inline-boundary).
- `on_packet_received` 40%  -> **PARTIAL, STRUCTURE MISMATCH (both)**. target 18 / base 17;
  6 SIZE (LOG_ERROR __FILE__/__LINE__ wall + allocated_size() inline-boundary) + 1 QTY
  (L30 if(m_on_error) branch-bucketing). All 3 branches + success path align.
- `on_packet_has_been_sent` 52%  -> **PARTIAL, STRUCTURE MISMATCH (both)**. target 10 /
  base 9; 4 SIZE (LOG_ERROR wall) + 1 QTY (L142 branch-bucketing). delete_packet + both
  error branches align.
- `on_packet_size_received<u8/u16>` PARTIAL  -> **STRUCTURE MISMATCH (both)**. target 24 /
  base 20 stmts (qty 4). Control flow (operation_aborted return, sizeof(T) check, header
  re-read recursion) matches; residual is the LOG_ERROR __FILE__/__LINE__ wall + the
  4-stmt success-path async_read split (target keeps buffer_to_receive_into/bind rows as
  separate statements our LTCG folds). `--view structure-diff` cannot auto-align (target
  param `const unsigned int` vs base `unsigned int` + the network::packet_socket name
  shadow); compared via rva-pinned single-side dumps.
- `send` (socket) 55%  -> **STRUCTURE FIXED (qty 4 -> 3)**. Restored `packet<T>::clone()`'s
  body in `packet_inline.h` -- it was an empty `/* no source */` stub. Target inlines clone()
  to `m_buffer_size = 0; append( other.buffer(), other.buffer_size() )` (the legacy
  `network::packet::clone` form); `packet<T>` is a `friend` of `base_packet` so the private
  const `buffer()`/`buffer_size()` accessors are reachable on the `const& other`. After the
  fix + rebuild the copy step emits as a real statement (base 4 -> 5 stmts, qty 4 -> 3).
  Residual: target keeps `other.buffer()`/`buffer_size()` as out-of-line calls; base inlines
  the trivial accessors to direct `[other+0]`/`[other+4]` field reads (LTCG inline-boundary,
  no source lever).

The LOG_ERROR residual across the on_packet_* functions is the `__FILE__` baked into every
log call: `C:\survarium\sources\...` in target vs our `Z:\home\...` build path -> a permanent
byte diff on logging-dominated functions (same wall as http_client's LOG residuals).

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
- `tcp_packet_socket::send` calls `cloned->clone( packet )`, and `clone()` (a member of
  `packet<T>`) does the copy: `m_buffer_size = 0; append( other.buffer(), other.buffer_size() )`.
  `base_packet::buffer() const` is PRIVATE, but `base_packet` declares
  `template < typename T > friend class packet;`, so `packet<T>::clone()` legally reaches the
  private const accessors on the `base_packet const& other` -- no access widening needed. (The
  earlier note that the socket "cannot call buffer() so we took the hit" was wrong: the socket
  doesn't call buffer() directly, clone() does, and clone() is a friend.)
