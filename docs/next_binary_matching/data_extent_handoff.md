# Handoff — render relocation and data sieve

**Status:** the direct render audit is implemented and both source-data sieves
have been measured by authoritative builds. The retail-aligned source-data
queue is closed: the current audit has zero PDB-identity-aligned complete data
mismatches. Remaining function-use rows belong to code shape, missing bodies,
inlining, vtables, ICF, or the documented build-path wall.

## What is measured

`python3 -m vostok data render-relocs` starts from every physical PE `HIGHLOW`
relocation in a relevant render function whose target is in `.rdata`, `.data`,
or loader-zero BSS. A site appears exactly once on each image. Pairing uses, in
order, exact function offset, decoded access sequence, and a unique complete
function relocation fingerprint for duplicate static functions.

The generated artifacts are:

- `binaries/gen/render_reloc_audit.tsv`: every retail and candidate site,
  access shape, datum identity/addend, extent result, normalized bytes,
  pointer-cell layout, and resolved pointer targets;
- `binaries/gen/render_extentless_data.tsv`: one dossier per PDB start without
  a trustworthy complete extent, including all xrefs, observed end, next
  starts/referents/relocations, candidate votes, hashes, and byte previews;
- `binaries/gen/render_function_data.tsv`: the deduplicated datum set used by
  each paired function, plus missing uses and missing definitions;
- `binaries/gen/render_reloc_report.json`: counts, input hashes, and output
  hashes;
- `docs/next_binary_matching/render_data_problems.md`: the tracked human
  report. Machine-complete rows stay in the TSVs; the Markdown omits settled
  extentless rows and records only real differences or unresolved evidence.

Compiler-owned constants pair by their complete normalized bytes, independent
of `.data` versus `.rdata` and incidental PDB pool owner. Named data with no
complete byte identity pairs by stable PDB identity and addend. This covers the
multiple-symbol case without transferring candidate RVAs or inventing source
symbols. Named initializer bytes and pointer targets remain independently
visible in the complete-datum audit.

## Final authoritative-build census

The final linked sieve contains:

- 18,629 retail and 18,475 candidate relocation sites;
- 17,985 paired sites, 644 retail-only sites, and 490 candidate-only sites;
- 17,873 paired sites with the same decoded access shape;
- 10,395 exact complete comparisons, 65 normalized-byte differences, 13
  relocation-layout differences, and zero resolved-target differences;
- 2,483 unique extentless dossiers. Of these, 2,193 are exact, 77 are exact
  with unrelated boundary outliers, and 105 are exact through unique PDB
  identity fallback. Only two remain `REFERENT_DIFF`, both retail-versus-local
  build-path strings.

At function level, whole-byte canonicalization reduces the old noisy queue to:

| Status | Functions |
|---|---:|
| `EXACT` | 1,854 |
| `USE_DIFF` | 49 |
| `BASE_DEFINITION_MISSING` | 2 |
| `TARGET_DEFINITION_MISSING` | 2 |
| `DEFINITION_MISSING_BOTH` | 9 |

The first build reduced complete byte mismatches from 434 to 65, eliminated all
40 wrong relocation targets, and reduced `USE_DIFF` from 82 to 55. Manual
review of the 70 unique mismatched complete windows found 69 wrong-pairing
windows and one same-PDB-identity source mismatch: `s_clouds_time` has a null
command name in the built candidate but retail stores `"clouds_time"`.

## Source corrections recovered from retail evidence

Complete PDB-identity windows established the following initializer and command
metadata corrections:

- zero `core::g_log_flags` (`log_to_stdout`);
- restore the retail defaults and command metadata in render `options`,
  `resource_manager`, `clouds`, `renderer`, `render_model`, `radiance_volume`,
  `portal_sector_system`, `stage_light_propagation_volumes`, `stage_lights`,
  `stage_postprocess`, `stage_shadow_direct`, and `stage_visibility`;
- restore the `r_` prefixes on the three grass commands;
- restore the retail fog-box vertex and face arrays;
- restore `lpv_layout`'s `TEXCOORD` semantic.

Direct function-use evidence established additional source facts:

- `lpv_layout` and `shadow_layout` have internal const linkage;
- the four `hardware_[1-4]weights_skinning_vertex_layout` arrays have external
  mutable linkage at global scope. Retail's PDB symbols are unqualified, not
  members of `vostok::render`;
- every `-1.0f` sentinel in the default `cloud_key_parameters` constructor is
  actually `+1.0f`, as are the ambient/direct-light defaults in
  `environment_temp`;
- `volume_fog_parameters` uses `+1.0f`; the same header datum reaches
  `stage_particles` and `stage_sun`;
- sphere side vertices store `position.w = 1.0f`;
- the shadow-frustum scale is `(2, 3, 2)`, not `(1, 3, 1)`;
- sky-dome source keeps `i / 15` as the named local and applies
  `deg2rad(100)` in the trigonometric expressions;
- atmosphere source keeps `rotation / 180` as the named local and applies
  `pi` in the trigonometric expressions;
- `post_process_parameters::environment_rain_radius_scale` is `1.1f`, not
  `0.001f`. Retail loads `1.1f` and stores it to the PDB member offset `0x2bc`;
- `effect_gstage_default_materials` passes `s_z_only_0`, not `first_lod`.

These are retail-source corrections, not guesses from fuzzy score. The old
candidate's bytes, retail PDB identity/type or member offset, and retail
instruction use agree for each one.

The second source-only sieve added seven more evidence-backed corrections:

- `s_clouds_time` uses the retail command name `"clouds_time"`;
- `environment_temp::environment_temp` stores `+1.0f` in
  `keys[2].wind_speed`;
- `stage_clouds` initializes `m_wind_direction` to `(1, 0, 1)`;
- the four hardware-skinning layout definitions are moved out of
  `namespace vostok::render` to reproduce the retail PDB identities.

The final authoritative build measured all seven together. The
`s_clouds_time` initializer, both cloud constructors, and all four hardware
layout uses are now `EXACT`; the four arrays also have the retail's unqualified
external mutable identities. PDB-identity-aligned source-data candidates fell
to zero.

The sky-dome and atmosphere source retains the retail named declarations
`move_xz` and `angle_in_rad`, respectively. The optimized candidate PDB still
omits those two locals, so their remaining local-preservation/statement-size
work belongs to the render structure campaign rather than another literal or
initializer edit.

## Rows that are not data-initializer edits

The remaining static queue already contains several concrete non-data classes:

- retail `C:\\survarium\\sources...` versus candidate `Z:\\home\\...` `__FILE__`
  strings are the documented build-path wall;
- the extra retail `s_dxt_env_probe_cc` copy belongs to
  `renderer_probes_and_ao.obj`; our TU is archive-unreferenced because its real
  external code body is still carcass. Do not fabricate an anchor;
- Boost stored-vtable extras and different vtable slot destinations are
  code/template/ICF work;
- renderer color helpers retain source literals such as `0.5f`, `0.7f`, and
  `1.0f` on retail while the old candidate exposes converted constants such as
  `127.5f`. That is inline/call shape, not a wrong color datum;
- the target-only `1.25f` in `stage_clouds` is already present in source under
  a multiply by zero; similar zero/one target-only rows are optimizer or
  missing-body use differences;
- four extentless rows have no candidate vote because `/GL` removes two
  fully-recovered, zero-caller candidate bodies that retail retained. Three are
  strings used by `make_copy_with_srgb_format`; the fourth is a float used by
  `stage_light_propagation_volumes::render_to_sky_rms`. Measuring them would
  require a fabricated retention anchor, not further source or data recovery;
  the vtable target rows remain owned by code/structure matching.

All 55 fresh `USE_DIFF` rows have been classified. Four are the now-corrected
hardware-layout scope mismatch, and two are the corrected cloud literals. The
remainder are code-shape/inlining differences, target-only body references, or
values already present in target-shaped source. The nine
`DEFINITION_MISSING_BOTH` rows are retail-versus-local `__FILE__` strings. The
two `BASE_DEFINITION_MISSING` rows are the folded `s_dxt_env_probe_cc` dynamic
initializer/destructor, and the two `TARGET_DEFINITION_MISSING` rows are
code/folding differences rather than absent retail data definitions.

Function `USE_DIFF` means only that the linked bodies currently address
different deduplicated sets. It becomes a data edit only after the retail load
is mapped to a source expression or named datum. A value already present in
the target-shaped source must not be changed merely to remove a stale-base row.

## Meaning of “extentless”

An extentless datum has a retail PDB identity and start RVA but no trustworthy
complete size. It is not unnamed or absent. The evidence window is bounded by
section limits, the next known start, semantic compiler records (`__real`,
`__xmm`, guards, narrow strings, and vftables), xrefs, and data relocations.
That window is diagnostic and is never silently promoted to a reviewed extent.

Reviewed PDB-size overrides belong in
`config/retail/pdb_data_extents.tsv`. PDB-owned symbols must not be duplicated
in `config/retail/data_symbols.tsv`, which remains only the reviewed non-PDB
registry.

## Return to code matching

1. Keep code-cone, vtable, ICF, and build-path walls queryable in the report and
   return them to the normal render-to-100 campaign.
2. Treat a future `USE_DIFF` as data work only after retail instructions map
   its missing referent to a genuinely wrong source expression or definition.
3. Accumulate any newly proved data facts into another complete static sieve;
   compile and measure the batch once rather than rebuilding per row.
4. Require the PDB-identity-aligned source-data candidate count to remain zero.

The normal data checks remain:

```sh
python3 -m vostok data missing --check
python3 -m vostok data render-relocs --check
python3 -m vostok data check
```
