<!-- SPDX-License-Identifier: GPL-3.0-or-later -->
# Design versus programmer branch: SDK differences

## Why retain both references?

The design snapshot preserves an earlier, internally connected authoring
implementation. The programmer snapshot changes engine interfaces, expands
runtime systems, and develops some editor tools further, while disabling
other editor paths. The useful distinction is therefore **authoring behavior
and integration state**, not "editor source versus runtime-only source".
Both contain substantial editor and engine source.

The names `dsgn` and `prog` are consistent with separate designer-facing and
programmer-facing integration tracks. That description is an inference from
their names and code, not a recovered statement of GSC's original branching
policy. The supplied snapshot has no original upstream branch history with
which to prove the exact organizational reason or fork date.

Our repository's `xray-2.0-prog` history also contains a large source import on
top of earlier design-derived commits, followed by local Master Gold build
fixes. It is not an untouched upstream Git branch. This matters when interpreting
files that remain in its tree but are no longer active implementations.

The practical reason to keep `dsgn` is already concrete: its composite-visual
authoring implementation is active, whereas `prog` comments out that code and
its entry route. Its level editor also creates and ticks the navigation world
where `prog` comments those calls out. Those are SDK recovery leads, not just
different filenames.

## Scope of this comparison

This compares the `xr2_patch_dev_01/.../xr2-dsgn` source snapshot with the local
`xray-2.0-prog` branch inspected on 8 September 2026. It does **not** compare
against the reconstructed Survarium `v0.10b` branch or the later patches in
`xray-builds`. Console and packaging cleanup in this reference branch is
separate from the functional differences below.

Source contents and relevant call sites were inspected. Neither historical
SDK was built or run for this comparison. "Active" below means implemented
and connected in source, not demonstrated working in a running editor.
"Disabled" is scoped to the concrete implementation/call path identified;
it does not prove that no alternative implementation exists anywhere.

All paths below are relative to `sources/xray/` unless stated otherwise. The
same path on `xray-2.0-prog` is the comparison point, except where a different
file is explicitly named.

## SDK features to preserve first

### 1. Composite-visual model authoring: a real disabled feature

In [model_editor.cpp](../sources/xray/editor/model/sources/model_editor.cpp),
`model_editor::Show` handles the `composite_visual` context by constructing
`edit_object_composite_visual`. In `prog`, that branch is commented out.
The associated `edit_object_composite_visual*` implementation files are also
commented out there; their continued presence does not mean the feature works.

The design-side implementation includes:

- [Object lifetime and contents panel](../sources/xray/editor/model/sources/edit_object_composite_visual.cpp):
  contained visuals, selection bounds, resource cleanup, and panel restoration.
- [Actions](../sources/xray/editor/model/sources/edit_object_composite_visual_actions.cpp):
  adding/removing visuals, contents-panel visibility, and pivot-edit action
  registration.
- [Items](../sources/xray/editor/model/sources/edit_object_composite_visual_item.cpp)
  and [properties](../sources/xray/editor/model/sources/edit_object_composite_visual_properties.cpp):
  per-item transforms and the property-editor connection.
- [Serialization](../sources/xray/editor/model/sources/edit_object_composite_visual_serialize.cpp):
  an `items` array with `model_name`, `position`, `rotation`, `scale`, and
  `pivot`; loading/reverting; and assembling transformed collision vertices
  and rebased indices from the contained models.

The configuration is saved under
`resources/composite_models/<name>.composite_model/composite_render`, while
the collision writer uses its own source-resource paths. Those path and format
assumptions must be checked against the receiving SDK rather than copied
blindly. The source also contains unfinished/commented collision-export
alternatives; this is not a claim of a polished complete product feature.

**Recovery value:** high. Preserve the entire small authoring chain, including
actions, item/property types, serialization, resource queries, and the render
model wrapper contract. Restoring only the menu/dispatch entry is insufficient.

### 2. Editor navigation-world integration: active versus commented hooks

In [editor_world.cpp](../sources/xray/editor/world/sources/editor_world.cpp),
the design editor creates an AI navigation world after render resources are
ready, updates the graph generator's view frustum, and ticks the navigation
world. `prog` comments out those creation/update/tick calls. In
[project.cpp](../sources/xray/editor/world/sources/project.cpp), design-side
project clearing calls `get_graph_generator()->clear_geometry()`; that call
is also commented out in `prog`.

The underlying [ai_navigation](../sources/xray/ai_navigation) code is present
in both trees and is mostly shared. Thus the important difference is the
editor-to-navigation connection, not an entirely missing pathfinding library.
Keep the [world interface](../sources/xray/ai_navigation/world.h),
[navigation implementation](../sources/xray/ai_navigation/sources/navigation_world.cpp),
and graph-generator sources with the editor call sites.

**Recovery value:** high, but incomplete until tested. The design source itself
contains two navigation-world creation calls in the render-resource-ready
path. That is a reason to review ownership and lifecycle during a future port,
not to silently fix this preserved reference or call it a working navigation
SDK. Other commands, geometry feeding, cleanup, and renderer interfaces still
need end-to-end verification.

## Features changed or extended in `prog`, not simply lost

### 3. Model editing, collision, portals, and BSP tools

Design uses the older `edit_object_mesh` organization in
[edit_object_base.h](../sources/xray/editor/model/sources/edit_object_base.h).
`prog` adds `edit_object_mesh.{h,cpp}` and separate
`edit_object_solid_mesh.cpp` / `edit_object_skeletal_mesh.cpp` paths, selected
by its `model_editor::Show` implementation. Its
`render_model_wrapper.{h,cpp}` changes from the design-side single wrapper to
base/solid/skeletal wrappers, with skeletal animation and bone-matrix state.

This is particularly easy to misread in a file diff: portal property-building
methods disappear from design's
[edit_object_properties.cpp](../sources/xray/editor/model/sources/edit_object_properties.cpp),
but comparable portal controls exist in `prog`'s
`edit_object_solid_mesh.cpp`. `prog` also adds `bsp_tree_panel`,
`bsp_tree_polygon`, `indexed_edge`, `indexed_polygon`, and associated renderable
types. Do not describe portal/BSP authoring as wholly absent from `prog`.

**SDK implication:** use the design branch to recover the disabled composite
workflow, but assess the programmer branch's solid/skeletal model structure
and BSP tooling as potentially useful additions. They have different wrapper
and property-owner types; do not mix individual files without adapting those
interfaces.

### 4. Scene, renderer, viewport, and UI contracts

[render/facade/common_types.h](../sources/xray/render/facade/common_types.h)
has a small `editor_renderer_configuration` in design, with terrain and
particle-world flags. `prog` introduces `scene_configuration`, adding
SpeedTree, grass, and sky flags, plus `output_window_configuration` containing
the window handle and Flash-renderer choice.

The difference propagates through `render/engine/sources/scene_manager.*`,
scene/output-window resource creation, and the editor's preview windows.
See `editor/world/sources/editor_world.cpp`,
`editor/model/sources/model_editor.cpp`, and
`editor/animation/sources/animation_editor.cpp` for both ends of the editor
resource-query contract. `prog` also passes scene-view information at UI,
console, and render-statistics draw call sites where design uses older APIs.

**SDK implication:** this is a cross-cutting porting boundary. A design-side
tool may have the desired authoring logic but still need its scene creation,
resource user-data, viewport, and draw calls adapted to the destination engine.
The programmer renderer has additional implementation files and systems; it
is not merely a renamed design renderer.

### 5. Maya export and asset format assumptions

Both trees contain [maya](../sources/xray/maya) and
[maya_animation](../sources/xray/maya_animation). Their existence alone is
not a design-only advantage. The inspected `maya_animation` implementation
files match between these snapshots apart from project/packaging concerns.

[solid_visual_exporter.cpp](../sources/xray/maya/sources/solid_visual_exporter.cpp)
differs substantially: `prog` explicitly looks for the additional `vis1`
geometry and exports it under `render/lod1`, with refactored visual writing
and validation. Both versions write render and collision-related data.

**SDK implication:** preserve the design exporter alongside its consuming
resource code, but consider the programmer exporter for LOD support. Check
names, surface properties, collision/HQ-collision output, packed-resource
layout, and Maya API version together. An exporter compiling successfully is
not proof that its output is compatible with Survarium.

### 6. World editor and project inspection

The design tree contains the level editor, terrain tools, project/library
handling, object transforms, patrol-graph editing, logic/job editing,
lighting/material controls, and editor UI resources under
[editor/world](../sources/xray/editor/world). These are retained, not discarded
as unrelated tools. Much of the corresponding source is also present in
`prog`; this comparison does not establish those whole systems as design-only.

Beyond the navigation hooks above, `prog` adds project statistics collection:
`project::gather_statistic`, object/folder traversal, a `Statistic` action in
`level_editor_actions.cpp`, and `scene_statistic` support. Several world-object
and renderer-facing methods change with its newer model/render interfaces.

**SDK implication:** retain the design authoring baseline and compare specific
missing behaviors. Do not replace an entire world-editor subsystem merely
because a newer branch comments out one integration hook.

## Shared editor systems and supporting changes

| Area | What the comparison shows | Where to inspect / reuse concern |
|---|---|---|
| Animation editor | Most source is shared. `prog` changes scene/window setup, disables the inspected profiling calls, changes close/settings handling, and adds a model wrapper. | `editor/animation/sources/animation_editor.cpp`, `animation_collection_document.cpp`, and programmer `model_wrapper.cpp`. Preserve timeline/collection tooling, but adapt preview and lifecycle APIs. |
| Particle editor | The underlying particle module is largely shared; editor-facing changes include render-resource setup and linkage. | `editor/particle/sources/particle_editor.cpp`, `particle/`. A shared feature, not evidence of a design-only particle SDK. |
| Sound editor | Most editor code is shared; scene/debug-render call sites change. | `editor/sound/sources/sound_scene_document.cpp`, `sound/`. Keep preview/document tooling and check scene-view interfaces. |
| Dialog editor | The inspected implementation is effectively shared apart from linkage changes. | `editor/dialog/`. Retain it, but do not count its presence as a difference. |
| Managed editor base/controls | Shared foundations with changes in scene-view and tree-control implementations. | `editor/base/sources/scene_view_panel.cpp`, `editor/controls/sources/tree_node*`, `tree_view*`. C++/CLI type and control APIs are part of the integration boundary. |
| WPF controls | The inspected common C#/XAML source is unchanged; `prog` adds `property/control_container.cs` and `property_editors/item/color_range_editor.xaml.cs`. | `wpf_controls/`, plus `sources/WPFToolkit.Extended` and `sources/WeifenLuo.WinFormsUI.Docking`. These support real SDK UI and are retained. |
| AI, physics, animation runtime | Large shared source areas; animation and collision also have programmer-side edits/additions. | `ai/`, `physics/`, `animation/`, `collision/`. A functional transplant still needs its data types and resource consumers checked. |
| Core, resources, filesystem | `prog` changes parts of core/resource handling and adds filesystem/VFS implementation files. | `core/`, `fs/`, `vfs/`, `resources*.h`. Tools depend on mount/query/config semantics; retaining similarly named APIs does not guarantee compatible behavior. |
| Game and networking | `prog` expands networking and includes `client`, `game_server`, `lobby_server`, and `login_server` trees absent from the selected design snapshot. | Those module roots and `network/`, `game/`. This is runtime growth, not SDK functionality to import into design by default. |
| Dependencies | The version lists identify Boost 1.40 versus 1.48 and STLport 5.1.5 versus 5.2.1. Programmer sources additionally include MySQL/OpenSSL/Scaleform-related trees and newer Maya SDK header trees. | `sources/versions.txt` and the corresponding dependency directories. Preserve the SDK tool's actual dependencies; do not silently substitute library versions. |

## What to use this branch for

For future SDK restoration, start with two concrete candidates: composite
model authoring and navigation-editor integration. Use the design source to
recover their complete behavioral chains, then adapt them to the chosen
engine rather than resurrecting all of the old runtime.

For each candidate, verify four boundaries before calling it recovered:

1. The editor command actually creates/reaches the tool.
2. Its controls, model wrappers, render scene, and resource callbacks agree
   with the receiving engine.
3. Saved assets can be read back and consumed by that engine.
4. Opening, editing, saving, closing, and reopening the tool works with real
   assets, including cleanup and failure paths.

Do not discard design-side code just because the same filename appears on
`prog`; commented implementations and inactive dispatch paths are the main
counterexample here. Equally, do not mistake a method moved into a new class
for a lost feature. This reference deliberately preserves the relevant SDK
source while leaving functional restoration to separate, testable work.
