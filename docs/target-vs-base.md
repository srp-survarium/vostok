# Target vs. Base — what the PDB/EXE extraction tooling reveals

A synthesis of every comparison the `vostok-pdb-parser` tooling can now make
between the **target** (original `survarium.exe` / `survarium.pdb`, the shipped
game) and the **base** (our VS2008/Wine build, `survarium-dx11-win32-gold`).

The flag-level detail lives in [build-flags.md](build-flags.md); this report
folds in everything else and answers the blunt question: **what is the same,
what is different, and which differences are actionable for matching.**

| Tool (branch) | What it extracts | Compares? |
|---|---|---|
| `pdb_build_info`   | per-project compiler flags / CRT / LTCG | yes (§2) |
| `pe_build_info` *(improve/pe-build-info)* | PE header, Rich header, CodeView, sections, version | yes (§1) |
| `pdb_link_order` *(improve/link-order)* | per-module link order from section contributions | yes (§3) |
| `pe_rtti` *(improve/rtti-vftables)* | RTTI/polymorphic class set from the EXE | yes (§4) |
| `pdb_parser` *(static / locals / layout-asserts / typedefs)* | the generated reference "carcass" | improves both sides (§5) |

---

## 1. Identity & toolchain (`pe_build_info --compare`)

**Same** — it is unmistakably the same program built with the same toolchain family:

| field | value (both) |
|---|---|
| machine | `0x014c` (i386) |
| linker version | 9.0 (VS2008) |
| subsystem | 2 (GUI) |
| image base | `0x00010000` |
| characteristics | `0x0122` |
| `STLPORT_` section | identical vsize/rawsize |

**Different:**

| field | target | base | why |
|---|---|---|---|
| timestamp | 2013-05-11 | rebuild date | cosmetic (build clock) |
| entry point | `0x00179fae` | `0x000e69a2` | different code layout |
| size of image | `0x04d43000` | `0x06319000` | layout / completeness |
| `.text` vsize | `0x7e2c8b` | `0x5a4ef9` | **base has less code — partial build** |
| `.rdata` vsize | `0x1b5e03` | `0x0f4527` | fewer rdata/vtables/strings |
| `.rsrc` | present (`0x893d4`) | **absent** | base ships no resource section |
| CodeView | `C:\survarium\…\survarium.pdb` | `Z:\home\you\…` (Wine path) | build-environment artifact |

### Rich header (`@comp.id`) — the toolchain fingerprint

This is the most interesting find. The auxiliary tools **match exactly**:

```
prodid 0x96 build 20413  : 2 / 2   (match)
prodid 0xab build 30319  : 3 / 3   (match, VS2010 cvtres/linker tooling)
prodid 0x6d build 50727  : 1 / 1   (match, VC8 component)
```

But the **C/C++ compiler objects diverge in build number**, not just count:

```
prodid 0x83  target: 725 @ 30729(SP1)      base: 204 @ 21022(RTM) + 26 @ 30729(SP1)
prodid 0x84  target: 374 @ 30729 + 2 @ RTM  base: 263 @ 30729  + 74 @ 21022(RTM)
prodid 0x95  target:  82 @ 30729(SP1)       base:  67 @ 21022(RTM)
```

→ **The target is built almost uniformly with VS2008 SP1 (build 30729). The base
build mixes a large share of VS2008 RTM (build 21022) objects.** Objects compiled
by the RTM `cl` will not byte-match SP1 output. This is a concrete toolchain-drift
signal — the Wine toolchain (or some of its prebuilt inputs) is not uniformly
SP1. Worth pinning down which compilands come out as 21022.

(`.rsrc` absent ⇒ both report no `VS_VERSIONINFO`; the game EXE simply has none.)

---

## 2. Compiler flags (`pdb_build_info --compare`) — see build-flags.md

**Same:** 58 projects present on both sides; the vast majority have **identical
flag sets** (CRT model `/MT`, exception/RTTI/security settings, opt level).

**Different (actionable) — now FIXED on this branch:**
* `vostok_sound` — target built **`/Od`, non-LTCG**; base built **`/Ox` + LTCG**.
  Fixed: `sound.vcproj` `Master Gold|Win32` → `Optimization=0`, `WholeProgramOptimization=0`.
* `zlib` — target `-O2` non-LTCG (built in its `Release` config); base LTCG.
  Fixed: `zlib.vcproj` `Release|Win32` → `WholeProgramOptimization=0` (the `.sln`
  maps `Master Gold|Win32`→`Release|Win32` for zlib; `Optimization` was already `2`).
  *Both fixes change how the base builds; the numbers below predate a rebuild —
  rebuild + re-`compare` to confirm `MATCH`.*
* `libcmt` / `libgfx` — nominal only (debug-format split, file-count), prebuilt CRT/GFx; not touched.

**Different (not bugs):** project grouping (`libjpeg`/`libpng`/`zlib` standalone
in target vs folded into `libgfx_*` in base) and per-library source counts
(matching progress, e.g. `vostok_game` 105 vs 46).

---

## 3. Link order (`pdb_link_order --compare`)

**Same:** the program's **first ~24 modules link in the exact same order** —
`finger_print`, `pch`, the survarium entry points, then the `engine\*` units.
The high-level skeleton is reproduced.

**Different:**
* **251 order inversions** relative to base.
* base has **1560 of the target's 2260** code modules → **953 modules are
  target-only** (not yet built into base).
* A handful of modules move a long way, e.g. `core\resources_unmanaged_resource.obj`
  sits at rank 177 in the target but 1253 in base.
* a few **base-only** modules are test/scaffolding (`core\resources_test.obj`,
  `core\geometry_primitives.obj`, `sound\spl_cook.obj`).

→ The link layout matches at the top and diverges as coverage thins out. Useful
once per-obj matching is "done" and the EXE-level layout becomes the target.

---

## 4. RTTI / polymorphic classes (`pe_rtti --compare`)

Recovered straight from the EXE's `.data` type descriptors, independent of the PDB.

| | target | base | in both |
|---|---|---|---|
| non-template classes | 1072 | 856 | **774** |
| template instantiations | 670 | 398 | 320 |

**Same:** 774 polymorphic classes and 320 template instantiations exist on both
sides — the shared, already-built type surface.

**Different:** **298 target-only classes** — of which **272 are `vostok::` /
`survarium::` engine classes** with vtables that the base build does not yet
produce (e.g. `survarium::ai_sound_player`, `survarium::animation_space_graph`,
the whole `booby_trap*` family). This is a precise, name-level **matching
backlog of polymorphic types**. The 82 base-only classes are mostly test/extra.

---

## 5. The generated reference "carcass" (`pdb_parser` improvements)

These don't diff target vs base — they make the generated reference more
faithful **for whichever PDB you feed**, which is the input to all matching work:

| improvement | what it adds | volume (target PDB) |
|---|---|---|
| `static` keyword | internal-linkage free functions now marked `static` | 757 functions |
| LOCALS frame order | locals sorted by `S_BPREL32` offset, annotated `@ -0xNN` | all functions |
| `pstr`/`pvoid` typedefs | `char*`→`pstr`, `void*`→`pvoid` (matches engine aliases) | 2570 / 25558 |
| `--emit-layout-asserts` | `sizeof` guards from PDB type info | 1986 classes |

> **`--emit-layout-asserts` opens a *sixth* comparison axis we don't otherwise
> have:** generate the assert header from the target PDB and from the base PDB
> and diff them, and any `sizeof(T)` that changed is a struct whose **layout
> drifted between the two builds** — a compile-time matching guard at the
> data-structure level.

---

## Bottom line

**What is the same** — and confirms base is genuinely reconstructing the target:
* same architecture, image base, subsystem, CRT model, PE characteristics;
* same compiler family (VS2008) and identical flags for most projects;
* the auxiliary toolchain components (cvtres/linker/masm) match build-for-build;
* the first ~24 link modules are laid down identically;
* 774 RTTI classes and 320 template instantiations are shared.

**What is different**, ranked by how actionable it is for matching:

1. **Config bugs — FIXED on this branch.** `vostok_sound` and `zlib` were built
   with the wrong optimization/LTCG settings vs the shipped game. Corrected in
   the `.vcproj` (sound → `/Od` non-LTCG; zlib → non-LTCG); rebuild the base to
   confirm. Surfaced by §2.
2. **Toolchain drift (investigate).** Base mixes VS2008 **RTM (21022)** objects
   where the target is uniformly **SP1 (30729)**. RTM-compiled objects can't
   byte-match. Surfaced by §1 (Rich header).
3. **Completeness (just progress).** Base is a partial build: ~69% of code
   modules (1560/2260), ~72% of polymorphic classes (774/1072), a smaller
   `.text`. The 272 target-only engine RTTI classes (§4) and 953 target-only
   link modules (§3) are the concrete backlog.
4. **Cosmetic / environmental.** Timestamp, PDB GUID, the Wine `Z:\` PDB path,
   and the absent `.rsrc` section — expected build-environment differences, not
   matching blockers.

---

### Reproduce

```bash
# §1
pe_build_info  --exe survarium.exe --compare survarium-dx11-win32-gold.exe
# §2  (see build-flags.md)
pdb_build_info --pdb survarium.pdb --compare survarium-dx11-win32-gold.pdb
# §3
pdb_link_order --pdb survarium.pdb --compare survarium-dx11-win32-gold.pdb
# §4
pe_rtti        --exe survarium.exe --compare survarium-dx11-win32-gold.exe
```
