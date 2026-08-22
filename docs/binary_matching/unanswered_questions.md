# Unanswered questions / tooling wishlist

Capabilities the matching agent would benefit from but does not have yet. When a
run hits a wall that better tooling would solve, add it here (or sharpen an
existing entry) instead of working around it silently. Keep each item: what is
missing, why it hurts, a sketch of the fix.

## Format
```
### <capability>
WANT: <what the agent wants to do>
WHY:  <the loop step it slows down or blocks>
IDEA: <a possible implementation, if any>
STATUS: missing | in-progress | partial
```

## Open items

### Semantic search of C++ source
WANT: "find the function/type that does X" or "where is this idiom used" across
all `sources/vostok`, by meaning, not exact tokens.
WHY: matching constantly needs the right overload, the existing helper, or a
prior example of a pattern; `rg` only finds literal strings.
IDEA: embed the tree, expose a query tool the agent can call.
STATUS: missing

### Type/declaration lookup in the target structure
WANT: "give me the declaration of `class foo`" from `binaries/structure/target`
as a command.
WHY: the missing-types step (loop section 4) is currently a manual grep through
PDB-generated headers.
IDEA: a small lookup over the target structure keyed by symbol name, returning
the decl plus its source location.
STATUS: missing

### Assembly of a (suspected) inlined function
WANT: when the diff suggests a call was inlined (missing/extra `call` + large
statement-offset delta, section 5), pull the assembly of the *inlined* function
itself - so the agent can see what it compiles to and reproduce that body inline in
the caller's source.
WHY: to match an inline cluster you need the inlinee's code, not just the knowledge
that something was inlined; and pragmas are unresolved (we don't steer inlining),
so reading the inlinee directly is the lever the agent does have.
IDEA: `pdb_fetch --view target --function <inlinee>` already prints any *standalone*
function's asm, so the common case works today. Gap: a function inlined
*everywhere* has no standalone copy in the target - nothing to fetch on that side.
Fall back to the base side's un-inlined copy (`--base-index ... --view base`) or its
`structure`/source. A helper that, given a caller + a suspected inlinee, returns
whichever exists (target standalone -> base standalone -> source) would close it.
STATUS: partial (standalone inlinees fetchable now; inlined-everywhere ones are the gap)

<!-- Append new wishlist items below, same format. -->

---

## Closed

### Cluster detection from PDB inline-site records  [closed - not possible]
WANT: auto-detect which source functions are inlined into one target span (the
"cluster" unit of work in agentic_loop.md).
WHY: when A is inlined into B and cannot be separated, they match as one unit.
FINDING: MSVC 8.0 / VS2008 PDBs carry NO inline-site info - 0 `S_INLINESITE`
records across all modules in both `survarium.pdb` (2396 modules, 47,792 procs)
and our base PDB, checked at the raw CodeView-kind level with 0 parse errors. That
debug info only appears in later toolchains. So clusters cannot be derived from the
PDB; infer inlining reactively from the diff (a missing/extra `call` plus a large
statement-offset delta), per section 5.

## Why does generate_structure skip vostok/scaleform?

The shipped PDB has 10 `vostok/scaleform/sources/*.cpp` compilands (+4 headers
with addressed inlines) - the rich index sees all ~275 functions - but
`binaries/structure/target/sources/` contains no `vostok/scaleform/` dir at
all; every other of the 1,187 compilands is emitted. Found 2026-06-12 during
the game carcass rebuild (the flash/scaleform types were initially mistaken
for all-inlined because of this). Parser-side question: an exclusion list, a
path-classification bug, or something about that lib's debug info?

## Which GFx library should scaleform link, and should we compile Render_BufferGeneric.cpp?

Both builds link a prebuilt GFx, but not the same one. `survarium.pdb` names the
vendor's `C:\w\42216f4658640829\Scaleform\Releases\GFx_4.2.21\Obj\Win32\Msvc90\
Shipping\GFx\*.obj` (Render_HAL, Render_BufferGeneric, Render_Matrix4x4, ...);
we link `binaries.prebuilt/Win32/libraries/shipping/libgfx*.lib`, a gitignored
~197 MB out-of-tree build of the **4.2.22** sources whose members are named
`Z:\...\surv\vostok_4\binaries\Win32\intermediates\gfx\libgfx\*.obj`. Two
consequences, both measured 2026-08-16 (see
`patterns/prebuilt-lib-supplies-comdat.md`):

1. Our libgfx contributes **no line info** to the base PDB, so anything it
   supplies has no base symbol for the delinker and cannot pair at all -
   `MatrixState::MatrixState()` and `MatrixState(HAL*)` alone are 1564 target
   bytes stuck at 0%, about half of scaleform's whole remaining byte gap. The
   target side delinks fine. Rebuilding libgfx with `/Zi` under this tree's
   engine path would make those rows scorable; matching the shipped build's
   switches (ours emits x87 `fld1`/`fldz`, the target SSE `xorps`/`movss`) would
   make them matchable.

2. `sources/vostok/scaleform/sources/scaleform.vcproj` lists
   `..\..\..\scaleform\Src\Render\Render_BufferGeneric.cpp` as built in
   `Master Gold|Win32`, and we do compile it - it is the only GFx implementation
   `.cpp` we build. The shipped build apparently did **not**: `survarium.pdb`
   carries `c:\survarium\sources\scaleform\src\render\render_buffergeneric.h`
   but no `...\render_buffergeneric.cpp` compiland, the target index has zero
   `RBGenericImpl` symbols, and the vendor `Render_BufferGeneric.obj` is in the
   link. Our copy therefore wins the COMDAT and produces ~15 base-only symbols
   the target has nowhere. Excluding the file would align the build inputs (and
   would turn `InitHAL`'s `RenderBufferManagerGeneric(true)` allocation into the
   stock-convention call the target has, worth a handful of bytes), but the
   recovered `.vcproj` is itself target evidence and says the opposite, so the
   two sources of evidence conflict. Deliberately NOT changed; needs a decision
   plus a full-build measurement, since libgfx is linked by render too.

## zlib link provenance resolved (2026-08-22)

Retail and the reconstructed executable now link one implementation: engine
zlib 1.2.3. Both contain one deflate 1.2.3 banner and one inflate 1.2.3 banner,
with no 1.2.7 implementation banner. All nine target zlib translation units are
present on the base side.

The target xrefs show that the real `inflate*` callers are the shipped
Scaleform objects (`ByteArray::uncompress`, `ZLibFile`, and `ZlibSupport`), not
the resource decompression path; resources use PPMd. Retail compiled those GFx
objects against the 1.2.7 headers but resolved their calls against zlibN 1.2.3.

The discrepancy was the `libgfx_zlib.lib` default-library pragma in
`engine_scaleform_initialize.cpp`. It introduced the bundled 1.2.7 archive,
caused duplicate zlib definitions, and kept the engine inflate units out of the
link. Removing that pragma lets the existing GFx callers pull zlibN directly,
so the temporary application-level inflate pin is also gone.

The ledger is 99.99% over 46 zlib functions. The remaining 12-byte `_zcfree`
and 19-byte `_zcalloc` residuals are relocation/ICF attribution details; the
large deflate and inflate bodies are byte-exact.
