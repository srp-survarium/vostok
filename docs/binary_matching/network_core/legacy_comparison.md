# network_core vs temp/network_legacy - comparison audit

> Closed record. `temp/network_legacy/` was deleted from the tree; retrieve any
> file with `git show 3320ded27:temp/network_legacy/<file>`.

Doc-only audit (2026-06-10) of every network_core piece with a legacy ancestor
(see "Legacy ancestry" in [README.md](README.md)). "Ours" = the stack tip
`origin/match/network_core-packet-leaves` (4b759e67, contains #283+#285+#288);
"legacy" = `temp/network_legacy/`. Byte evidence from the target PDB index
(`pdb_fetch`) and `strings` on the original exe is marked **[proven]** /
**[disproven]**; everything else is a text-shape prior.

## Headline byte evidence

1. **[proven] `handler_allocator::allocate/deallocate` legacy bodies are in the
   target.** `boost_asio_handler_alloc_helpers::allocate< custom_alloc_handler<
   bind(udp_match_client::handle_receive) > >` (rva 0x122BA0, 94 bytes) inlines
   the boost-example body verbatim: `movzx [this+0x400]` (`in_use_`) test,
   `cmp size, 0x400 / jae`, `mov byte [this+0x400], 1`, return storage address,
   else-branch `call operator new`. Our `allocate( ){ return NULL; }` /
   empty `deallocate` stubs are wrong. 12 alloc-helper instantiations over
   `custom_alloc_handler` exist in the target index; the udp_match_connection
   ctor 86% residual also names "handler_allocator stub's trailing byte init".
2. **[disproven] legacy `resize` tail.** Legacy ends `m_buffer_size =
   m_allocated_size;`; target `packet< tcp_packet >::resize` tail is
   `mov edx,[ebp+8] / mov [ecx+4],edx` - it stores the **parameter**. Our
   `m_buffer_size = size;` is correct; do not adopt legacy here.
3. **[disproven] legacy `make_custom_alloc_handler` direct return.** The repo
   already proved the shipped spelling is the NAMED return (result-copy bytes
   present in tcp_packet_client::start_reading at 100%); legacy's
   `return custom_alloc_handler<...>( a, h );` would regress it.
4. **[mixed, proven] resolver log strings.** Target exe carries the SPACED forms
   `"host name has been resolved!"` / `"host name is being resolved..."` plus
   the underscore forms `"NOT host_name_has_been_resolved!"` /
   `"connection_has_been_established!"` - exactly the mix our async_connector
   already uses. Shipped code edited legacy's strings; matcher got them right.
5. **[inconclusive] target standalone `packet_reader::r` (custom-conv esi/edi
   COMDAT) calls plain `memcpy`, no assert eaters** - it is the whole-program-
   optimized emission, so it cannot discriminate our 1-ASSERT+memcpy body from
   legacy's 3x`ASSERT_U` + `memory::copy` (which LTCG could fold to memcpy).
   The /Od inline-site statement counts remain the place to test.
6. **[proven, post-audit] `packet_reader::advance` carries the legacy ASSERT_U
   triple.** The target 218B debug-quality emission (0x121650) shows, per
   assert: identity(false) eater flag + test + the EXPRESSION evaluation
   (buffer()/buffer_size() reads, cmp/sbb bool) + expression_eater call - the
   gold `ASSERT_U` shape with legacy r()'s exact bounds expressions; plain
   `ASSERT` emits only a bare 0xc eater with the expression discarded.
7. **[disproven, post-audit] legacy `clone` friend reads.** The target clone
   row at tcp_packet_socket::send (0x123f08) lowers `other`'s buffer and size
   as out-of-line CALLS pushed before append - direct `other.m_buffer` reads
   can never compile into calls, so shipped clone() goes through the accessors.
8. **[proven, structure-verifier follow-up] the tcp_packet_socket "0% block" was an
   ACCESS-SPECIFIER mismatch, not template-pairing noise.** The target mangles
   `on_packet_received` / `on_packet_has_been_sent` / `on_packet_size_received<T>` /
   `new_packet` as `AAE` (private); our all-public class declared them `QAE`, so
   objdiff's mangled-name join silently failed and reported 0%. Declaring them
   `private:` repaired the pairing: new_packet 0 -> 100, on_packet_has_been_sent
   0 -> 94.88, on_packet_received 0 -> 94.21, on_packet_size_received<u8>/<u16>
   0 -> 85.21 (unit 25 -> 88.14), zero regressions. The earlier "identity join
   fails on `const unsigned int`" only explains the RICH-INDEX demangle mismatch
   (the PDB type records `u32 const bytes_transferred`, now restored in source);
   the objdiff failure was the access bit.

## Findings table

| component | legacy file (temp/network_legacy/) | our file (network_core/) | closeness | what we missed (legacy differs) | adoption candidate / expected impact | OUTCOME (this branch) |
|---|---|---|---|---|---|---|
| handler_allocator | sources/handler_allocator.h (class) | handler_allocator.h | **LOW** (ctor ok, both method bodies stubbed) | real `allocate` (`!in_use_ && size < storage_.size` -> storage, else `::operator new`) and `deallocate` (`pointer == storage_.address()` -> `in_use_=false`, else `::operator delete`) | **YES [proven]** - adopt verbatim; feeds 12 alloc-helper COMDATs (tcp read/write ops, udp recvfrom/send_to) + udp_match_connection ctor 86% residual | **held** - base alloc-helper COMDAT (0x899e0, 94B) now instruction-identical to target 0x122ba0; recv_op::do_complete 76->100, 2x async_receive 98.06->100; udp-named instantiations stay 0% in report (ICF/pairing artifact, target-only names) |
| custom_alloc_handler + make_ | sources/handler_allocator.h (template) | custom_alloc_handler.h | **HIGH** | nothing; legacy's direct-return make_ is wrong vs shipped named-return [disproven] | no | - |
| packet writer family | packet.h + packet_inline.h | packet{,_inline}.h + base_packet.h + tcp_packet{,_inline}.h | **HIGH** (reallocate/dtor/append-core/reserve already legacy-verbatim, incl. the -3/+3 length-prefix dance and `real_buffer` local) | (a) `clone`: legacy reads `other.m_buffer/m_buffer_size` directly (friend access) - ours calls accessors, base inlines them while target SIZE-diffs at send()'s clone row; (b) `append(pcstr)` + `append(pcstr,u8)` are empty stubs, legacy has bodies (`strlen` u8-cast; `ASSERT(len<=u8(-1)); append(len); append(buf,len)`); (c) resize tail [disproven - keep ours] | **YES (a,b)** - (a) may move tcp_packet_socket::send 55%; (b) fills 2 of the packet_inline.h 0/12 stubs if those instances exist in target | **(a) disproved, (b) neutral-kept** - (a) the target clone row (0x123f08) lowers both operands as out-of-line CALLS before append (push call-result x2); direct friend reads can never produce calls, so the accessor spelling is shipped truth - reverted after test (the friend-read had RAISED send 64.7 -> 67.5 over the wrong structure, the classic %-trap). (b) no append(pcstr[,u8]) instantiation exists in the target index; legacy bodies adopted over the empty stubs, byte-neutral (report-changes empty) |
| packet_reader | packet_reader.h + packet_reader_inline.h | packet_reader{,_inline}.h | **MEDIUM-HIGH** (ctor identical; r/r_string rewritten) | (a) `r()`: legacy = 3x`ASSERT_U` bounds + `memory::copy(dst, dst_size, src, size)` (the engine's own `memory_reader_inline.h` sibling uses this exact shape) vs our 1 ASSERT + `memcpy`; (b) `r<T>()`: legacy routes through `r(&result,sizeof,sizeof)` with named `result` local vs our direct deref; (c) `r_string(pstr,u8)`: legacy is ONE call stmt `r(string, buffer_size, string_length)` vs our hand-inlined memcpy+advance (2 stmts - statement-count divergence at /Od sites); (d) `r_string<Count>`: legacy clamps `(u8)std::min(sizeof(string), size_t(255))` - we dropped it | **YES (test-first)** - standalone COMDATs are optimized/inconclusive [see #5]; test at the /Od carcass statement counts of inline sites; (c)/(d) are line-table-visible | **held** - unit 14.40 -> 34.77 (advance 27.27 -> 65.87 after switching to the ASSERT_U bounds triple, 4/4 stmts, +0x9/row = buffer() inline-vs-call); inline-site judges: game deserialize chains +30..47pp each (inventory_item 0 -> 46.9, player_state 7.1 -> 49.7, body_part 17.6 -> 57.9, player_input 33.2 -> 63.1, booby_trap 15.6 -> 58.6), flow_emulator add_packet 70.8 -> 78.7; player_input::deserialize 3/3 stmts r<float2> rows within +0x2 of target. CAVEAT: needed a game_core forced rebuild - regen-merged ninja kept stale header-deps, so LTCG inlined STALE game_core IL for ctor/r<T> until temp_include_all.cpp was touched |
| tcp_packet_socket | sources/packet_socket{,_inline}.h | tcp_packet_socket{,_inline}.h | **HIGH** control-flow, **MEDIUM** statement shape | (a) legacy error paths are FLAT blocks with early `return;` - ours else-if chains; the verified ONLY-target rows (on_packet_received L30, on_packet_has_been_sent L142) are exactly the extra branch stmt that shape emits; (b) `on_packet_size_received` tail: legacy `if ( identity(sizeof(T) < sizeof(u16)) ) ... else ASSERT(...)` recursion guard + early `return;` after the body-read async_read = the 4 ONLY-target QTY rows; (c) header buffer read: legacy `*static_cast<T const*>(static_cast<pcvoid>(&m_header_buffer))` vs our `static_cast<T>(m_header_buffer)`; (d) legacy `!bytes_transferred` vs our `== 0` (cosmetic). `send()` was legitimately rewritten to sync `boost::asio::write` (target-verified) - keep ours | **YES (a,b,c)** - main lever on the 0/8 unit + the on_packet_* PARTIALs | **(a,b,c) held** - on_packet_received 18/18 stmts (was 17+1 ONLY-target), on_packet_has_been_sent 10/10 (was 9+1); on_packet_size_received 22/22 BOTH instantiations (identity guard 0x1d, else row 0x2, ASSERT eater 0x12, pointer-cast deref 0xf/0x11 all byte-exact vs target); residual = LOG_ERROR FILE/LINE wall + resize()/allocated_size() inline-vs-call |
| tcp_packet_client | sources/client_impl.{h,cpp} | tcp_packet_client.{h,cpp} | **HIGH** (legacy ported; mostly 100%) | dtor: legacy `if ( !is_connected( ) ) return; disconnect( );` - the early-return guard emits exactly the target's `jne .disconnect; jmp .end` pair the verifier logged as a no-lever QTY diff; ours condensed it to `if ( c ) disconnect( );` | **YES** - spell dtor as `if ( !m_async_connector.has_connection_established( ) ) return; disconnect( );` -> 90.98 likely ~100 | **held** - dtor 90.98 -> 94.88, structure-diff now `STRUCTURE MATCH` 3/3 stmts (the 2-byte `return;` row emits); residual is epilogue ICF'd function::clear call symbols + esi regalloc, not the guard |
| async_connector | sources/client_impl.cpp (resolve/on_resolved/connect/on_connected) | sources/async_connector.cpp | **MEDIUM** (on_resolved unpaired/unverified) | (a) legacy `++iterator;` is its OWN statement, then `if ( iterator != resolver::iterator( ) )` retry that calls `resolver->async_resolve( *iterator, bind(&on_resolved,...) )` - ours folds `++` into the if and CONNECTS on retry instead of re-resolving (different control flow); (b) legacy query is 3-arg `query( tcp::v4( ), host, port )` vs our 2-arg; (c) strings already correct [proven] | **YES (test-first)** - on_resolved is the unit objdiff cannot pair; legacy shape is the best prior for a re-match attempt | not attempted (out of scope here) |
| client_error_codes | sources/client_error_codes.h | client_error_codes_enum.h | MEDIUM (same names, order changed) | nothing - shipped enum reordered, ours is PDB-pinned | no | - |
| udp_match_connection / udp_match_client / udp_match_packet / sequence_number / predicates / stats / udp allocators | none (rewrite-era; legacy pool was `network_world::new_packet`) | udp_match_*.{h,cpp}, sequence_number*, *_predicate.h | n/a | no ancestor exists; the walls there (handle_receive 93.54 function1-ctor scheduling + buffer() inline-vs-call, enqueue 94.52 log COMDAT, client ctor 79.20 math::max/function1=, process_incoming_packet 49.60 / call_predicate 61.81 inline-vs-call) are LTCG call-boundary class - legacy text offers no alternative spelling | no | - (no-lever walls, untouched per constraints) |
| io_service wrapper, asio_* glue, readers/writers web, buffer_*_device, std_string, server/client_session | sources/io_service.*, asio_*.h, reader*/writer*, server* | - | n/a | dead for matching (no PDB counterpart; README "No PDB counterpart" list) | no | - |

## Prioritized adoption list (for future matcher units)

1. **handler_allocator legacy bodies [proven]** - paste legacy `allocate`/
   `deallocate` verbatim (snake-case the spelling only). Cracks: the wrong-byte
   stubs inside every `boost_asio_handler_alloc_helpers::allocate<
   custom_alloc_handler<...> >` COMDAT (12 instantiations, tcp+udp), the
   udp_match_connection ctor 86% "trailing byte init" residual, and makes the
   handler_allocator.h 0% honest.
2. **Legacy early-return guard shapes** - tcp_packet_client dtor
   (`if ( !... ) return;` -> expected ~100 from 90.98) and the flat
   early-return error blocks in tcp_packet_socket::on_packet_received /
   on_packet_has_been_sent (kills the ONLY-target L30/L142 rows). Cheap,
   structure-diff-verifiable.
3. **`on_packet_size_received` legacy tail** - restore the
   `identity( sizeof(T) < sizeof(u16) )` guard + `else ASSERT` + early
   `return;` shape and the pointer-cast header-buffer deref; expected to close
   most of the target-24/base-20 QTY gap in the tcp_packet_socket_inline 0/8
   unit.
4. **packet_reader legacy bodies (byte-test first)** - `memory::copy` +
   3 asserts in `r()`, single-call `r_string(pstr,u8)`, `std::min` clamp in
   `r_string<Count>`, `r<T>` via `r(&result,...)`. Standalone target COMDATs
   are optimized and inconclusive; test against the /Od inline-site carcass
   statement counts (the one-stmt-vs-two `r_string` difference is line-table
   visible).
5. **packet writer leftovers** - `clone()` via direct `other.m_buffer/
   m_buffer_size` (friend access; targets the send() clone SIZE row) and the
   legacy `append(pcstr)` / `append(pcstr,u8)` bodies for the empty stubs in
   packet_inline.h (0/12).

Also worth a re-match attempt with legacy text in hand: `async_connector::
on_resolved` (separate `++iterator;` statement + re-resolve retry + 3-arg
query) - it is currently unpaired and "best-effort".

## Overall closeness verdict

The user's hypothesis holds for the TCP half of the module: tcp_packet_client,
tcp_packet's reallocate/dtor, the append core, the packet_reader ctor and the
custom_alloc_handler machinery are legacy text near-verbatim, and the matched
units that already ported legacy bodies are the high scorers. The remaining
legacy-explainable misses are concentrated in (a) bodies we stubbed
(handler_allocator - byte-proven), (b) control-flow shapes we condensed
(early-return guards, identity() recursion guard), and (c) helper-call idioms
we hand-inlined (memory::copy / r_string). The UDP half has no ancestor and its
residuals are LTCG call-boundary walls legacy cannot move. Caveat: 2 of 5 byte
probes in this audit DISPROVED naive legacy adoption (resize tail, make_
direct-return) - legacy is a strong prior, never a substitute for the
structure-diff.
