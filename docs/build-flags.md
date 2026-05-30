# Build-flag comparison — target vs. base

Generated with `pdb_build_info` (vostok-pdb-parser) from the per-compiland
`S_COMPILE3` / `S_ENVBLOCK` records in each PDB.

```
TARGET = survarium.pdb                    (original game)
BASE   = survarium-dx11-win32-gold.pdb    (our VS2008 build)
```

Reproduce:

```bash
cargo run --release --bin pdb_build_info -- \
  --pdb     ../vcproj2ninja/survarium.pdb \
  --compare ../vcproj2ninja/survarium-dx11-win32-gold.pdb
```

Projects: **target = 69**, **base = 62**, in-both = 58, target-only = 11, base-only = 4.

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

Of the 58 projects present in both, only the ones below differ. Everything
else matches byte-for-flag (see §3).

### 1a. Real flag differences — `vostok_sound` and `zlib` — **FIXED**

These were the only two engine-relevant flag mismatches, and they were the same
kind of mismatch: **the original built them WITHOUT LTCG (real per-file
command line recorded), but our base build compiled them WITH LTCG.**

| project | target flags | base flags (before fix) |
|---|---|---|
| `vostok_sound` | `/MT -GF -GS- -GT -MP -Ob2 -Od -Oi -Oy -TP -Zi -arch:SSE2` | `[no cmdline] LTCG` |
| `zlib` | `/MT -GS- -GT -MP -O2 -Ob2 -Oi -Ot -Oy -TC -Zi -arch:SSE` | `[no cmdline] LTCG` |

**`vostok_sound` in the target was compiled with `-Od` (optimization disabled)**
and *no* `/GL`, whereas our base build turned LTCG and `/Ox` on for it. `zlib`
matched on everything but LTCG (the target built it `-O2`, non-LTCG, in its
`Release` config — which the `.sln` maps `Master Gold|Win32` onto).

**Fixed in the `.vcproj` on this branch** (the rest of both flag sets already
matched the target verbatim — confirmed against the raw PDB command lines):

* `sound.vcproj`, `Master Gold|Win32`: `Optimization` `3`→`0` (`/Ox`→`/Od`) and
  `WholeProgramOptimization` `1`→`0` (drop `/GL`).
* `zlib.vcproj`, `Release|Win32`: `WholeProgramOptimization` `1`→`0`
  (`Optimization` was already `2` = `/O2`).

> These edits change *how the base will build*; the comparison numbers in this
> report still reflect the **pre-fix** base binary. Rebuild the base and re-run
> `pdb_build_info --compare` to confirm both now report `MATCH`.

### 1b. Nominal flag differences — CRT libs only (not our code)

| project | status | note |
|---|---|---|
| `libcmt` | DIFF-FLAGS | MSVC CRT. Same five `-O1` flag-sets; the diff is only a `-Z7` vs `-Zi` debug-format split and the count of files w/o cmdline (66 vs 67). Prebuilt CRT — not matched. |
| `libgfx`  | DIFF-FLAGS | Scaleform GFx. Identical flags; base just has `(+1 file w/o cmdline)`. Prebuilt — not matched. |

### 1c. Projects only on one side

**Target-only** (in the game, absent from our build):

| project | sources | config |
|---|---|---|
| `libeay32-vc90-mt-s` | 420 | `-O2 -Ob2 -Ox` (OpenSSL) |
| `ssleay32-vc90-mt-s` | 37 | `-O2 -Ob2 -Ox` (OpenSSL) |
| `libjpeg` | 26 | `[no cmdline] /GS,no-debug` |
| `libpng` | 14 | `[no cmdline] /GS,no-debug` |
| `pcre` | 11 | `[no cmdline] /GS,no-debug` |
| `libgfxexpat` | 3 | `[no cmdline] /GS,no-debug` |
| `vostok_libfoundation-static` | 13 | `[no cmdline] LTCG,/GS` |
| `vostok_scaleform-static-gold` | 12 | `[no cmdline] LTCG` |
| `shell32` / `ws2_32` / `x3daudio` | 1 each | import libs |

**Base-only** (in our build, absent from the game):

| project | sources | config |
|---|---|---|
| `libgfx_libjpeg` | 26 | `[no cmdline] /GS,no-debug` |
| `libgfx_libpng` | 14 | `[no cmdline] /GS,no-debug` |
| `libgfx_zlib` | 8 | `[no cmdline] /GS,no-debug` |
| `vostok_rtp-static-gold` | 16 | `[no cmdline] LTCG` |

Most of the target-only/base-only split is **grouping/naming**, not a flag
difference: the game ships JPEG/PNG/zlib as standalone `libjpeg`/`libpng`
libs, while our build archives them into Scaleform's `libgfx_*` libs. Their
underlying flags (`/GS,no-debug`) are identical.

### 1d. Source-count drift (matching progress, not flags)

Many `vostok_*` libs carry the same flags on both sides but far fewer sources
on base — these are simply the modules still being matched, not flag bugs:

```
vostok_game            target 105  / base 46
vostok_game_core       target 128  / base 62
vostok_render_engine   target 211  / base 147
vostok_animation       target  67  / base 41
vostok_network         target  16  / base  6
vostok_network_core    target   9  / base  3
```

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

`bullet`, `ogg`, `opcode`, `vorbis`, `scaleform`, `libfoundation` ship as
`[no cmdline] LTCG`; level unknown (no matching `.vcproj` in-tree).

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
