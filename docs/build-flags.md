# Build-flag comparison — target vs. base

Generated with `pdb_build_info` (vostok-pdb-parser) from the per-compiland
`S_COMPILE3` / `S_ENVBLOCK` records in each PDB.

```
TARGET = survarium.pdb                    (original game)
BASE   = survarium-dx11-win32-gold.pdb    (our VS2008 build)
```

Reproduce:

```bash
pdb_build_info \
  --pdb binaries/nix-store/survarium-game/survarium.pdb \
  --compare binaries/Win32/survarium-dx11-win32-gold.pdb
```

Projects: **target = 69**, **base = 68**, in-both = 65, target-only = 4, base-only = 3.

> **Why some projects show `[no cmdline]`.** Projects compiled with `/GL`
> (link-time code generation) emit no per-file command line into the env
> block — the front-end defers code generation to link time, so only the
> coarse `S_COMPILE3` flags (`LTCG`, `/GS`, no-debug) survive. The actual
> `-O` level for those compilands is **not recoverable from the PDB**.
>
> **`/GL` (LTCG) is *not* an optimization-level flag.** It does not mean the
> code is `-O2`/`-Ox`. LTCG only enables *link-time* work — cross-module
> inlining, cross-module calling-convention selection (custom register
> conventions), and dead-code/COMDAT folding. Whether a translation unit is
> optimized is set independently by its per-file `Optimization` attribute
> (`/Od`, `/O1`, `/O2`, `/Ox`). A project can be `/GL` **and** `/Od`. For the
> `[no cmdline] LTCG` projects we therefore read the optimization level from
> the original **`.vcproj`** instead (see §2), since the PDB hides it.

---

## 1. Differences between target and base

Of the 65 projects present in both, only the ones below differ. Everything
else matches byte-for-flag (see §3).

### 1a. Engine configurations

Every shared `vostok_*` project now reports `MATCH`. In particular:

* `vostok_sound-static-gold` is `/Od`, SSE2, and non-LTCG on both sides.
* `vostok_libfoundation-static` has 13 retained compilands on both sides, with
  the same `Release(static)` archive identity and coarse `LTCG,/GS` flags.
* All three render libraries report the same LTCG flag set as retail.

### 1b. Remaining shared-project differences

| project | status | note |
|---|---|---|
| `libcmt` | DIFF-FLAGS | Prebuilt MSVC CRT; the retained debug-format variants and source counts differ. |
| `libgfxexpat` | PARTIAL | Retail retains only coarse `/GS,no-debug`; the from-source SDK build retains its full non-LTCG command line. |
| `pcre` | PARTIAL | Same coarse-PDB-versus-full-command-line distinction as `libgfxexpat`. |
| `survarium - PC - DirectX 11` | PARTIAL | Both sides are LTCG; retail additionally records `no-debug` in the coarse flags. |

### 1c. Projects only on one side

**Target-only** (in the game, absent from our build):

| project | sources | config |
|---|---|---|
| `libcpmt` | 1 | Prebuilt MSVC C++ CRT member. |
| `libjpeg` | 26 | `[no cmdline] /GS,no-debug` |
| `libpng` | 14 | `[no cmdline] /GS,no-debug` |
| `zlib` | 9 | `/MT /O2 /Ob2 /Oi /Ot /Oy`, SSE, non-LTCG. |

**Base-only** (in our build, absent from the game):

| project | sources | config |
|---|---|---|
| `libgfx_libjpeg` | 26 | From-source SDK JPEG archive. |
| `libgfx_libpng` | 14 | From-source SDK PNG archive. |
| `zlibn` | 9 | `/MD /O2 /Ob2 /Oi /Ot /Oy`, SSE, non-LTCG. |

JPEG and PNG remain archive-grouping differences. The one-sided `zlib`/`zlibn`
records are a real runtime-library identity difference and must not be treated
as proof that their flags match.

### 1d. Retained-compiland count drift

The remaining shared Vostok count differences are small:

```
vostok_animation       target  67  / base  66
vostok_core            target 136  / base 138
vostok_game_core       target 128  / base 127
vostok_physics         target  13  / base  12
vostok_render_engine   target 211  / base 206
vostok_render_facade   target  18  / base  16
vostok_scaleform       target  12  / base  13
vostok_sound           target  54  / base  56
```

These counts describe final-PDB retention and COMDAT ownership, not the number
of source files present in the repository.

---

## 2. Which projects were compiled WITH optimizations vs. WITHOUT

Optimization is the per-project **`Optimization`** attribute (`/Od`,`/O1`,
`/O2`,`/Ox`) — **not** the presence of LTCG. Two sources:

* **non-LTCG projects** — the `-O` flag is in the PDB command line directly.
* **LTCG projects** (all `vostok_*-static-gold` + the EXE) — the PDB hides the
  `-O` level, so it is read from the project's `Master Gold|Win32`
  configuration in `vostok/sources/vostok/**/*.vcproj`.

> Every engine `Master Gold` config also carries `/Ob2 /Oi /Ot(or /Os) /Oy
> /GT /GL` regardless of the umbrella `-O`. So even an `Optimization="0"`
> (`/Od`) engine lib still gets inline expansion + intrinsics; "not optimized"
> here means **no `-O` umbrella**, which is what governs the bulk of codegen.

### Engine static libs — by `.vcproj` `Optimization` (`/GL` LTCG except where noted)

| `Optimization` | meaning | projects |
|---|---|---|
| `3` = `/Ox` (full) | optimized | `animation`, `collision`, `core`, `engine`, `game`, `input`, `physics`, `render_core_pc_dx11`, `render_engine_pc_dx11`, `render_facade`, `ui`, **`survarium - PC - DirectX 11`** (EXE) |
| `2` = `/O2` (max speed) | optimized | `game_server`, `login_server` (server/tool targets) |
| *omitted* = custom | **partial** — no `-O` umbrella, but `/Ob2 /Oi /Ot /Oy` on | `ai`, `ai_navigation`, `debug`, `fs`, `network`, `particle`, `vfs` |
| `0` = `/Od` | **NOT optimized** | **`game_core`**, **`logging`**, **`network_core`** (LTCG); **`sound`** (now `/Od`, **non-LTCG** — fixed, §1a) |

> **⚠ Four targets are `/Od` (optimization disabled):** `game_core`, `logging`,
> `network_core` and `sound`. Code in these must be matched against *unoptimized*
> MSVC output — do not expect inlining/strength-reduction of the umbrella `-O`
> kind. (Inline expansion `/Ob2` is still on.)
>
> **`sound` was fixed on this branch.** `sound.vcproj` previously said
> `/Ox`+`/GL`, but the shipped game's `vostok_sound` was built **`-Od`,
> non-LTCG** (per the target PDB cmdline, §1a). Its `Master Gold|Win32` config
> is now `Optimization=0` + `WholeProgramOptimization=0` to match.

### Non-LTCG projects — `-O` straight from the PDB cmdline

| project | `-O` | optimized? |
|---|---|---|
| `zlib` (target) | `-O2 -Ob2 -Oi -Ot -Oy` | yes |
| `vostok_vorbisfile` | *no `-O`* → default `/Od` | **no** |
| `delayimp`, `dinput8`, `dxerr`, `dxguid` | `-Os -Ox -Oy-` | yes (favor size) |
| `libcmt`, `libcpmt` (CRT) | `-O1` | yes |
| `libeay32`, `ssleay32` (OpenSSL) | `-O2 -Ob2 -Ox` | yes |
| `libgfx`, `libgfx_as2`, `libgfx_as3` | `-Ob2 -Ot -Ox` | yes |
| `stlport_static` | `-O2` | yes |
| `vostok_sound` (target) | `-Od -Ob2 -Oi -Oy` | **no** (umbrella `/Od`) |

### Third-party LTCG libs — `-O` not in PDB, no `.vcproj` here

`bullet`, `ogg`, `opcode`, `vorbis`, `scaleform`, and `libfoundation` ship as
`[no cmdline] LTCG`; the PDB proves their coarse flags but hides the `-O` level.
Their reconstructed project/SDK configurations are therefore verified through
the emitted code and retained PDB records as well as this flag report.

### Prebuilt libs — only coarse `S_COMPILE3` (`/GS,no-debug`), `-O` unknown

```
libjpeg   libpng   pcre   libgfxexpat   libboost_system-vc90-mt-sp-1_48
speedtreecore_v5.2_vc90mt_static   speedtreeforest_v5.2_vc90mt_static
```

### N/A — import-only libs (no compiled code)

```
advapi32 d3d11 d3d9 d3dcompiler d3dx11 d3dx9 dxgi gdi32 kernel32 ole32
oleaut32 psapi shell32 user32 wbemuuid winmm ws2_32 x3daudio xinput
```

---

## 3. Projects matching on both sides

The remaining in-both projects had identical configs and are omitted above.
The full per-project listing is the `pdb_build_info --compare` output; the only
status flags it raised were `DIFF-FLAGS` (§1b), `PARTIAL` (§1a), and `MATCH`
for everything else.
