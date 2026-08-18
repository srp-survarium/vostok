# Materials, effects, and shaders

This document describes the Survarium v0.100b material pipeline and the
workflow to preserve while porting maps and assets from other games. The
material system is data-driven, but it is not a shader graph: material data
selects C++ effect descriptors, and those descriptors select precompiled HLSL
permutations and bind their resources.

## Three different material systems

| Name | Purpose | Shipped location |
| --- | --- | --- |
| Render material | Defines stages, effect families, textures, numeric values, and render switches | `material_instances/**/*.material` |
| Render base material | SDK-authored template inherited by render material instances | Authoring source was `resources/sources/materials/**/*.material`; base sources are not present in the v0.100b resource pack |
| Game material | Defines physical/gameplay interaction pairs such as impacts, decals, sounds, and physics groups | `game_materials/game.materials` and `game_materials/material.pairs` |

Game materials and render materials may refer to the same visible surface in
content, but they are separate runtime systems. A game material does not choose
an HLSL shader.

The unpacked v0.100b resource set currently contains 392 cooked render material
instances. The tracked `resources/` tree currently contains recovered shader
source, not the original material authoring trees.

## How a render material reaches a shader

```text
model surface
  -> material-instance resource path
  -> cooked material config (base values + instance overrides)
  -> one config block per enabled render stage
  -> stage effect_id
  -> registered C++ effect descriptor
  -> shader_configuration defines + shader filenames
  -> precompiled DXBC permutation
  -> textures, constants, samplers, and render state bound by the effect
  -> renderer applies that stage's technique/pass
```

A `.material` file therefore does not contain HLSL. It contains choices such
as `effect_id = g_stage_default`, `use_nmap`, `use_troughness`, `two_sided`,
texture names, colors, and scalar ranges. The selected C++ effect descriptor
turns those choices into two kinds of state:

1. Compile-time shader configuration, such as `CONFIG_TNORMAL`,
   `CONFIG_TROUGHNESS`, `CONFIG_ALPHA_TEST`, and the vertex-input type. These
   values select a permutation from the shipped shader cache.
2. Runtime effect state: texture bindings, constant values, culling, depth,
   blending, stencil, and the techniques/passes used by the render stage.

For example,
[`effect_gstage_default_materials.cpp`](../sources/vostok/render/engine/sources/effect_gstage_default_materials.cpp)
reads the material switches, fills `shader_configuration`, selects
`vertex_base.vs` and `gbuffer_pass.ps`, and binds values such as `t_base`,
`t_normal`, `t_roughness`, and `specular_fresnel_roughness_parameters`.
[`effect_material_base.cpp`](../sources/vostok/render/engine/sources/effect_material_base.cpp)
creates the technique/pass and applies its blend, depth, and cull mode.

The renderer stores the result as one `material_effects` entry per render
stage. G-buffer, lighting, shadow, forward, particle, and post-process stages
use the appropriate entry and call its effect pass; they do not inspect the
authoring material directly.

## The original SDK authoring workflow

The recovered editor source proves that render materials were authored through
the SDK's C++/CLI/WPF Material Editor:

- Material mode browsed `resources/materials` and edited `.material` base
  templates.
- Material Instance mode browsed `resources/material_instances` and edited
  `.material` instances.
- A base material declared its enabled stages under `material.common.stages`.
  Adding a stage initially assigned `effect_id = none`.
- Each stage loaded its property schema from
  `resources/stages/<stage>.stage` and loaded the selected effect's property
  descriptors.
- A base material could mark stage/effect properties as
  `is_model_parameter`. Only those properties were exposed as legal instance
  overrides.
- A material instance stored `instance.base_material` and used Lua supertables
  to inherit the base material and per-stage effect tables.
- Saving wrote Lua source under `resources/sources/materials/` or
  `resources/sources/material_instances/`.
- Preview converted the Lua config to an in-memory binary config and submitted
  it through the runtime `material_class` cook. Preview therefore exercised
  the same effect construction used by the game.

The relevant implementations are
[`material_editor.cpp`](../sources/vostok/editor/world/sources/material_editor.cpp),
[`material.cpp`](../sources/vostok/editor/world/sources/material.cpp),
[`material_instance.cpp`](../sources/vostok/editor/world/sources/material_instance.cpp),
and
[`material_stage.cpp`](../sources/vostok/editor/world/sources/material_stage.cpp).

## Runtime implementation

At runtime, `material_cook` resolves a logical material name to
`resources/material_instances/<name>.material`, loads its binary config, and
wraps it as a render `material` resource.

`material_effects_instance_cook` then:

1. Adds model-specific inputs such as vertex-input type and cull mode.
2. Walks every render stage present in the material config.
3. Resolves each stage's `effect_id` through `effect_manager`.
4. Merges effect defaults with the material values and hashes the resulting
   custom config.
5. Requests one `render_effect` resource per stage. A G-buffer material also
   supplies its configuration to the shadow effect.
6. Stores the successfully built effects and material flags in a
   `material_effects_instance`.

See
[`material_cook.cpp`](../sources/vostok/render/engine/sources/material_cook.cpp)
and
[`material_effects_instance_cook.cpp`](../sources/vostok/render/engine/sources/material_effects_instance_cook.cpp).

The Master Gold v0.100b client does not compile HLSL. Effect construction
selects bytecode already present under `shaders/sm_4_0/<shader>/<permutation>`.
The shader blob, define registry, masks, compiler version, and byte-exact
validation workflow are documented in [shaders.md](shaders.md).

## Offline shader workflow

Shader/effect programmers and material artists worked on separate layers:

1. Programmers edited HLSL and the C++ effect descriptors that define passes,
   shader names, configuration switches, bindings, and render state.
2. The offline `shader_compiler.exe` used `defines.def` or `defines_full.def`
   plus each shader's declared/masked defines to build the DXBC permutation
   cache.
3. Material artists selected an effect family and its exposed properties in
   the SDK. They did not edit or generate HLSL per asset.
4. The resource build cooked Lua material sources to binary configs and packed
   both material configs and shader bytecode into the game resources.
5. The retail client combined a material config, mesh vertex type, global
   render options, and the effect descriptor to select an existing shader
   permutation.

The recovered batch files
`resources/sources/shaders/sm_4_0/compile_shader.cmd` and
`compile_shader_NP_full.cmd` preserve the old offline compiler invocation.

## Workflow to restore in this repository

The porting pipeline should retain the original separation of concerns:

```text
source-game importer
  -> normalized material description
  -> choose a Vostok base material
  -> write a Vostok material instance with legal overrides
  -> convert/copy referenced textures
  -> validate effect, bindings, and required shader permutations
  -> preview through the runtime material cook
  -> cook binary resources for the target map
```

The repository should ultimately own these source inputs:

```text
resources/sources/materials/           base material templates
resources/sources/material_instances/  per-asset and per-map instances
resources/sources/stages/               stage property schemas
resources/sources/shaders/              HLSL and shared shader headers
```

Generated binary configs and DXBC caches should remain build artifacts. The
human-readable material sources, importer mappings, and validation rules should
be versioned.

For map ports, do not create a new shader for every foreign material. Import
the source game's material into a neutral description first, then map it onto
a small, explicit set of Vostok base materials: opaque, alpha-tested,
transparent/forward, decal, terrain, water, vegetation, emissive, and any
proved special cases. Texture/value differences belong in instances; a new
effect or HLSL family is warranted only when the required rendering model is
not expressible by an existing Vostok effect.

Each imported material should be checked for:

- a valid base material and stage/effect combination;
- a vertex-input type supported by that material stage;
- existing texture resources with the expected semantic and color space;
- all required precompiled shader permutations;
- constant and texture names present in DXBC reflection;
- correct opaque/alpha-test/blend, cull, depth, and shadow behavior;
- successful SDK/runtime-cook preview before map packaging;
- a separate game-material assignment where collision/impact behavior matters.

## Recovering the missing base-material sources

The retail resource pack has no `materials/` directory, but each cooked
material instance contains the merged material tree and retains its
`instance.base_material` name. This gives a bounded recovery path:

1. Decode all 392 binary material configs to a stable textual form.
2. Group instances by `base_material`.
3. Recover stage membership, `effect_id`, defaults, and property descriptors
   using the merged configs together with the SDK stage/effect schemas.
4. Use `is_model_parameter` to separate template-owned values from legal
   instance overrides.
5. Emit base Lua materials and minimal instance Lua files.
6. Re-cook them and compare their effective binary config trees against the
   shipped instances.

Later-version materials may be useful evidence, but they belong in
`vostok-shader-evolution` alongside the v0.1.1 shader material. They must not
be silently treated as v0.100b source truth.
