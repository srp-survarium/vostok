# Enabling a whole library to build (stubs everywhere)

How to take a module whose TUs are mostly `ExcludedFromBuild="true"` (zero base
symbols, never compiled) and make the *entire* module compile - even with stub
bodies - so its PDB structure is reproduced as real C++ and matchers can then
fill in the function bodies one by one. Worked example: `network_core`.

This complements [loop_performance.md](loop_performance.md) "Enabling an
ExcludedFromBuild .cpp" (the single-TU recipe) and [MATCHING.md](MATCHING.md)
(the byte-match conventions). Here the goal is *buildability of the type
skeleton*, not byte-matching - bodies stay stubs.

## Where the ground-truth structure actually lives (read this first)
The PDB does NOT hand you a clean header. The information is split across two
kinds of generated carcass, and you reassemble the header from BOTH:

- **`binaries/structure/target/sources/<module>/sources/*.cpp`** - one carcass per
  TU. These carry the *breakpoints / statements* the compiler emitted for that TU:
  the out-of-line function bodies, their `'NN'` source-line numbers, locals, and
  block structure. They do NOT contain class definitions.
- **`binaries/structure/target/sources/<module>/*.h`** and
  **`binaries/structure/target/headers/...`** - the header carcasses. These are the
  actual `.h` files that existed in the original source (MOST were header-inline).
  They carry the inline member bodies, but the *class definition itself is not
  structured* - the member list/offsets are scattered, and member functions appear
  only where some TU instantiated them. You reconstruct the class shape from here
  plus the layout the PDB records (the `/* 0xNN */` offset comments + size).

  The `sources/` tree can hold `.h` carcasses too, not just `.cpp` - a header that
  was fully inlined still shows up as a per-compiland `.h` carcass.

### `_1.h` / `_2.h` suffixes = the SAME header seen in different TUs - take the UNION
A header that was `#include`d into several TUs gets one carcass *per compiland*,
suffixed `_1`, `_2`, ... (e.g. `packet_inline.h`, `packet_inline_1.h`,
`packet_inline_2.h`; `packet_reader_inline.h` .. `_4.h`). Each variant lists only
the inline functions that TU actually emitted - a DIFFERENT subset each time. The
real header is the **union of all the suffixed variants**. To combine them
correctly, **align by the `'NN'` source-line numbers** the carcass prints (the
`<...>|...|...:'NN'` and `[0xVA]` annotations are the original source line of each
function) - same line number across variants = the same function; merge, don't
duplicate. The union of every variant's functions, ordered by line number, is the
header's full inline member set.

### Monomorphised types were TEMPLATES - put the template back
The PDB only stores *instantiated* types, so a class that was a template in source
appears MONOMORPHISED - e.g. `packet<tcp_packet>` and `packet<udp_match_packet>`
as two separate carcasses, or `sequence_number<u8>` / `sequence_number<u16>`, or
`tcp_packet_socket<...ip::tcp::socket...>`. Do NOT write the monomorphised classes;
**reconstruct the single primary `template < ... > class`** whose member set is the
UNION across all its instantiations (combine them exactly like the `_N.h` union).
The concrete `<...>` carcasses are just instantiations of that one template.

## The shape of the problem
The generated `binaries/structure/target/sources/<module>/...` carcasses and our
hand-written `sources/vostok/<module>/*.h` headers are pdb-parser DUMPS, not valid
C++. They were never compiled. Typical defects you will hit, in order of frequency:

1. **`class X public : Y {`** - pdb-parser emits the base list wrong. Fix to
   `class X : public Y {`.
2. **Explicit template specialization with no primary template.** The dump shows
   `class packet<tcp_packet> { ... }` or `class tcp_packet_socket<...socket...> { ... }`
   or `class sequence_number<u8> { ... }`. C++ needs the PRIMARY template first.
   Rewrite as `template < typename T > class name { ... }` whose member set is the
   union the PDB shows across all instantiations; the concrete `<...>` uses become
   ordinary instantiations.
3. **`class boost::function<...>;` / `class boost::asio::ip::udp::socket;`** as
   "forward declarations". You cannot forward-declare a library template
   specialization like that. DELETE these lines and `#include` the real boost
   header instead (the pch already pulls `<boost/asio.hpp>`; add
   `<boost/function.hpp>`, `<boost/array.hpp>`, `<boost/intrusive/set.hpp>` etc. as
   needed). For our OWN sibling types, a plain `class foo;` forward-decl is fine.
4. **Out-of-line member definitions for a class that is never declared.** E.g.
   `udp_match_packets_allocator.h` defines `udp_match_packets_allocator::ctor(...)`
   but no `class udp_match_packets_allocator` exists anywhere. You must write the
   class declaration (members + offsets from the PDB) before the definition.
5. **Referenced-but-undefined sibling types.** Grep the module: a type that appears
   only in `#include`-less references (`udp_match_client_session`,
   `udp_match_packets_orderer`, `disconnect_event_types_enum`, nested
   `channel`/`comparer`/`state`/`*_enum`) has NO definition. Create it - a real
   `class`/`struct`/`enum` declaration with the members/values the carcasses imply,
   bodies stubbed. Pin layout with `STATIC_SIZE_ASSERT( type, 0xNN )` using the
   size the PDB carcass records.
6. **Stub bodies that don't satisfy the return type.** An enabled TU must COMPILE:
   a non-void stub needs a `return {};`/`return false;`/`return NULL;`. The
   carcass often already has one; add it where missing (place it ABOVE the
   `// FUNCTION BODY` comment so the carcass stays intact for the future matcher).

## The recipe (per TU, keep the build GREEN at every step)
1. **Stand up the headers the TU includes** so they are valid C++ (defects 1-6
   above). Forward-declare where a pointer/reference suffices; full-define where a
   member is held by value (layout/size needed). Faithful member ORDER and OFFSETS
   matter (`/* 0xNN */` comments + `STATIC_SIZE_ASSERT`) - the matcher relies on
   them later.
2. **Un-exclude the `.cpp`** in `sources/vostok/<module>/sources/<module>.vcproj`:
   delete its `<FileConfiguration Name="Master Gold|Win32" ExcludedFromBuild="true">`
   block so the `<File>` looks like a built sibling. (This `.vcproj` edit is the
   only TRACKED build-graph change; the `binaries/ninja/*` files are gitignored.)
3. **Fix the TU's own `#include` form.** Copy a *built* sibling's include style -
   `#include <vostok/<module>/<header>.h>` (angle + full path), NOT
   `#include "header.h"` (a sources/-local quote include is a frequent C1083).
4. **Add a reachability anchor** in `sources/vostok/game_core/sources/temp_include_all.cpp`:
   a `void use_<module>_<thing>() { ... }` that touches the new symbols, plus a call
   in `IncludeAll::IncludeAll()`. Without an anchor the linker may drop the TU's
   symbols and they won't appear in objdiff. (Stubs-only modules still want the
   anchor so the obj is pulled into the image.)
5. **`python3 scripts/regen_ninja.py`** - module-granular merge; only the changed
   module's `.ninja`+rsps are rewritten (the other ~51 modules stay byte-identical,
   no cascade). `--dry-run` previews the delta.
6. **Validate compile fast with the module build:** `python3 scripts/rebuild.py <module>`
   (~30s, no relink). Iterate the header/include cascade one ~30s build per error.
   Only when it compiles clean run the full no-arg `python3 scripts/rebuild.py`
   (~70s incremental relink) so the EXE links and `report.json` refreshes.
7. **If a header cascade is too deep to compile cheaply, RE-EXCLUDE the TU and keep
   the build green.** An enabled-but-uncompilable TU breaks the whole base build
   (no EXE -> no delink -> stale `report.json` for every worker). Never leave a red
   build to bank "progress".

## Order TUs by tractability (do the leaves first)
Enable the most self-contained TU first; each green TU de-risks the next.
For `network_core` the dependency order was roughly:
- `network_core_entry_point.cpp` - free functions (`initialize`/`finalize`/
  `get_ip_address`/...), only boost::asio + `memory::base_allocator`. New header
  `network_core_entry_point.h` with 5 free-fn decls.
- `tcp_packet_client.cpp` - needs `tcp_packet_socket<...>` (rewrite to a primary
  template) and `async_connector` (already a built header).
- `udp_network_flow_emulator.cpp` - needs `random32`/`vectora` (exist in core
  headers; just include) + NEW `udp_network_flow_emulator_options`,
  `udp_match_packets_orderer`.
- `udp_match_connection.cpp` - the hub: `udp_match_packet` (+ `helper`, free
  `new_/delete_udp_match_packet`), `sequence_number` PRIMARY template, nested
  `channel`/`comparer`/`state`/`low_level_message_type_enum`,
  `disconnect_event_types_enum`, `udp_match_packets_count_enum`,
  `udp_match_client_session`, `udp_match_packets_orderer`.
- `udp_match_client.cpp` - depends on a green `udp_match_connection`.
- `udp_match_server.h` has NO TU ("a header for a class we have no source for") -
  it only needs to COMPILE when transitively included; stub it but expect no obj.

## Don't break the byte-match you already have
- A TU that was ALREADY enabled and partially matched (e.g. `async_connector`,
  `http_client`, `tcp_packet`) - do NOT restructure its headers' member ORDER or
  add/remove members to make a sibling compile. Layout changes there shift real
  bytes. Add new siblings' types in NEW or untouched headers.
- Stub bodies are fine for buildability, but a stub that changes an
  ALREADY-MATCHED function's bytes is a regression - leave matched `.cpp` bodies
  alone; only touch never-compiled stubs.
- After enabling, run the full `rebuild.py` and check `report-changes.json` is
  empty (no regressions) before committing - same gate as a match commit.

## Gotchas hit in practice (network_core entry-point enable)
- **Trailing `// TYPEDEFS` scratch blocks in the carcass are NOT real code.** A
  carcass `.cpp`/`.h` ends with a `// TYPEDEFS` dump of every typedef the compiland
  saw - frequently with DUPLICATE names (two `typedef ... iterator_type;`) that
  collide (`C2040: differs in levels of indirection`). Comment the whole block out;
  it is pdb-parser scratch, not source. (Cost one module build on
  `network_core_entry_point.cpp`.)
- **A non-void stub must `return` something.** `get_ip_address` returns
  `std::string`; the empty carcass body won't compile. Add `return std::string();`
  ABOVE the `// FUNCTION BODY` comment so the carcass annotations survive for the
  future matcher.
- **Anchor a sources/-local header by forward-declaring its free functions.**
  `network_core_entry_point.h` lives under the module's `sources/` (not the public
  `vostok/network_core/` include path), so game_core's `temp_include_all.cpp` can't
  `#include` it. Forward-declare the free functions inside a
  `namespace network_core { ... }` block in the anchor TU instead.
- **Enabling a TU produces ICF report-changes noise - read it, don't panic.** After
  the relink, `report-changes.json` shows a flurry of `100% -> 0%` on `empty_stub`,
  `[thunk] vcall`, `boost::_bi::storageN`, `float3::float3()`, etc. These are
  identical-COMDAT-folding (ICF) reps moving to a different winning obj when the new
  TU adds code - NOT logic regressions (the symbol still exists at 100% elsewhere).
  Filter them out; only a `from%->to%` drop on a function YOUR change actually
  edited is a real regression. (network_core async_connector + entry-point enables
  both saw this; both were clean.)
- **A pre-broken local ninja graph can block ALL builds.** `binaries/ninja/*` are
  gitignored generated artifacts; a stale `#include`-dep to a deleted header (we hit
  `animation_playback_state.h`, referenced by ~11 `.ninja` files but gone from the
  tree) makes ninja abort with "missing and no known rule to make it" before any
  compile. `regen_ninja.py`'s merge keeps unchanged modules byte-identical so it
  won't refresh the stale dep; either surgically drop the bad implicit-input line
  (`sed -i '/<header>\.h \$$/d' binaries/ninja/*.ninja`) or force-regen the affected
  modules. This is environmental, unrelated to the module you're enabling.

## Concurrency warning
If matchers are editing the same module's headers in parallel worktrees,
serialize: enabling a TU rewrites shared headers (`packet.h`, `sequence_number.h`,
...) and WILL collide. Coordinate through the orchestrator/landing queue - never
two writers on the same header at once.
