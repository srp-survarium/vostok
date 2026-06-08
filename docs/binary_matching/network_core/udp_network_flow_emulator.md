# udp_network_flow_emulator

Out-of-line functions of `vostok::network_core::udp_network_flow_emulator`
(`sources/vostok/network_core/sources/udp_network_flow_emulator.cpp`).

## Enabling the TU

The `.cpp` was never compiled into the base (excluded in `network_core.vcproj`).
To match it we:

1. Un-excluded `udp_network_flow_emulator.cpp` in
   `sources/vostok/network_core/sources/network_core.vcproj` (removed the
   `ExcludedFromBuild="true"` from its `Master Gold|Win32` FileConfiguration).
   `make_packet_lost` calls `udp_match_connection::is_low_level_packet`, which lives
   in `udp_match_connection.cpp` (also excluded) - so that TU was un-excluded too to
   resolve the link.
2. `scripts/regen_ninja.py` to merge the two new sources into the ninja graph
   (+2 sources, network_core module only rebuilt).
3. Anchor `use_network_core_udp_network_flow_emulator()` added to
   `game_core/sources/temp_include_all.cpp` (constructs an emulator over a
   `stack_allocator` + a `single_size_buffer_allocator<300,...>` over a static arena +
   a default `udp_network_flow_emulator_options`, then calls `tick` and
   `on_packet_received`). Called from `IncludeAll::IncludeAll()`.

Enabling a fresh TU re-folds COMDATs and the delinker re-attributes many folded
boost/STL template bodies to different representatives, so the README overall %
drops (~48.20 -> ~47.97) with regressions spread across *every* module (render,
particle, sound, ...). This is the known, accepted ICF/fold-churn artifact, not a
real source regression (a single TU cannot regress render). Build stays green.

## Header friend additions

`udp_match_packet` befriends `delayed_packets_predicate` and
`udp_network_flow_emulator` (both read `last_send_time_in_ms` / `m_buffer` /
`append` directly, as the asm does raw `[packet+0x20]` / `[packet+0x2B]`).

## Per-function results (rich-index diff + report.json fuzzy %)

- `on_packet_received`  **100% DONE**. `random_f(1.f) > m_lost_packet_probability ?
  add_packet(...) : make_packet_lost(...)`. (`m_lost_packets_random` at this+0x10.)
- `make_packet_lost`    **100% DONE**. `const bool is_low_level_packet =
  udp_match_connection::is_low_level_packet( base_packet( buffer, buffer_size ) );`
  the LOG body is compiled out in MASTER_GOLD; `endpoint` + the bool are unreferenced.
- ctor                  **87.59% PARTIAL**. Init-list matches except the three
  random32 members: target seeds them `mov ecx, 995A34h/35h/36h` (consecutive, a
  reloc into .rdata per the obj); base default-constructs (seed 0, `xor ecx,ecx`).
  Source seed expression unrecovered (likely `(u32)&<const>`); rest is byte-exact.
- ~dtor                 **58.46% PARTIAL**. `while ( m_delayed_packets.begin() !=
  m_delayed_packets.end() ) { delete_udp_match_packet( m_packets_allocator,
  m_delayed_packets.back().first ); m_delayed_packets.pop_back(); }` - structure
  aligns 4/4 stmts; residual is /Od iterator-temp materialization in the condition
  (target stores begin/end to temps + direct cmp/je; base converts != to a bool
  sete/test/jne, smaller frame).
- `add_packet`          **73.08% PARTIAL**. Reader + two `r<u16>` reads (unused),
  `new_udp_match_packet`, `last_send_time_in_ms = m_ping_random( max-min ) + min +
  time_in_ms`, immediate-appear if `size()+unack >= total_size()/4 - 1`, `memory::copy`
  6-byte header, `append( buffer+6, buffer_size-6 )` (buffer_size in a reg = LTCG arg),
  `push_back( make_pair( packet, endpoint ) )`. Logic/calls all align; residual is /Od
  reg-alloc (edx<->ecx) + a larger target frame (0xB8 vs 0x94, more temp slots) and the
  sequence-id reads landing on different source lines.
- `tick`                **STUB/INPROGRESS** (the big one). remove_if/erase with
  `delayed_packets_predicate`, random_shuffle via `m_out_of_order_random`, then an
  appear loop reading two u16s and invoking the functor + delete. Predicate
  operator() is instantiated by this remove_if (so it has no standalone base symbol
  until tick is written).
- `delayed_packets_predicate::operator()` written (`m_time_in_ms < first->
  last_send_time_in_ms ? false : (push_back(message), true)`) but only emitted once
  `tick`'s `remove_if` instantiates it - currently 0% (not instantiated).
