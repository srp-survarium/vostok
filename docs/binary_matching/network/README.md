# network - matching notes

- Namespace: `vostok::network` (+ one GLOBAL-namespace TU-local: `client_destroyer`
  in `match_client.cpp`).
- Build: `/Ob2 /Oi /Ot /Oy` + `/GL` (full optimization + whole-program LTCG),
  `Master Gold` - see [../MATCHING.md](../MATCHING.md). NB: the recurring
  inline-vs-call residuals (boost::function::operator=, intrusive_ptr
  operator*/operator=/copy-ctor, boost::array operator[], packet_reader::pointer)
  are whole-program inline-budget decisions, NOT source-steerable from a single TU.
- Sources: `sources/vostok/network/` (public headers at module root,
  orders/responses/impls under `sources/`).
- The engine owns the real module lifetime through `network::initialize()` and
  `network::finalize()`; game login UI supplies the live login-client path.
- Carcass rebuilt from canonical structure (see
  [../library_carcass_rebuild.md](../library_carcass_rebuild.md)); the 2012-era
  client/server/packet web it replaced was parked in `temp/network_legacy/`,
  since deleted from the tree - `git show 885e1d4a4:temp/network_legacy/<file>` for
  body reference.

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

## Login stack (login_client + login_client_impl_*, 53 fns)

- **Access**: every `on_*`/`*_on_connected`/`create_client`/`sign_in_impl`/impl-plumbing
  method is `AAE` private in the target; the public surface is ctor/dtor +
  `sign_up/sign_in/sign_out` + getters and the target-public connection-settings
  fields used by the game login UI.
- **`login_client::sign_up` has NO standalone target symbol** - LTCG inlined it into its
  optimized-module caller; reconstructed out-of-line in `login_client.cpp` (the impl chain
  binds `boost::ref( sign_up_info )`, so it must pass the long-lived `m_sign_up_info`). The
  target retains `login_client_impl::sign_up` despite having no object-level code or data
  xref, so `network_entry_point.cpp` mirrors that linker retention explicitly.
- **Globals**: `s_net_client_account_name` / `s_net_client_account_password_` (trailing
  underscore!) are GLOBAL-scope extern char[128]; the cc_string statics register them as
  "account_name"/"account_password" (user_specific, serializable). `destroy_client` is a
  GLOBAL-scope static (unmangled PDB name), like tcp_packet_client's.
- **Protocol bytes**: client->server sign_up/sign_in/sign_out = 0/1/2
  (`login_client_message_types_enum`); server answers use the PDB-extracted
  `login_server_message_types_enum` (0x08 servers_connection_info .. 0x1f invalid) - the
  message_types.h reconciliation resolved the #303 sushi@TODO. Client version string
  "0.100b" goes out in a zero-padded char[8].
- **SSL**: `m_ssl_context( boost::asio::ssl::context::sslv23 )` (single-arg ctor, method 9),
  verify file `"../../resources/ssl/survarium_login_server.crt"`, `verify_peer`;
  account name travels in the clear over `m_socket`, password/sign-up info over
  `m_ssl_stream` after `async_handshake`. Ping = 4-byte session_id over `m_ping_socket`
  (udp, `login_udp_port` 25100) on a 1-second deadline timer, `ping_retry_count` 10.
- **LOG line pins**: every `LOG_*` site sits on its original physical line in all 8 impl
  TUs (the `push <line>` immediates byte-match); keep the file layouts line-stable.
- **Recurring residuals** (see assembly_patterns.md): function4::operator()
  inline-vs-call (the 47-67% handlers), boost::function-assign/ctor copy lowering,
  LOG-helper function-ctor scheduling, strip_pointer fold on destroy_client.

## Per-function logs
One `<function>.md` in this folder per function that needed real effort (see
[../agentic_loop.md](../agentic_loop.md) section 7). Live status is in the
`STATE[...]` markers in the sources; these files hold the deeper write-ups.
