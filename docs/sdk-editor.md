# The SDK editor and the non-matching configurations

The matching campaign builds one configuration, `Master Gold|Win32` - the
retail image the ledger is measured against. The Survarium SDK editor is a
separate program (`vostok_editor.dll` plus its plug-ins), a C++/CLI + WPF
application that links the same engine but is compiled with ordinary,
non-retail settings. Its recovery lives on the `sdk/v0100b-editor` branch; this
page records what of that infrastructure is on the mainline and what still
blocks a full editor build here.

## What is integrated

The solution carries three first-class configurations. `Master Gold|Win32` is
the matching build and is untouched; `Release|Win32` and `Debug|Win32` are the
same engine sources with ordinary VC9 optimization settings, for work that is
not binary matching (the editor, tools, debugging an ordinary build).

```sh
python3 -m vostok build                                  # Master Gold (matching)
python3 -m vostok.build.ninja --configuration release    # game exe, Release|Win32
python3 -m vostok.build.ninja --configuration debug      # game exe, Debug|Win32
python3 -m vostok.build.ninja --configuration release logging   # one project
python3 -m vostok.build.ninja_regen --configuration release      # just the graph
```

Each non-gold configuration generates its own ninja graph on first use into
`binaries/ninja-{release,debug}` and links its own image
(`survarium-dx11-win32-dynamic[-debug].exe`) beside the gold one. Nothing that
reproduces the retail image - the two shipped `__DATE__`s, the C:\survarium
source root, the LTCG library order, the clean-PDB and post-link steps - is
shared with them, so a Release or Debug build cannot move what `vostok build`
measures. See the "Other configurations" section of the README.

### No whole-program optimization

`Release|Win32` sets `WholeProgramOptimization`, i.e. `/GL` + `/LTCG`, on every
project (`Debug|Win32` is already WPO-off). To get a Release build without the
whole-program pass - faster to link, easier to step through, no cross-TU
inlining:

```sh
python3 -m vostok.build.ninja --configuration release --no-lto
```

`--no-lto` strips `/GL` and `/LTCG` from the generated response files into a
separate `binaries/ninja-release-nolto` graph, so the two variants never
thrash each other. It is refused for gold, which is always LTCG because retail
is.

## What still blocks the editor

The editor itself (`vostok_editor.dll` and its managed plug-ins) does **not**
build on the mainline yet. Two walls, both independent of the configuration
work above:

1. **The toolchain has no .NET payload.** The editor's managed and C++/CLI
   assemblies need the CLR v2 authoring stack (`alink.dll`, `mscoree`, the
   WPF/.NET 3.0/3.5 reference assemblies). The pinned `vostok-toolchain` ships
   only `msvc/`, `winsdk/`, `dxsdk/` - no `netfx/` - so nothing can compile or
   link a `/clr` or WPF target here. Providing it means repackaging a VS2008
   install that includes the .NET SDK into the toolchain tarball (the
   `sdk/v0100b-editor` branch extends `create-toolchain-release.py` and adds a
   `stage_netfx` step to the setup for exactly this); that repackaging cannot be
   done from this tree.

2. **The non-`MASTER_GOLD` engine code is incompletely reconstructed.** The
   matching campaign only ever compiles `MASTER_GOLD`, so the engine's
   `#ifndef MASTER_GOLD` and `VOSTOK_SDK_EDITOR_RUNTIME` paths - the editor and
   debug hooks - were never built and reference members and functions that were
   never filled in. A `Release|Win32` build of the game exe compiles those
   paths and stops on them: e.g. `animation_player::reset(bool)` is declared
   twice once `MASTER_GOLD` is off (C2535), and `render::debug::g_mt_allocator`,
   `res_effect::used_shaders`, `n_ary_tree_animation_node::driving_animation`,
   `material_manager::on_material_source_changed` and a dozen more are
   referenced but absent. The `sdk/v0100b-editor` branch supplies these as
   additions inside the existing `#ifndef MASTER_GOLD` guards - matching-neutral
   by construction, since the gold build never sees them - together with the
   editor sources under `sources/vostok/editor/**`, the engine adapters
   (`editor_renderer`, the DX11 editor host, the bi-spline animation and
   terrain-command families), and the managed build orchestration
   (`scripts/vostok/sdk_build.py`, `sdk_runtime.py`). Porting that engine-side
   surface is the remaining work; it is large and lives one commit at a time
   behind the walls above.

Until both are addressed, the mainline delivers the buildable Release/Debug
configurations and the tooling to drive them; the editor build is recovered
on `sdk/v0100b-editor`.
