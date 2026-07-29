# Rebuilding a whole library's carcass from canonical structure

How to take a Vostok module that is mostly `ExcludedFromBuild` (never compiled, no
base symbols) and rebuild its ENTIRE type skeleton as real C++ - from the generated
canonical structure, with stub bodies - so the module compiles and matchers can then
fill in function bodies one at a time. Worked example: `network_core` (47 files,
rebuilt clean from `feature/agentic-matching-loop-2`).

This is the *clean redo* of the older patch-in-place approach (PR #267's
`enabling_a_library.md`, never merged - its content is subsumed here). Before
replacing the old per-type files, move them to `temp/<module>_legacy/`; that
recoverable tree is the topology/body-salvage queue. Regenerate every interface from
the canonical dump using a separate emptying structure queue as the proof of type
completeness. Drain the two queues together, one real header and its related methods
at a time. Legacy headers recover file placement, grouping, includes, and candidate
bodies, but never override the canonical layout.

## The method in one paragraph
Branch off the integration base. Move EVERY replaceable per-type header and `.cpp`
carcass in the module to `temp/<module>_legacy/`; keep only the handful of files that
are not PDB types (below). Copy the canonical `headers/` and `sources/` trees for the
module into `temp/<module>_structure_queue/`; that second temp tree is the target-type
work-list. Despite their names, generated `headers/` is a synthetic
namespace-keyed, one-type-record-per-file pool, while generated `sources/` contains
every path-preserved file with at least one function - including `.cpp`, `.h`, and
inline/include files. The latter gives strong paths for function-bearing headers,
but the structure generator still does not recover the complete original header
tree. Never copy generated `headers/` verbatim as the new interface. Rebuild each
real header by relating synthetic type records and function-bearing paths to the
legacy header tree and current consumers, porting straightforward methods as their
declarations become available.
Delete entries from either queue only after reproducing, porting, or explicitly
triaging them. When both queues are EMPTY, every canonical type is represented and
every legacy item is accounted for. Then enable the remaining TUs and drive the full
build green.

## Two queues, one header at a time

The PDB structure gives exact types but incomplete source topology:

- generated `headers/` files are synthetic type records and do not identify final
  header grouping;
- generated `sources/` paths cover any file with at least one function, including
  headers, but cannot place data-only/type-only headers;
- function-bearing paths and source-line records identify ownership for only some
  types;
- `_N` variants and nested-type files may describe fragments that belong together.

These properties come directly from the generator:

- every run deletes and recreates its generated `sources/` and `headers/` trees;
- `sources/` walks DBI compilands, groups functions by recorded filename, strips
  the configured engine-path prefix, and preserves the remainder;
- the writer uses create-new semantics, so a recorded path seen in multiple
  compilands becomes `<stem>_1`, `<stem>_2`, and so on;
- `headers/` walks the global type stream and synthesizes filenames from namespace
  and resident type names (with truncation and punctuation replacement).

Therefore a `sources/` path is useful provenance, while a `headers/` filename is
not. Merge suffixed source variants by symbol/source-line evidence, and seed a
durable temp queue from a completed generation before running the generator again.

Use both queues simultaneously. For each real header, reconcile canonical type and
compiland entries with legacy filenames/includes, current consumers, symbol
ownership, source-line clues, and dependency order. The legacy tree is authoritative
for likely file relationships; the target PDB and symbols are authoritative for
declarations and layout. A generated file and a real header are not necessarily
one-to-one. Drain the corresponding structure and legacy entries only after the new
header compiles.

## Reconstruct from owners down

Rebuild the target ownership/call tree from its roots: public facade and entry
points first, then worlds/managers/resource owners, their immediate types and
virtual surfaces, and finally the helpers those owners require. Do not begin with
small convenient leaf helpers. Under `/O2` + LTCG, a helper compiled before its
real owner exists can be discarded, inlined into the bootstrap path, folded with
another implementation, or emitted from the wrong COMDAT island. A high or low
score obtained in that state says little about the target helper. Measure it again
after its owning path exists.

During this bootstrap, use the repository's per-module anchor system instead of
wiring temporary calls into arbitrary production consumers. The old monolithic
`temp_include_all.{h,cpp}`/"temp includes" scheme is deprecated:

1. Each owning project compiles one or more `sources/anchor_<module>*.cpp` files.
   They contain narrowly grouped `use_<thing>()` functions and one public
   `vostok::anchor_<module>()` dispatcher. A module owns its own `use_*` symbols;
   never duplicate them in a central include-everything TU.
2. Declare the module dispatcher in `game_core/sources/anchor.h` and call it once
   from `survarium::IncludeAll::IncludeAll()` in
   `game_core/sources/anchor.cpp`.
3. Keep the one `IncludeAll` instance on the real executable-rooted startup path
   in `game/sources/game_entry_point.cpp`; an instance under a stripped stub does
   not retain the cone.
4. Self-guard anchors with a never-true volatile condition and source arguments
   through volatile placeholders. This retains references without running the
   scaffolding or letting LTCG specialize target bodies from constants.

This system keeps the partial tree reachable without assigning false permanent
ownership. As the top-down pass reaches a real owner, move its references from the
module anchor into the recovered call path; retire empty `use_*` functions,
anchor files, declarations, dispatcher calls, and the final `IncludeAll` instance.

Before implementing each owner, inspect analogous engine code for conventions:
nearby Vostok libraries and the corresponding X-Ray-era subsystem can reveal
expected include layering, allocator and resource-cook idioms, containers, entry
point structure, naming, assertions, and failure handling. Use that material as a
style and architecture prior. It must not override target PDB layouts, signatures,
symbols, source ownership, or measured binary evidence.

## Shared-namespace header pools (the `game` complication)

`headers/` in the canonical dump is keyed by NAMESPACE, not by module. For
`vostok::<module>` libraries the two coincide; for the `game` module the pool is
`headers/survarium/` - one flat dir shared by game, game_core and the pc exe
(728 files). The queue then starts with the WHOLE pool and gets triaged file by
file; record every removal + reason in a `temp/triage_log.md` so the queue stays
the single source of truth:

1. **Already rebuilt elsewhere**: basename (plus nested `outer__inner.h` and
   `_N`-variant forms) matches a header in the sibling module's tree
   (`sources/vostok/game_core/`) - within one namespace a type name is unique
   engine-wide, so a filename match IS the type. Remove.
2. **Defined inside an other-named file**: definition-grep
   (`(class|struct|enum) <name>`) over `sources/vostok/` catches TU-local
   predicates and types that live inside another module's `.cpp`/`.h`. Remove
   only when the hit is in the SAME namespace; a hit in a `vostok::*` module
   (e.g. `flash_renderer` also exists in `vostok::render`) is a name collision,
   NOT coverage - keep the file and flag it CHECK.
3. **The module's own compiland tree** (`sources/vostok/<module>/sources/` in
   the dump) pins a stem to the module directly - those headers are yours.
4. Whatever survives all three is figured out one by one during reproduction
   (consumer grep, legacy tree, rich-index symbols).

## What you KEEP from the original module (everything else is regenerated)
These are common library boilerplate, not PDB types, so the canonical dump does not
contain them:

- `sources/<module>.vcproj` - the build graph (you edit it to un-exclude TUs).
- `library_linkage.h` - `#pragma comment(lib, ...)`.
- `sources/pch.h` / `sources/pch.cpp` - the precompiled header.
- `api.h` / `engine.h` - the `VOSTOK_<MODULE>_API` export macro + empty engine struct.
- `sources/<module>_entry_point.cpp` (+ its `.h`) - the module bootstrap
  (initialize/finalize/memory_allocator/...). The entry-point free functions ARE real
  addressed target functions, so REFRESH their bodies to the fresh carcass format, but
  the file itself is hand-maintained infra you keep.

Everything else - every `*.h` type header, every `*_inline.h`, every `sources/*.cpp` -
is moved into the legacy queue and rebuilt against canonical structure.

## Ground truth = `binaries/structure/target/` (NEVER guess)
- `headers/vostok/<module>/` - synthetic one-file-per-type records: exact members,
  `/* 0xNN */` offsets, access specifiers, `STATIC_SIZE_ASSERT`, method signatures,
  `{ /* no source */ }` inline bodies. These are reconstruction ingredients, not
  original headers. Nested types are separate `Outer__Inner.h` files; enums under
  `enums/`.
- `sources/vostok/<module>/` - path-preserved carcasses for every recorded file
  with at least one function. This is not a `.cpp`-only pool: ordinary headers and
  `*_inline.h` files appear here when they own inline definitions with addressed
  `// FUNCTION BODY[0xVA]` blocks; `.cpp` files carry out-of-line definitions.

You reassemble each real header from BOTH views. **When the canonical header and a real
symbol disagree, the symbol wins** (`binaries/rich/target/index.jsonl`, `mangled`
field; access letter `Q/R`=public, `I/J`=protected, `A/B`=private, virtual variants
`E/F/M/N/U/V`).

Regen with `python3 scripts/generate_structure.py target` (gitignored). It is
deterministic from the shipped PDB, so a present tree is reusable; only refresh if the
carcasses are old-format (addressless `// FUNCTION BODY`).

## The transformations (canonical dump -> compilable C++)
1. **`class X public : Y {`** -> `class X : public Y {`.
2. **`/* INCLUDES */ class boost::a::b<...>;` junk** -> delete; `#include` the real
   header (pch has `<boost/asio.hpp>`; add `<boost/function.hpp>`, `<boost/array.hpp>`,
   `<boost/intrusive/set.hpp>`, `<boost/noncopyable.hpp>`, ... as members need). For our
   OWN siblings a plain `class foo;` forward-decl is fine (pointer/reference members).
3. **Monomorphised type was a TEMPLATE.** `packet<tcp_packet>` + `packet<udp_match_packet>`,
   `sequence_number<u8>`/`<u16>`, `tcp_packet_socket<...socket...>` -> reconstruct the
   primary `template < typename T > class` whose member set is the UNION across the
   instantiations. Put per-instantiation `STATIC_SIZE_ASSERT` after the template.
4. **`_1.h` / `_2.h` suffixes = the SAME header from different TUs - take the UNION,
   aligned by the `'NN'` source-line numbers.** Each variant emits only the inline
   functions that TU used; same line number across variants = same function (merge,
   don't duplicate). The variants can even DISAGREE on a member type (e.g. `sequence_id`
   as `<u8>` vs `<u16>`); pick the primary (non-suffixed) and let the matcher confirm.
5. **Nested type printed standalone** (`struct Outer::Inner {}`, `enum Outer::state {}`)
   -> declare it INSIDE the outer class, in dependency order (a `comparer` used by a
   `channel`'s `boost::intrusive::set` must precede it). `STATIC_SIZE_ASSERT` expands to
   `namespace {...}` so nested-type asserts go at namespace scope AFTER the outer class.
6. **Template MEMBER functions are missing from the header dump** (the PDB type record
   omits uninstantiated template members) but appear in the carcasses as
   `r<unsigned int>()`, `on_packet_size_received<u16>()`, `process_incoming_packet<P>()`.
   Symbol wins: add the `template < typename T >` member.
7. **boost::intrusive member-hook / `vostok::intrusive_list` print the member as a BYTE
   OFFSET** (`member_hook<T, hook, 8>`, `intrusive_list<T, T*, 28, ...>`) - that offset
   is the PDB encoding of a member pointer. Replace with `&T::member`
   (`&udp_match_packet::set_member_hook`, `&udp_match_packet::next`). The member is
   usually private, so add `friend class <owner>;` to T - a friend's NESTED classes
   share its access, so one friend covers the owner's nested `channel`/typedef too.
8. **Long container types -> a member `typedef`** (engine convention): e.g.
   `typedef intrusive_list<...> udp_match_packet_list;` inside the owner, then declare
   members as that typedef. The member-pointer access check happens in the owner's
   context (hence the friend).
9. **Bitfields** print as `unhandled type /* Bitfield(... TypeIndex(0x20), length 6 ...)*/
   name`. `TypeIndex(0x20)` = `unsigned char` -> `u8 name : 6;`. Watch tail padding
   (the `STATIC_SIZE_ASSERT` accounts for it; the compiler adds it).
10. **`__formal` = a genuinely-unused param** (MSVC's unnamed placeholder). TWO `__formal`
    in one signature collide - leave them UNNAMED in the definition, or name them +
    `VOSTOK_UNREFERENCED_PARAMETER(S)` with a "PDB: __formal, genuinely unused" note so
    the next agent keeps it.
11. **Trailing `// TYPEDEFS` scratch** in a carcass is pdb-parser noise (often duplicate
    `iterator_type` -> C2040) - delete it. Cross-type junk (a stray dtor from another
    module in a same-named carcass) - ignore it.

## Stub-body policy (no functional reconstruction)
- Leave inline bodies as `{ /* no source */ }`; keep the FRESH addressed
  `// FUNCTION BODY[0xVA]` carcass verbatim in every out-of-line stub.
- A NON-void body that gets compiled needs a return. For a TEMPLATE member, uninstantiated
  bodies are not checked, so `{ /* no source */ }` is fine until a TU instantiates it.
  For a CONCRETE class, an inline non-void body IS compiled at include time - add the
  minimal canonical-sourced return (`return NULL/0/false;`, or `return m_member;` for a
  trivial getter). This is a buildability stub, NOT behaviour.
- **Reference members, const members, and non-default-constructible value members force
  the ctor to carry an init-list** even though the body is a stub. If a base/member ctor
  needs an argument the enclosing ctor does not have, pass a buildability placeholder
  (`0`/`NULL`/a module global) and comment it - a matcher supplies the real value.
- A member type with only an `explicit T(arg)` ctor (e.g. `sequence_number`) makes every
  holder non-default-constructible. Add a `T() { /* no source */ }` default ctor (no
  layout impact; the PDB just never recorded an uninstantiated one) rather than forcing
  every holder to init it.
- An out-of-line definition drops `explicit` / `static` / default args (those live on the
  declaration only).

## Driving the build green (after the temp queue is empty)
1. **Module memory infra**: if a TU wires an allocator it does not receive as a param,
   restore `sources/<module>_memory.h` (`extern memory::base_allocator* g_allocator` +
   `NEW/DELETE` macros) and define the storage in the entry-point TU.
2. **Un-exclude each `.cpp`** in `<module>.vcproj` - delete its `Master Gold|Win32`
   `ExcludedFromBuild="true"` (the only TRACKED build-graph change). Order TUs leaf-first.
3. **Anchor + force-include**: in `game/sources/temp_include_all.cpp` (the
   engine-wide anchor TU, owned by the startup-root game module), add a
   `use_<module>_<thing>()` that touches new symbols, and call it from
   `IncludeAll::IncludeAll()`. A header that NO enabled TU includes must be
   `#include`d here too, so the compiler actually validates it (the rule: every header
   compiles even if its bodies are empty). Forward-declare sources/-local free functions
   in the anchor (those headers are off the public include path).
4. `python3 scripts/rebuild.py <module>` (~30s, no relink) per TU - it regenerates the
   ninja graph itself (write-if-changed), so the .vcproj edit is picked up automatically. Only when the module compiles clean, run the full `python3 scripts/rebuild.py`.
5. **Commit after EACH green TU** (a stray `git reset` once wiped uncommitted work).
6. If a header cascade is too deep to compile cheaply, RE-EXCLUDE the TU and keep the
   build green - an enabled-but-uncompilable TU breaks the whole base build for everyone.
7. **Consumers of deleted headers: REMOVE the `#include` line, don't comment it.**
   vcproj2ninja's include scanner is over-inclusive and picks up even commented
   includes, so a `//#include <vostok/<module>/x.h>` of a deleted header leaves a
   dangling ninja dep ("missing and no known rule to make it") in every consumer TU.

## Draining the legacy topology/body queue

The canonical carcass owns declarations, layouts, and signatures. The parked legacy
tree supplies likely header relationships and candidate method bodies:

1. Match a legacy method to its canonical declaration by name and structure.
2. Port straightforward bodies, adapting them to canonical members, signatures, and
   module ownership. Preserve the fresh addressed carcass evidence until matching.
3. Do not force bodies that depend on removed subsystems or incompatible types.
   Record their disposition in the queue's triage log.
4. Remove a method from its legacy file only after it was ported or triaged; delete
   the file when empty.
5. Keep the module green after every harvest batch. Porting is seeding, not proof of
   a binary match; the normal matcher loop still decides that.

See `game/legacy_harvest.md` for the worked protocol.

## Acceptance gates
- The structure queue's `headers` and `sources` trees are EMPTY.
- The legacy queue is EMPTY, with every unported item accounted for in its triage
  log.
- `grep -rn 'FUNCTION BODY$' sources/vostok/<module>` returns nothing (no addressless
  carcass) and no file carries the old generation's `Created` date.
- `python3 scripts/rebuild.py` GREEN; `report-changes.json` only ICF-folding noise
  (`empty_stub`, `[thunk] vcall`, `boost::_bi::storageN`, `float3::float3()` flipping
  `100%->0%` as the winning COMDAT moves obj) - NOT a real regression.
