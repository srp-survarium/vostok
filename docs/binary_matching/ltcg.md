# MSVC 2008 LTCG: what it does to code, and what a fast loop can expect

This note catalogues what link-time code generation (`/GL` + `/LTCG`) in
VS2008 SP1 does to the engine's code, measured against the project's own
toolchain, and maps it onto the ledger. It answers three questions:

1. Which transformations does LTCG perform, when, and which source shapes steer them?
2. For a residual blamed on LTCG: is it really LTCG, or is the source shape wrong?
3. What would a fast iteration loop that compiles one TU *without* `/GL` get
   right, and where would it mislead?

The short answers:

* **The biggest LTCG effect in this code base is not cross-TU inlining but a
  COMDAT owner-class barrier (T2).** A header inline function is inlined only
  into callers of the same optimisation class as the IL copy the linker kept.
  It explains half of the sampled LTCG-blamed rows, the whole "empty-call
  retention" family (`identity`, `strip_pointer`, `is_aimed`) and most
  inline-vs-call walls in the `/Od` modules.
* About a third of the sampled LTCG-blamed rows are not LTCG at all
  (temporaries, statement order, a different overload, `__LINE__`).
* A per-TU compile without `/GL` reproduces our own LTCG output for 15% of
  functions (20 real TUs, 485 functions). By retail-observable criteria, 93% of
  the open engine rows fall in the class where it cannot be trusted. **As a
  byte verdict, a non-LTCG loop is not worth building.**
* As a *structure* verdict it is useful. In those 345 functions a non-`/GL`
  statement partition never matched retail when LTCG's did not (0 false
  matches), and it found 73% of the real matches.
* **The whole-engine LTCG relink takes 1 min 50 s** (wall clock, replaying the
  real link command over main's libraries). Compilation, not LTCG codegen, is
  what makes builds slow. A single-TU loop with real LTCG (recompile, relink,
  diff one unit) gives exact retail semantics in about 2.5 minutes.

Toolchain: `cl` 15.00.30729.01 (VS2008 SP1) and `link` 9.00, from the repo
flake (`nix develop` in a worktree, `VOSTOK_DIR` = that worktree, isolated
`WINEPREFIX`). All numbers here are from 2026-10-11 on `v0.10b` @ `4b0ba0969`;
the real-TU measurements use `1cf6b9acb`, the source state of main's last base build.

## Contents

- [Method and evidence](#method-and-evidence)
- [The three optimisation classes](#the-three-optimisation-classes)
- [Catalogue](#catalogue) - T1 to T13
- [What LTCG does not do](#what-ltcg-does-not-do)
- [Decision guide: LTCG or source shape?](#decision-guide-ltcg-or-source-shape)
- [Sample: 40 LTCG-blamed ledger rows](#sample-40-ltcg-blamed-ledger-rows)
- [Fast non-LTCG loop: what to expect](#fast-non-ltcg-loop-what-to-expect)
- [`__usercall` annotations (msvc8.0_patcher): assessment](#__usercall-annotations-msvc80_patcher-assessment)
- [Tooling](#tooling)
- [Open questions](#open-questions)

## Method and evidence

**Lab.** Toy programs live in `~/Projects/survarium/campaign/ltcg-lab/`
(outside the repo). `lab.py EXP` builds `exp/EXP/*.cpp` twice with the engine's
real flags:

* per-TU `cl` without `/GL`, then a normal link;
* `cl /GL`, then `link /LTCG`.

Both links use `/OPT:REF /OPT:ICF`, the CRT, and a `/MAP`. Every function is
disassembled by map address, with call targets symbolized
(`out/EXP/{nogl,gl}/funcs.asm`). `exp/*/flags.json` overrides flags per file
(e.g. one TU at `/Od`). Static-unity builds (one `.cpp` that `#include`s the
callee TUs) are variants of the same experiments. The approach is the one
`vostok tool pdb-order-probe` uses: fixed inputs, one change per pair, the
exact command lines kept in each output's `build.log`.

Engine flags, from `binaries/ninja/rsp/game_cl_*.rsp`:

    /Ox /Ob2 /Oi /Oy /GT /GL /GF /MT /GS- /arch:SSE2 /fp:fast /W4 /Zi /TP
    link: /LTCG /OPT:REF /OPT:ICF /DEBUG /LARGEADDRESSAWARE /BASE:0x10000 /FIXED:No

No engine module uses `/Gy`, and none uses `/EH` (only nvcore, nvimage,
squish, OpenEXR, FreeImage and vorbisfile do). The layout study
(`link-layout.md`, branch `layout/retail-order`) confirms there is no `/Gy` in
retail either.

**Real engine TUs.** I compiled 20 TUs (12 from `/Ox` modules, 3 no-`/Og`,
5 `/Od`) without `/GL`, using their module's rsp flags
(`gen/realtu.py`). Their objects were compared with main's delinked LTCG base
objects and with the delinked retail objects, through `objdiff-cli` and through
a relocation-name-insensitive comparison (`gen/shapecmp.py`). Statement
partitions were compared from `llvm-readobj --codeview` line tables against the
PDB evidence.

**Retail-wide scans.** `gen/conv_scan.py` scans all 43,141 retail functions
(`binaries/pdb/target/evidence.sqlite`). For each it records the declared
convention, the registers live at entry, `ret N`, and the call list.
`gen/trust.py` derives the fast-loop trust class of each ledger row from that
scan.

## The three optimisation classes

LTCG behaviour depends on the optimisation class of each TU, not just on
`/GL`. The engine has three classes (rsp files; `build-flags.md` agrees):

| class | flags | modules |
|---|---|---|
| **/Og** | `/Ox` or `/O2` (+`/Ob2 /Oi /Oy /GL`) | animation, collision, core, core_2, engine, game, input, physics, render_*, ui, scaleform, bullet, ode, opcode, vorbis, ogg, lua, luabind, the EXE |
| **/Od-class** | `/Od /Ob2 /Oi /Oy /GL` | game_core, network_core, logging |
| **/Od-class** | `/Ob2 /Oi /Ot /Oy /GL`, no `-O` umbrella (so no `/Og`) | ai, ai_navigation, debug, fs, network, particle, vfs |
| no LTCG | as recorded | sound (`/Od`), zlib, vorbisfile, prebuilt GFx/stlport/CRT/OpenSSL/SpeedTree |

For LTCG the two `/Od`-class rows behave the same. Both emit framed
`push ebp / mov ebp,esp` code, neither is ever inlined across TUs, and
neither takes a custom convention. As the retail scan shows:

| module (retail) | functions | custom entry convention |
|---|---:|---:|
| core | 1,324 | 51.5% |
| stlport (instantiated in engine TUs) | 2,457 | 51.4% |
| render | 2,751 | 42.4% |
| bullet | 1,390 | 39.4% |
| animation | 793 | 38.2% |
| game | 1,607 | 30.2% |
| game_core (`/Od`) | 1,351 | 5.7% (all header COMDATs kept from an `/Og` TU) |
| ai, vfs, particle, network (no `/Og`) | 1,776 | 1.2-2.5% |
| zlib (non-`/GL`, C statics) | 46 | 54% |
| GFx prebuilt | 18,674 | 2.0% |

## Catalogue

Each entry covers:

- what the transformation does and when it triggers
- the lab repro
- the asm before and after
- the source levers
- how to recognise it in a diff
- **non-LTCG view** and **predictable?**: what a per-TU compile without `/GL`
  shows, and whether the difference can be normalised mechanically

### T1. Cross-TU inlining

**What.** LTCG inlines functions across TUs as if they shared a TU. The
decision depends on the callee's size and the call site; the number of call
sites does not matter.

**Measured** (`exp/inl`, `exp/band`, `exp/site`, `exp/budget`). Callees with
2 to 16 statements, of shapes `br` (if/else), `ar` (arithmetic), `st` (store)
and `mx` (mixed), each called from 1, 2 or 4 sites:

| shape | per-TU (static-unity) inlines up to | LTCG inlines up to |
|---|---|---|
| if/else | 5 statements (28 instrs) | 6 statements (33 instrs) |
| arithmetic | 7 (31 instrs) | 9 (41 instrs) |
| stores | 8 (24 instrs) | 9 (27 instrs) |
| mixed | 5 (26 instrs) | 7 (35 instrs) |

* The LTCG budget is a superset of the per-TU one: never fewer inlines, one to
  two statements more (roughly 30 to 45 callee instructions). One, two or four
  call sites give the same decision.
* Per site:
  * inside a loop, the budget grows (br8 was inlined in a loop, not outside it);
  * a call behind a rarely-true branch (`if (g_in == 12345)`) is not inlined,
    even for br6;
  * all-constant arguments make any of these callees inline;
  * an unused result of a callee with side effects becomes a tail `jmp`.
* There is no caller-side cap: 160 inlined copies of a 4-statement callee in
  one caller, in both modes.
* Recursive callees are expanded one level (`fact` was peeled once into `main`).
* Constructors and destructors inline like any other function (`exp/inl`
  `call_ctor`: identical in both modes).
* `__forceinline` written only on a `.cpp` definition gives it inline linkage.
  Other TUs then fail to link (LNK2019 in both modes), so cross-TU forceinline
  exists only through headers.
* `/Od`-class callees are never inlined across TUs. `/Od`-class callers never
  inline functions that are not declared inline (`exp/mix`, `exp/mix2`). See
  T2 for functions that are declared inline.

```
; exp/basic, g_sink = tiny_add(x, 3);   tiny_add in b.cpp
per-TU:  push 3 / push esi / call ?tiny_add@@YAHHH@Z / mov [g_sink],eax
LTCG:    lea eax,[esi+3] / mov [g_sink],eax
```

**Statements.** Inlined code is attributed to the call-site line. VS2008 emits
no inline-site records, so inlining changes a statement's SIZE, never its line.

**Levers.** Callee size and shape; the site's context (loop, branch, constant
arguments); whether the callee is `/Od`-class.

**Recognise.** A call on one side, a run of instructions on the other, at one
statement (a `SIZE` row). `sema ltcg` prints the kept copy's class to separate
T1 from T2.

**Non-LTCG view.** The call stays a call.

**Predictable?** Partly. A unity compile (see the fast-loop section)
reproduces every decision except the boundary band and the loop bonus.

### T2. COMDAT owner-class inline barrier (the big one)

**What.** Each TU that uses a header inline function carries an IL copy of it.
The linker keeps the copy of the first module it loads in pass 1 (the rule
documented in `link-layout.md`). Under LTCG a call site inlines that function
**only if the caller and the kept copy share an optimisation class** ({`/Od`,
no-`/Og`} versus `/Og`). Otherwise the caller calls the kept copy out of line,
with the kept copy's convention.

**Measured** (`gen/owner_matrix.sh`). The caller is `/Od`, `/Ot` (no `/Og`) or
`/Ox`. The kept owner is chosen by link order. A cell shows the out-of-line
calls in the caller:

| kept copy \ caller | `/Od` | `/Ot` | `/Ox` |
|---|---|---|---|
| `/Od` | 0 | 0 | **2** |
| `/Ot` | 0 | 0 | **2** |
| `/Ox` | **2** | **2** | 0 |
| without `/GL` | 0 | 0 | 0 |

A kept copy from a non-`/GL` object, such as a prebuilt library, does **not**
block inlining (`gen/nogl_owner.sh`): the `/GL` caller inlines from its own IL.

**Retail evidence.**

* `identity<bool>` from `VOSTOK_EMPTY_EXPRESSION`, called from `/Od` code:
  `mov byte [ebp-1],0 / lea eax,[ebp-1] / call <1-byte ret>`. That is a call to
  the `/Og` copy, with its `/Og` custom convention (arg in eax).
  `gen/identpch.sh` reproduces this byte for byte once an `/Ox` object loads
  first. With the `/Od` object first, or without `/GL`, the call disappears.
* `strip_pointer<doug_lea_allocator>`: the retail kept copy is framed
  (`push ebp / mov ebp,esp / mov eax,[ebp+8] / pop ebp / ret`), so `/Ox` game
  code calls it (`push eax / call / add esp,4`). Our kept copy is `/Og`, so ours
  inlines it. That is the "strip_pointer per-site LTCG" wall in the memory
  notes, now explained.
* `weapon_core::is_aimed` and the other accessors: the layout study's
  `missing-reference` case. The kept copy came from `game\weapon.obj` (`/Ox`).
* On 20 real TUs, LTCG makes *more* calls than a per-TU compile in every
  `/Od`-class module: game_core 496→824, network_core 75→201, vfs 118→177,
  ai 108→153, network 72→114. It also does so in `/Og` modules whose callees
  are owned by `/Od`-class TUs (collision 16→29).

**Levers.** Which TUs reference the inline function, and in which load order:
the first loaded referencing module owns it. Retail's owner is readable from
the PDB section contributions (`vostok tool link-layout owners` on the layout
branch). Whether the function is a header inline at all also matters: an
out-of-line definition in a `.cpp` is never inlined into `/Od`-class code.

**Recognise.**

* An inline-vs-call difference where the called copy's prologue class differs
  from the caller's class: a framed copy called from `/Og` code, or an
  unframed (custom-convention) copy called from `/Od` code.
* In `/Od` code, any call to an empty or tiny ICF-folded body.
* `sema ltcg` prints "owner-class barrier" for these.

**Non-LTCG view.** Every TU inlines its own copy, so the call is absent (or,
in the opposite case, present). This is the most frequent single difference
between a per-TU compile and the LTCG image.

**Predictable?** Yes, given the owner, which retail exposes for every copy
that survives out of line. It cannot be reproduced in a per-TU compile without
editing the callee's declaration (for example `__declspec(noinline)` via a
forced-include redeclaration, which works only for free functions and
templates, not in-class members).

### T3. Custom calling conventions (callee side)

**What.** A function whose every call site LTCG controls gets a register
convention chosen for its own body: arguments in eax, ecx, edx, esi, edi or
ebx, `this` in eax, stack cleanup moved to the caller, dead arguments dropped.

**Measured** (`exp/conv`):

| declared | LTCG convention seen |
|---|---|
| `a1(int)` | a in ecx |
| `a2(a,b)` | a edx, b ecx |
| `a3(a,b,c)` | a esi, b edx, c ecx |
| `a4(a,b,c,d)` | a edi, b stack, c edx, d ecx; caller pops |
| `__stdcall s3(a,b,c)` | esi, edx, ecx; `ret` instead of `ret 0Ch` |
| `__fastcall f3(a,b,c)` | c moves to esi; `ret` instead of `ret 4` |
| `S::m1(int)` (thiscall) | **this in eax**, a on stack, `ret 4` kept (callee-pop) |
| `S::cm(int)` (`__cdecl` member) | this in eax, caller-pop |
| `S::st(int)` (static member) | a in eax |
| `bigret2(int)` (hidden return pointer) | pointer in eax, k in esi |
| `chr(char, short)` | c in al, s on stack |
| `i64(__int64, int)` | 64-bit on stack, int in eax |
| `fl2(float,float)`, `dbl(double)` | stack; result in st0 (retail: 78 of 43,141 functions read an xmm register before writing it, so xmm arguments are rare) |
| retail `~stage_light_propagation_volumes` | `this` **on the stack** with `ret 4` |

* The register assignment is chosen **from the callee's body, not its callers**.
  `exp/callers`: the same assignment results whether or not another TU adds
  differently shaped calls.
* Kept standard (verified):
  * address taken (`g_fp = addr_taken`)
  * virtual, when called through the vtable
  * `__declspec(dllexport)`
  * recursive
  * contains `__try`
  * called from any non-`/GL` object (`exp/mix`)
  * compiled `/Od`-class
* For a virtual destructor, only `??_G` (the vtable entry) is pinned; `??1` may
  still become custom (retail does this).

**Retail scale.** 7,198 of 43,141 functions have a custom entry; 1,866 take
`this` in eax. 2,803 use esi/edi/ebx/ebp for arguments (ebp: 30). 529 take
three or more register arguments.

**Levers.** Whatever pins a function to the standard convention: a function
pointer or `boost::bind` taking its address, `virtual`, export, recursion,
SEH. The `/Od` class also pins it, as does any non-`/GL` caller. When retail
is custom and ours standard, look for the pin in our source.

**Recognise.** `entry` and `ret` lines in `sema ltcg`. The body is identical
after renaming registers, except at the entry and at `ret N`.

**Non-LTCG view.** The standard convention: `mov r,[esp+N]` loads, `ret N`,
`this` in ecx.

**Predictable?** The registers are readable from retail, and a function that
differs *only* in its entry normalises mechanically (stack loads become live-in
registers, then strip `ret N` and rename). But only 2 of 485 real functions
differed only there; the rest differ in more ways. A per-TU compile reproduces
custom conventions for **static free functions** in a unity TU (see the fast
loop), never for members, even in an anonymous namespace.

### T4. Call-site marshalling (callers adapt)

**What.** Every LTCG caller passes arguments to a custom-convention callee the
way the callee wants: registers instead of pushes, no `add esp`, merged
cleanups. This includes `/Od`-class callers: an `/Od` body that calls an `/Og`
callee passes `this` in eax (`exp/mix`, `from_od`).

```
; exp/conv, a3(x, y, z)
per-TU:  push edi / push ebp / push ebx / call a3 / add esp,0Ch
LTCG:    mov ecx,edi / mov edx,ebp / mov esi,ebx / call a3
```

**Levers.** The callee: see T3. A caller cannot change it.

**Recognise.** Shared callees with different push counts or argument registers
(`marshal` lines in `sema ltcg`). When retail pushes where we pass registers,
retail's callee was pinned (T3) or owned by an `/Od`-class TU (T2).

**Non-LTCG view.** Pushes and caller cleanup at every call.

**Predictable?** Yes, given each callee's retail convention: rewrite
argument setup per call site. It disturbs register allocation and scheduling
around the call, so the result is approximate, not exact.

### T5. Interprocedural constant propagation, dead arguments, specialised remnants

**What.** When every call site passes the same constant, LTCG bakes it into
the callee and stops passing it. Arguments the callee never reads are dropped.

```
; exp/ipo, same_const(x, 9) at both sites; diff_const(x, 3) and (x, 4)
LTCG:  mov eax,ecx / call same_const          ; 9 baked in
       push 3 / mov eax,ecx / call diff_const ; still passed
; exp/conv, ptrarg(arr, 4): n = 4 baked in, so the loop is fully unrolled
       mov eax,[ecx+0Ch] / add eax,[ecx+8] / add eax,[ecx+4] / add eax,[ecx] / ret
; exp/conv, deadp(a, b, c): b is unused, so only a and c are passed (edx, ecx)
```

**Specialised remnants.** Without `/Gy`, the out-of-line body of an extern
function that every caller inlined is still emitted, and specialised by IPCP.
`exp/basic`: `tiny_add` was inlined everywhere, but its remnant is
`add eax,3 / ret`. Retail carries such bodies; they reflect the inlined
callers, not a standalone compile.

**Return values** are not propagated: `ret_const()` always returns 42, but the
caller still computes `inc eax` after the call.

**Levers.** The whole program's argument values: one caller with a different
value disables the specialisation.

**Recognise.** A parameter read on one side and replaced by an immediate on
the other; an argument that is never set up; `ret N` smaller by 4 per dropped
stack argument.

**Non-LTCG view.** Every argument is passed and read. A per-TU compile does
**not** perform IPCP, even for statics.

**Predictable?** No. It depends on every call site in the program, so it can
only be flagged.

### T6. Interprocedural register allocation (IPRA)

**What.** LTCG knows exactly which registers a callee clobbers, so a caller
keeps values in eax/ecx/edx across calls that do not touch them.

```
; exp/ipo: p, q, r are live across two calls to leaf(), which clobbers only eax
per-TU:  push ebx/esi/edi; values kept in callee-saved registers
LTCG:    lea edx,[esi*8] / lea ecx,[eax+eax*4] / call leaf / call leaf / add eax,edx ... sub ecx,edx
```

**Levers.** The callee's body (its clobber set).

**Recognise.** Same instructions, different register names, with caller-saved
registers live across a call (`regalloc` in `sema ltcg`).

**Non-LTCG view.** Callee-saved registers and spills.

**Predictable?** Per-TU, static callees in a unity TU get IPRA too. Otherwise
it can only be flagged.

### T7. Whole-program data folding

**What.**

* `extern const int k = 37` folds to an immediate in other TUs (`imul eax,eax,25h`).
* A non-const global that is **never written anywhere in the program**
  (`int g = 7`) folds to `add ecx,7`.
* A constant index into a const table folds (`k_table[2]` → `mov [g],1Eh`).
* A `const char* const` folds to the literal's address, but the character it
  points to is still loaded.
* Float constants stay loads.
* Written globals stay loads.

**Levers.** `const`-ness and whether anything in the program writes the object.

**Recognise.** A memory operand on one side, an immediate on the other.

**Non-LTCG view.** Every cross-TU access is a load from the symbol.

**Predictable?** For `const` objects yes, by substituting the value. For
never-written globals, only with a whole-program write analysis; flag it.
**Address-only** differences (the same symbol at a different RVA) are not code
and disappear when relocations are compared by symbol (objdiff does this).
Placement is covered in `link-layout.md`.

### T8. Pure-call deletion and tail calls

A call whose result is unused and whose callee has no side effects is deleted.
An unused result of a call with side effects becomes a `jmp`. Both happen
per-TU too, for callees the compiler can see. Under LTCG that is any callee.

### T9. Exception-handling frame elimination

With `/EHsc`, LTCG infers interprocedurally that a callee cannot throw and
drops the `fs:[0]` frame and the state stores. This happens even with a
non-inline destructor (`exp/eh2`, `eh_local`); with a throwing callee the frame
stays (`eh_throw`). Per-TU, the frame is always present for unknown callees.
This only matters in `/EHsc` modules (nvcore, nvimage, squish, OpenEXR,
FreeImage); engine modules have no `/EH`. Functions containing `__try` keep the
standard convention and are not inlined.

### T10. Stack frame padding and realignment

LTCG pads frames so that 8-byte locals stay aligned across the known call graph:

* `exp/align`: `sub esp,0Ch` where the per-TU compile has 8, and `14h` instead of `10h`.
* Where entry alignment is unknown (a virtual function, or `main`), it realigns
  dynamically: `and esp,-8`, and `-64` for `main` in `exp/conv`.
* `movdqa` locals get `and esp,-16` in both modes.
* `/Od`-class frames are never padded (`exp/alignod`). **A frame-size delta in
  an `/Od` body is source** (an extra local or temporary), or a T2 cascade
  where an inlined helper brings its temporaries.

### T11. ICF and COMDAT selection

* `/OPT:ICF` folds identical COMDATs (header inlines, templates) in both modes.
* Identical non-COMDAT functions, including two bodies made identical by IPCP,
  are never folded because nothing uses `/Gy`.
* The representative's name is the first loaded module's
  (`link-layout.md`: 1,505 of 1,814 retail fold groups have the same members
  and representative in our build).
* Folding is why one 1-byte `ret` carries 154 names in retail.
* Compare callees by fold group, never by name: `sema ltcg` and the ledger do.
* `/GF` literal pooling across TUs happens in both modes (literals are COMDATs).

### T12. Code layout

Without `/Gy`, each LTCG compiland is one non-COMDAT blob. In the toys, LTCG
emitted a TU's functions in **reverse** definition order, while the per-TU link
keeps definition order. The layout study measures the real ordering (COMDATs
in load order, then non-LTCG archives, then per-compiland blobs in reverse
library waves) and finds that link-line levers move no function bytes. See
`link-layout.md`; it is not repeated here. Functions are 16-byte aligned in
both modes. There is no hot/cold splitting or outlining without PGO, and the
project uses no PGO.

### T13. Mixed `/GL` and non-`/GL` objects

Non-`/GL` objects (sound, zlib, vorbisfile, every prebuilt library) are plain
machine code to LTCG:

* their functions are never inlined into `/GL` code;
* their callers use the standard convention;
* a `/GL` function called from any non-`/GL` object keeps the standard
  convention (`exp/mix`: `ox_small` stays cdecl because `plain.cpp` calls it).

Mixing is legal and silent. The memory notes record that zlib went to 99.99%
once it was built non-`/GL`, the case where retail made plain cross-TU calls.

## What LTCG does not do

| LTCG does not... | evidence |
|---|---|
| devirtualize, even with one implementation or a global object of known type | `exp/basic` `make_impl`, `exp/data` `use_vt`: `mov ecx,<obj> / call [vtbl+N]` |
| inline a function compiled `/Od` or no-`/Og` into anything | `exp/mix`, `exp/mix2` |
| inline a non-inline-declared function into `/Od`-class code | `exp/mix` `from_od` |
| inline a header inline across optimisation classes | T2 matrix |
| propagate return values | `exp/ipo` `use_ret_const` |
| pass float or double arguments in registers (scalar case) | `exp/conv` `fl2`, `dbl` |
| give member functions custom conventions per TU (only LTCG does) | `exp/conv` `uans` variant |
| fold identical non-COMDAT functions | `exp/data` `same_body_b/c`, `spec1/2` |
| pad `/Od`-class frames | `exp/alignod` |
| record inline sites in the PDB | `vostok-pdb inspect --view structure` on `out/basic/gl` |
| split hot and cold code, outline, or reorder statements beyond scheduling | none observed |
| change class layout or vtable order | none observed |

## Decision guide: LTCG or source shape?

Start with `python3 -m vostok sema ltcg <fn>`, then:

1. **Frame-size delta in an `/Od`-class body** (game_core, network_core,
   logging, ai, vfs, fs, network, debug, particle): source. LTCG never pads
   these frames. Look for an extra local, a named temporary, or a by-value
   argument. If one side also calls a helper the other inlines, it is a T2
   cascade.
2. **Inline-vs-call:**
   * the kept copy's class differs from the caller's: **T2**. The lever is the
     COMDAT owner (which TU references the function first), or making it a
     header inline;
   * same class: not a barrier. The callee is not inline, is over the budget
     (T1), or **its body differs**. Example: our `base_node::is_link` is
     `is_soft_link() || is_hard_link()`, while retail inlined one
     `and 300h` test;
   * callee outside the image index (CRT, import, prebuilt): it is never
     inlined, so the source calls something else.
3. **The `entry` or `ret` line differs (T3):** retail custom and ours standard
   means something pins ours (address taken, `virtual`, bind, export,
   non-`/GL` caller). Retail standard and ours custom means the reverse.
4. **`marshal` differs at a shared callee (T4):** the residual belongs to the
   callee, not to this function.
5. **An immediate where the other side loads memory (T5/T7):** argument
   values or never-written globals somewhere in the program.
6. **`regalloc` only:** IPRA or allocation noise. Fix the callees first; park
   the row if they are already exact.
7. **None of the above** (operand order, statement order, branch inversion,
   shrink-wrapping, a different overload, `__LINE__`): **source shape**. LTCG
   does not reorder statements or change control flow.

## Sample: 40 LTCG-blamed ledger rows

The sample is 40 rows with `max < 100` whose ledger note blames "LTCG",
"convention" or "inline-vs-call" (615 such rows exist; stratified by module,
seed 11). Each was read with the target/base diff (`gen/rowdump.py`) and the
tagger.

| # | module | function | verdict | mechanism / reason | lever |
|---|---|---|---|---|---|
| 0 | game_core | `jump_logic::deactivate` | LTCG | T2→T4: retail calls `fsm::states` with this in eax (an `/Og` copy); ours passes ecx (standard) | make `states()` a header inline owned by an `/Og` TU |
| 1 | game_core | `weapon_core_fire_state::get_user_hands_expression` | source | `/Od` frame +8: an extra temporary | expression shape |
| 2 | game_core | `damage_model::deserialize` | source | `/Od` frame +4: result stored to a temporary before use | expression shape |
| 3 | game_core | `game_material_manager_cook::delete_resource` | LTCG | T2: we call the `/Og` `strip_pointer` copy; retail's `/Od`-class copy is inlined | COMDAT owner |
| 4 | game_core | `…shotgun_reload_one_round_substate::on_animation_end` | LTCG | T4: `intrusive_ptr::operator==` register assignment differs (x/y swapped) | the callee's copy or body |
| 5 | game_core | `serialize_affect` | source | a different `packet::append` overload (by buffer vs by value) | call the u8 overload |
| 6 | game_core | `pistol_weapon_core_reload_state::get_weapon_lexeme_pair` | LTCG | T2→T4: `ammo_in_magazine` this in eax (`/Og` copy) | header inline (done on the layout branch) |
| 7 | render | `static_render_surface::add_shadow_vertices` | LTCG (partial) | T4: retail pushes all args to `fill_source_vertices_impl<>`; ours passes eax | find what pins retail's callee (not found) |
| 8 | render | `statistics_value<int>::statistics_value` | source | store order | member/statement order |
| 9 | render | `portal_sector_system::select_models` | source | frame +16 and FP expression order | expression shape |
| 10 | render | `~stage_light_propagation_volumes` | LTCG (partial) | T3: retail `this` on the stack + `ret 4`; ours thiscall (`??_G` is the vtable entry, so `??1` can be custom) | pin not found |
| 11 | render | `radiance_volume::propagate_lighting_iter` | unexplained | three extra `xor ecx,ecx`; allocation | - |
| 12 | render | `stage_lights::stage_lights` | address only | one global at another RVA; no code difference | data placement |
| 13 | render | `on_fs_iterator_materials_ready_children` | LTCG | T2: our `file_type_pointer::operator bool` copy is `/Od`-class (fs) | COMDAT owner |
| 14 | game | `game::~game` | LTCG (partial) | T4 at one call, plus early literal materialisation | callee |
| 15 | game | `lobby_client::read_profile_slots_restrictions` | LTCG | T2: retail's `strip_pointer` copy is framed, so `/Og` game calls it | COMDAT owner |
| 16 | game | `options_resolution_selector::initialize` | source | shrink-wrapped ebx/ebp saves follow the CFG shape | early-return shape |
| 17 | game | `game_options::process_key_input` | unexplained | `/Og` frame −16 and register cascade | - |
| 18 | game | `lobby_menu::fill_found_players` | LTCG (partial) | `GFx::Value::operator=` inline-vs-call (T1 or T2; owner unverified) | check the owner |
| 19 | game | `animation_space_graph_cook::generate_graph_edges` | LTCG | T3: retail takes the graph in ecx with `ret`; ours stack + `ret 4` | find the pin in our source |
| 20 | vfs | `free_node_impl` | source | `is_link` same-class copy not inlined: our body is two calls with `\|\|`, retail one mask test | `is_link` = one `has_flags` |
| 21 | vfs | `set_inline_data<1>` | LTCG + source | T2 (an `identity` instantiation) plus a `__LINE__` immediate | owner; line geometry |
| 22 | vfs | `unlock_node` | source | as #20 | as #20 |
| 23 | vostok | `tuples<…>::tuples` | LTCG (partial) | `/Od`-class COMDAT frame +0x14: T2 cascade inside the kept copy | owner |
| 24 | vostok | `tuples::concat` | unexplained | push-order register swap | - |
| 25 | vostok | `variant<32>::try_get<…>` | source | `__LINE__` immediate only | line geometry |
| 26 | vostok | `single_size_buffer_allocator<128>::allocate` | LTCG | T2: retail inlines the policy helper; ours calls it | owner |
| 27 | animation | `n_ary_tree_animation_time_calculator::visit` | source | branch layout and expression shape | source |
| 28 | animation | `animation_player::deserialize_state` | source (mostly) | branch inversion; some T4 | source first |
| 29 | animation | `n_ary_tree::update_time_synchronization_group` | LTCG | T4: retail callee takes the node in eax; ours pushes | callee's pin |
| 30 | fs | `physical_path_info::children_begin` | LTCG (partial) | no-`/Og` frame +4: a temporary or T2 cascade | - |
| 31 | fs | `append_relative_path<…>` | LTCG | T5/T3: retail passes an extra argument in ecx that ours dropped (dead or constant over our callers) | the callers' argument values |
| 32 | core | `binary_config_cook::on_fs_iterators_ready` | source | statement order | source |
| 33 | core | `query_result::on_create_resource_end` | LTCG | T4: retail pushes where ours passes ecx/edx | callee's pin |
| 34 | ai | `fsm::fsm` | LTCG | T2 in both directions | owner |
| 35 | network_core | `udp_match_client::udp_match_client` | LTCG | T2 plus ICF names | owner |
| 36 | network_core | `udp_match_connection::call_predicate<…>` | LTCG (partial) | `/Od`-class COMDAT frame +0x24: T2 cascade | owner |
| 37 | network | `destroy_http_client` | LTCG | T2: we call the `/Og` `strip_pointer`; retail inlines its framed copy | owner |
| 38 | physics | `bullet_physics_world::initialize` | unexplained | two-instruction register swap | - |
| 39 | scaleform | `D3D1x::HAL::DrawProcessedPrimitive` | LTCG (partial) | a small helper inline-vs-call (T1/T2; owner unverified) | check the owner |

**Tally.** Mechanisms attributed for 25 rows (LTCG or LTCG-partial; #21 also
carries a source part):

| mechanism | rows | of which partial |
|---|---:|---:|
| T2, owner-class barrier, incl. cascades | 13 | 2 |
| T1 or T2, owner not yet verified | 2 | 2 |
| T4, callee convention at call sites | 6 | 2 |
| T3, own convention | 2 | 1 |
| T5, dead argument | 1 | 0 |
| T2 in a `/Od`-class frame (#30, unverified) | 1 | 1 |

The other 15:

* **Not LTCG (12 + 1):** temporaries, statement order, an overload,
  `__LINE__`, an inline refused because our callee body differs, plus one
  data-address-only row (#12). #28 counts here, though it also shows some T4.
* **Unexplained (4):** small allocation or scheduling noise.

Two observations:

* game_core holds 186 of the 615 LTCG-blamed open rows, yet `/Od` code only
  sees T2 and T4. Of the 7 sampled, 3 were source shape.
* The "inline-vs-call wall" in the `/Od`-class modules is T2, and its lever is
  the COMDAT owner. It is not "non-steerable".

## Fast non-LTCG loop: what to expect

The idea: compile one TU without `/GL` (seconds), compare its functions with
retail, normalise or flag the expected LTCG differences, and confirm with a
real LTCG build only at the end. Treat it as a **triage loop, never a verdict**.

### Measured fidelity (20 real TUs, 485 comparable functions)

The comparison ignores relocation names and branch targets. "Non-GL" means
per-TU without `/GL`; "LTCG" means our base, from the same source.

| | count |
|---|---:|
| non-GL identical to LTCG | 64, plus 9 with mechanical normalisation (branch offsets, register renaming, entry convention) |
| LTCG identical to retail | 293 |
| non-GL identical to retail | 63, of which 61 are LTCG-exact too |
| **precision** of a non-GL exact match | **61/63 = 97%** |
| **recall** of LTCG exacts | **61/293 = 21%** |

Why non-GL and LTCG differ before normalisation (421 functions):

| cause | functions | mechanism |
|---|---:|---|
| different number of calls | 279 | T1 cross-TU inlining (LTCG fewer calls: game) and T2 barrier (LTCG more calls: game_core, network_core, vfs, ai, network, collision) |
| same calls, different push counts | 73 | T4 callee conventions, T3 own convention |
| leaf functions that still differ | 45 | T3 own convention, T5 IPCP, T7 folding |
| same calls and pushes | 24 | T6 IPRA, allocation |

### Trust classes

A function's class comes from retail-observable facts alone (`gen/trust.py`,
and the `fastloop` line of `sema ltcg`). It is calibrated on the 463 functions
above that also appear in the retail scan:

| class | rule | n | non-GL = LTCG |
|---|---|---:|---:|
| **exact** | `/Od`-class module; standard own entry; every retail callee outside LTCG, or standard-convention and not header-defined | 30 | **93%** |
| **approximate** | the same in an `/Og` module (cross-TU inlining into it is invisible in retail) | 69 | 45% |
| **unreliable** | own custom entry, or a custom-convention callee, or a header-defined callee called out of line (T2) | 364 | 1.6% |
| non-GL in retail | sound, zlib, prebuilt GFx | - | no LTCG at all: exact by construction |

Three rules decide the class:

* **Exact:** every LTCG mechanism that can reach the function is absent.
* **Approximate:** cross-TU inlining (T1) and the barrier in the reverse
  direction cannot be seen from retail, because inlined code leaves no call.
* **Unreliable:** a mechanism is visibly present.

Header-defined callees count because a non-GL compile inlines every header
inline, while LTCG calls cross-class copies.

### Expected-difference rules

What a non-GL object shows for each mechanism, against retail:

| | non-GL object shows | retail shows | rule |
|---|---|---|---|
| T1 | `call f` | f's body inline at that statement | flag, unless f is `/Od`-class (never inlined) |
| T2 | header inline expanded | `call` to the kept copy (or the reverse) | predictable from retail's kept copy class: flag as expected |
| T3 | `mov r,[esp+N]`, `ret N`, this in ecx | register live-ins, `ret`/`ret N'`, this in eax | normalisable when it is the only difference (rare: 2 of 485) |
| T4 | push/push/call/add esp | mov reg/call | flag per call site whose callee is custom in retail |
| T5 | argument passed and read | immediate, or nothing passed | flag; not reproducible |
| T6 | callee-saved registers, spills | caller-saved registers live across calls | flag; equal after register renaming only in simple cases |
| T7 | load from a global | immediate | flag `const` or never-written globals; address-only differences normalise by symbol |
| T9 | EH frame | none | only in `/EHsc` modules |
| T10 | `sub esp,N` | `sub esp,N+4/8`, or `and esp,-8` | `/Og` only; flag; never in `/Od` |
| T11/T12 | callee names | ICF representatives, different order | compare callees by fold group (`sema ltcg` does this) |

### Structure (statement partition): the useful part

The matching order puts statements and line geometry first. Per-TU
`/Z7` line tables (`llvm-readobj --codeview`) were compared with the LTCG
PDB statements, using lines relative to each function's first line:

| | count |
|---|---:|
| functions | 345 |
| partition non-GL = LTCG | 217 |
| LTCG partition = retail | 206 |
| non-GL partition = retail | 150 |
| **false structure matches** (non-GL = retail, LTCG ≠ retail) | **0** |
| missed matches (LTCG = retail, non-GL ≠ retail) | 56 |
| same verdict, among byte-"unreliable" functions | 233/268 (87%) |
| same verdict, in the exact class | 29/29 |

In 47 of the 56 misses, the non-GL build has extra statements that LTCG
compiles to zero bytes: the call to an inlined empty helper vanishes, or a
constant-folded branch does. So:

* **A structure match in a non-GL compile is a structure match under LTCG.**
* A non-GL `BASE_ONLY` statement that is a call into another TU may vanish
  under LTCG. Do not "fix" it before the LTCG build confirms it.

### The deciding number

Over the committed ledger at `4b0ba0969`, with GFx (prebuilt, no LTCG)
excluded, there are 1,849 open engine rows (`max < 100`):

| class | rows |
|---|---:|
| exact | **6** |
| non-GL in retail | 10 |
| approximate | 118 |
| unreliable | **1,715 (93%)** |

The reasons overlap:

* 1,368 call a custom-convention callee;
* 1,343 call a header-defined function out of line;
* 507 have their own custom entry;
* 372 have convention reasons only (T3 and/or T4, no T2).

For comparison, among the 17,600 engine rows already at 100: 646 are exact,
4,553 approximate (2,656 + 1,897 header COMDATs), 11,909 unreliable, and
492 non-GL.

The open rows are the ones that interact with LTCG, almost by definition. **A
non-GL byte loop would be trustworthy for about 1% of the remaining work.**

### Approximations that do not need a patched compiler

* **Unity TU.** `#include` the callee `.cpp` files into one TU and compile it
  without `/GL`. Callers then get T1 decisions identical to LTCG below the
  budget band (`exp/inlunity`: 38 of 103 callers byte-identical, all misses in
  the 6-statement band or from conventions). Definition order in the unity
  file does not matter.
* **Static unity** additionally declares the callee functions `static`
  (`exp/inlstatic`, `exp/conv` `us` variant). The per-TU compiler then gives
  them **the same custom conventions, IPRA and dead-argument removal as LTCG**:
  * 95 of 103 callers and 43 of 43 callee bodies identical to LTCG;
  * 21 of 27 functions in `exp/conv` identical;
  * the misses are member functions (never custom per TU, even in an
    anonymous namespace), IPCP (never per TU), and the inline boundary band.

  The function under test keeps its own standard entry, because its callers
  live elsewhere. Since conventions are callee-determined, a unity with a
  *subset* of the callers is enough (`exp/callers`). In the engine this
  requires rewriting `extern` to `static` in temporary copies; it does not
  help members, which are the majority (30,762 of 43,141 retail functions are
  thiscall).
* **Mini-LTCG.** Relink the TU's `/GL` object with a subset of the last
  build's objects under `/FORCE:UNRESOLVED` (`gen/minilink.py`, about 1 s for a
  module). It does not work: 3 of 26 and 2 of 37 functions matched the full
  build even with the whole module. The subset changes T2 owners and leaves
  callees unresolved, so their standard conventions are assumed. All engine
  objects together hit `LNK1000`.

### Data ordering and placement

Separate the two kinds of data difference:

* **Address-only:** the same symbol at another RVA. It normalises by comparing
  relocation targets by symbol (objdiff does this, sample #12).
* **Value folding (T7):** an immediate replaces a load. This is a code
  difference.

Placement and ownership of data and literals are measured in `link-layout.md`
(branch `layout/retail-order`): 94-96% owner agreement, and link-line levers
move no code. Placement-sensitive codegen was not observed in the lab beyond
T7.

### Recommended loop

1. **Triage, seconds:** compile the TU without `/GL` (`gen/realtu.py` shows
   how: the module rsp minus `/GL`/`/MP`, own PCH). Read the **structure** from
   its line tables and trust a match. For functions in the *exact* class, also
   trust a byte match. For everything else, read `sema ltcg` and the T-rules
   above as "expected differences".
2. **Verdict, about 2.5 minutes:**
   * recompile the TU with `/GL` (ninja does this);
   * re-run `lib` for its module (archive member order is neutral under `/GL`;
     `pdb-order-probe`);
   * relink with `/LTCG` (measured: 1 min 50 s for the whole engine);
   * delink and diff only that unit.

   This is exact LTCG semantics. Today's `vostok build --code-only` spends its
   remaining minutes on whole-image derivations. A unit-scoped
   delink-and-diff after the link would make this the inner loop. That is not
   built here.
3. **Commit:** the full `vostok build`, as CLAUDE.md requires.

Do not add direct objects ahead of the libraries to save the `lib` step: that
changes pass-1 load order, and with it T2 owners and code.

## `__usercall` annotations (msvc8.0_patcher): assessment

`github.com/sushi-shi/msvc8.0_patcher` (a fork of widberg's, used by
FUELDecompilation) patches MSVC 8.0 so that IDA-style annotations compile.
The source was read in `ltcg-lab/ext/msvc8.0_patcher`. Example annotations:
`int __usercall __edx __spoils __ecx f(int x, int y)` and
`int __userpurge __esi __ebx C::f(int x, int y)`.

How it works:

* `cl.exe` predefines `__usercall`/`__userpurge` as `__fastcall <magic int>`
  and the registers as integers.
* `c1`/`c1xx` intercept `yylex` after `__fastcall`, parse the integer list into
  a `REGS_DATA {flags, size, regs[8], spoils}` record, and serialise it into
  the IL after fastcall function attributes.
* `c2` reads it back. It substitutes the 2-entry `FASTCALL_REGS` table in
  parameter register allocation, forces callee- or caller-clean, and masks the
  scratch-register interference with `spoils`.

**1. Coverage of the catalogue.**

| | neutralised? |
|---|---|
| T3 own convention | yes, for register and stack arguments, `this` placement and cleanup. Annotate the function under test with retail's observed convention (the `conv_scan` live-ins and `ret N`) |
| T4 call-site marshalling | yes, by annotating callee declarations |
| T6 IPRA | partly, via `__spoils`; see the gaps |
| T5 dead arguments | no; a dropped parameter cannot be expressed without changing the signature |
| T5 IPCP, T7 data folding | no |
| T1 cross-TU inlining, T2 owner barrier | no; inlining decisions are not conventions |
| T9-T12 | no |

On the ledger, 372 of the 1,849 open engine rows are unreliable for
convention reasons *only*. Those are the rows annotations could move to
"approximate". 1,343 involve T2 and stay unreliable.

**2. Porting to VS2008 SP1.** Every patch site is a hard-coded address and
data offset in SHA-256-pinned VS2005 binaries:

| binary | symbols | patches |
|---|---:|---:|
| `cl.exe` | 4 | 2 |
| `c1.dll` | 23 | 7 (C only; this project needs it only for vorbis/ogg/zlib-style C TUs) |
| `c1xx.dll` | 37 | 13 |
| `c2.dll` | 13 | 4 |

The patches also rely on hidden structure offsets (`FUNCTION_ATTRIBUTES.regs_data`
at +0x10, the convention bits at +0x20, stack-frame offsets such as
`[esp+118h]` inside `save_extra`), and on the IL record layout, which changed
between 14.00 and 15.00. Everything must be re-found by reverse-engineering
`c1xx.dll` and `c2.dll` 15.00.30729.01. That is 77 addresses (26 of them patch sites)
plus about 10 structure offsets, with no public symbols for the compiler DLLs
to lean on. Rough estimate: **one to three weeks** for someone fluent in MSVC
internals, plus an ICE-hunting tail. It is feasible, but it is a project, and
the payoff is capped at the 372 rows above.

**3. Its known gaps, and what they would cost here.**

* **Register arguments are treated as clobbered.** The README says only
  eax/ecx/edx args match reliably. In retail, **2,803 of 7,198
  custom-convention functions (39%)** take arguments in esi/edi/ebx/ebp.
* **No preserve control.** The compiler saves registers that LTCG knew a
  callee keeps, so T6 cannot be reproduced exactly.
* **ebp:** 30 retail functions take an argument in ebp, and frame-pointer
  choice differs. Minor.
* **No xmm parameters:** 78 retail functions read an xmm register on entry. Minor.
* **No `this` on the stack with callee pop?** That shape is expressible as
  `__userpurge` with no registers. Verify it.

**4. Keep it out of the sources.** Retail never had these annotations, so they
must live only in the fast loop:

* **Side table.** `config/ltcg_conventions.tsv` (or a generated artifact),
  keyed by mangled name: registers in parameter order, cleanup, spoils.
  `conv_scan` can derive it from the retail evidence DB.
* **Overlay.** Before a fast-loop compile, a driver writes *temporary copies*
  of the TU and of the headers that declare annotated functions, inserting the
  annotation at each declaration. It puts the copies in an overlay include
  directory first on `/I` (the same mechanism as Scaleform's
  `sdk-overlay`). A forced-include header cannot do this, because a
  redeclaration with a different convention is C2373. Declaration sites come
  from libclang (`vostok-pdb index source` already uses it).
* The real sources, the ledger and every commit never see an annotation. The
  LTCG build stays the verdict.

**Verdict.** Measured against the 2.5-minute real-LTCG single-TU loop, the
patcher buys little. It improves the seconds-scale triage for conventions,
and does nothing for the dominant T2 and T1.

## Tooling

* `python3 -m vostok sema ltcg <fn>` (`scripts/vostok/sema/ltcg.py`) tags one
  function's target/base residual with the mechanisms above. It prints
  `entry`, `ret`, `marshal`, `calls` (with the kept copy's class and an
  owner-barrier verdict), `frame`, `eh` and `regalloc`, plus the function's
  fast-loop trust class. Example:

      calls     only target calls vostok::memory::strip_pointer<vostok::memory::doug_lea_allocator>:
                kept copy is /Od-class, the caller is not: owner-class barrier (T2) - ...
      fastloop  unreliable: calls a custom-convention callee; ...

* Lab scripts, in `~/Projects/survarium/campaign/ltcg-lab/gen/`, not in the repo:
  * `conv_scan.py`: retail-wide convention scan
  * `trust.py`: per-row trust class
  * `realtu.py`: real TU without `/GL`
  * `shapecmp.py`: relocation-insensitive object comparison
  * `minilink.py`: mini-LTCG probe
  * `owner_matrix.sh`, `identpch.sh`, `nogl_owner.sh`: T2 probes

## Open questions

* What exactly sets the inline budget? The band suggests IL-node counting
  after the callee's own optimisation, not source statements.
* Why do some retail callees keep the standard convention where ours are
  custom (#7, #10, #29, #33)? Each needs its pin (address, vtable or bind
  reference) found in our source or proved absent.
* `link-layout.md` reports definition order inside a module; the toys show
  reverse order for LTCG non-COMDAT blobs. Reconcile the two with the
  definition-order probe.
* A unit-scoped delink-and-diff after an incremental LTCG link would make the
  2.5-minute loop real; the build pipeline currently derives the whole image.
