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
  configs incl. `Master Gold|Win32`); `pch.cpp` is the only non-excluded TU,
  every other `.cpp` and header is `ExcludedFromBuild` for all configs (carcass
  policy: TU enablement is a later leaf-first pass).
- The module is reached by the build because `game_core` declares a
  `ProjectDependencies` edge on it in `sources/vostok v2.0.sln` (mirroring its
  existing edge on `network_core`). Registering the project alone is NOT enough:
  vcproj2ninja only emits a `.ninja` for projects in the exe's dependency cone,
  so without that edge `rebuild.py scaleform` reports `unknown target`.
- `scaleform_memory.h` reuses game's `survarium::g_allocator` (same namespace,
  storage in `game_entry_point.cpp`) - the PDB shows no module-own allocator;
  Scaleform heap traffic flows through the malloc/free hooks handed to
  `scaleform_engine::initialize` (`xrSysAllocMalloc`).

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
  vendored SDK - no SDK acquisition needed. They stay `ExcludedFromBuild` for
  now only because no enabled TU reaches them yet; when matching reaches them,
  include the vendored `Src/Render/D3D1x` headers the way the render module
  does and reconcile the file split.
- All function bodies are `// FUNCTION BODY[0xVA]` stubs (no functional
  reconstruction) - matchers fill them once the TUs are enabled.
