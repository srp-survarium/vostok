# SPDX-License-Identifier: GPL-3.0-or-later
"""The engine's shader-define registry, in permutation-string order.

The blob path's permutation component is built by
shader_binary_source_cook::translate_query: for each define in
shader_macros::fill_shader_macro_list order, append the define's decimal
value string if its NAME is listed in shaders/masks[shader][ext], else "_".
An unmasked define is always exactly one underscore; a masked define is one
or more digits, so digit-runs span registry-adjacent masked defines.

Order, recovered from engine source (sources/vostok/render/core/dx11/):

* first the option-backed globals - options.cpp registers render_cc console
  commands whose constructor HEAD-INSERTS into options::first_render_command,
  so fill_global_macros walks them in REVERSE declaration order;
* then GLOBAL_MASTER_GOLD and the configuration defines, in
  shader_macros.cpp fill_shader_configuration_macros call order.

17 + 65 = 82 entries, which is exactly the length of every all-underscore
permutation shipped - the count is the proof the recovered order is the
target's. BOOL defines render as "0"/"1"; U32 as unbounded decimals.
"""

from __future__ import annotations

BOOL = "bool"
U32 = "u32"

# (name, kind) - kind bounds how many digits a value may occupy.
REGISTRY: list[tuple[str, str]] = [
    # --- globals: reverse of options.cpp declaration order -----------------
    ("GLOBAL_SHADING_QUALITY", U32),
    ("GLOBAL_POST_PROCESS_QUALITY", U32),
    ("GLOBAL_LIGHTING_QUALITY", U32),
    ("GLOBAL_USE_VEGETATION_TRAMPLE", BOOL),
    ("GLOBAL_USE_POISSON_DISC_SHADOW_FILTER", BOOL),
    ("GLOBAL_USE_SCREENSPACE_REFLECTIONS_MASK", BOOL),
    ("GLOBAL_UPDATE_SHADOWS_EVERY_FRAME", BOOL),
    ("GLOBAL_USE_16BIT_RT", BOOL),
    ("GLOBAL_SSAO_NUM_SAMPLES", U32),
    ("GLOBAL_NUM_MAX_LIGHT_INSTANCES", U32),
    ("GLOBAL_CASCADED_SHADOW_MAP_SIZE", U32),
    ("GLOBAL_USE_GOD_RAYS", BOOL),
    ("GLOBAL_SHADOWMAP_QUALITY", U32),
    ("GLOBAL_SPOT_SHADOWMAP_SIZE", U32),
    ("GLOBAL_SHADOWMAP_SIZE", U32),
    ("GLOBAL_ORGANIC_IRRADIANCE_TEXTURE_SIZE", U32),
    ("GLOBAL_ALLOW_STEEPPARALLAX", BOOL),
    # --- configuration: fill_shader_configuration_macros order -------------
    ("GLOBAL_MASTER_GOLD", BOOL),
    ("CONFIG_TDIFFUSE", BOOL),
    ("CONFIG_TNORMAL", BOOL),
    ("CONFIG_TDETAIL_NORMAL", BOOL),
    ("CONFIG_ALPHA_TEST", BOOL),
    ("CONFIG_PARALLAX", BOOL),
    ("CONFIG_TRANSLUCENCY", BOOL),
    ("CONFIG_TSPECULAR_COLOR", BOOL),
    ("CONFIG_TFRESNEL", BOOL),
    ("CONFIG_TROUGHNESS", BOOL),
    ("CONFIG_EMISSIVE", U32),
    ("CONFIG_TDIFFUSE_POWER", BOOL),
    ("CONFIG_TDETAIL", BOOL),
    ("CONFIG_USE_SUBUV", BOOL),
    ("CONFIG_TTRANSPARENCY", BOOL),
    ("CONFIG_REFLECTION", U32),
    ("CONFIG_USE_GRASS_FRESNEL_EFFECT", BOOL),
    ("CONFIG_LIGHT_TYPE", U32),
    ("CONFIG_VERTEX_INPUT_TYPE", U32),
    ("CONFIG_USE_BOKEH_DOF", BOOL),
    ("CONFIG_USE_BOKEH_IMAGE", BOOL),
    ("CONFIG_VARIATION_MASK", BOOL),
    ("CONFIG_SHADOWED_LIGHT", BOOL),
    ("CONFIG_TALPHABLENDED_DIFFUSE", BOOL),
    ("CONFIG_TALPHABLENDED_NORMAL", BOOL),
    ("CONFIG_USE_BOUND_NORMALS", BOOL),
    ("CONFIG_USE_UP_DIRECTIONAL_NORMALS", BOOL),
    ("CONFIG_USE_DIFFUSE_AS_SPECULAR", BOOL),
    ("CONFIG_NUM_USED_TERRAIN_LAYERS", U32),
    ("CONFIG_USED_TERRAIN_HEIGHT_MASK", U32),
    ("CONFIG_LOD_INDEX", U32),
    ("CONFIG_USE_SPECULAR_LIGHTING", BOOL),
    ("CONFIG_VERTEX_BLENDED_DIFFUSE", BOOL),
    ("CONFIG_VERTEX_BLENDED_MASK", BOOL),
    ("CONFIG_VERTEX_BLENDED_NORMAL", BOOL),
    ("CONFIG_VERTEX_BLENDED_SPECULAR", BOOL),
    ("CONFIG_USE_INDIRECT_SPECULAR", BOOL),
    ("CONFIG_USE_IMAGE_GRAIN", BOOL),
    ("CONFIG_ENV_PROBE_CLIP_BY_NORMAL", BOOL),
    ("CONFIG_ENV_PROBE_WITH_SHADOWS", BOOL),
    ("CONFIG_ENV_PROBE_GEOMETRY_TYPE", BOOL),
    ("CONFIG_USE_DIFFUSE_MASKED_COLOR", BOOL),
    ("CONFIG_USE_DIFFUSE_MASKED_COLOR_BY_HUE", BOOL),
    ("CONFIG_USE_UV_SCROLLING", BOOL),
    ("CONFIG_USE_NORMAL_WAVES", BOOL),
    ("CONFIG_USE_FUZZINESS", BOOL),
    ("CONFIG_TERRAIN_BLEND_MODE", BOOL),
    ("CONFIG_USE_SOFT_EDGES", BOOL),
    ("CONFIG_USE_THICKNESS_MAP", BOOL),
    ("CONFIG_USE_SUBSURFACE_SCATTERING_MASK_MAP", BOOL),
    ("CONFIG_USE_OLTA", BOOL),
    ("CONFIG_WIND_MOTION", U32),
    ("CONFIG_DECAL_MATERIAL", BOOL),
    ("CONFIG_DECAL_TYPE", U32),
    ("CONFIG_POST_PROCESS_BLUR_KERNEL", U32),
    ("CONFIG_TANISOTROPIC_DIRECTION", BOOL),
    ("CONFIG_ANISOTROPIC_MATERIAL", BOOL),
    ("CONFIG_USE_AO_TEXTURE", BOOL),
    ("CONFIG_USE_ORGANIC_SCATTERING_AMOUNT_MASK", BOOL),
    ("CONFIG_USE_ORGANIC_SCATTERING_DEPTH_TEXTURE", BOOL),
    ("CONFIG_USE_ORGANIC_BACK_ILLUMINATION_TEXTURE", BOOL),
    ("CONFIG_USE_ORGANIC_SUBDERMAL_TEXTURE", BOOL),
    ("CONFIG_SEQUENCE", BOOL),
    ("CONFIG_REFLECTION_MASK", BOOL),
    ("CONFIG_LOCAL_REFLECTIONS", BOOL),
]

assert len(REGISTRY) == 82, len(REGISTRY)

# A define that a shader USES but that shaders/masks does not list for it is
# absent from the permutation string, yet the compile still saw a value - the
# engine's global macro list always supplies one. Such values are recoverable
# only from the blobs, so they are added here one at a time with their proof.
GLOBAL_DEFAULTS: dict[str, str] = {
    # 10/1024 = 0.009766, the neighbourhood step in fix_irradiance_texture.ps.
    "GLOBAL_ORGANIC_IRRADIANCE_TEXTURE_SIZE": "1024",
}


def parse_permutations(perms: list[str]) -> dict[str, dict[str, str]] | None:
    """Solve one shader's permutation strings against the registry.

    Every permutation of a shader renders the same masked-define set, so the
    solver finds the set S of registry slots that hold digits consistently
    across all strings, splitting digit-runs over registry-adjacent masked
    slots (BOOL slots take exactly one digit, U32 one or more). Returns
    {perm: {define_name: value}} or None if no consistent solution exists.
    Multiple solutions are possible for adjacent U32s; the caller must treat
    the result as a candidate and let byte-identical compilation confirm it.
    """
    solutions = _parse_one(perms[0])
    for perm in perms[1:]:
        if len(solutions) == 1:
            break
        keep = []
        for slots, _ in solutions:
            if any(s == slots for s, _ in _parse_one(perm)):
                keep.append((slots, _))
        solutions = keep or solutions
    if not solutions:
        return None
    slots = solutions[0][0]
    out: dict[str, dict[str, str]] = {}
    for perm in perms:
        values = _apply(perm, slots)
        if values is None:
            return None
        out[perm] = values
    return out


def _parse_one(perm: str) -> list[tuple[tuple[int, ...], dict[str, str]]]:
    """All (masked-slot-tuple, values) readings of a single permutation."""
    results: list[tuple[tuple[int, ...], dict[str, str]]] = []

    def rec(reg_i: int, pos: int, slots: list[int], values: dict[str, str]):
        if reg_i == len(REGISTRY):
            if pos == len(perm):
                results.append((tuple(slots), dict(values)))
            return
        if pos >= len(perm):
            return
        name, kind = REGISTRY[reg_i]
        if perm[pos] == "_":
            rec(reg_i + 1, pos + 1, slots, values)
            return
        max_len = 1 if kind == BOOL else 4
        for n in range(1, max_len + 1):
            chunk = perm[pos:pos + n]
            if len(chunk) < n or not chunk.isdigit():
                break
            if kind == BOOL and chunk not in ("0", "1"):
                break
            slots.append(reg_i)
            values[name] = chunk
            rec(reg_i + 1, pos + n, slots, values)
            slots.pop()
            del values[name]

    rec(0, 0, [], {})
    return results


def _apply(perm: str, slots: tuple[int, ...]) -> dict[str, str] | None:
    """Read one permutation constrained to a known masked-slot set."""
    for got_slots, values in _parse_one(perm):
        if got_slots == slots:
            return values
    return None
