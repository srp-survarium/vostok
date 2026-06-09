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
- ctor                  **90.69% PARTIAL** -- STRUCTURE MATCH. structure-diff is
  target 0 / base 0 stmts (member-init list collapses to the ctor decl line on both
  sides), 0 size / 0 quantity diffs. Residual 90.69% is purely the three seed
  immediates 0x995a34/35/36 (base operand-masked to 0, target not) -- a delinker
  asymmetry, not a source-shape miss.
- ~dtor                 **58.46% PARTIAL** -- STRUCTURE MATCH (verifier re-confirmed;
  disproves sushi "structure is WRONG", review_todos row left OPEN for sushi sign-off).
  structure-diff: 4/4 stmts align 1:1, **quantity-diffs 0**, size-diffs 2 (the `while`
  condition + the `delete_udp_match_packet` row). The 2 SIZE diffs are STLport
  iterator-compare lowering (target bool-returning op cmp/sete vs base raw-ptr inline
  cmp/je), codegen not structure.
- `add_packet`          **73.13% PARTIAL** -- STRUCTURE MISMATCH (both). 10/10 stmts;
  lower half (memory::copy -> make_pair -> push_back) is identical. Divergence: the two
  dead `reader.r<u16>()` reads -- target keeps both calls (ONLY target L109/L112), our
  LTCG DCEs them to `xor;mov [seq],0` (ONLY base). SIZE diffs on the m_ping_random
  operator()-vs-inline depth. Whole-statement dead-read DCE, unsteerable without a real
  seq-id use.
- `tick`                **56.47% INPROGRESS** -- STRUCTURE MISMATCH (both). target 15 /
  base 13 stmts; 8 ONLY-target stmts (dead seq-id reads + per-packet reader size args via
  the mis-symbolized base_packet accessors that stub to 0) + 5 SIZE diffs on the surviving
  readers. Loop body shape is right; needs the real packet-size accessors to recover the
  dead reads, then re-measure. This remove_if now instantiates the predicate.
- `delayed_packets_predicate::operator()` **100% DONE** -- STRUCTURE MATCH
  (byte-identical). Now instantiated by tick's remove_if; base symbol present at the
  network_core mangling. Both sides 6/6 stmts at IDENTICAL offsets/sizes (0x33 bytes).
  (`--view structure-diff` cannot auto-align it -- target/base demangled names differ
  only by the `vostok::network_core::` prefix -- so compared via single-side dumps.)
