# The shipped shaders, their recipe, and the roundtrip proof

For the SDK material authoring workflow and the runtime path from a material
stage to one of these shader permutations, see [materials.md](materials.md).

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
    python3 -m vostok.shaders.probe --jobs 12 --list NAME...   # same verdicts,
                                                    # concurrent, and it prints
                                                    # the permutation string of
                                                    # every DIFF

`disasm` writes era-exact listings (named cbuffers, bindings, signatures —
DXBC keeps its reflection) under `binaries/shaders/disasm/`. That tree is the
**ground for reconstruction**: the reference a rewritten HLSL file is judged
against, with `roundtrip` as the acceptance test. It is regenerable, so it
lives with the artifacts, not in git.

`roundtrip` compiles a recovered source with
`fxc /Zpr /O3 /T <profile>_4_0 /E main` under Wine and byte-compares the DXBC
against the shipped blob — container, checksum, and all.

`probe` is the same `roundtrip_one` behind a thread pool. It takes the whole
tree from about **38 minutes to 65 seconds**, which is the difference between
measuring a shared-header change properly and guessing at it from a per-shader
run. Use `roundtrip` when you want the canonical serial output; use `probe`
while iterating, and `--perms FILE` to re-run only the permutations that failed.

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
the ship name, and applying it let both drop local workarounds). **The
`s_border` half of that reading was wrong** — ship's own header, recovered
verbatim later that day, declares `s_border` *and* `s_anisotropic`, in that
order, between `s_linear` and `s_base`. See "Ship source, verbatim" below.

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


### Campaign state, 2026-08-18

**Every one of the 261 shipped names now has a source.** `subsurface_scattering.ps`
was the last blank file; it and `translucency.ps` closed together, 16/16. What
remains is per-permutation residue, not missing files.

Closed on this pass, all byte-identical and all confirmed by real roundtrip
output rather than by report: the indirect-lighting family (9 — both
`apply_indirect_lighting` shaders, `gbuffer_to_screen`, `volume_fog`,
`hiz_fill_culling_results_buffer`, `olta_blend`, `apply_decal_normals_blend`,
`wet_sufrace_normal_modify`, `fix_irradiance_texture`); `subsurface_scattering`
+ `translucency` (16); `decal_base` **0/30 → 30/30** with the atmosphere trio
and the editor pair (35); the `apply_distortion` pair (2);
`motion_blur.ps` **0/2 → 2/2**; and `reflection_mask.ps` **0/1 → 1/1**.

Final closure:

| shader | state | note |
| --- | --- | --- |
| `forward_lighting.ps` | 600/600 | every shipped permutation is byte-identical |

The full-tree sweep is now **3,402/3,402**. Every shipped v0.100b shader
permutation is byte-identical.

**Operand kind decides the split of a mixed multiply; chain depth decides the
order of two stores.** This one is recorded at length because three passes read
it wrong and roughly 120 spellings were spent on the wrong model. The distortion
pair's residual looked like "fxc emits the lanes of an `o0` store in `x,y,z,w`
order and ship does not". It is not lane order at all:

* **a mixed cbuffer/literal vector multiply splits cbuffer-lane-first, by
  operand *kind*, not by destination lane.** Inverting the constant vector makes
  fxc emit the `y` store before the `x` store — the cb lane leads whichever
  destination lane it feeds. So a mixed vector can never produce a
  literal-lane-first order, no matter how the writes are arranged;
* **chain depth beats lane index.** Giving one lane two extra multiplies moves
  its store after the other's immediately.

The fix was one line: applying the flip as its own `float2(1.0f, -1.0f)`
*before* the scale, so its `x` lane is `1.0f` and folds away, leaving a one-lane
immediate multiply that is a **separate DAG node** rather than a split lane. fxc
reassociates it onto the tail of the `y` chain, making `y` two deep against
`x`'s one, and the unfinished chain runs to completion first:

```hlsl
output.distortion.xy = (dist_vector * float2(1.0f, -1.0f)) * distortion_scale.xy;
```

Source order within that expression matters — applying the scale first fuses
both lanes into one `mul o0.xy` and loses 20 bytes. `copy_image.vs` and
`fill_sky_ao_map.ps` were the right existence proofs from the start; they were
read as "multi-instruction chains" when what mattered was "*deeper* chain".

**And the negative that explains the wasted effort: statement order is inert.**
Eighteen disjoint mask assignments tested in both orders, every pair
byte-identical. fxc rebuilds the IR canonically, so reordering writes cannot
change anything — which is exactly why so many spellings collapsed to the same
output and made the problem look unreachable. Note this does **not** contradict
the placement rule above: moving a *statement that defines a value* changes when
that value is ready, whereas permuting independent stores does not.

**`saturate(x)` and `clamp(x, 0.f, 1.f)` emit the same `_sat` and are NOT
interchangeable.** This closed the `ssao_filter_upsample` pair after about 70
spellings had failed, and it corrects what was recorded here earlier — that
`saturate()`, `clamp(x,0,1)`, `min(max(x,0),1)` and `max(min(x,1),0)` were all
equally transparent to fxc's non-negativity prover. They are not:

* an outer **`saturate()` is a saturate *node***, so the prover reads `[0,1]`
  off it and folds a downstream `max(a/b, 0.f)` away;
* an outer **`clamp()` / `min(max(...))` is fused into the producing
  instruction's `_sat` destination modifier** without ever becoming a node, and
  the prover is blind to it.

Position matters as much as spelling: the inner clamp must sit on the ratio
(ship's `div_sat`) and the outer on `1.f - ratio`. The whole family — clamp or
min/max outside, any of the three inside — is byte-identical; only an outer
`saturate()` triggers the fold.

The trap worth remembering is *where* the clamp was. Two cheap probes had
pointed at `ao`: dropping only `ao`'s saturate keeps the `max` and costs exactly
the `mov_sat` (2104 = ship's 2124 − 20). That looked like the proof running
through `ao`, and it is not — the clamp that mattered was the **weight's**
(`weight = clamp(1.f - saturate(range), 0.f, 1.f)`). Dropping only the weight's
saturate keeps the `max` at ship's exact size with `div` where ship has
`div_sat`, which is the probe that actually localises it.

**Constant laundering is dead as a technique**: `asfloat(0u)`, an icb element,
`-0.f`, `saturate(0.f)` and `saturate(1.f)` all fold before the prover runs, as
do `asfloat(asuint(x))` bitcasts, inlined helpers, `out`/`inout` parameters and
struct-returning helpers. None is opaque to it.

**fxc emits independent statement *groups* in source order**, and a
multiply/divide pair written as two statements lets the divide sink to its use.
`sun.ps` needed `main` to compute the first cascade's *homogeneous* projection
itself and pass it in, leaving the perspective divide inside the callee — the
shape `forward_lighting.ps` already uses. With the whole projection inside the
callee the four `dp4`s land after the light-colour block instead of before it.
Two smaller ones from the same file: **a `float2` accumulator divided once emits
one vectorised mul where two scalar accumulators divided separately emit two**,
and a per-tap fetch pair wants named locals with both samples issued before
either accumulate — `+=` directly on the sample interleaves them.

`reflection_mask.ps` closes the `STAT`-only class. Its inert
`clip(rsqrt(dot(reflected, reflected)))` must sit after the live early return:
fxc counts the implicit move, then proves the clip non-negative and removes it
before SHDR. Moving the same clip above the branch leaves SHDR identical but
changes `MovInstructionCount` back from 4 to 3, so source order is part of the
evidence even when the emitted instructions are unchanged.

**A latent defect worth checking wherever the vertex-input dispatch is
inlined.** `CONFIG_VERTEX_INPUT_TYPE == 12` (postprocess) must leave
`parameters.tc` at **zero**; only type 0 planar-maps. The distortion family had
type 12 falling into the null branch and inheriting its planar tc, and — this is
the part that matters — **the wrong code cost exactly as many bytes as the right
code**, so a size-only check could never have shown it. Most files that inline
the dispatch already carry the correct arm (`gbuffer_pass`, `forward_base`,
`forward_lighting`, `z_only`, `gbuffer_emissive_pass`, `gbuffer_nomaterial_pass`,
`fill_reflective_shadow_map`, `depth_accumulate` all do; `subsurface_scattering.ps`
is byte-proven across types 0–14 with its own type-12 branch). `decal_base.ps`
and `environment_probe_lighting.ps` do not.

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

**The one that unlocks schedule-shaped residuals.** `if (cond) discard;` and
`clip(cond ? -1.f : 1.f)` emit the *same* `discard_nz` instruction, and they are
**not** interchangeable. With the `if` form fxc treats the discard as an
early-out and hoists its entire dependency chain above independent work,
re-scheduling and re-allocating everything around it; `clip()` emits the
identical instruction and leaves the schedule alone. This single substitution
took `decal_base.ps` from 0/30 to 30/30 — the `if` form had been costing a
fifth temporary and pushing the tangent basis below the discard — and closed
both parked clouds-blend residuals with one line each. Their file headers had
recorded the residual as "one mov placement" around the alpha write; it was
never the alpha write, it was the horizon cut above it, and five source shapes
had been tried on the wrong statement. **If a parked residual has the shape
"same instructions, same count, wrong register numbering or wrong statement
order", and the shader contains a discard, try this first.**

Two more from the same family:

* `pow(x, 3.0f)` and `x*x*x` differ — the latter emits the second `mul` with
  its operands reversed;
* a clip-to-uv map spelled `(a.xy + a.w) * 0.5f / a.w` keeps its own divide;
  spelled `a.xy/a.w*0.5f + 0.5f`, fxc shares one divide with a neighbouring
  `normalize(a.xy/a.w)` and loses two instructions.

**A parked "one constant in the wrong place" residual is usually statement
placement, not spelling.** This is the second cross-cutting finding of the
campaign, and it now has three independent confirmations —
`wet_sufrace_normal_modify.ps`, `apply_indirect_lighting_diffuse.ps` and
`skylight.ps`. Skylight had been parked since the first day with a tried-list
of about ten spellings for its ssao `*2.5` (float4 forms, `/0.4f`, parentheses,
operand orders); every one was irrelevant. With the ssao sample written *below*
the ramp block, fxc folds the two scalar muls into one chain and sinks the
constant to its end; moving that single statement **above** the eye-ray/ramp
block keeps `mul r1.x, r1.x, l(2.5)` on the sampled value, and the file is
byte-identical. **Whenever the ops and their count already match but a constant
sits at the wrong end of a chain, move statements before rewriting
expressions.**

Three more from the g-buffer families:

* **a NaN literal in a blob is a constant fold, not a written value.**
  `l(0xffc00000)` in the no-material g-buffer blobs is `0/0`, arising from
  encoding the identity normal `float3(0,0,1)` through `sqrt(-8*z+8)`. It is
  what made "zero normal" look plausible and it is wrong — those permutations
  store the *identity* normal. Never reconstruct such a literal as a written
  NaN or as a zero vector;
* **a cbuffer that appears in only some permutations of one shader tells you
  which arm reads a constant.** `static_globals` shows up in
  `gbuffer_nomaterial_pass.ps` only for vertex-input types 7 and 8, every
  member `[unused]` — a 752-byte gap that identified `mul(m_V, normal)` as the
  particle normal rule before a single instruction was read;
* **fxc normalizes the interpolated tangent basis for grassmesh (type 11) and
  for static meshes at LOD 1, but not at LOD 0.** That is a ship source fact in
  `get_material_parameters`, now byte-proven from two families.

Earned on `translucency.ps` and `subsurface_scattering.ps`:

* **operand-order inversion is a first guess, not a law.** It held for
  scalar × cbuffer-vector (`sun_color*x` emitting `mul x, sun_color`) and
  failed for a final `mad` (`albedo*result` emitting `mad albedo, result`
  directly). Four of seven residual fixes across those two files were single
  operand swaps found by flipping one expression and recompiling — cheaper than
  reasoning about it;
* **the first `and` of a bool chain orders its operands by register, not by
  source.** `translucency.ps`'s four cascade containment tests come from one
  source function, yet the listing shows two different operand orders for the
  leading `and`, purely because the `ge` results landed in different registers.
  Do not chase that as a source difference;
* **statement *grouping* is part of the register allocator**, not only
  placement. Writing three channel taps into a named `float3` and adding it
  once per direction collapsed 38 temps to ship's 8 and fixed the whole
  schedule; three separate `sum.x += …; sum.y += …; sum.z += …` statements do
  **not** coalesce into the `add r1.xyz` ship emits;
* **`pow` with a small integer exponent expands, a large one does not**:
  `pow(x,5.f)` becomes `x*x`, `x2*x2`, `x*x4` — three muls, no log/exp — while
  `pow(x,12.f)` stays `log`/`mul`/`exp`. So a bare log/mul/exp means a
  non-expandable exponent and a mul chain means a small integer one; neither
  should be "fixed" into the other;
* **`bool * int * float` is a literal ship idiom.** The cascade select is
  `inside_n * outside_0 * … * t_n.z`, where `inside_n` is a `bool` and
  `outside_n` is `int outside = inside ? 0 : 1;`. That exact typing is what
  produces `and 1` / `imul` / `itof` for cascades 1–3 and `and 0x3f800000` for
  cascade 0. `!c` would emit `not`; `c ? 0 : 1` emits the `movc` ship has;
* **`convert_to_linear_space` is distinguishable from a raw `pow(x,2.2f)` by
  the `abs`.** `MASTER_GOLD` is unset in these blobs, so
  `ABS_TO_REMOVE_WARNING` expands to `abs`: the albedo conversion shows
  `log |r3.xxyz|` while a gamma decode written inline shows a bare `log`. Both
  files needed the raw `pow` in one place and the header function in another;
* **the 16-entry Poisson disc is shared across families.** `subsurface_scattering.ps`
  uses the same table `sun.ps` filters its cascades with, at exactly twice the
  scale, and its ×0.4/×0.1 radii are exact products of it. That makes the disc
  ship truth recovered twice over rather than a guess made while reconstructing
  `sun.ps`.

**A removed `clip` can still determine `STAT`.** `reflection_mask.ps` closed
when the final fallthrough gained
`clip(rsqrt(dot(reflected, reflected)))`. The `rsqrt` is non-negative, so the
clip and its discard path vanish before SHDR; its implicit move survives long
enough to raise `MovInstructionCount` from 3 to ship's 4. The location matters:
placing the identical statement before the live branch emits the same SHDR but
leaves the counter at 3.

`motion_blur.ps` closed by expressing the already-proven tap sequence as the
literal vectors fxc emits: Q0 starts at `.y` and wraps through `.x`, while Q3
groups taps 2 and 5 in one coordinate vector. This preserves the loop and tap
order but prevents fxc from canonicalising the offsets into sequential lanes;
both quality permutations now round-trip byte-identically.

## Shared-header truth recovered so far

Reconstruction is the only thing that proves what the *headers* looked like at
ship, and each fix pays across every file that includes them. Applied:

| header | was | ship (proven by) |
| --- | --- | --- |
| `common_cbuffers.h` | 3-member `material_parameters`; `static_globals` without `m_V2W`/`screen_res`; loose `fresnel_at_0_degree`/`m_V2W` | 38 members, `alpha_ref_parameter` at 476; `m_V2W` and live `screen_res` inside `static_globals`; no loose globals (`depth_accumulate.ps`, 73 static_globals RDEFs) |
| `common_samplers.h` | `s_border` | `s_border` **and** `s_anisotropic`, in that order, between `s_linear` and `s_base`, plus `s_prev_z` and `s_shmap` (ship's own header — the earlier reading, that `s_border` was a drift *of* `s_anisotropic`, was wrong: they coexist) |
| `gbuffer.h` | `near_far_invn_invf` inside a `g_buffer_packing` cbuffer; raw-xyz normal/depth readers | loose uniform; Lambert-azimuthal normal decode, linear `.x` depth (`skylight.ps`) |
| `dof_functions.h` | `pow(saturate(...), focus_power)` and a `>=` ternary | the ternary is on `<` (`complex_post_process_blend.ps` 12 blobs, `gather_bloom.ps`). The `pow` **is** there in ship's source — what changed is `focus_power`, which ship defines as the literal `1.0h` with `dof_parameters.z` commented out beside it, so `pow(x, 1)` folds away. "No pow at all" described the bytecode, not the source |
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

## Ship source, verbatim: the build-816 debug blobs

Everything above this line was **inferred from bytecode**. This section is not.

Build 816 (v0.1.1a, five days after 802) shipped seven blobs compiled `/Zi` at
the default optimisation level instead of `/O3` — somebody's debugging session
went out with the build. They are the only seven in the whole v0.1 line with an
`SDBG` chunk, and an `SDBG` chunk embeds **the source text fxc was handed**:
comments, dead code, author names, commented-out experiments. The seven are
`god_rays.ps` and the six `GLOBAL_POST_PROCESS_QUALITY = 3` permutations of
`complex_post_process_blend.ps`.

Split out one file per source, CRLF preserved, in
`vostok-shader-evolution/recovered/ship-headers/` (`split_pool.py` beside them
regenerates the whole directory from build 816's `resources.db`). Read the
`SDBG` string pool as a **pool, not a concatenation**: each file's text is
appended in turn preceded by its name, but only when that name does not already
occur earlier in the pool — which it usually does, inside the `#include` line
that pulled it in. So `gbuffer.h`'s text is followed directly by
`gamma_correction.h`'s with no marker between them; the reliable cut is each
file's own include-guard terminator.

**Twelve headers came out, not fourteen.** `common.h` and `debug.h` are *not*
in either pool — the only occurrences of those two names anywhere in the chunks
are the `#include` lines that name them. Neither file contains a single
declaration (`common.h` is nothing but `#include`s, `debug.h` nothing but the
`ABS_TO_REMOVE_WARNING` macro), so neither contributes a symbol to the debug
info and the file table drops both. The same rule explains why `god_rays.ps`'s
pool omits `common_iostructs.h` while the blend's carries it: god rays declares
its own `vertex_output_struct` and references none of the shared ones. Their
contents remain unrecovered; the pool ordering does pin `common.h`'s include
list to `common_iostructs.h`, `common_samplers.h`, `common_cbuffers.h`,
`common_functions.h`, in that order.

**Why this source is admissible evidence for 802.** 802 and 816 ship 261 names
each, and only five of them differ in code: `gbuffer_pass.ps`,
`terrain_gbuffer_pass.ps`, `environment_probe_lighting.ps`, `god_rays.ps` and
`complex_post_process_blend.ps` (that last one only because six of its
permutations got the `/Zi` treatment). 251 names byte-identical across the
window is a strong bound on how far a shared header can have moved — and the
roundtrip is the actual test, applied one header at a time.

### What landed

Eleven of the twelve went into `resources/sources/shaders/sm_4_0/` **verbatim**
(LF instead of CRLF, nothing else), and the full-tree roundtrip was unchanged
after each: `common_samplers.h`, `common_cbuffers.h`, `common_functions.h`,
`common_iostructs.h`, `gamma_correction.h`, `dof_functions.h`,
`light_ps_helper.h`, `light_accumulator.h`, `colored_light.h`,
`log_luv_color_space.h`, `luminance.h`. **The 802↔816 delta across all eleven
is nil** — every one of them compiles 802's blobs byte-for-byte as it stands in
816. The tree's versions of them were the X-Ray-derived source drop with
campaign patches on top; ship's are Vostok rewrites dated 2012, `half`-typed
throughout, and they carry a good deal the tree never had:

* `common_functions.h` gains `MAX_LIGHTING_COLOR = 8.0h` with
  `PACK_LIGHTING_COLOR`/`UNPACK_LIGHTING_COLOR` and their LPV siblings
  (`MAX_LPV_LIGHTING_COLOR = 4.0h`), `g_ssao_max_value = 2.5h` with
  `pack_ssao`/`unpack_ssao`, `desaturate`, `hdr_color_to_rgba8`,
  `IN_HDR_COLOR`/`OUT_HDR_COLOR` — and **loses** every X-Ray leftover the tree
  was carrying (`p_hemi`, `v_hemi`, `calc_fogging`, `calc_model_hemi_r1`,
  `gbuf_pack_normal`, `alpha_to_coverage`, the `USABLE_BIT_*` block). Not one
  of those was referenced anywhere else in the tree;
* `light_ps_helper.h` gains the real `get_light_accumulator`, quoted in full
  below, and `common_cbuffers.h` loses the `MOVED_FROM_dynamic_transforms` and
  `shader_params` cbuffers (nothing read `L_material` or `alpha_ref`);
* `light_accumulator.h` gains `sun_light_accumulator`
  (`half4 diffuse, half4 specular, half2 depth_and_shadow`).

Three collisions had to be resolved, all of them our reconstructions
compensating for what our headers lacked, none of them an 802↔816 difference,
and none of them costing a byte:

* `sun.ps` declared its own `sun_light_accumulator` as the render-target
  struct. A pixel shader's return type needs `SV_Target` semantics so it cannot
  *be* the header's struct; renamed to `sun_light_accumulator_targets`. Note
  the header's three members are exactly what that pass writes, in order — its
  `depth_and_shadow` is `sun.ps`'s `float2( shadow_depth, shadow )`;
* `s_shmap` was declared locally in `shadow.h`, `spot_shadow.h`, `sun.ps`,
  `rain.ps`, `forward_simple_water.ps` and `wet_sufrace_normal_modify.ps`, and
  `s_prev_z` in `temporal_mask.ps`. Ship declares both in `common_samplers.h` —
  `s_prev_z` between `s_material1` and `s_jitter`, `s_shmap` last of all, after
  `s_hemi`. All seven local declarations deleted; every affected shader still
  compiles byte-identically, so ship's slot order is the order they were
  reconstructed against anyway (`rain.ps`'s file header had recorded "declared
  here, not in common_samplers.h" as if it were a finding — it was not).

`complex_post_process_blend.ps` itself landed **verbatim, 12/12
byte-identical**: 679 lines of ship's own source, with the commented-out
`scattered_color`, the `USE_LUMINANCE_FROM_HISTOGRAM` switch, the four unused
tone mappers, the `in_rect` mip-viewer debug block and the dead
`#if 0 // show tonemapping curve` tail. That is proof its source did not move
between 802 and 816 — the six changed blobs are the `/Zi` compile and nothing
else.

### The one header that did not land: `gbuffer.h`

Ship's `gbuffer.h` is a different header from ours, not a drifted one. Ours is
the X-Ray original, switched by `GLOBAL_GBUFFER_POS_PACKING` /
`GLOBAL_GBUFFER_NORMAL_PACKING`; ship's has no packing conditionals at all, a
three-argument `gbuffer_read_rt_position`, a `gbuffer_read_rt_color` that
*returns* a gamma-decoded `half3`, no `gbuffer_read_material_parameters`, a
`g_data` carrying `fresnel`/`roughness` where ours carries `specular_power`,
and a family we lack entirely: `pack_spec_trans_and_roughness`,
`gbuffer_read_spec_trans_and_roughness`, `has_translucency_flag`,
`gbuffer_read_fresnel`, `gbuffer_read_translucency`, `decode_n`, `decode_n2`,
`make_view_mat`, `gbuffer_read_frame_depth_linear`. It also includes
`gamma_correction.h`, exactly as the shipped dep tables said it should.

Dropped in verbatim it costs **121 permutations across 24 names**, and the
cause is localised to a single statement. Ship's `gbuffer_read_rt_normal`
finishes `normalize(decode_n2(data.xy))` with

```hlsl
	normal.z			*=	-1.0f;
```

and ours does not. Adding that one line to *our* header — changing nothing else
— reproduces 120 of the 121 (the remaining one is
`lpv_inject_occluders_camera_view.vs`, which calls the five-argument
`gbuffer_read_rt_position`). So the g-buffer normal's sign convention is
opposite between ship's header and the ~24 reconstructions built on ours, and
landing ship's header means re-deriving all of them. That is a structural
divergence, not a back-datable constant, and it is left alone: ship's file
stays in `recovered/ship-headers/gbuffer.h` for the next pass. The consumers
that would have to move with it are `capsule_light.h`, `sphere_light.h`,
`plane_spot_light.h`, `spot_light.h`, `obb_light.h`, `parallel_light.h` and
`point_light.h` (all seven call `gbuffer_read_material_parameters`),
`gbuffer_to_screen.ps` (three-argument `gbuffer_read_rt_color`),
`ssao_accumulation.ps` and `lpv_inject_occluders_camera_view.vs` (five-argument
`gbuffer_read_rt_position`, and `g_data.specular_power`).

### `god_rays.ps`: a real change in five days

`god_rays.ps` is the one file where ship's 816 source genuinely does not
compile to 802's blob — 2,864 bytes against 802's 2,664. It went in anyway,
with ship's structure, comments and dead code intact, and **four back-dating
edits took it to byte-identical on the first attempt**. Each one is a 802→816
change:

| 802 | 816 |
| --- | --- |
| `half ratio = screen_res.x / screen_res.y;` | `0.7*screen_res.x / screen_res.y` |
| `dist = exp(-dist*20);` | `dist = saturate(pow(exp(-dist*20), 1));` |
| `min(pow(alpha, god_rays_color_blend_power), 1.0h)` as the lerp factor | `saturate(pow(alpha, god_rays_color_blend_power))` |
| `return god_rays_intensity * alpha * out_c;` | `… * out_c*1 + god_rays_intensity * alpha2 * out_c*2`, with `alpha2 = pow(alpha, 5*pow(1-alpha, 1))` |
| `PACK_LIGHTING_COLOR( final_color * 5.0h )` | `PACK_LIGHTING_COLOR( final_color )` |

The `min` versus `saturate` row is the third independent confirmation of the
rule recorded above: the 802 blob emits a standalone `min r0.y, r0.y,
l(1.000000)` where an outer `saturate()` would have fused into the preceding
`exp` as `exp_sat`. The `*5.0h` is a guess about spelling confirmed by byte
identity — what is proven is the product, `mul_sat r0.xyz, r0.xyzx,
l(0.625000)`, and 0.625 is 5/8 against ship's `MAX_LIGHTING_COLOR` of 8.

Read as a rendering change: in five days god rays grew a second, sharply
falling additive lobe (`pow(alpha, 5(1-alpha))` peaks where alpha is high),
doubled that lobe's weight, dropped the single-lobe 5× gain, gained a 0.7
horizontal squeeze on the radial-distance metric, and clamped the falloff.

### `light_ps_helper.h`, quoted

Nothing in the tree calls `get_light_accumulator` today — every accumulator
shader inlines its own — so landing the header was byte-neutral. It is still
the most valuable thing in the pool, because it is the ground truth for the
open light-accumulator residuals:

```hlsl
#define LIGHT_ACCUMULATOR_MAGNIFY_FACTOR	1.f

light_accumulator get_light_accumulator ( half3 light_color, light_factors light_factors )
{
	light_accumulator result;
	result.diffuse  = half4( ( convert_to_linear_space(light_color) * light_intensity * max(light_factors.diffuse,  0.0h) * light_diffuse_influence_factor ) / LIGHT_ACCUMULATOR_MAGNIFY_FACTOR, 0.f );
	result.specular = half4( ( convert_to_linear_space(light_color) * light_intensity * max(light_factors.specular, 0.0h) * light_specular_influence_factor) / LIGHT_ACCUMULATOR_MAGNIFY_FACTOR, 0.f );
	result.diffuse  =	PACK_LIGHTING_COLOR( result.diffuse );
	result.specular =	PACK_LIGHTING_COLOR( result.specular );
	return result;
}
```

Four things it settles that the campaign had been guessing at: the gamma decode
is on `light_color` **alone**, not on the product; `light_intensity` multiplies
*before* the factor, not after; the `max(…, 0.0h)` sits on the light factor
before any multiply; and the pack is `PACK_LIGHTING_COLOR` over the whole
`half4`, i.e. `pow(saturate(c/8.0h), 0.5h)`, which fxc lowers to `rsq`+`div` —
the `1.f/rsqrt(saturate(c*0.125f))` spelling the reconstructions arrived at
independently. The `result.diffuse.w = light_factors.diffuse < 0 ? length(…)`
line the tree's version computed is **commented out** at ship, twice over.

### `half` versus `float`, measured

Ship writes `half` everywhere and the tree was written in `float`, so landing
these headers retypes a great deal of shared code. Every measurement taken says
that is free at `ps_4_0` with this compiler:

* the `h` literal suffix does **not** round. `static const float k = 2.2h;`
  and `= 2.2f;` both emit `l(2.200000)`; `= 2.19921875f;` (the IEEE-half
  rounding of 2.2) emits `l(2.199219)` and breaks everything downstream. Same
  for a `half3(0.2125h, …)` constant vector against `float3(0.2125f, …)`;
* `pow(saturate(c/8.0h), 0.5h)` and `pow(saturate(c/8.0f), 0.5f)` both lower to
  `mul_sat`/`rsq`/`div` — the `h` suffix does not block the square-root
  special case;
* `get_light_accumulator` called from ship's `half`-typed header, and a
  character-identical `float`-typed local copy of the same body, compile to the
  same 1,492 bytes — and so do both of those again with `light_accumulator.h`'s
  structs retyped to `float`. Four configurations, one output;
* the full tree is unchanged at 3,364/3,402 with all eleven headers `half`-typed.

A report of `565 → 556` on `forward_lighting.ps` attributed to this typing could
not be reproduced in any of the above, and does not show in the full-tree
figure. Two things were poisoning measurements that afternoon and either would
produce it: two workers writing shared headers and `forward_lighting.ps` during
each other's runs, and — the trap worth naming — **a per-shader run is not a
measurement of a shared-header change.** 257 of the 261 names include at least
one of these headers. Take the full tree before and after, or the number means
nothing. If the retype does turn out to cost slots from a quiet tree, it is a
mechanical back-date of the types in `light_ps_helper.h`,
`light_accumulator.h` and `colored_light.h`, and ship's structure survives it.

## Every shipped name now has a source

`subsurface_scattering.ps` was the last shader with no source at all; it and
`translucency.ps` closed together, byte-identical across all 16 permutations.
All 261 of build 802's shipped shader names now have a reconstruction, and what
remains is per-permutation residue rather than blank files.

The two are worth reading as a pair, because they are the engine's two answers
to light that does not stop at a surface:

* **`translucency.ps`** is deferred *sun through thin geometry*. It rebuilds
  view position from `s_eye_ray_corner` and g-buffer depth, projects through
  the four cascade matrices, and takes the depth of the **first containing**
  cascade minus `t_sun_translucensy_help_data.y` — the lit surface's own
  cascade depth — as the thickness the light crossed. Material translucency is
  a 5-bit field split across the normal target (3 bits from `.z`'s 10-bit word,
  2 from `.w`), gamma-decoded and gated on bit 7 of the diffuse alpha.
  Attenuation is `pow(1 - saturate(thickness/(translucency*0.035)), 10)`, and
  the colour is a flat wrap term, a back-lit `N·L`, and a
  `pow(saturate(-V·L),3)` forward-scatter lobe, packed
  `1/rsqrt(saturate(c*0.125))`;
* **`subsurface_scattering.ps`** is the screen-space skin resolve, drawn with
  the object's own geometry: 49 bilateral taps of the diffuse light
  accumulator — the centre plus **16 Poisson directions × 3 radii**, the radii
  feeding R/G/B of one accumulator so the widest scatter lands in red. The
  radius is divided by `max(view_z * 0.333, 0.125)` then multiplied by
  `screen_res.zw`, so the kernel shrinks with distance and is expressed in
  texels; a tap whose depth differs from the centre by more than 0.01 collapses
  back to the centre uv, emitted as `lt`/`and 0x3f800000`/`mad` rather than a
  branch. The average is mixed 50/50 with the undiffused centre, a rim term
  `pow(1-saturate(-V·N_world),5) * pow(saturate(-V·L),12)` is added, LPV
  indirect fades in where scatter is dim, and the result is
  `albedo_linear*scattered + specular*translucency`.

**Header truth these two pinned but did not apply.** The 5-bit translucency
unpack (`.z*1023 >> 7 | .w*3 << 3`, `*1/31`, `min 1`, `pow 2.2`) is
byte-identical in both and at ship almost certainly lived in `gbuffer.h`, whose
read side it is; it is duplicated locally in both files because a `gbuffer.h`
edit needs its own full-tree proof. `subsurface_scattering.ps`'s dep table names
only `material.h`, `vertex_input.h`, `gamma_correction.h` and `gbuffer.h`, so
the per-type `vertex_output_struct` and its loose globals came from ship's
`*_vertex_input.h` headers — the recovered ones are drifted
(`null_mesh_vertex_input.h` adds a `tc : TEXCOORD1` the shipped ISGN lacks), so
the dispatch is local, exactly as `gbuffer_pass.ps` already does it. Two
per-type `$Globals` layouts are now byte-pinned in that file's `#if` block:
particle (VIT 7/8) declares `up_view_vector`, `right_view_vector`,
`use_align_by_dir`, `view_location`, `use_fixed_axis`, `rotation_fixed_axis`,
`locked_no_ratate_axis_index` ahead of `near_far_invn_invf`; grassmesh (VIT 11)
declares `patch_parameters`. And skeleton types 3–6 putting world position at
TEXCOORD5 and view position at TEXCOORD3 — listed above as proven but not
applied — is now confirmed independently by a second family.

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
