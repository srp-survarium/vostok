# network_core matching progress

One line per matched/parked unit (newest last).

- `async_connector` (6 fns: ctor 0x544ff0, reset 0x544fd0, connect(iterator) 0x5451f0,
  connect(socket) 0x5458a0, on_connected 0x545070, on_resolved 0x5452b0) ->
  **BLOCKED**. `async_connector.cpp` is `ExcludedFromBuild="true"` in
  `network_core.vcproj` (absent from `network_core.ninja`, no base-side symbols), and
  compiling it needs the never-compiled PDB-stub headers `async_connector.h` /
  `handler_allocator.h` stood up + a real `client_error_codes_enum` + the
  base_allocator/delete_helper/tcp_packet/logging/asio template chain. Deep cascade —
  parked without a rebuild; verified target asm + access mangling + reconstructed
  bodies recorded in `async_connector.md`. No rebuild spent; no regressions possible
  (only docs + comment-only edits to an unbuilt .cpp/.h).
