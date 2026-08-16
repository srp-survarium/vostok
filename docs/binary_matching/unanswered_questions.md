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

## zlib reads 0.99% - three causes, none of them "unmatched source" (2026-08-16)

We do not MATCH vendor deps; built from the same source with the same toolchain
they should be exact by construction. zlib scoring ~0 therefore means our BUILD
differs from theirs. Investigated; it decomposes into three independent faults.

**1. `_deflate0` is a delinker artifact, and it costs deflate.c its whole score.**
The base PDB contains **no** `_deflate0` (0 occurrences) - the rich index has a
normal `_deflate`. The name appears only in the delinked
`binaries/objdiff/base/zlib/deflate.c.obj`, where the delinker disambiguated a
DUPLICATE definition by appending a counter. There are two zlibs in the link:
`sources/zlib` and GFx's bundled `libgfx_zlib.lib`, which also defines
`_deflate`/`_inflate`. objdiff then cannot match target `_deflate` against base
`_deflate0`, so every function in the unit scores 0.0 - hence the suspiciously
exact zeros across all nine zlib units. **Tooling, not source.**

**2. Our `_deflate` is nearly twice the target's.**
    target  _deflate  rva=0x534b10  size=0x7d9  (2,009 B)
    base    _deflate  rva=0x52fea0  size=0xe50  (3,664 B)
Same function, 1.8x the code. Different flags or a different zlib build. Our
tree is ZLIB_VERSION 1.2.3. Not explained yet - this is the real question.

**3. The entire inflate side is absent from our build.**
Target delinks nine zlib TUs; we emit three (compress.c, deflate.c, trees.c).
No base object exists for adler32.c, crc32.c, inflate.c, inffast.c, inftrees.c
or zutil.c, and the base index has no `_inflate` at all (target: 0x1510 bytes).
Either those TUs are not in the vcproj, or nothing reachable calls them and
/OPT:REF strips them - the same reachability question the anchors exist for.

Order to attack: (1) is mechanical and unblocks measurement of (2); (3) is a
build-graph question answerable without touching source. NONE of this is
matching work.

Note the same class of question applies to `gfx` (96.55%, 90 KB unmatched) and
`boost`/`stlport`: for a vendor dep, a residual is a build-input discrepancy to
explain, not a queue to work.
