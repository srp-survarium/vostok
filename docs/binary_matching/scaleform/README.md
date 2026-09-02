# scaleform - matching notes

The `vostok/scaleform` module: survarium's thin glue over the Scaleform GFx 4.x
SDK (the `survarium::flash_*` / `scaleform_*` wrappers the UI uses) plus a fork
of the Scaleform D3D11 render HAL (`d3d1x_*`). Scaffolded in game **batch 12**
(see [game/rebuild_batches.md](../game/rebuild_batches.md)).

- Namespace: mostly `survarium::` (the flash wrappers - same shared namespace as
  `game` / `game_core`), with the render-HAL fork in the SDK's own
  `Scaleform::Render::D3D1x::` namespace.
- Build: `/Ox` (`Optimization=3`) + `SF_BUILD_SHIPPING` + LTCG, `Master Gold` -
  matches render_engine's settings (the shipped scaleform objects, incl. the
  D3D1x SDK files, were built optimized; the shipped `BeginScene` etc. are clearly
  `/Ox`). The earlier `/Od` config was a fabrication that left every scaleform TU
  Od-vs-Ox mismatched (all unpaired/low-%); flipping to `/Ox`+`SF_BUILD_SHIPPING`
  lifted the whole module. See [../MATCHING.md](../MATCHING.md).
- Sources: `sources/vostok/scaleform/` (infra at module root: `api.h`,
  `library_linkage.h`; everything else under `sources/`).
- Include dirs add the Scaleform SDK (`$(SolutionDir)/scaleform/Include`,
  `$(SolutionDir)/scaleform/Src`), cloned from `render_engine_pc_dx11.vcproj`
  (the other consumer of GFx headers). Vendored SDK lives at
  `sources/scaleform/`.

## The structure-generator gap (symbol-list provenance)

This is the ONLY engine module the structure generator (`pdb_parser`) never
emitted a carcass for - a parser gap (see
[../unanswered_questions.md](../unanswered_questions.md)), recorded in
the triage log (`git show 885e1d4a4:temp/triage_log.md`, "Pass 5"). So unlike every other carcass, there is no
canonical `binaries/structure/target/sources/vostok/scaleform/` tree to
reassemble from.

The ground truth instead is `binaries/rich/target/index.jsonl` (the addressed
symbol set). The batch-12 work-items were fabricated from it - one `*.md` per
compiland/header listing every addressed VA + demangled signature - and parked
in `temp/structure_queue/sources_scaleform/` (deleted as each was reproduced;
the dir is now empty = module complete). When a stub signature was ambiguous,
`pdb_fetch --view carcass/structure --function <fully::qualified::name>`
recovered it.

~275 addressed functions across 10 compilands; the flash type bodies that game
left as `{ /* no source */ }` inline stubs are OUT-OF-LINE here
(`flash_movie`/`flash_value` in `movie.cpp`/`value.cpp`, etc.).

## Build wiring

- `scaleform.vcproj` clones `network_core`'s config matrix (all 10
  configs incl. `Master Gold|Win32`). As of the 2026-06-13 enablement pass the
  flash-glue TUs are compiled for `Master Gold|Win32` (see
  [TU enablement status](#tu-enablement-status-2026-06-13)); the four `d3d1x_*`
  HAL TUs stay `ExcludedFromBuild` (SDK-template reconciliation, deferred to the
  matcher loop). All other configs keep everything excluded (carcass policy).

## The GFx include-environment fix (2026-06-13)

Enabling the flash TUs first hit a Windows-SDK header cascade: GFx headers
(`GFx_Player.h`) transitively pull the real `<windows.h>`, but the scaleform
`pch.h` went straight to `<vostok/extensions.h>`, which (via `os_extensions.h`)
leaves `HANDLE`/`HWND`/`APIENTRY`/`INVALID_HANDLE_VALUE`/`BOOL`/`pvoid` defined
as **fake macros**. Those macros corrupt `windef.h` / `winnt.h` / `rpcasync.h`
when the real headers parse them (the visible symptom was
`rpcasync.h(109): syntax error : missing ';' before '*'` plus `windef.h(208)
HWND__` and `type_extensions.h pvoid` redefinitions).

The render engine - the other in-tree GFx consumer
(`render/engine/sources/flash_renderer.cpp`) - avoids this because its `pch.h`
runs the `os_preinclude.h` -> `os_include.h` ritual (via `render_include.h`),
which `#undef`s the fake macros and includes `<windows.h>` cleanly *first* (so
GFx's later `<windows.h>` is a guarded no-op). `scaleform/sources/pch.h` now
mirrors that: it adds `<vostok/macro_platform.h>`, then after `extensions.h`
runs `os_preinclude.h` (+ the render's `#undef NOUSER/NOMSG/NOGDI/NOTEXTMETRIC`)
and `os_include.h` before `scaleform_memory.h`. That single pch change cleared
the entire ~160-error rpcasync/windef cascade. The `.vcproj` include dirs and
defines were already identical to render engine's (no change needed there).
- The module is reached by the build because `game_core` declares a
  `ProjectDependencies` edge on it in `sources/vostok v2.0.sln` (mirroring its
  existing edge on `network_core`). Registering the project alone is NOT enough:
  vcproj2ninja only emits a `.ninja` for projects in the exe's dependency cone,
  so without that edge `vostok build scaleform` reports `unknown target`.
- `scaleform_memory.h` reuses game's `survarium::g_allocator` (same namespace,
  storage in `game_entry_point.cpp`) - the PDB shows no module-own allocator;
  Scaleform heap traffic flows through the malloc/free hooks handed to
  `scaleform_engine::initialize` (`xrSysAllocMalloc`).

## TU enablement status (2026-06-13)

`nix develop --command python3 -m vostok build scaleform` -> **Build OK**;
`vostok_scaleform-static-gold.lib` links and the full `survarium.exe` links green
(`report-changes.json`: 0 regressed / 0 improved / 0 removed / 0 added - no
render unit moved).

Enabled and compiling (`Master Gold|Win32`, 7 objs incl. `pch`):

- `command_queue.cpp`, `factory.cpp`, `movie.cpp`, `renderer.cpp`, `value.cpp`
  - the `survarium::flash_*` glue. Compiled cleanly once the GFx env was fixed.
- `d3d1x_shaderdescs.cpp` - the generated shader-descriptions TU; compiles
  against the vendored SDK with no header reconciliation needed.

Re-excluded (kept `ExcludedFromBuild="true"` for `Master Gold|Win32`) -
SDK-template reconciliation is real matcher-loop work, not cheap buildability
stubbing, so deferred to keep the module green:

- `d3d1x_meshcache.cpp` - the fabricated `d3d1x_meshcache.h`/`d3d1x_sync.h`
  carcass headers diverge from the vendored 4.0.15 `Render/*` templates: a wave
  of C3668 (override drift), C2512 (no default ctor on
  `MeshBuffer`/`MeshCache`/`AllocAddr` template bases), C2146/C2253 (missing
  `GetBufferType`/`BufferType`), C2118 negative subscript (STATIC_SIZE_ASSERT
  against a wrong fabricated layout), and `SU_TotalSize`/`SU_Count` not-a-member
  in `Render_Shader.h`.
- `d3d1x_hal.cpp` - **no canonical carcass** (parser skips `D3D1x_HAL.obj` with
  a PDB `UnexpectedEof`); the fabricated `d3d1x_hal.h` `#include`s
  `Render/Render_ShaderHAL.h` that the vendored SDK doesn't ship (C1083), and
  depends on the meshcache/shader templates above.
- `d3d1x_shader.cpp` - **no canonical carcass** (parser skips
  `D3D1x_Shader.obj`); same `Render_Shader.h` `SU_*` / `StaticShaderManager`
  template drift as meshcache.

## d3d1x_texture SDK-seed pilot (2026-06-21)

`d3d1x_texture.cpp` is now **built vostok-local + measurable + matched** (26/30
out-of-line fns at 100%, the rest 75-99%). The SDK-seed recipe (sushi pilot):

1. The vostok `d3d1x_texture.cpp` is the vendored SDK `D3D1x_Texture.cpp` body
   verbatim (line numbers preserved, so PDB line attribution matches the
   target), including the **real SDK header** `Render/D3D1x/D3D1x_Texture.h`
   instead of the fabricated `d3d1x_texture.h` carcass. The whole scaleform
   include env (`scaleform/Src` + `scaleform/Include`) already resolves every SF
   `Render/*` / `Kernel/*` include - render_engine compiles the same SDK file.
2. Unexclude it for `Master Gold|Win32` in `scaleform.vcproj`.
3. Exclude the SDK `D3D1x_Texture.cpp` from `render_engine_pc_dx11.vcproj`
   (Master Gold only) so the **vostok object wins ICF** and the base PDB records
   the `vostok/scaleform/sources/d3d1x_texture.cpp` path (the dummy-paired unit
   becomes really paired).
4. **The optimization fix is load-bearing**: a per-file `Optimization=3` override
   is *ignored* (C4653: inconsistent with the Od precompiled header), so the
   whole scaleform config must be `/Ox` + `SF_BUILD_SHIPPING` (= render_engine's
   settings). Built at Od, the vostok object is *lower quality* than the SDK Ox
   object it displaces under ICF -> the shared folded SDK COMDATs lose their Ox
   owner and the headline regresses (-15 exact). At Ox the vostok object is
   byte-identical to the SDK object, so the ICF fold is harmless and the whole
   scaleform module lifts (+47 exact; overall +58).

## d3d1x rest SDK-seed (2026-06-21)

The remaining four `d3d1x_*` TUs now follow the same recipe and are **built
vostok-local + measurable**. Their SDK twins already `#include "pch.h"` and use
the full `Render/D3D1x/*` include paths, so the seed is a verbatim copy of the
SDK `.cpp` (CR + trailing-whitespace stripped; line numbers preserved); no
include rewrite was needed (unlike texture's bare `"D3D1x_Texture.h"`). Each is
unexcluded for `Master Gold|Win32` in `scaleform.vcproj` and its SDK twin is
excluded from `render_engine_pc_dx11.vcproj` (Master Gold). exe links green.

- `d3d1x_meshcache.cpp` - 19/22 out-of-line fns at 100% (the 3 misses 88-97%).
- `d3d1x_shader.cpp` - 8/17 at 100%; rest 79-99% (codegen residual).
- `d3d1x_hal.cpp` - the big one (45 fns); 3 byte-exact but ~35 sit at 99.5-99.9%
  (instruction-scheduling/LTCG residual over a clean structure); a few real
  divergences (`CreatePrimitiveFill` 0, `DrawProcessed*` 82-86%,
  `createBlendStates` 90%) where vostok routed DX access through its own renderer.
- `d3d1x_shaderdescs.cpp` - built + measurable but **fundamentally divergent**:
  the shipped `GetShaderIndex` is a 1237-stmt / 0x3d74-byte fully-inlined giant
  switch over a *different generated shader-descriptor table* than the SDK twin
  (which compiles to a 1-stmt stub). `IsShaderVersionSupported` is a STRUCTURE
  MATCH but bytes diverge. Left built (measurable beats an unmeasurable carcass);
  matching its generated table is out of scope for the seed pass.

Net (DB byte-exact): scaleform exact 103 -> 180 (+77); overall 4,356 -> 4,445
(+89). The 77 report-changes "regressions" are ICF fold-rep churn (`scalar
deleting destructor` / vcall-thunk closures across unrelated modules); best-ever
(max) columns held, no true byte-exact loss (196 improved vs 77 churn).

The `d3d1x_texture.h` carcass header stays (only `d3d1x_hal.h`, still excluded,
includes it); its inline-fn `.h` unit is still dummy-paired (the inlines now emit
from the SDK header path) - a separate follow-up.

Canonical-carcass reconciliation: 8 of the 10 compilands now have real canonical
carcasses under `binaries/structure/target/sources/vostok/scaleform/sources/`
(command_queue, factory, movie, renderer, value, d3d1x_meshcache,
d3d1x_shaderdescs, d3d1x_texture). The enabled flash-glue `.cpp`s already align
with them on the addressed-VA set (verbatim `// FUNCTION BODY[0xVA]` carcasses);
only minor body-set drift remains (e.g. `factory.cpp` differs on `0x7d9330` /
`0x7f0a90` / `0xae2f0` vs a stray `0xae240`) - left for the matcher loop, which
fills the bodies anyway. The canonical dumps themselves are NOT drop-in
compilable (mangled-name include guards, `void (*)(...)` return-type syntax,
no includes), so they stay reference-only.

## SDK swapped 4.0.15 -> 4.2.22 (2026-06-20)

The binary links GFx **4.2.21** (the exe embeds "4.2.21"; `Render::HAL` is
`STATIC_SIZE_ASSERT 0x1D0`), so the vendored tree was upgraded 4.0.15 ->
**4.2.22** (one build off 4.2.21, the closest checkout available;
`<scaleform_sdk checkout>`, `Include/GFxVersion.h` = `"4.2.22"`). This
**reverses the 2026-06-13 "not a newer SDK" conclusion below** - the divergence
is real and lives in the SHARED base render classes (`Render::HAL`,
`Render::Texture`/`TextureManager`/`MappedTextureBase`, `Render::MatrixState`,
the new `Render/Render_ShaderHAL.h`), not just `D3D1x/`.

The swap (379 files, +425k/-283k lines):

- Replaced the 340 vendored files that exist at the same relpath in 4.2.22;
  removed 21 obsolete 4.0.15-only files (the D3D9/GL/D3D1x shader files the 4.2
  unified-shader framework reorganized, plus `GFx_FontProvider_NGP.h`); added 18
  new-in-4.2 files the include closure needs (`Render_ShaderHAL.h`,
  `Render_TextureCache*.h`, `Render_DrawableImage.h`, `Render_Events.h`, the new
  `D3D1x_Shader{Binary,Descs}.cpp` + `D3D1x_Events.h`/`D3D1x_Sync.h`/
  `D3D1x_ShaderDescs.h`, and a handful of platform headers). Scope mirrors the
  curated vendored subset (NOT the whole multi-thousand-file SDK); driven by the
  transitive `#include` closure from the engine's GFx entry headers.
- The vendored tree (`sources/scaleform/`, consumed by the render engine's
  in-tree D3D1x HAL) carries render-side reconciliation patches:
  `Src/Render/D3D1x/D3D1x_Config.h` hardcodes `#define SF_D3D_VERSION 11` (4.2
  made it a required build-config define; the build config doesn't set it), and
  every D3D1x `.cpp` the render engine compiles gets `#include "pch.h"` first
  (`render_engine_pc_dx11.vcproj` forces `/Yu"pch.h"`). **NOTE:** the from-source
  GFx-lib build (see the recipe section) is PRISTINE and standalone - it does NOT
  apply these, nor any HeapMH `g_mt_allocator` allocator patch (the earlier
  recipe's misread - the engine feeds GFx an allocator at runtime via
  `Scaleform::SysAlloc`), nor the engine `pch.h`. The render-side `pch`/config
  patches stay because the render engine compiles those D3D1x TUs as engine
  compilands; the GFx libs are a separate, unpatched prebuilt.
- Build wiring: `render_engine_pc_dx11.vcproj`'s `hal` filter was retargeted to
  the 4.2.22 D3D1x file set (HALSetup/ShaderManager/Shaders dropped;
  ShaderBinary/ShaderDescs/Events/Sync added). `flash_renderer.cpp`'s
  `InitHAL(HALInitParams(dev, ctx, HALConfig_DynamicShaderCompile))` was changed
  to pass `0` - 4.2 dropped that config flag (`HALConfigFlags` is now empty;
  shaders are precompiled descs).

### The GFx lib suite is a PREBUILT, not part of the engine LTCG (CORRECTED 2026-06-21)

The shipped PDB's build records are authoritative for every compiland's exact
`cl` flags (`pdb_build_info --pdb $SURVARIUM_BIN/survarium.pdb --grep libgfx
--full`). They overturn the earlier GUESSED recipe (this section previously
described a `/Od` + `/GL` + `MASTER_GOLD` + HeapMH-allocator-patch + engine-`pch.h`
build). The records show the GFx libs were built:

- **NON-`/GL`, `/Ox`** - finished machine code, staged to
  `C:\survarium\binaries.prebuilt\Win32\libraries\shipping\` exactly like
  boost / openssl, **NOT in the engine whole-program LTCG**. The wrong `/GL`
  recipe pulled GFx INTO the engine LTCG and regressed ~293 engine math / physics
  / bullet / collision functions (`vostok::math::sin/cos/tan/pow`,
  `float3_pod::normalize`, `float4x4::create_*`, the bullet/collision inline
  bodies) from 100% as the cross-module inliner repartitioned. Taking GFx out of
  the LTCG fixes that regression, the 2 GB-contiguous `lib` OOM (machine-code
  archives are small, so no as3 4-way split), and the flag drift - all at once.
- **From PRISTINE 4.2.22 SDK source, standalone** - no survarium patches (no
  HeapMH `g_mt_allocator` routing: the engine feeds GFx an allocator at RUNTIME
  via `Scaleform::SysAlloc` / `GFx::System::Init`, see the `xrSysAllocMalloc` in
  `engine_scaleform_initialize.cpp`), no engine `pch.h` prepend (the fake-`HANDLE`
  macro cascade came from survarium's `extensions.h`, which a standalone GFx build
  never includes), no `MASTER_GOLD`, no `VOSTOK_STATIC_LIBRARIES`, no `/arch`,
  no `/Od`.

**The suite (from the PDB project set), staged to `shipping/`:**

| lib            | TUs | note |
| :------------- | --: | :--- |
| `libgfx`       | 177 | GFx + Kernel + Render + Text; AMP_* TUs compile empty under `SF_BUILD_SHIPPING` |
| `libgfx_as2`   |   6 | SCU: `AS2_All.cpp` + IME/XML/Support |
| `libgfx_as3`   |  25 | SCU: `AS3_All.cpp`, `AS3_Abc*`, `AS3_Obj_*_All.cpp` (the amalgamation TUs - NOT 272 per-class files) |
| `libgfxexpat`  |   1 | `XML_Expat.cpp` |

There is **NO `libgfx_air` and NO `libgfxsound_fmod`** in the shipped binary (zero
such projects/symbols). The codec C libs (`libjpeg`/`libpng`/`pcre`/`zlib`) carry
**no debug `cmd` record** in the PDB - they are not engine-matchable; they only
need to RESOLVE the `jpeg_*` / `png_*` / `inflate` symbols `libgfx` pulls. We
build `libgfx_zlib` / `libgfx_libpng` / `libgfx_libjpeg` from the SDK `3rdParty`
source as plain Release C static libs (the exe `#pragma comment(lib,...)` names
them with the `libgfx_` prefix).

**The C++ recipe (libgfx / as2 / as3, identical config):**

    -Ox -Ob2 -Ot -Zp8 -Z7 -MT -W4 -GS- -Gy -GR- -GF -WX -Zl -FD -MP
    -errorreport:none -TP  + -w44264 -w44062 -w44265 -w44287 -w44289 -w44296
    -w44431 -w44545 -w44546 -w44547 -w44548 -w44549 -w44623
    defines: WIN32 _WINDOWS SF_BUILD_STATICLIB NDEBUG SF_BUILD_SHIPPING _MBCS
             _VC80_UPGRADE=0x0710
    -I Include Src 3rdParty/{zlib-1.2.7,jpeg-8d,libpng-1.5.13,expat-2.1.0/lib,
       pcre,glext,PlatformSDK,cri/pc/include,fmod/pc/Win32/inc} + DXSDK Include

`SF_BUILD_SHIPPING` `#undef`s `SF_AMP_SERVER`, gutting the `Amp_*` TUs - the
shipped binary has ZERO AMP symbols (`Scaleform::AMP` / `AmpServer` / `Amp_*` all
absent), which is also why the render engine's `render_engine_pc_dx11.vcproj`
carries `SF_BUILD_SHIPPING` (its D3D1x HAL consumes GFx headers; the `Render::
Texture::Copy` / AMP references the foreign 4.0.15 lib used to mask are gone).

**The build (`vostok.build.gfx`):** authors a per-lib `cl` response file
straight from this recipe and compiles each TU directly under Wine
(`wine cmd /c cl @rsp`), then `lib`s the objects - **NO vcproj2ninja / sln /
ninja**. `ninja.exe` under Wine deadlocks after ~70-80 `cl` spawns, and a direct
machine-code build has no in-graph step anyway. System includes (VC / WinSDK /
DXSDK) come from Wine's `%INCLUDE%` (set in the Wine registry by
`vostok tool toolchain`), so the rsp lists only the GFx `-I` dirs. The per-TU
mspdbsrv pipe-EOF reaper (`vostok.build.gfx_mspdbsrv`, PR #280 ported to the direct
driver) is kept - `/Z7` + `/FD` + `/Fd` still touches `mspdbsrv.exe`. Build it:

    nix develop --command python3 -m vostok.build.gfx

  Source is the PRISTINE external SDK (`<scaleform_sdk checkout>`,
  4.2.22; `$SCALEFORM_SDK` overrides). No `scaleform_build/` overlay (deleted -
  it existed only to carry the now-removed survarium patches + pch prepend).

**Staging.** Output is `binaries.prebuilt/Win32/libraries/shipping/<name>.lib`,
where the exe's `#pragma comment(lib,"<name>.lib")` resolves it. The libs are
plain prebuilts: un-wired from the sln, `game_core` takes no dependency edge, and
`vostok tool libs` skips the foreign 4.0.15 distribution `libgfx*.lib` under
`scaleform/Lib/.../Release/` so a `vostok tool toolchain` pass never clobbers them.
Do NOT run `vostok tool toolchain --force libs` after building.

### PDB source-checksum confirms 4.2.22 == shipped 4.2.21 (no drift)

The game PDB records the GFx source under
`C:\w\42216f4658640829\Scaleform\Releases\GFx_4.2.21\Src\...` - the shipped
exe was built from GFx **4.2.21**. `pdb_diff --source-dir` (per-compiland MD5) of
the vendored 4.2.22 tree vs the game PDB reports `matched=7 diff=0` - every SDK
file the game compiled is byte-identical between 4.2.22 and 4.2.21, so 4.2.22 is
the correct source.

**Byte-match measurement gap:** the delinker strips a single engine-path prefix
(`c:/survarium/sources` for target), but the GFx compilands are recorded under
`C:\w\...\GFx_4.2.21\Src\...`, so they do NOT map into
`binaries/objdiff/target` and the GFx TUs do not auto-pair in `report.json` /
the ledger. The GFx libs do not move the match-score table; the point of building
them is a GREEN exe link with GFx OUT of the LTCG so the ~293 engine functions the
bad `/GL` build poisoned recover. Per-symbol GFx byte-match would need the GFx
prefix wired into the delinker (follow-up).

## What is stubbed / deferred

- **The `d3d1x_*` render HAL is survarium's IN-TREE COPY of the vendored
  Scaleform 4.0.15 D3D11 HAL - NOT a different SDK version** (an earlier note
  here called it "a newer SDK"; that was an over-conclusion, corrected
  2026-06-13 - and now **re-corrected 2026-06-20: it IS a newer SDK, 4.2.x**,
  see the swap section above). Evidence: the vendored SDK at `sources/scaleform/` is 4.0.15
  (`Include/GFxVersion.h`) and already ships the D3D1x HAL
  (`Src/Render/D3D1x/D3D1x_*.{h,cpp}`); the shipped `D3D1x::HAL` symbols match
  4.0.15 exactly - same base `Render::HAL` (`class HAL : public Render::HAL`),
  same methods (`InitHAL(HALInitParams)`, `DrawProcessedPrimitive`,
  `PushMask_BeginSubmit`, `beginMaskDisplay`, `MapVertexFormat`,
  `GetTextureManager`/`GetMeshCache`); and the engine's render module already
  compiles against these vendored headers (`render/engine/sources/flash_renderer.cpp`
  includes `Render/D3D1x/D3D1x_HAL.h`). Survarium just copied the 4.0.15 HAL
  source into its own `vostok/scaleform` module (so it compiles as a survarium
  compiland with an engine path, instead of linking a prebuilt Scaleform lib);
  the file split differs slightly (survarium's `d3d1x_hal.cpp` ~= vendored
  `D3D1x_HAL.cpp` + `D3D1x_HALSetup.cpp`; `d3d1x_shaderdescs.cpp` is the
  generated shader-descriptions TU). So these ARE matchable against the
  vendored SDK - no SDK acquisition needed. `d3d1x_shaderdescs.cpp` is enabled;
  the other four (`d3d1x_hal/_meshcache/_shader/_texture`) are re-excluded (see
  [TU enablement status](#tu-enablement-status-2026-06-13) for the per-TU
  blocker). When matching reaches them, include the vendored `Src/Render/D3D1x`
  headers the way the render module does, reconcile the fabricated carcass
  headers against the 4.0.15 templates, and reconcile the file split
  (`d3d1x_hal.cpp` ~= `D3D1x_HAL.cpp` + `D3D1x_HALSetup.cpp`).
- All function bodies are `// FUNCTION BODY[0xVA]` stubs (no functional
  reconstruction) - matchers fill them once the TUs are enabled.

### Render_Shader.h / Render_ShaderHAL.h D3D1x template wall (2026-06-21)

The `StaticShaderManager<D3D1x::...>` and `ShaderHAL<D3D1x::...>` member
instantiations are real, built, scored template members (50-100% across the two
headers). After the source SHAPE is matched, the residual on the non-100% ones
is the **survarium-modified GFx SDK**, which our vendored 4.0.15 `Render_*.h`
does not carry - NOT a source-shape gap, so park it:
- **ShaderDesc / FF_ enum VALUES diverge.** survarium's shader-permutation enum
  values are ~half the stock ones (`ST_DrawableMerge` 0x7000 vs stock 0xE000;
  the `ST_base_*` permutation bases shifted one bit), and the `FF_*` fillflags
  differ too (`FF_Blending` 0x10 vs our 0x20, `FF_Cxform` 0x04 vs 0x08,
  `FF_AlphaWrite|FF_Cxform` 0x06 vs 0x0C). Every `SetStaticShader`/`shader += ...`
  immediate is therefore off by a constant - the whole SetDrawable*/SetFill/
  SetPrimitiveFill/StaticShaderForFill byte residual. This is the
  `ST_base_CxformAc vs stock ST_base_Inv` swap: survarium dropped the Invert
  shader permutation - the stock `if (fillflags & FF_Invert) shader +=
  ST_base_Inv;` line is ABSENT from survarium's `StaticShaderForFill` (removing it
  is a faithful SHAPE fix, took it 94->98.8% STRUCTURE MATCH; the rest is the
  enum values). Same for SetPrimitiveFill's stock outer `(fillFlags &
  (FF_Blending|FF_Cxform)) != ...` guard around the cxform loop - absent in
  survarium (removed: 41->40 stmts vs 35 target).
- **Class LAYOUT / virtuality diverge.** `D3D1x::ShaderManager` is ~0xC000 bytes
  smaller in target (fewer permutation slots), shifting EVERY `ShaderHAL` member
  offset by 0xBF64 (the ctor + every Drawable* method residual). The
  render-target-stack element is 0x2F0 vs our 0x310. `MatrixState::GetUVP` is a
  VIRTUAL call in target (`call [vtbl+0x18]`) but non-virtual in our header (the
  SetMatrix residual). These are vendored-SDK type diffs - fixing them edits the
  GFx headers/enum (out of scope per the matcher brief); leave the % at the
  shape ceiling.
