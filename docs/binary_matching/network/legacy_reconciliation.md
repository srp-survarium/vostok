# network module - legacy source reconciliation

> Closed record. `temp/network_legacy/` was deleted from the tree; retrieve any
> file with `git show 885e1d4a4:temp/network_legacy/<file>`.

Reconciles the reconstructed network bodies in `sources/vostok/network/sources/`
against the authentic original source parked in `temp/network_legacy/`. Policy
(sushi): legacy spelling/structure takes priority for any function or sub-pattern
with a legacy counterpart, **UNLESS it yields worse structure or worse %** - and
every such rejection is documented here.

## Key finding: legacy is an EARLIER variant, not the shipped v0.100b source

`temp/network_legacy/` is the engine's earlier *generic TCP* network library
(`client`/`client_impl`/`client_session`/`server`, asio handler machinery, the
TCP order/response headers). The shipped v0.100b network module is a richer, later
design: a UDP `match_client` + an SSL `login_client` + an `http_client` built on a
`tcp_packet_client` that adds an on_error path, clones via `tcp_packet`, and routes
through `string_order`/`send_order` instead of the legacy `connect_order`. So legacy
informs the **idiom** (command-object + network-thread + early-return guards +
ASSERT_U-reads-dead-local), not the exact bytes. The carcass had already driven every
function to STRUCTURE MATCH from the target asm; legacy is **adopted only where the
shipped class genuinely coincides** (it already did) and **rejected where it would
regress**. Net: the whole module is STRUCTURE MATCH (quantity + named locals + order),
residuals are the documented `/Ob2 /Oi /Ot /Oy /GL` inline-vs-call walls, and no
legacy spelling improved structure or %.

---

## tcp_packet_client / http_client / network_world / network_entry_point

All four units + the string headers were ALREADY STRUCTURE MATCH (zero
TARGET_ONLY/BASE_ONLY; named locals == target; 1:1 order). Only byte-neutral cleanup
applied (stripped 18 stale `STATE[..]` markers).

| Function | Legacy ref | Decision | Reason |
|---|---|---|---|
| tcp_packet_client ctor | `client::client` | KEEP (== legacy shape) | member-init + `add_order(functor_order(bind create_client))` identical; STRUCTURE MATCH |
| tcp_packet_client::~ | `client::~client` | KEEP shipped (file-static destroy_client) | target PDB symbol is the unmangled private `destroy_client` -> file-static; legacy member form regresses the symbol |
| tcp_packet_client::create_client | `client.cpp:43` | KEEP shipped (adds set_on_error) | shipped class has the on_error path legacy lacks; 6 stmts match |
| tcp_packet_client::connect | `client.cpp:62` | REJECT legacy (connect_order) | shipped binds port into the functor + routes through `string_order` -> 100% MATCH; legacy `connect_order` changes the order class + stmt shape and regresses |
| tcp_packet_client::send | `client.cpp:83` | REJECT legacy (no clone) | target `send(tcp_packet const&)` CLONES into the orders allocator (`cloned_packet` local == target locals(1)); legacy drops the clone stmt -> QUANTITY + local regress |
| tcp_packet_client::on_packet_received_impl | `client.cpp:94` | REJECT legacy (packet_reader local) | shipped takes `packet_reader&` param -> target records 0 named locals; legacy adds a named local = LOCALS regress |
| tcp_packet_client::on_packet_received | `client.cpp:102` | KEEP shipped (== legacy shape) | early-return guard + clone + add_response; 5 stmts + 1 local (cloned_packet) match |
| set_on_* / on_connected / on_disconnected | `client.cpp` | KEEP (== legacy shape) | identical `m_x = arg;` setters + early-return-guard bodies |
| http_client (whole TU) | (no legacy counterpart) | KEEP shipped | shipped-only; reconstructed from target; 8/10 100%, destroy_http_client SIZE-only |
| network_world (whole TU) | `network_world.cpp` | KEEP shipped (richer) | legacy = earlier variant: `tick()` no-arg + by-value io_service vs shipped `tick(bool)` + NEW'd io_service pointer; no count>=10 break in process_responses; empty clear_resources. Shipped matches 13/13; legacy regresses quantity |
| network_entry_point (whole TU) | `io_service.cpp`/`boost_api.cpp` | KEEP shipped | create/destroy_world, memory_allocator, init/finalize all 100%; legacy are different TUs |
| string_order.h / string_response.h | `send_order.h`/`response.h` | REJECT legacy (different class) | shipped string_order/response are 3-functor/3-string variants with no legacy counterpart; `execute()` ICF-folds to ONE byte-identical COMDAT both sides (0x95B, 7 stmts) -> STRUCTURE MATCH |

---

## login subsystem (login_client*.cpp)

All 49 PAIRED functions STRUCTURE MATCH (quantity + order + locals). No direct legacy
file (newer SSL protocol layer); reconciled against the generic `client_impl`
machinery. One real fix: `on_sign_in_answer_received` locals `const u8` -> `u8`.
`login_client::sign_up` is BASE_ONLY (target fully inlined it).

| Function / pattern | Legacy ref | Decision | Reason |
|---|---|---|---|
| `resolve` (LOG/ASSERT/state, `new resolver`, `char port[6]`, `_itoa_s`, query ctor, async_resolve+bind) | `client_impl::resolve` | adopted (already authentic) | carcass shape matches legacy `resolve` 1:1; structure 7/7 MATCH |
| `_itoa_s` spelling in `resolve` | legacy 3-arg `_itoa_s` | rejected | target asm uses 4-arg `_itoa_s(m_host_port, port, 6, 10)` (`push 6`); residual -0xc is pure temp-spill scheduling, not source-level |
| `on_resolved` (error/retry ladder, iterator end-checks, `delete resolver`, guarded functor) | `client_impl::on_resolved` | adopted (already authentic) | same skeleton; structure MATCH |
| `on_connected` / `connect` (braced error block + early return, guarded boost::function) | `client_impl::on_connected`/`connect` | adopted (already authentic) | login already uses legacy idioms; 15/15, 9/9 MATCH |
| `~login_client_impl` (do/switch state machine) | `client_impl::~client_impl` (`if(!is_connected())return; disconnect();`) | rejected | login dtor is a richer protocol do-while/switch; legacy's 2-stmt form is a strict subset of the target's 19-stmt skeleton |
| `close_connection` (LOG, ping-timer cancel, SSL re-construct, socket shutdown/close) | `client_impl::close_connection` (`shutdown(); close();`) | rejected | login additionally tears down/reconstructs the SSL stream + cancels ping timer; legacy's 2-stmt form is a strict subset of the 8-stmt skeleton |
| `on_sign_in_answer_received` locals `length1`/`length2` | n/a (login-bespoke buffer walk) | fixed | target records `u8` (no const); removed `const` to match the target local set |

---

## match_client subsystem (match_client*.cpp + UDP order/response headers)

All 29 functions STRUCTURE MATCH (quantity + locals). `temp/network_legacy/` is the
generic TCP module; `match_client` is the UDP subsystem - the concrete order/response
classes are UDP-specific with NO legacy signature, so legacy informs the IDIOM only.
Nothing adopted (all already faithful or legacy regresses).

| Function / type | Legacy ref | Decision | Reason |
|---|---|---|---|
| `match_client_impl::on_packet_received` (previous_state local) | `client_impl` state idiom; `core` `ASSERT(previous_state==...)` | REJECT legacy-style ASSERT_U reader | target records 0 named locals; ours emits `[ebp-4]`. An `ASSERT_U(UNKNOWN_EXPRESSION_T(...))` reader did NOT drop the symbol - it emitted a real 12th eater stmt (BASE_ONLY, 27%->2.7%), a QUANTITY regression. The symbol gap is a target-side LTCG/PDB artifact; previous_state is authentic (target line-45 store). Keep natural shape |
| `match_client_impl` set_on_packet_received/disconnect/connect | `client_impl` counterparts | KEEP current | legacy forwards to `m_packet_socket` (TCP); UDP impl conditions on `m_state`; current STRUCTURE MATCHes (3/3, 3/3, 2/2) |
| `~match_client_impl` | legacy `~client_impl` early-return guard | KEEP current (no guard) | target = 1 stmt (`VOSTOK_DELETE_IMPL`); legacy guard would ADD stmts the target lacks |
| `connect_order` | legacy 4-arg `connect_order` | KEEP current 5-arg | UDP-path ctor `(strings_allocator, host, packets_allocator, packet, connector)`; legacy is TCP; current MATCHes (execute 1/1, dtor 4/4) |
| `order.h` / `response.h` | legacy public `next_for_*`, no noncopyable base | KEEP current (noncopyable + friend + explicit ctor) | codegen-invisible empty-base; no target fn distinguishes; channel typedefs resolve via friend |
| `two_way_threads_channel.h` | legacy NAMESPACE-scope typedefs | KEEP current (member typedefs) | same instantiated layout (0x118); member-typedef pairs with the order/response friend |
| `functor_order` / `functor_response` | legacy 2 members, no explicit dtor | KEEP current (explicit ctor + `~`) | `explicit` + out-of-line `~` codegen-invisible / required for the matched vtable |
