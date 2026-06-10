# network - matching notes

- Namespace: `vostok::network` (+ one GLOBAL-namespace TU-local: `client_destroyer`
  in `match_client.cpp`).
- Build: `/Od` (optimization off) + LTCG, `Master Gold` - see [../MATCHING.md](../MATCHING.md).
- Sources: `sources/vostok/network/` (public headers at module root,
  orders/responses/impls under `sources/`).
- Reachability anchor: `use_network_clients()` in `game_core`'s
  `temp_include_all.cpp`; the engine module is a real consumer
  (`create_world`/`destroy_world`/`world` vtable via `api.h` + `world.h`).
- Carcass rebuilt from canonical structure (see
  [../library_carcass_rebuild.md](../library_carcass_rebuild.md)); the 2012-era
  client/server/packet web it replaced is parked verbatim in
  `temp/network_legacy/` for body reference.

## Module-wide notes

- **The module is an evolution of the parked legacy code.** The orders/responses/
  channel web (`order`, `response`, `functor_*`, `connect/send/receive_*`,
  `two_way_threads_channel`, `network_world`) survived with small changes - the
  packet types moved to `network_core` (`tcp_packet`, `udp_match_packet`), and
  the legacy `client` became `network::tcp_packet_client` over
  `network_core::tcp_packet_client`. Legacy bodies were ported where they fit
  (`STATE[PARTIAL]`, "legacy body ported"); treat them as strong priors, not
  verified matches.
- **`login_client`/`match_client` + impls are new** (no legacy lineage) - pure
  addressed carcass stubs.
- **`match_client_impl` has THREE PDB layout variants** (2048- vs 8192-packet
  storage; `udp_match_client` 0xB20 vs 0xB28) from per-TU type drift in the
  original build. The real one is pinned by the target ctor disassembly
  (0x776240: `0x2000 * 0x12C` arena) and our matched `network_core` types:
  8192 storage, total 0x258BA8 (= dump variant `match_client_impl_2.h`).
- **`functor_order::execute` ICF-folds with `functor_response::execute`**; the
  target attributes the survivor to `sources/functor_response.h`. The legacy
  separate-file layout already produced a 100% `functor_response.h.obj` match -
  keep it.
- **`string_response::execute` likewise folds into `string_order::execute`** (no
  code attributed to `string_response.h` in the target). The body is still REAL -
  write it in `string_response.h` and verify by NAME (`pdb_fetch --view diff
  --function string_response::execute` pairs against the fold survivor at rva
  0x49490; byte-identical as of the responses match); the unit-level objdiff score
  stays `None` because the survivor lives in `string_order.h`'s unit.
- The exe link needs the OpenSSL libs (`libeay32-vc90-mt-s.lib ssleay32-vc90-mt-s.lib`
  + `/LIBPATH:.../sources/openssl/libraries`); they are in the vcproj
  (`survarium_pc_dx11.vcproj` AdditionalDependencies) but the ninja regen can drop
  them from `survarium_-_PC_-_DirectX_11_link.rsp` - re-add them there if the exe
  link fails on `_SSL_*`/`_BIO_*` unresolved externals.
- `udp_match_fixed_packets_allocator< N >` is a real template (instantiated at
  2048 and 8192); `vostok::network_core::udp_match_packets_allocator_ptr` is an
  `intrusive_ptr` with `threading::multi_threading_policy` (the typedef + policy
  friend live in `network_core/udp_match_packets_allocator.h`).
- `sign_up_info` is `vostok`-level (login_server headers); the PDB pins
  `email[100]`, i.e. `max_email_length = 100` in `login_server/constants.h`.
- `login_client.cpp` carries two console-command statics
  (`s_net_client_account_{name,password}_cc`) - registration args are guessed,
  only the dynamic initializers are addressed in the PDB.
- Watch the param-name shadow: methods taking `sign_up_info const& sign_up_info`
  must qualify later TYPE uses in the same signature as `vostok::sign_up_info`.

## Per-function logs
One `<function>.md` in this folder per function that needed real effort (see
[../agentic_loop.md](../agentic_loop.md) section 7). Live status is in the
`STATE[...]` markers in the sources; these files hold the deeper write-ups.
