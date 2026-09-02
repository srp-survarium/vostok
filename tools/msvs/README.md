# tools/msvs - the studio's Visual Studio 2008 tooling

Original GSC / Vostok Games material, kept verbatim as a source of clues about
how the engine was meant to be built and debugged. None of it is used by the
Nix/Linux build in this repository (Linux is the only supported host); it is
documentation of the studio's conventions, not tooling we run.

Everything here still says "X-Ray": the wizards predate the X-Ray 2.0 -> Vostok
rename, and the `xray::` namespace in the visualizers is the one the engine
later spelled `vostok::`.

## What is here

| Path | What it is | What it tells us |
| :-- | :-- | :-- |
| `Wizards/xray wizards.sln` + `Wizards/<kind>/` | Five VS2008 "new project" wizards: **X-Ray Application PC**, **Library**, **Utility**, **Maya Plugin**, **Application Xbox 360**. Each has a `.vsz`/`.vsdir` registration, a `Scripts/1033/default.js` that wires the new project into the solution (dependencies on `core`, `engine`, ...), and `Templates/1033/` with the files a fresh project starts from. | The canonical skeleton of every engine module: `sources/pch.{h,cpp}`, `sources/library_linkage.cpp`, `sources/entry_point.cpp`, `sources/memory.h`, plus per kind `api.h`/`engine.h`/`world.{h,cpp}` for a library, `application.{h,cpp}`/`math_error.cpp`/`finger_print.cpp`/`game_module_proxy.{h,cpp}`/`.rc` for the PC application (with and without splash). When a reconstructed unit lacks one of these, the template shows what the studio's own generator would have emitted, down to the banner and include order. |
| `autoexp.dat` | The VS2008 debugger's `[AutoExpand]` and `[Visualizer]` rules (3,089 lines) for `xray::` types: `shared_string`, the `math::` vectors and colours, `vectora`/`fixed_vector`/`buffer_vector`, the `map`/`set` families across the `ai`, `editor`, `dialog_editor`, `model_editor` namespaces, STLport containers. | Member names and layouts the studio considered canonical for each container, as the debugger read them - an independent check on the reconstructed headers. |
| `step_over.reg` | Debugger `NoStepInto` rules (UTF-16 registry file): `xray::raw_ptr`, `non_null_ptr`, `loose_ptr`, `intrusive_ptr`, `operator new`, the CRT security cookie and RTC checks. | Which wrappers the studio treated as transparent; confirms the smart-pointer family that existed at the time. |
| `HLSL Compiler.rules` (VS2008) and `HLSL Compiler.{props,targets,xml}` (VS2010 MSBuild port) | A custom build rule for `*.vs` / `*.ps`: runs `resources/sources/shaders/shader_compiler_vs.exe` on each shader source and emits `$(IntDir)\<name>.<ext>.obj`. | The shipped shader build step as the studio ran it inside the IDE; see [docs/shaders.md](../../docs/shaders.md) for the compiler, the blobs and the roundtrip proof. |
| `install_tools.cmd` | Registers `VsWizard.dll`, copies the wizards into the VS2008 install, installs `autoexp.dat` and `step_over.reg`. | How a studio workstation was provisioned. |

## Open questions worth a look

- The library wizard's `world.{h,cpp}` and `engine.h` templates spell the
  module entry protocol (`create_world`/`destroy_world`, allocator
  registration) that `game_module_proxy` and every `*_entry_point.cpp` follow;
  a diff against the reconstructed modules may explain remaining ordering
  differences in those units.
- `autoexp.dat` names members of editor-namespace containers that no shipped
  binary carries; it is the only record of them.
