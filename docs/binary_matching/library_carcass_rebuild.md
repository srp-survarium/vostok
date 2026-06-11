# Rebuilding a whole library's carcass from canonical structure

How to take a Vostok module that is mostly `ExcludedFromBuild` (never compiled, no
base symbols) and rebuild its ENTIRE type skeleton as real C++ - from the generated
canonical structure, with stub bodies - so the module compiles and matchers can then
fill in function bodies one at a time. Worked example: `network_core` (47 files,
rebuilt clean from `feature/agentic-matching-loop-2`).

This is the *clean redo* of the older patch-in-place approach (PR #267's
`enabling_a_library.md`, never merged - its content is subsumed here). Here we throw
the old per-type files away and regenerate
every one from the canonical dump, using an emptying **temp queue** as the proof of
completeness.

## The method in one paragraph
Branch off the integration base. Delete EVERY per-type header and `.cpp` carcass in
the module - keep only the handful of files that are not PDB types (below). Copy the
canonical `headers/` and `sources/` trees for the module into a TEMP dir; that temp
dir is your work-list. Reproduce each canonical file as a compilable header/source in
the real tree, then DELETE it from the temp dir. When the temp dir is EMPTY, every
canonical class/struct/enum/carcass has been reproduced - that is the completeness
signal. Then enable the TUs and drive the build green.

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
is deleted and rebuilt from canonical.

## Ground truth = `binaries/structure/target/` (NEVER guess)
- `headers/vostok/<module>/` - one file per type: exact members, `/* 0xNN */` offsets,
  access specifiers, `STATIC_SIZE_ASSERT`, method signatures, `{ /* no source */ }`
  inline bodies. Nested types are separate `Outer__Inner.h` files; enums under `enums/`.
- `sources/vostok/<module>/` - the per-compiland carcasses: `*_inline.h` carry the
  inline definitions with addressed `// FUNCTION BODY[0xVA]` blocks; `sources/*.cpp`
  carry the out-of-line definitions.

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
3. **Anchor + force-include**: in `game_core/sources/temp_include_all.cpp`, add a
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

## Acceptance gates
- Both temp dirs (`headers` + `sources`) EMPTY.
- `grep -rn 'FUNCTION BODY$' sources/vostok/<module>` returns nothing (no addressless
  carcass) and no file carries the old generation's `Created` date.
- `python3 scripts/rebuild.py` GREEN; `report-changes.json` only ICF-folding noise
  (`empty_stub`, `[thunk] vcall`, `boost::_bi::storageN`, `float3::float3()` flipping
  `100%->0%` as the winning COMDAT moves obj) - NOT a real regression.
