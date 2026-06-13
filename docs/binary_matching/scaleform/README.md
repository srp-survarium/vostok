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

## What is stubbed / deferred

- **The `d3d1x_*` render HAL is survarium's IN-TREE COPY of the vendored
  Scaleform 4.0.15 D3D11 HAL - NOT a different SDK version** (an earlier note
  here called it "a newer SDK"; that was an over-conclusion, corrected
  2026-06-13). Evidence: the vendored SDK at `sources/scaleform/` is 4.0.15
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
