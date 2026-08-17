# The shipped shaders, their recipe, and the roundtrip proof

Established 2026-08-17, for the map-porting first-class-citizens work (see
`resource-porter/docs/roadmap.md`, Phase 1): before authoring any shader of
our own, prove we can reproduce the ones the client already trusts.

## How a shader ships

`resources.db` carries 3,403 blobs under `shaders/sm_4_0/<name>.{vs,ps,gs}/`,
261 distinct shader names. The last path component is the **permutation
string**: one value per engine define in registry order, `_` for unset —
all-underscore means "no define set", and 194 names ship such a plain
permutation. `shaders/masks` is a self-describing config mapping each effect
name to the define names its vs/ps stages consume (decodable with
`vostok-port dump-db-config`).

Each blob is:

    u32 dep_count
    dep_count x { char name[260]; u32 unix_mtime }   # include closure;
                                                     # entry 0 = the shader,
                                                     # under resources.sources/
    DXBC ...                                         # to end of file

The dependency paths spell `resources.sources/shaders/sm_4_0/...` — the same
layout as this repo's `resources/sources/shaders/`, which is the recovered
original source tree. The mtimes date the ship compile to early May 2013.

The client **cannot compile HLSL**: Master Gold dropped the recompile arm of
`shader_binary_source_cook` (no `compile_shader_task`, no
`save_binary_shader`; `resource_manager::reload_shader_sources` is a stub).
It only reads blobs back and reflects on them — signatures from bytecode,
constants bound by name. X-Ray shipped shader *source* and compiled at
runtime; Vostok kept the source layout and dropped the runtime compiler.

## The compiler, named by the blobs themselves

Every blob's RDEF chunk records its creator:
`Microsoft (R) HLSL Shader Compiler 9.29.952.3111` — the June 2010 DirectX
SDK's D3DCompiler_43. The RDEF flags word is `0x8108`:

| flag | meaning |
| --- | --- |
| 0x0008 | `PACK_MATRIX_ROW_MAJOR` (the tool source's one explicit flag) |
| 0x0100 | `NO_PRESHADER` (fxc adds this itself for 4_0 targets) |
| 0x8000 | `OPTIMIZATION_LEVEL3` |

Note `/O3`: `sources/vostok/shader_compiler/` passes only ROW_MAJOR, so the
shipped blobs were built by a configuration the recovered tool source does not
show. The blobs outrank the tool source.

The flake's `dxsdk-shader-compiler` package extracts `fxc.exe` and the native
`D3DCompiler_43.dll` from `DXSDK_Jun10.exe` (fetched from Microsoft, pinned by
hash); the dev shell exports it as `DXSDK_SHADER_COMPILER` and pins a gcroot.
Wine's builtin d3dcompiler is a reimplementation and must not be substituted —
the native DLL sits beside fxc.exe, so Wine's app-directory loading picks it.

## The roundtrip

    nix develop .#with-resources        # blobs via VOSTOK_RESOURCES_UNPACKED
    python3 -m vostok.shaders coverage
    python3 -m vostok.shaders dump clouds.ps
    python3 -m vostok.shaders disasm                # ship-truth asm, one per name
    python3 -m vostok.shaders disasm gbuffer_pass.ps --all-perms
    python3 -m vostok.shaders roundtrip             # exits nonzero on any DIFF

`disasm` writes era-exact listings (named cbuffers, bindings, signatures —
DXBC keeps its reflection) under `binaries/shaders/disasm/`. That tree is the
**ground for reconstruction**: the reference a rewritten HLSL file is judged
against, with `roundtrip` as the acceptance test. It is regenerable, so it
lives with the artifacts, not in git.

`roundtrip` compiles a recovered source with
`fxc /Zpr /O3 /T <profile>_4_0 /E main` under Wine and byte-compares the DXBC
against the shipped blob — container, checksum, and all.

**What the sources are, and are not.** `resources/sources/shaders/` arrived
with the repo's initial source drop, alongside the engine C++ — it is the
developers' own HLSL, comments, dead code and all (the commented-out blocks
citing `sunmask`, `USE_SJITTER`, `shadowtest_sun` are verbatim X-Ray R2
identifiers: these files were rewritten from X-Ray's shaders, old lines left
in). Nothing in this tree is derived from blobs. But it is **one snapshot of
a moving dev tree**, and the blobs were compiled from another — so the
roundtrip doubles as per-file authentication.

First sweep, over the plain (no-define) permutations — 2026-08-17:

| verdict | count | meaning |
| --- | ---: | --- |
| identical | 33 | the file is byte-provably the ship revision |
| DIFF | 47 | a different revision than shipped — almost all differ in *size* (`clouds.ps` is a 692-byte stub against a 5,280-byte ship compile; `post_process_fxaa.ps` is a different FXAA generation) |
| skipped | 3,322 | permutation sets defines (mapping undecoded), or no recovered source |

Same day, after the first header reconstructions (see below): **43 identical,
38 DIFF**. Rewriting `common_cbuffers.h`'s `static_globals` to the ship layout
(m_V2W inside, `eye_position_view_space`, live `screen_res`, loose globals
deleted) and un-wrapping `near_far_invn_invf` from its `g_buffer_packing`
cbuffer flipped ten drifted shaders to byte-identical — their only drift was
the header. Ship truth for those headers is now proven ten ways.

## Reconstruction, begun

`skylight.ps` — absent from the source drop — was reconstructed from its
listing on the same day: six-color ambient cube along a VS-interpolated
basis, directional wrap term, the world-height skylight ramp
(`pow(saturate((h-lower)/band), exp)` — the band the map port re-anchors),
SSAO modulation, `1.f/rsqrt(...)` light-accumulator packing (the source
literally wrote `1/rsqrt`, not `sqrt` — the blob proves it). It sits one
instruction-pair from byte-identity: ship materializes the ssao `*2.5` as
its own mul; every source shape tried gets canonicalised to `(ramp*ssao)*
2.5`. The residual is recorded in the file header; same slot count, same
registers, same RDEF. Its byte-identity, and the gbuffer.h read-path
variants it pinned (Lambert-azimuthal normal decode, linear `.x` depth),
are constrained by the one plain skylight permutation — the define-bearing
`gbuffer_pass.ps` family will constrain the rest.

261 shipped names, 176 recovered files covering 111 of them, 150 shipped
names with **no source at all** — including the material shaders the porting
work cares most about (`gbuffer_pass.ps`, `terrain_gbuffer_pass.ps`,
`skylight.ps`). Run `coverage` for the live figure.

The consequence for authoring: only a file the roundtrip stamps `identical`
may be treated as ship truth. A DIFF file is *related* source — a reading
aid, not a base. For the 47 drifted and the 150 missing, ship-state source
exists only as blobs, recoverable by disassembly (`fxc /dumpbin /Fc`) plus
reconstruction, with the roundtrip as the acceptance test; a reconstruction
that round-trips byte-identical replaces the drifted file in place (git keeps
the snapshot), converging this tree toward ship state the same way `sources/`
converges toward the shipped executable.

## The recovery campaign: every shipped shader, byte-proven

Goal: all 261 shipped names — every one of the 3,403 permutations —
recompiled from source in `resources/sources/shaders/` byte-identically, the
same way `sources/` converges on the executable. The roundtrip is the ledger:
`python3 -m vostok.shaders roundtrip` derives the live state; nothing else
records it.

**Phase 1 — the permutation vocabulary** (prerequisite for 67 names that ship
only define-bearing permutations, `gbuffer_pass.ps` and
`terrain_gbuffer_pass.ps` among them). The blob path's last component is
built by `shader_binary_source_cook::translate_query`: walk the engine's
global macro list in order; append the value string if the define's name is
in `shaders/masks[name][ext]`, else `_`. The list is 82 entries — proven by
the 82-char plain permutations — and its order is recovered from source:
17 option-backed globals in *reverse* declaration order
(`options.cpp`'s `render_cc` chain head-inserts), then `GLOBAL_MASTER_GOLD`
and 64 configuration defines in `fill_shader_configuration_macros` order.
Values render as decimals; underscores anchor absolute slots; a digit-run
spans adjacent masked defines and is split by cross-permutation constraint
solving, with byte-identical compilation as the final oracle.
`vostok.shaders` carries the registry (`registry.py`) and parser; `roundtrip`
passes the parsed `/D` set.

**Phase 2 — drifted sources** (38 after the header fixes): each has related
source and ship asm; reconstruct by diffing intent, gated by the roundtrip.

**Phase 3 — missing sources** (~150): written from the asm tree like
`skylight.ps` was. Order by shared-header leverage: the gbuffer/material
family first (they pin store-side `gbuffer.h` the way skylight pinned
read-side), then light accumulators, post-process, effects.

**Phase 4 — the full-permutation sweep**: every permutation of every name
compiled and compared; residuals that resist byte-identity are parked with
the tried-list in the file header, skylight-style, and counted.

### Campaign state, 2026-08-17 evening

The permutation vocabulary (Phase 1) is **done** — `registry.py` solves every
shipped permutation string, so every blob is judged and the only `skip` left
means "no source file yet". Sweep progression that day: 33 identical → 43 →
1,332 → **2,547 of 3,402**, and climbing as families land.

Proven 100%: `vertex_base.vs` + its four hash-siblings (196),
`forward_base.ps` (120), `depth_accumulate.ps` (69),
`motion_vectors_accumulation.ps` (50), `terrain_gbuffer_pass.ps` (30),
the seven editor shaders (98), `clouds.ps`/`clouds.vs`,
`post_process_fxaa.ps` (stock FXAA 3.11 preset 39, with a recovered
`fxaa.h`), the atmosphere family, and ~70 single-blob utilities.
Nearly complete: **`gbuffer_pass.ps` 1,072/1,136** and
**`forward_lighting.ps` 563/600** — both with their residual clusters and
tried-lists recorded in their file headers.

Reconstruction is also what recovers **shared-header ship truth**, and each
fix pays across the tree: the 38-member `material_parameters` cbuffer,
`static_globals`, the g-buffer read *and* store packings, the ship
`CONFIG_VERTEX_INPUT_TYPE` map, and `common_samplers.h`'s `s_anisotropic`
(the recovered header called it `s_border`; two independent families proved
the ship name, and applying it let both drop local workarounds).

Also complete since: **`forward_probe_lighting.ps` 300/300**,
`depth_accumulate_batched.ps` (69), `fill_reflective_shadow_map_backed.ps`
(15), `geometry_depth_pass.ps` (14), `fill_reflective_shadow_map_position.ps`
(14), the ssao/temporal/sharpen group (7 of 9 files clean), `blur_accumulate.ps`
(8), and ~15 more single-blob shaders.

**19 shipped names still have no source**, 46 permutations in total — the
biggest are `subsurface_scattering.ps` (15), `forward_sky.ps` (15),
`forward_simple_water.ps` (15) and the two `apply_indirect_lighting_*`
shaders. Parked with tried-lists: `environment_probe_lighting.ps` (one `*4`
that fxc hoists), the `ssao_filter_upsample` pair (one `max` fxc proves
away), `skylight.ps` (one mul), the distortion pair, the clouds-blend pair,
`gbuffer_nomaterial_pass.ps` (per-VIT layouts), and the static-input
permutations of `z_only.ps` and `gbuffer_emissive_pass.ps`.

## fxc 9.29 fingerprints (earned per file, reusable)

Byte-identity usually fails on *shape*, not semantics. The idioms below were
each proven by a reconstruction and are the first things to try on a DIFF:

* **operand order survives** into the emitted op — `scalar*vector` swaps
  relative to source spelling (write `value*color` to get `mul color, value`),
  `max(a,b)`/`&&`/`dp2` all keep their order, and `a + b` decides which side
  fuses into a `mad`;
* `sample` = `.Sample`, `sample_l` = `.SampleLevel(...,0)`;
* `1.f/rsqrt(x)` emits `rsq`+`div` where `sqrt(x)` would emit `sqrt`;
* a comparison spelled `-x > 0` emits `lt 0, -x`;
* `[flatten] if (c) return X;` emits `retc`; a plain `if` emits a real branch;
* `UNROLL` is dead — ship wrote `[unroll]` directly;
* `log`/`mul 2.2`/`exp` is `pow(abs(x), 2.2)`, i.e. `convert_to_linear_space`;
* bool→float is `and 0x3f800000`;
* **unused things are still declared**: uniforms occupy `$Globals` in
  declaration order, interpolants occupy ISGN slots, and an entirely
  unreferenced cbuffer is the one thing fxc drops;
* **semantic spelling is preserved** — `SV_POSITION` vs `SV_Position`,
  `SV_TARGET` vs `SV_Target` differ per TU; copy the listing;
* a rounded-looking constant is often exact: pull the float bits out of the
  DXBC (`editor_geometry_complexity`'s ramp is `1/9999`, not `0.0001`;
  `sky_ambient_occlusion`'s height scale is `1/(255*255*255)`; a literal
  printed `l(0.000000)` is routinely a real number);
* **the ISGN is a struct census** — fxc never trims unused interpolants, so a
  four-element input signature means a four-member struct, not a big struct
  with dead members. That is what exposed the LPV static input;
* ISGN elements are listed in **register/component order**, not declaration
  order (a `TEXCOORD7` packed into `r6.z` precedes `TEXCOORD6` at `r7`);
* `scalar*vector` operand order follows **evaluation order**, so moving the
  scalar's defining statement earlier flips the emitted `mul` operands —
  statement placement is a tool, not just style;
* `a + b` fuses the **first** operand into the `mad`.

Earned on the indirect-lighting family, and general enough to try first:

* **`[branch]` is live in ship sources**, exactly as `[unroll]` is.
  `gbuffer_to_screen.ps` only reaches byte-identity written
  `else [branch] if (type == 25)`; without the attribute fxc flattens that arm
  and the fallback into one `movc`, three instructions short, and shifts a
  register three branches earlier. Bracing it as `else { [branch] if … }` does
  **not** work — the attribute has to sit on the `else if` itself;
* **operand order inverts across the board**, not only for `scalar*vector`: an
  emitted `op A, B` comes from source `B op A` for `mul`, `min`, `max`, `or`
  and `and` alike. The exception is two cbuffer reads from *different*
  registers, which fxc sorts by register index regardless of how they were
  written;
* **statement placement is a register allocator, not just a scheduler.**
  `wet_sufrace_normal_modify.ps` went from 23 diff lines to byte-identical
  purely by moving its `ring_uv` block above `flow_height0`, and
  `apply_indirect_lighting_diffuse.ps` needed `sh_coefficients` moved *down* to
  its point of use. When the ops match and only register numbering differs,
  move statements before rewriting anything;
* **per-component `max` is a distinct source shape from a vectorised one**:
  `indirect.r = max(dot(...), 0)` written three times emits two `max`es (a
  vectorised pair plus a scalar) where `indirect = max(indirect, 0)` emits one.
  A one-instruction difference here cascaded through a whole shader;
* `exp2()` emits a bare `exp`; `exp()` emits `mul 1.442695` then `exp`;
* `round_pi` in the old sources is `ceil()`;
* a scalar compared `x == false` emits `ieq x, 0` where `!x` emits `not`;
* `mul r.xyz, v, l(1,2,-2)` is a genuine `v * float3(1,2,-2)`; writing the three
  component multiplies separately does not produce it.

**Where the trail goes cold.** Two shaders are byte-identical except for a
`STAT` counter, and that is a real limit rather than a near miss.
`reflection_mask.ps` matches ship across RDEF, ISGN, OSGN and the entire
1,152-byte SHDR chunk; the only differing byte in 2,300 is the mov counter at
`STAT[0x4C]` — ship 4, ours 3. That counter is taken *before* fxc's final
peephole (shipped `fix_irradiance_texture.ps` records 21 movs for a listing
showing 5), so ship's source held one extra mov that the optimiser folded away
and no amount of reading the listing will show which. `motion_blur.ps` is the
same class: same sizes, same taps in the same order, but fxc packs the per-tap
offsets into a different component rotation (ship `l(0.0135,-0.0675,-0.0405,
-0.0135)` read `.yyzz`/`.wwxx`; every source shape tried gives
`l(-0.0675,-0.0405,-0.0135,0.0135)` read `.xxyy`/`.zzww`).

## Shared-header truth recovered so far

Reconstruction is the only thing that proves what the *headers* looked like at
ship, and each fix pays across every file that includes them. Applied:

| header | was | ship (proven by) |
| --- | --- | --- |
| `common_cbuffers.h` | 3-member `material_parameters`; `static_globals` without `m_V2W`/`screen_res`; loose `fresnel_at_0_degree`/`m_V2W` | 38 members, `alpha_ref_parameter` at 476; `m_V2W` and live `screen_res` inside `static_globals`; no loose globals (`depth_accumulate.ps`, 73 static_globals RDEFs) |
| `common_samplers.h` | `s_border` | **`s_anisotropic`** between `s_linear` and `s_base` (`forward_lighting.ps` 600 blobs, `terrain_gbuffer_pass.ps` 30, `forward_probe_lighting.ps` 300 — three independent families) |
| `gbuffer.h` | `near_far_invn_invf` inside a `g_buffer_packing` cbuffer; raw-xyz normal/depth readers | loose uniform; Lambert-azimuthal normal decode, linear `.x` depth (`skylight.ps`) |
| `dof_functions.h` | `pow(saturate(...), focus_power)` and a `>=` ternary | no `pow` at all (`focus_power` unread) and the ternary on `<` (`complex_post_process_blend.ps` 12 blobs, `gather_bloom.ps`) |
| `psf_alpha_test.h` | `clip(alpha - 0.8)` | `clip(alpha - 0.25)` (all 34 alpha-test perms of `depth_accumulate_batched.ps`) |

Proven but **not** applied, because the affected reconstructions carry the
right shapes locally and a header edit would need its own verification pass:

* `gbuffer.h` should include `gamma_correction.h` (→`debug.h`) — every
  shipped dep table that names `gbuffer.h` lists them next;
* `common.h` should **not** include `common_policies.h` — no shipped dep
  table lists it (harmless for DXBC, so this is dating evidence, not a bug);
* `psf_normal.h`'s `psf_tnormal` samples through `s_anisotropic`, not
  `s_normal`;
* `vertex_input.h` per-type truth: for skeleton types 3–6 **world position is
  at TEXCOORD5**, previous position at TEXCOORD4, view position at
  TEXCOORD3; the shared `get_material_parameters` sets `parameters.normal`
  per type (identity for 0/9/10/12/13/14, the unnormalized third column of
  the tangent basis for 1–6, normalized for 11, `mul(m_V, normal)` for 7/8);
* three ship headers the tree lacks entirely:
  `static_mesh_vertex_input_{lpv,shadow,shadow_batched}.h`, plus `wind.h`,
  `sharpen_common.h`, `skeleton_{1..4}_bones_mesh_vertex_input.h`,
  `user_vertex_input.h`, `atmospheric_scattering_common.h`.

## The last unreconstructed shader: `subsurface_scattering.ps`

15 permutations (VERTEX_INPUT_TYPE 0–14, plus SUBUV on type 8), ~598
instructions, no pre-ship ancestor in `old/` — the skin/organic family there
is a different, multi-pass generation. Structure decoded from the VIT-0
listing, so the next worker starts here rather than from scratch:

* **screen-space gather, 49 taps**: a centre tap plus **16 Poisson
  directions × 3 radii** (`×1`, `×0.4`, `×0.1` — the ×0.4/×0.1 constants in
  the blob are exactly the base times those factors, so the source scales a
  16-entry table rather than storing 48);
* the radius is divided by `max(view_z * 0.333, 0.125)` and then multiplied
  by `screen_res.zw`, so the kernel shrinks with distance and is expressed in
  texels;
* **each tap is bilateral**: sample `t_position` (through `s_position`) at
  the tap, and if `|centre_depth − tap_depth| > 0.01` the tap collapses back
  to the centre uv — emitted as `lt`/`and 0x3f800000`/`mad`, not a branch;
* every tap reads `t_diffuse_lighting` through `s_material1` and **decodes
  the light accumulator** as `saturate(x)² * 8` — the exact inverse of the
  `1.f/rsqrt(saturate(c*0.125))` store that `skylight.ps` and the
  accumulators write. The three radii feed three separate accumulator
  channels (the sample swizzles rotate `t3.xyzw`/`t3.yxzw`/`t3.zxyw`), which
  is the usual red-scatters-furthest arrangement;
* the tail unpacks a 5-bit field from `t_normal` (`*1023`, `>>7`, `<<3`,
  `*1/31`, `min 1`, `pow 2.2`), forces it to 1 when `t_diffuse.w*255 >> 7`
  is set, reads `t_specular_lighting`, converts the albedo to linear, and
  writes `albedo_linear * scattered + saturate(specular)² * 8 * factor`.

Exact float bits for the 16 directions are recoverable the usual way (scan
the blob's DXBC for words whose `%.6f` matches the listing).

## What comes next

* **Decode the permutation mapping** — `shaders/masks` + the engine define
  registry (`shader_defines.h`, `effect_options_descriptor.cpp`) turn a
  permutation string into `/D` defines; then `roundtrip` extends beyond the
  plain permutations.
* **Reconstruct the missing 150** — fxc's own `/dumpbin /Fc` disassembles a
  shipped blob with named registers (DXBC keeps its reflection chunks), and
  the roundtrip is the acceptance test for a reconstruction: recovered HLSL
  is correct when its compile is byte-identical. Start with
  `gbuffer_pass.ps` and `terrain_gbuffer_pass.ps`; the latter's disassembly
  likely explains the white-ground failure recorded in
  `resource-porter/docs/investigation/soc-terrain-detail-2026-08-17.md`.
* **Author new shaders** with the same recipe, packaged in the blob container
  above — that is Phase 1 of the porting roadmap.
