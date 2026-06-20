# scaleform - matching notes

The `vostok/scaleform` module: survarium's thin glue over the Scaleform GFx 4.x
SDK (the `survarium::flash_*` / `scaleform_*` wrappers the UI uses) plus a fork
of the Scaleform D3D11 render HAL (`d3d1x_*`). Scaffolded in game **batch 12**
(see [game/rebuild_batches.md](../game/rebuild_batches.md)).

- Namespace: mostly `survarium::` (the flash wrappers - same shared namespace as
  `game` / `game_core`), with the render-HAL fork in the SDK's own
  `Scaleform::Render::D3D1x::` namespace.
- Build: `/Od` + LTCG, `Master Gold` - see [../MATCHING.md](../MATCHING.md).
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
`temp/triage_log.md` "Pass 5". So unlike every other carcass, there is no
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
  so without that edge `rebuild.py scaleform` reports `unknown target`.
- `scaleform_memory.h` reuses game's `survarium::g_allocator` (same namespace,
  storage in `game_entry_point.cpp`) - the PDB shows no module-own allocator;
  Scaleform heap traffic flows through the malloc/free hooks handed to
  `scaleform_engine::initialize` (`xrSysAllocMalloc`).

## TU enablement status (2026-06-13)

`nix develop --command python3 scripts/rebuild.py scaleform` -> **Build OK**;
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

- `d3d1x_meshcache.cpp`, `d3d1x_texture.cpp` - the fabricated
  `d3d1x_meshcache.h`/`d3d1x_texture.h`/`d3d1x_sync.h` carcass headers diverge
  from the vendored 4.0.15 `Render/*` templates: a wave of C3668 (override
  drift), C2512 (no default ctor on `MeshBuffer`/`MeshCache`/`AllocAddr`
  template bases), C2146/C2253 (missing `GetBufferType`/`BufferType`),
  C2118 negative subscript (STATIC_SIZE_ASSERT against a wrong fabricated
  layout), and `SU_TotalSize`/`SU_Count` not-a-member in `Render_Shader.h`.
- `d3d1x_hal.cpp` - **no canonical carcass** (parser skips `D3D1x_HAL.obj` with
  a PDB `UnexpectedEof`); the fabricated `d3d1x_hal.h` `#include`s
  `Render/Render_ShaderHAL.h` that the vendored SDK doesn't ship (C1083), and
  depends on the meshcache/shader templates above.
- `d3d1x_shader.cpp` - **no canonical carcass** (parser skips
  `D3D1x_Shader.obj`); same `Render_Shader.h` `SU_*` / `StaticShaderManager`
  template drift as meshcache.

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
`/home/sheep/Projects/scaleform_sdk`, `Include/GFxVersion.h` = `"4.2.22"`). This
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
- The vendored tree carries **survarium patches** that a blind file-replace
  reverts; these were re-applied onto 4.2.22:
  (a) `Src/Kernel/HeapMH/HeapMH_SysAllocMalloc.h` Win32 `Alloc`/`Free`/`Realloc`
  route through `vostok::memory::g_mt_allocator` (the engine `#define`s
  `_aligned_malloc` to a `void` unreachable-code trap);
  (b) `Src/Render/D3D1x/D3D1x_Config.h` hardcodes `#define SF_D3D_VERSION 11`
  (4.2 made it a required build-config define; the build config doesn't set it);
  (c) every D3D1x `.cpp` the **render engine** compiles gets `#include "pch.h"`
  first (`render_engine_pc_dx11.vcproj` forces `/Yu"pch.h"`).
- Build wiring: `render_engine_pc_dx11.vcproj`'s `hal` filter was retargeted to
  the 4.2.22 D3D1x file set (HALSetup/ShaderManager/Shaders dropped;
  ShaderBinary/ShaderDescs/Events/Sync added). `flash_renderer.cpp`'s
  `InitHAL(HALInitParams(dev, ctx, HALConfig_DynamicShaderCompile))` was changed
  to pass `0` - 4.2 dropped that config flag (`HALConfigFlags` is now empty;
  shaders are precompiled descs).

### Result: compiles 100%, link blocked on the prebuilt 4.0.15 libgfx.lib

After the swap **every TU compiles** (0 C-errors across the full
`survarium.exe`: the whole 4.2.22 SDK, the render engine's D3D1x HAL, the
flash-glue, all engine code). The EXE link fails with **44 `LNK2001`
unresolved-external** symbols - all base render classes the render engine's
4.2.22 D3D1x HAL derives from (`Render::HAL::{initHAL,applyBlendMode,BeginScene,
EndScene,FinishFrame,...}`, `Render::MatrixState::*` (ctor now takes `HAL*`,
several methods virtualized `QAE`->`UAE`), `Render::TextureManager::*`,
`Render::Texture::*`, `MappedTextureBase::Unmap`, `RenderSync`,
`RBGenericImpl::RenderBufferManager`, `SysAlloc::shutdownHeapEngine`).

Root cause: the exe links a **prebuilt 4.0.15 `libgfx.lib`** (208 MB, untracked,
`sources/scaleform/Lib/Win32/Msvc90/Release/`, pulled by
`#pragma comment(lib,"libgfx.lib")` in `engine_scaleform_initialize.cpp`). It
DOES define those base classes - but at **4.0.15 ABI** (e.g.
`?initHAL@HAL@...@@IAE_N...` vs 4.2.22 `MAE`; `MatrixState` ctor `@@QAE@XZ` vs
`@@QAE@PAVHAL@...`; `shutdownHeapEngine` on `SysAllocBase`/`SysAllocPaged`
returning `void` vs `SysAlloc` returning `bool`). The base-render ABI changed
between versions, so the 4.0.15 lib cannot satisfy 4.2.22 D3D1x. **Reaching a
GREEN link needs a 4.2.22 `libgfx.lib`** - the 4.2.22 SDK ships NO prebuilt
`libgfx*.lib` (only its own Projects to build them), and compiling the base
`Render_*.cpp`/`SF_System.cpp` from source into the render engine instead is a
deep, duplicate-symbol-prone reimplementation of that lib = real step-4+ work,
deliberately NOT attempted here (assess-first). The header swap + build wiring
is committed; the prebuilt-lib replacement is the open next step.

### PDB source-checksum confirms 4.2.22 == shipped 4.2.21 (no drift)

The game PDB records the GFx source under
`C:\w\42216f4658640829\Scaleform\Releases\GFx_4.2.21\Src\...` - the shipped exe
was built from GFx **4.2.21**. `pdb_diff --source-dir` (per-compiland MD5) of our
vendored 4.2.22 tree against the game PDB:

    pdb_diff --target-pdb $SURVARIUM_BIN/survarium.pdb \
      --target-engine-path 'C:\w\42216f4658640829\Scaleform\Releases\GFx_4.2.21' \
      --source-dir sources/scaleform
    # matched=7  diff=0  base-only=0  target-only=197

**`diff=0`**: every SDK file we vendor that the game compiled is byte-identical
between 4.2.22 and the shipped 4.2.21 (the 7 explicit MATCHes are the
`Render/ImageFiles/*.cpp`; the rest of our subset is headers, which carry no
PDB checksum). So 4.2.22 is the correct source - no hidden point-release drift.

**`target-only=197`** (88 gfx, 69 render, 40 kernel `.cpp`): the full SDK
compiland set that the shipped binary built into `libgfx.lib` but our curated
subset does not carry as compiled TUs - including exactly the base-render TUs the
4.2.22 D3D1x HAL link needs (`render_hal.cpp`, `render_image.cpp`,
`render_cxform.cpp`, `render_sync.cpp`, `render_buffergeneric.cpp`,
`render_meshcache.cpp`). This 197-file set is the concrete inventory for the
step-4+ "rebuild libgfx.lib at 4.2.22" decision: re-render the prebuilt lib from
these (the SDK ships only its own Projects, no prebuilt `libgfx*.lib`), or pull
the needed subset into the build as compiled TUs.

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
