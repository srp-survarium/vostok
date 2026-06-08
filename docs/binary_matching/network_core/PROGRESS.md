# network_core matching progress (orchestrator ledger)

Most of `network_core` is `ExcludedFromBuild="true"` (zero base symbols) until a TU
is enabled (vcproj edit + `scripts/regen_ninja.py`). See
[loop_performance.md](../loop_performance.md) "Enabling an ExcludedFromBuild .cpp".

## Ledger
One line per matched function: `module::function -> STATE -> PR (regressions)`.

<!-- Each match commit appends its line below. -->
- network_core::async_connector (enabled TU `async_connector.cpp` - 6 fns):
  - reset() -> STATE[100%|DONE]
  - async_connector::async_connector() -> STATE[27%|PARTIAL] (body byte-correct; LTCG
    ICF fold reps for the 2 boost::function default ctors + 1 extra /Od temp slot)
  - connect(iterator const&) -> STATE[24%|PARTIAL] (structurally byte-identical;
    anon-ns placeholder hash + register-alloc noise depress the %)
  - on_connected(error_code, iterator) -> STATE[64%|PARTIAL] (body byte-correct;
    ICF fold reps + register alloc; LOG string + __LINE__ aligned to target)
  - on_resolved(resolver, error_code, iterator) -> STATE[PARTIAL] (~1510/1520 bytes;
    objdiff can't pair - target param `* const`/QAV vs base `*`/PAV; `* const` breaks
    delete_helper overload)
  - connect(socket,host,port,cb,err) -> STATE[PARTIAL] (~179/177 instrs; logging-heavy
    resolver NEW + query + boost::bind)
  - Headers stood up (were never-compiled PDB stubs): async_connector.h,
    handler_allocator.h, NEW client_error_codes_enum.h, NEW sources/network_core_memory.h
    (extern g_allocator + NEW/DELETE macros; g_allocator defined in async_connector.cpp).
  - Anchor: use_network_core_async_connector in game_core temp_include_all.cpp.
  - objdiff report.json shows 0 for this unit: the 451KB boost-template obj fails to
    LOAD in objdiff ("os error 6"), so the unit is SKIPPED - these 6 can't regress the
    README score. report-changes after the final relink: empty (no regressions).
  - Build kept GREEN throughout (the vcproj enable stays; TU compiles).
- network_core::network_core_entry_point (enabled TU `network_core_entry_point.cpp` - 5 free fns):
  - initialize() -> STATE[100%|DONE] (empty body)
  - finalize() / memory_allocator() / get_connection_info_from_string() / get_ip_address() -> STUB
  - NEW header sources/network_core_entry_point.h (5 free-fn decls). Commented the
    carcass `// TYPEDEFS` scratch (dup `iterator_type` -> C2040). get_ip_address stub
    returns std::string().
  - Anchor: use_network_core_entry_point in game_core temp_include_all.cpp (free fns
    forward-declared there - the header is under the module's sources/, off the
    public include path).
  - Full rebuild GREEN; report-changes = ICF folding noise only (empty_stub / [thunk]
    vcall / boost storageN), no real logic regression.
- Structure audit (declarations-only, see structure_audit.md): the remaining excluded
  TUs (tcp_packet_client, udp_network_flow_emulator, udp_match_connection,
  udp_match_client) need primary templates (packet / sequence_number /
  tcp_packet_socket) + ~10 new sibling/nested types + invalid-syntax fixes in shared
  headers - left for the per-file matcher (concurrency: shared headers were under
  active edits). Methodology written up in ../enabling_a_library.md.
