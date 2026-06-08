# network_core full-structure handoff (continue from here)

Worktree: `/home/sheep/Projects/surv/vostok_6`, branch
`chore/network_core-structure-completeness`. Build is GREEN; ALL network_core `.cpp`
TUs are enabled and compile (async_connector, http_client, tcp_packet,
tcp_packet_client, udp_match_client, udp_match_connection, udp_network_flow_emulator,
network_core_entry_point), full `rebuild.py` links with no regressions.

## What's done
Reconstructed from canonical structure (`binaries/structure/target/headers/vostok/network_core/`)
and enabled: sequence_number, packet_reader, tcp_packet_socket, tcp_packet_client,
udp_match_packet (+helper), udp_match_stats, udp_match_connection (+channel/comparer/
state), udp_match_client, udp_network_flow_emulator (+options), udp_match_types
(disconnect enum / message_type_info / orderer), entry_point. See per-commit messages.

## What remains
1. Verify the already-valid enabled-TU headers against canonical and bump their
   `Created` date once confirmed (still on 12.10.2025): async_connector.h,
   base_packet.h, client_error_codes_enum.h, handler_allocator.h, http_client.h,
   packet.h, packet_inline.h (has one OLD-format `// FUNCTION BODY`), tcp_packet.h,
   tcp_packet_inline.h, network_core_memory.h, and the .cpp files async_connector.cpp,
   http_client.cpp, tcp_packet.cpp.
2. `udp_match_server.h` - reconstruct from canonical `udp_match_server.h` +
   `udp_match_server__comparer.h` + `udp_match_server__destroy_predicate.h` (+_1). It
   has NO `.cpp` (no TU); just make it COMPILE when included.
3. Classes in the canonical structure not yet stood up as our headers (must all exist
   in the final version): `udp_match_client_session` (+_1), `process_packet_predicate`,
   `move_to_list_predicate`, `udp_match_message_type_info` (currently inlined into
   udp_match_types.h - consider its own header), `udp_match_packets_allocator` (stub
   exists). Acceptance: every canonical type has a corresponding compilable decl.
4. Add reachability anchors in `game_core/sources/temp_include_all.cpp` for the udp
   TUs (use_network_core_udp_match_client / _connection / _network_flow_emulator),
   mirroring use_network_core_tcp_packet_client.
5. Acceptance gates: `grep -rn 'FUNCTION BODY$' sources/vostok/network_core` empty;
   no `12.10.2025` left; `python3 scripts/rebuild.py` GREEN, report-changes only ICF
   folding noise.

## RECOMMENDED: redo cleanly from a temp copy (per the user's plan)
Easier than patching: stage the full canonical structure, then resolve TU-by-TU.

1. `python3 scripts/generate_structure.py target` (refresh canonical; gitignored).
2. Copy the canonical type headers into a TEMP dir, e.g.
   `cp -r binaries/structure/target/headers/vostok/network_core /tmp/nc_headers` and
   `cp -r binaries/structure/target/sources/vostok/network_core/sources /tmp/nc_src`.
3. For our `sources/vostok/network_core/*.h`: for each canonical type, write the
   compilable header (apply the mechanical fixes in
   `../enabling_a_library.md`), then DELETE that file from the temp dir. When the temp
   dir is empty, every canonical class/struct/enum has been reproduced - that is the
   completeness signal.
4. For the `.cpp`s: same - copy each fresh carcass from temp, apply buildability fixes
   (angle include, ctor init-list, `__formal`->UNREFERENCED, drop typedef scratch,
   stub returns only where an enabled TU calls it), delete from temp when done.
5. Un-exclude each `.cpp` in `network_core.vcproj`, `regen_ninja.py`, and validate with
   `python3 scripts/ninja_build.py network_core` (~30s/iteration) before the full
   `rebuild.py`. Commit after EACH green TU (a stray `git reset` once wiped uncommitted
   work - commit early, commit often).

## Hard rules (see ../enabling_a_library.md for the full list)
- NEVER guess. Canonical `headers/` is truth; when it conflicts with a real symbol,
  the symbol (rich index mangled name) wins.
- Leave `{ /* no source */ }` inline bodies; keep the FRESH addressed
  `// FUNCTION BODY[VA]` carcass in every out-of-line stub, verbatim, no abbreviation.
- Long container types -> member typedef; boost::intrusive offset -> `&T::member`
  (+ friend); `__formal` -> named + UNREFERENCED with a "genuinely unused" comment.
