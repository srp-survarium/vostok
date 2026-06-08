# udp_match_connection

Matching the out-of-line members of `vostok::network_core::udp_match_connection`
(`sources/vostok/network_core/sources/udp_match_connection.cpp`).

Target rvas (rich/target index):
- on_error                0x545b70
- dump                    0x545b80
- fill_packet_header      0x545bc0
- enqueue_impl            0x545c60
- connect                 0x545d30
- packets_count           0x545d70
- enqueue                 0x545e20
- update_acknowledgements 0x545e70
- ctor                    0x5462a0
- is_low_level_packet     0x5464d0
- ~dtor                   0x546550
- instant_disconnect      0x5465a0
- disconnect              0x546870
- process_low_level_message 0x546ab0
- handle_send             0x546bd0
- send                    0x546fd0
- new_low_level_packet    0x546ec0
- send_packets_list       0x547180
- send_queued_packets     0x5474b0

## Anchor
The cpp was already enabled in the vcproj (3fa0debc) but dead-stripped (no anchor),
so its symbols were absent from `binaries/rich/base/index.jsonl`. Added
`use_network_core_udp_match_connection()` to
`sources/vostok/game_core/sources/temp_include_all.cpp` (called from
`IncludeAll::IncludeAll()`), plus a `test_udp_match_packets_orderer` concrete
subclass of the abstract `udp_match_packets_orderer` returning
`udp_match_message_type_info(false,false,0)` from both pure virtuals. It constructs
an `io_service` -> `udp::socket` + `udp::endpoint` +
`single_size_buffer_allocator<300,single_threading_policy>(NULL,0)` + the orderer,
builds a `udp_match_connection`, and calls its public methods (connect/enqueue/
send_queued_packets/disconnect/instant_disconnect/packets_count) + the public
static `is_low_level_packet`. The private callees (enqueue_impl, ...) resolve
transitively once their public callers reference them.

The "regressions" the rebuild prints (12 down / 41 up) are ICF-folding
representative swaps: enabling the anchor pulls new TUs into the link, so several
already-100% folded symbols get a different printed representative. Net positive,
no real matched function regressed.

## The wall: sibling-unit empty inline STUBs
This class is built almost entirely out of inline helpers that, on this branch, are
empty STUBs in OTHER units' headers:
- `sequence_number<u16>::serialize(pbyte&)` / `operator++` / `operator<=` (sequence_number_inline.h)
- `packet_reader::r<T>()` / `advance()` / `eof()` / ctor (packet_reader_inline.h)
- `delete_udp_match_packet()` / `new_udp_match_packet()` (udp_match_packet.h)
- `handler_allocator` ctor, `udp_match_stats` ctor

In the target these emit real out-of-line calls or real inlined writes; with empty
bodies our build either drops the call (frame/slot shift) or, for the
value-returning `sequence_number::operator++(s32)` (returns a class by value with NO
return statement), CRASHES LTCG with `LNK1257: code generation failed`. Per the
no-cross-unit rule these stubs belong to their own match units; I did not modify
them. So every function whose body leans on them is BLOCKED here, even when the
source shape is correct.

## Per-function results (rich-index --view diff %)
- on_error            100%  DONE     empty body.
- connect             89.5% DONE     bytes match; the only printed diff is the ICF empty_stub representative name. Body: ASSERT + `m_state = connected` + `if(packet) enqueue_impl(packet)`.
- ctor                86.0% DONE     full member-init list in declaration order (refs + all scalar/seq inits: 0 / disconnected(3) / 0xFFFF). Residual = ICF call-name noise + handler_allocator stub's trailing `[+0x400]=0` byte.
- packets_count       58.3% PARTIAL  control flow + ops match; base frame 0x18 larger (intrusive set size() temp) + ICF call-name noise.
- ~dtor               29.7% PARTIAL  auto-generated member-dtor cleanup; target caches `this` in esi, base keeps it in an ebp-slot (frame 8+push esi vs 0xC) - /Od register-alloc residual + ICF call-name noise. (An explicit ASSERT in the body made it WORSE - 21% - so reverted to `{}`; the empty_stub already comes from the member dtors.)
- enqueue             43.3% BLOCKED  faithful (ASSERT + delete in the else); delete_udp_match_packet stub drops the call + slots.
- enqueue_impl        18.8% BLOCKED  is_reliable branch + push_back match; the is_ordered block (sequence_number serialize/++) crashes LTCG if written, so left out.
- is_low_level_packet 22.7% BLOCKED  faithful packet_reader read/advance/eof flow; r<>/advance/eof stubs don't emit the target's out-of-line calls.
- dump                     BLOCKED  single guarded debug-dump statement (bool-guard call + 2-arg log call, both fold to empty_stub); exact logging helper API unresolved - left as VOSTOK_UNREFERENCED_PARAMETERS.
- handle_send / send / fill_packet_header / send_packets_list /
  new_low_level_packet / send_queued_packets / update_acknowledgements /
  process_low_level_message / instant_disconnect / disconnect
                          BLOCKED  STUB carcass kept; all gated on the same sibling-unit stubs (and disconnect/instant_disconnect additionally on `remove_if<remove_all_predicate>` + intrusive erase + delete_udp_match_packet).

## Next steps
The blocked functions become matchable once the sequence_number / packet_reader /
udp_match_packet inline helpers are themselves matched (real bodies). Those are
separate match units. fill_packet_header is the smallest blocked one and a good
first target after sequence_number::serialize(pbyte&) is real.
