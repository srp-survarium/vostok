# network_core - PDB structure-completeness audit

Audit of the target PDB structure for `network_core`
(`binaries/structure/target/sources/vostok/network_core/`) against our hand-written
headers (`sources/vostok/network_core/`). Goal: every class/struct/method/member/
enum/typedef/free-function the target shows should exist as a valid declaration in
our headers, in the file/position the carcass places it.

Method: the target carcasses come in `_N` variants (same header seen in multiple
TUs - take the UNION, aligned by the `'NN'` source-line numbers) and as MONOMORPHISED
types (templates instantiated per use - reconstruct the primary template).
See [../enabling_a_library.md](../enabling_a_library.md) for the full methodology.

## Build state
- The local ninja graph was pre-broken (stale `animation_playback_state.h` implicit
  dep in ~11 gitignored `.ninja` files; the header was deleted from the tree). This
  blocked EVERY build with "missing and no known rule to make it". Dropped the stale
  dep line; build is GREEN again. (Environmental, not a source change - the fix is in
  gitignored artifacts and does not appear in the PR.)
- Enabled `network_core_entry_point.cpp` (was `ExcludedFromBuild`): stood up
  `sources/network_core_entry_point.h` (5 free-fn decls), added a stub return,
  commented out the carcass typedef-scratch block, anchored it via
  `use_network_core_entry_point()` in `game_core/sources/temp_include_all.cpp`.
  Full `rebuild.py` GREEN; `network_core::initialize()` now matches 100%.
  report-changes: only ICF folding noise (`empty_stub`, `[thunk] vcall`, boost
  `storageN`), no real logic regression.

## Per-type coverage

| Target carcass (+ `_N` union)                  | Our header                          | Status | Notes / what is missing |
|---|---|---|---|
| `base_packet.h` (+`_1`)                         | `base_packet.h`                     | COMPLETE | `buffer()`/`buffer_size()` const, members `m_buffer`/`m_buffer_size` all present, size 0x8. |
| `packet.h`+`packet_1` / `packet_inline*` (UNION) | `packet.h` / `packet_inline.h`     | COMPLETE (decls) | Primary `template<T> packet`. Union shows ctor, `allocated_size`, `reallocate`, `resize`, `append(pcvoid,u32)`, `append(u8/u16/float/float2/float3)` - all declared. **DEFECT (pre-existing):** `packet<T>` references `m_allocated_size`/`m_allocator`/`m_buffer*` in `packet_inline.h` but declares no such members; base members `m_buffer`/`m_buffer_size` are PRIVATE in `base_packet`. Compiles only because no enabled TU instantiates the touching inlines. Matcher must add `protected: u32 m_allocated_size; memory::base_allocator& m_allocator;` and relax base access. NOT changed here (would shift bytes of the already-matched `tcp_packet`). |
| `packet_reader_inline*` (UNION `_1.._4`)        | `packet_reader.h` / `packet_reader_inline.h` | INCOMPLETE | Our `packet_reader` declares `r(void*, u32)` (2-arg) + `r_string(char*, u8)`. Target UNION shows: `r(void*, u32 destination_size, u32 size)` (**3-arg**), template `r<T>()` no-arg (`<u8>`,`<u16>`,`<u32>`), template `r_string<N>(char[N]&)`, plus `advance`/`eof`/`pointer`/`size_to_eof` (present). Missing: 3-arg `r`, `r<T>()`, `r_string<N>()`. NOT changed: `packet_reader` is consumed by udp TUs that are still excluded + likely under active matcher work; flagged for the matcher who enables `udp_match_connection`. |
| `sequence_number.h` / `sequence_number_inline*` | `sequence_number.h` / `..._inline.h` | INCOMPLETE | **DEFECT:** declared as explicit spec `class sequence_number<u8>` with NO primary template (invalid C++). Used as `<u8>` AND `<u16>`. Must become primary `template<T> class sequence_number` (union of members: ctor, `serialize(pbyte&)`, `serialize(udp_match_packet&)`, `==`,`!=`,`<`,`<=`,`>`,`>=`, `++`/`--` (pre/post), static `deserialize(packet_reader&)`, free `operator-<T>`, `m_number`). NOT changed: shared header under active matcher work; reconstruction belongs with the `udp_match_connection` enable. |
| `tcp_packet.h` / `tcp_packet_inline*`           | `tcp_packet.h` / `tcp_packet_inline.h` | COMPLETE | `tcp_packet : packet<tcp_packet>, boost::noncopyable`; ctor/dtor/`allocated_size`/`reallocate`, free `buffer_to_send`/`buffer_to_receive_into`. Enabled TU, matched. |
| `tcp_packet_socket_inline.h`                    | `tcp_packet_socket.h`               | INCOMPLETE | **DEFECT:** `class tcp_packet_socket<...socket...> public : core::noncopyable` - monomorphised + invalid base syntax. Must become primary `template<socket_type> class tcp_packet_socket : public core::noncopyable`. Union members: ctor/dtor, `send`, `next_layer`, `lowest_layer`, `start_receiving`, `stop_receiving`, `set_on_packet_received`, `set_on_error`, `on_packet_received`, `on_packet_size_received<u16>`, `on_packet_has_been_sent`, `new_packet`, `delete_packet`. Needed by `tcp_packet_client.cpp` (still excluded). |
| `udp_match_packet.h`+`_1`                       | `udp_match_packet.h`                | INCOMPLETE | `class udp_match_packet : packet<udp_match_packet>` present with full member layout (0x12C). Missing: nested `struct helper { static void call_destructor(udp_match_packet&); }`, free `new_udp_match_packet(allocator&)` / `delete_udp_match_packet(allocator&, udp_match_packet*&)`, `header_size()`. References undefined `udp_match_client_session`, `sequence_number<u16>` (need primary template). |
| `udp_match_packets_allocator.h`                 | `udp_match_packets_allocator.h`     | MISSING CLASS | **DEFECT:** out-of-line ctor `udp_match_packets_allocator::udp_match_packets_allocator(base_allocator&, void*, u32)` defined, but the CLASS is never declared anywhere. Must add the class declaration. |
| `udp_match_connection.h` / `..._inline*` (UNION) | `udp_match_connection.h`           | INCOMPLETE | **DEFECT:** `class udp_match_connection public : boost::noncopyable` (invalid). Missing nested types: `channel` (ctor/dtor inline in carcass), `comparer` (from `tree_type` typedef), `state` (member `m_state`), enums `low_level_message_type_enum`. Missing methods from `_inline` union: `call_predicate<predicate>`, `process_incoming_packet<predicate>`, `construct_packet`. References undefined `udp_match_packets_orderer`, `disconnect_event_types_enum`, `udp_match_packets_count_enum`. |
| `udp_match_client.h`                            | `udp_match_client.h`                | INCOMPLETE | **DEFECT:** `class udp_match_client public : boost::noncopyable` + a block of invalid `class boost::function<...>;` / `class boost::asio::...;` forward-decls. Layout (0xB28) + method set present. Needs the boost fwd-decls replaced by real includes and base-syntax fix. |
| `udp_match_server.h`                            | `udp_match_server.h`                | INCOMPLETE (no TU) | Same invalid patterns. Has NO `.cpp` (header for a class we have no source for). References undefined `udp_match_client_session`, `udp_match_packets_orderer`, nested `comparer`. Only needs to COMPILE when transitively included. |
| `udp_network_flow_emulator.h`                   | `udp_network_flow_emulator.h`       | INCOMPLETE | **DEFECT:** `class udp_network_flow_emulator public : core::noncopyable` + invalid fwd-decls. Layout (0x30) + methods present. References undefined `udp_network_flow_emulator_options`, `udp_match_packets_orderer`; needs `<vostok/...>` includes for `random32`/`vectora` (exist in core). |
| `udp_match_stats.h`+`_1`                         | `udp_match_stats.h`                 | COMPLETE | `udp_match_items_stats` / `udp_match_stream_stats` / `udp_match_stats` structs with full layout; free `operator>=` (x3), `operator-`. Valid C++. |
| `handler_allocator.h`                           | `handler_allocator.h`               | COMPLETE | Valid, matched (enabled via async_connector). |
| `http_client.h`                                 | `http_client.h`                     | COMPLETE | Enabled TU, matched. |

### Types referenced but with NO definition anywhere (must be created to enable the udp TUs)
`udp_match_client_session`, `udp_match_packets_orderer`,
`udp_network_flow_emulator_options`, `disconnect_event_types_enum`,
`udp_match_packets_count_enum`, `udp_match_connection::{channel,comparer,state,
low_level_message_type_enum}`, `udp_match_server::comparer`, `udp_match_packet::helper`,
`sequence_number` PRIMARY template, free `new_/delete_udp_match_packet`. (`random32`,
`vectora`, `timing::timer`, `core::noncopyable` already exist - just include.)

## What was changed in this PR
- `sources/.../network_core_entry_point.h` (NEW) - 5 free-fn decls.
- `sources/.../sources/network_core_entry_point.cpp` - stub return for
  `get_ip_address`, commented the carcass typedef-scratch block.
- `sources/.../sources/network_core.vcproj` - un-excluded `network_core_entry_point.cpp`.
- `sources/vostok/game_core/sources/temp_include_all.cpp` - `use_network_core_entry_point()` anchor + call.
- Docs: this file, `../enabling_a_library.md` (methodology), `PROGRESS.md` ledger line.

## What was deliberately NOT changed (flagged for the matchers)
The remaining excluded TUs (`tcp_packet_client`, `udp_network_flow_emulator`,
`udp_match_connection`, `udp_match_client`) need the type reconstruction listed
above - primary templates for `packet`/`sequence_number`/`tcp_packet_socket`, ~10
new sibling/nested types, and the invalid-syntax fixes. Those headers
(`packet.h`, `sequence_number.h`, `udp_match_connection.h`, ...) are SHARED and were
under active concurrent matcher edits in sibling worktrees; rewriting them here would
collide (see the no-two-writers rule in `../enabling_a_library.md`). They should be
enabled TU-by-TU by the matcher who owns that file, following the documented recipe.
