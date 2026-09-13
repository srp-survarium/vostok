# All-module datum-use audit

This report closes the first exhaustive pass over data referenced by paired
functions. The authoritative generated evidence remains the per-module TSV and
JSON files under `binaries/gen/`; this document records the conclusions that
should survive regeneration.

## Result

The initial current-source queue contained 84 open function datum-use rows.
After rebuilding every module, including all eight GFx libraries from the
current SDK overlays, the aggregate covers 34 modules and 71,415 retail plus
70,905 candidate relocation sites with `OPEN=0`.

The terminal function-row breakdown after the full datum-definition pass is:

- 16,934 direct `EXACT` rows;
- 57 `CALL_CONE_EXACT` rows;
- 51 source- and datum-diff-hash-bound `REVIEWED_WALL` rows.

The 51 reviews consist of 30 inline-boundary, six register-allocation, four
other compiler-materialization, two masked/generated-function, eight folded
referent-alias, and one CFG-boundary case. Their complete evidence is in
`config/codex_wall_reviews.tsv`. A source or datum-difference change makes the
corresponding review stale and reopens the gate.

## Source defects found

The audit produced target-faithful source corrections rather than merely
classifying every difference:

- allocation sites were normalized to the existing `VOSTOK_NEW_IMPL` and
  `VOSTOK_DELETE_IMPL` contract where retail uses the engine allocator;
- retail literals, console-command defaults, profile-slot tags, constants, and
  build-path/date inputs found by the earlier per-module pass were restored;
- the fourth bloom-vector component was corrected from `0.0f` to retail's
  `1.0f`;
- the player-status prefixes were recovered as `#+p:[ `, `#-p:[ `, `#q:[`,
  and `#t:[` rather than the semantic placeholder words;
- the Scaleform shader corpus and several missing SDK-overlay definitions were
  restored, preventing stale release libraries from hiding source datum uses;
- 38 AS3 `TypeInfo` flags, the GFx blend-mode table, and the 111-entry word-break
  table were restored in source and verified through rebuilt SDK archives;
- the complete player-template table, cloud-noise and LPV tables, camera near
  plane, animation frame-rate constants, and several target linkage/ownership
  declarations were recovered from the linked retail allocations;
- AI position tolerance, hit-target config keys, command-line diagnostics,
  resource paths/extensions, localization paths, and sound environment paths
  were restored to the retail literals;
- breath, dispersion, recoil, camera, ambient-volume, gravity, and light-id
  initializers were restored to the values and linkage observed in retail;
- Boost.Asio binder placeholders were restored to their PDB-proven spellings,
  and engine allocation sites use the existing `VOSTOK_NEW_IMPL` /
  `VOSTOK_DELETE_IMPL` contract;
- Scaleform's D3D blend-state table, SDK `TypeInfo` flags, word-break table,
  and missing explicit template emissions were restored from the target data.

The final closeout build found one remaining real defect in this set:
`weapon_dispersion_calculator` initialized three coefficients to `0.0f` and
its maximum to `1.0f`; retail's relocation set proves `1.0f`, `1.0f`, `1.0f`,
and `2.0f`. Correcting all four removed the last unreviewed `game_core` row.

## What remains outside this gate

`OPEN=0` means every paired function's observed absolute data relocations are
either identical, proved equivalent under the current source body, or have a
specific hash-scoped compiler/linker wall. It does not claim that every byte of
the executable's data sections has a paired source definition.

The same build reports 99.47% gross PDB-extent coverage, 99.43% whole-image
exactness, and 99.98% byte fidelity among paired definitions, with 33,511 exact
definitions. The projected objdiff denominator is 421,520,461 of 429,715,669
bytes (98.09%). It also retains 537 projection blockers and 1,495
otherwise-exact functions / 605,556 code bytes with strict referent debt. Those
are definition/ownership and compiler-context work for later passes; they are
not unreviewed datum uses in the completed function audit.

The integrity ratchet was remeasured against the expanded complete-extent and
consumer-owned denominator. That denominator reduced BASE_ONLY, RELOCS,
invalid extents, blockers, multiplicity, access-shape, and base-missing debt,
while exposing more normalized-byte, relocation-topology, target-only, and
strict-referent differences than the older identity-collapsing comparison.
Both directions are recorded at their measured values; the stable acceptance
criterion remains the hash-scoped 34-module `OPEN=0` function-use gate.

## Reproduction

Run a full build and then the strict gate:

```sh
python3 -m vostok build -j6
python3 -m vostok data check --gate
```

The gate passes only when all 34 module reports match the current executable
and ledger hashes, `binaries/gen/data_function_open.tsv` contains only its
header, every wall review is current, and every integrity-ratchet ceiling is
respected.
