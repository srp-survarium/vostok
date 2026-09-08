<!-- SPDX-License-Identifier: GPL-3.0-or-later -->
# X-Ray 2.0 design-source reference

`xray-2.0-dsgn` preserves the PC portion of the original design-source snapshot
supplied in `xray-builds/source/xr2_patch_dev_01/xr2_patch_dev/xr2-dsgn/sources`.
It is an SDK/editor reference, not a Survarium reconstruction or matching branch.
The archive's later development patches are not overlaid here.

The reason to retain it is functional, not merely historical: some authoring
paths are active here but commented out in `xray-2.0-prog`. In particular, the
composite-visual model editor and the editor's navigation-world integration
deserve preservation. Conversely, `prog` contains newer model, renderer, and
exporter work. Neither branch is a wholesale replacement for the other.

See [Design versus programmer branch: SDK differences](docs/dsgn-vs-prog.md)
for the evidence, code locations, features worth recovering, shared tooling,
and compatibility boundaries.

## What is retained

The engine, Windows editor, C++/CLI and WPF controls, asset-authoring tools,
Maya exporter sources, and supporting third-party source/header trees retain
their original names and contents. Original copyright and license notices
remain in place; this repository does not relicense the imported source.

Shared implementations such as `*_win_xbox360.cpp` and the allocator's
`win_xbox360_ps3` backend are retained because they also implement Windows
behavior. Console conditionals in otherwise shared source are not rewritten.
Spatial cells, terrain cells, and portable library algorithms are not console
SDKs merely because of their names.

## PC-only cleanup

The import excludes console-only engine files and projects, PS3/Xbox library
builds, console SDK wrappers and the Cell-only libspe2 backend, prebuilt libraries/executables/debug databases,
IDE caches, backup files, non-PC project bundles, and bundled WildMagic sample
applications/data. SDK/editor tools and their UI resources are not treated as
unrelated examples.

VS project and solution metadata has console platforms/configurations and
references to excluded files removed. The console SDK entry in
`sources/versions.txt` is removed. Ordinary retained implementation and header
files are copied byte-for-byte; the only content edits are to build/project,
solution, and version-list metadata. No Vostok matching fixes are mixed in.

The original `xray-builds` archives and extracted directories are untouched.
This is a source reference, not a claim that the historical solution builds
or that every editor feature works. Prebuilt dependencies and runnable editor
assets are deliberately not shipped here. Restoring an executable SDK needs
its own build and asset-compatibility work.
