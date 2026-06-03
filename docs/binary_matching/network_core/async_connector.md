# network_core::async_connector (6 STUBs)

Unit: all 6 member functions of `vostok::network_core::async_connector`
(`sources/vostok/network_core/sources/async_connector.cpp`):

| fn | target rva | access | size | shape |
|----|-----------|--------|------|-------|
| `reset()`                              | 0x544fd0 | QAE (public)  | 0x18  | `m_connection_state = host_name_is_unresolved;` |
| `async_connector()`                    | 0x544ff0 | QAE (public)  | 0x80  | member-init-list ctor (iterator + 2 boost::function + handler_allocator + ptr/enum) |
| `on_connected(ec, it)`                 | 0x545070 | AAE (private) | 0x17e | error branch + LOG_INFO + m_on_connected()/m_on_error() |
| `connect(iterator const&)`             | 0x5451f0 | AAE (private) | 0xb0  | `m_connection_state=...; async_connect(*m_socket, it, bind(on_connected))` |
| `on_resolved(resolver, ec, it)`        | 0x5452b0 | AAE (private) | 0x5a0 | iterate endpoints, connect(it), errors+LOG, delete resolver |
| `connect(socket, host, port, cb, err)` | 0x5458a0 | QAE (public)  | 0x2b0 | new resolver, itoa port, basic_resolver_query, async_resolve(bind(on_resolved)) |

## Outcome: BLOCKED (deep never-compiled-header cascade) — NO rebuild spent

`async_connector.cpp` is **deliberately excluded from the build**:
`sources/vostok/network_core/sources/network_core.vcproj` line ~1113 marks it
`ExcludedFromBuild="true"`, so `binaries/ninja/network_core.ninja` compiles only
`http_client.cpp` + `tcp_packet.cpp`. There is no `async_connector` symbol on the
base side at all (`grep -c async_connector binaries/rich/base/index.jsonl` -> 0;
no `binaries/objdiff/base/.../async_connector.cpp.obj`). The target side has it
(340 async_connector-related symbols, mostly the per-call boost::bind/asio handler
template instantiations).

To compile even ONE of these bodies, the following never-compiled PDB-stub headers
must first be stood up as real, compilable C++ (each is currently invalid C++ that
no built `.cpp` includes — same class as the documented `udp_match_packet.h`
blocker in loop_performance.md):

1. **`sources/vostok/network_core/async_connector.h`** — current file is a PDB stub
   (`class boost::function<void()>;`, `enum vostok::network_core::async_connector::connection_state_enum {...}` with no real definition). Needs a real class
   modeled on `http_client.h` (which compiles because it `#include <boost/asio.hpp>`),
   with real `boost::function`, the `connection_state_enum`, and `m_socket`/`m_allocator`.
2. **`sources/vostok/network_core/handler_allocator.h`** — also a PDB stub
   (`class handler_allocator public : core::noncopyable`,
   `class boost::aligned_storage<1024,4294967295>;`). Needs `core::noncopyable` base,
   a real `boost::aligned_storage<1024,4>` member + `bool in_use_`, `allocate`/`deallocate`.
3. **`client_error_codes_enum`** — referenced by every network_core header, defined
   nowhere in built code. Needs a real `enum client_error_codes_enum { ... }`.
4. The bodies further pull `vostok::memory::base_allocator::malloc_impl`,
   `vostok::memory::delete_helper<...>`, `tcp_packet` / `tcp_packet_socket<...>`
   (their headers are themselves uncompiled PDB stubs), the
   `vostok::logging::append` LOG_INFO machinery, and the exact
   `boost::asio::async_connect` / `resolver_service::async_resolve` /
   `basic_resolver_query` template instantiations.

### Build-system surgery additionally required (before any rebuild can score this)
- Remove `ExcludedFromBuild="true"` from the vcproj `async_connector.cpp` entry.
- Either re-run `vcproj2ninja` (setup-toolchain) OR hand-edit
  `binaries/ninja/network_core.ninja` (add `async_connector.obj` to the cl build
  edge outputs + inputs + the `lib` edge) and
  `binaries/ninja/rsp/network_core_cl_1.rsp` (add `.\async_connector.cpp`).
- Anchor via a new `use_network_core_async_connector` in
  `game_core/sources/temp_include_all.cpp` (defined + dispatched).

This is a multi-header standup + build-system change + 6 nontrivial asio bodies
(`on_resolved` alone is 0x5a0 bytes). It is exactly the "deep cascade — mark BLOCKED,
don't burn rebuilds" case from the orchestrator heads-up and loop_performance.md.
Parked BLOCKED; access specifiers corrected up front (cheap, no rebuild) so a future
deep pass starts from a correct header skeleton.

## Commands run (verbatim, in order)
- `git log --oneline -1; git status --short`
- `rg -l "async_connector" sources/`
- `grep -c "async_connector" binaries/rich/{base,target}/index.jsonl`  -> base 0, target 340
- `ls binaries/objdiff/{base,target}/vostok/network_core/`  -> no async_connector obj in base
- `grep -in "async_connector.cpp" binaries/ninja/network_core.ninja`  -> absent
- `grep -n "async_connector.cpp" -A8 sources/vostok/network_core/sources/network_core.vcproj`  -> ExcludedFromBuild="true"
- `pdb_rich_query --index binaries/rich/target/index.jsonl --function async_connector --list`
- `pdb_fetch --target-index ... --rva {0x544fd0,0x544ff0,0x5451f0,0x545070,0x5452b0,0x5458a0} --view target/callees/info`
- access mangling: `grep -oE '<fn>@async_connector@network_core@vostok@@[A-Z]+E' binaries/rich/target/index.jsonl`

## Access specifiers (read from target mangling, fixed in header)
- `reset` QAE -> public; `async_connector()` QAE -> public
- `connect(socket,...)` QAE -> public
- `connect(iterator const&)` AAE -> private
- `on_connected` AAE -> private; `on_resolved` AAE -> private

## Reconstructed bodies (for the future deep pass — verified against target asm)
- `reset()`: `m_connection_state = host_name_is_unresolved;`
- `async_connector()`: member-init-list in decl order: `m_host()`, `m_on_connected()`,
  `m_on_error()`, `m_allocator()`, `m_socket(NULL)`, `m_connection_state(host_name_is_unresolved)`.
  (delinker misnames the `boost::function<>` default ctors as `behaviour_cook_params`
  and the `handler_allocator` ctor as `finalize_impl`.)
- `connect(iterator const&)`: `m_connection_state = connection_is_being_established;`
  then `boost::asio::async_connect(*m_socket, iterator, boost::bind(&async_connector::on_connected, this, _1, _2));`
- `on_connected(ec, it)`: `if(error_code){ m_connection_state = host_name_is_unresolved;
  if(m_on_error) m_on_error(connection_failed_enumval, error_code); return; }
  LOG_INFO("connection_has_been_established"); m_connection_state = connection_has_been_established;
  if(m_on_connected) m_on_connected();`  (LOG string lives at .rdata, verbosity from logging::append)
- `connect(socket,...)`: store socket ptr; new resolver via base_allocator::malloc_impl
  + basic_io_object ctor; `char port[6]; _itoa_s(host_port, port, 10);`
  build `basic_resolver_query query(host, port, address_configured)`;
  `resolver->async_resolve(query, boost::bind(&async_connector::on_resolved, this, resolver, _1, _2));`
- `on_resolved(resolver, ec, it)`: error -> LOG + m_on_error; else iterate endpoints
  (`it.increment()/equal(end)`), `connect(it)`; finally `delete_helper(base_allocator, resolver)`.
