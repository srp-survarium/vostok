# Game build configurations

The recovered tree builds the Survarium game in Gold, Release, and Debug. The
non-Gold commands deliberately build the game runtime only; they do not claim
that the SDK editor or content-authoring tools are recovered.

## Commands

```sh
python3 -m vostok build

python3 -m vostok.build.ninja --configuration release --linkage dll
python3 -m vostok.build.ninja --configuration release --linkage static
python3 -m vostok.build.ninja --configuration debug --linkage dll
python3 -m vostok.build.ninja --configuration debug --linkage static

python3 -m vostok.build.ninja --configuration release --linkage dll --no-lto
python3 -m vostok.build.ninja --configuration release --linkage static --no-lto
```

Debug projects do not enable whole-program optimization, so `--no-lto` does
not create a distinct Debug code-generation mode. Release uses `/GL` and
`/LTCG` by default; `--no-lto` removes both and rebuilds the selected graph.
Gold always retains LTCG because it reproduces the retail image.

DLL and static graphs are separate under `binaries/ninja-{release,debug}` and
`binaries/ninja-{release,debug}-static`. Switching LTO on or off rewrites the
selected graph in place and causes the affected outputs to rebuild. The game
products are written under `binaries/Win32`:

- `survarium-dx11-win32-dynamic[-debug].exe` and
  `vostok_engine_pc_dx11[-debug].dll` for DLL builds;
- `survarium-dx11-win32-static[-debug].exe` for static builds.

The current reconstruction is not expected to reach playable gameplay in any
configuration. The supported result here is a complete compile and link.

## Why non-Gold used to pull in the SDK

In the recovered VS2008 solution, `MASTER_GOLD` is more than an optimization
choice. It is also the historical product boundary. Turning it off enables
runtime diagnostics, editor adapters, live asset conversion, and content
validation inside the same projects and sometimes inside the same translation
units as game runtime code. The original source tree treated the engine, game,
editor, and content pipeline as one product family; it did not provide a clean
`GAME_ONLY` solution configuration.

That is why a naive Release or Debug build appeared to require a large SDK
surface. The game does not use most of it. The configuration merely made those
authoring paths visible to the compiler, where they referenced unrecovered
v0.100b types and bodies.

The game build now has three authoring boundaries:

1. animation/Maya authoring and editor-mixer validation;
2. render live editing, source texture conversion, and the editor renderer;
3. navigation graph generation and its interactive tests.

Whole authoring translation units are removed in one table,
`GAME_EXCLUDED_SOURCES` in `scripts/vostok/build/solution.py`. The build writes
temporary project/solution views, leaving the authoritative projects intact
for later SDK recovery. A small number of `VOSTOK_GAME_BUILD` checks remain in
mixed runtime/authoring translation units where excluding the entire file
would also remove game code. Bullet trajectory visualization and sound
propagator snapshots are separate non-Gold diagnostics, not SDK dependencies.

`VOSTOK_GAME_DLL` has a different purpose: it selects DLL ABI/export behavior.
Those checks do not compile editor code out. Keeping the game/product boundary
separate from the linkage boundary is important; static and DLL games use the
same runtime surface.

The deferred authoring pieces are indexed in [todos/README.md](todos/README.md).
The full managed editor has an additional .NET/CLR toolchain dependency; see
[sdk-editor.md](sdk-editor.md).
