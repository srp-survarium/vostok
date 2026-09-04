# All-module datum-use audit

This report closes the first exhaustive pass over data referenced by paired
functions. The authoritative generated evidence remains the per-module TSV and
JSON files under `binaries/gen/`; this document records the conclusions that
should survive regeneration.

## Result

The initial current-source queue contained 84 open function datum-use rows.
After rebuilding every module, including all eight GFx libraries from the
current SDK overlays, the aggregate covers 34 modules and 71,415 retail plus
71,066 candidate relocation sites with `OPEN=0`.

The terminal function-row breakdown after the full datum-definition pass is:

- 16,767 direct `EXACT` rows;
- 152 `CURRENT_EXACT`, 24 `HASH_MAX_EXACT`, and 33 `CALL_CONE_EXACT` rows;
- 70 source- and datum-diff-hash-bound `REVIEWED_WALL` rows.

The 70 reviews consist of 47 inline-boundary, 11 register-allocation, four
other compiler materialization, three masked/generated-function, four folded
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
- `lobby_menu::on_operation_denied_received` now emits retail's unknown-type
  error, including its exact format string;
- `new_bt_primitive` constructs box and cylinder half-extents without the
  unrelated Vostok-to-Bullet Z-axis conversion and returns null in the default
  arm, matching retail's constants and control flow.

The historical fixes in the first five bullets are already present in the
campaign base. The final two were found by the zero-gate closeout build.

## What remains outside this gate

`OPEN=0` means every paired function's observed absolute data relocations are
either identical, proved equivalent under the current source body, or have a
specific hash-scoped compiler/linker wall. It does not claim that every byte of
the executable's data sections has a paired source definition.

The same build reports 98.57% gross PDB-extent coverage, 98.41% whole-image
exactness, and 99.96% byte fidelity among paired definitions, with 12,406 exact
definitions. The sole remaining normalized-byte row is a proved
static-initializer versus dynamic-initializer placement split whose runtime
writes agree. The projection also retains 31,123 blockers and 1,457
otherwise-exact functions / 587,746 code bytes with strict referent debt. Those
are definition/ownership and compiler-context work for later passes; they are
not unreviewed datum uses in the completed function audit.

The integrity ratchet was tightened to this new baseline. Physical-allocation
multiset accounting removed the old ambiguous bucket and exposed a slightly
larger truthful access-shape/base-missing denominator; those four increased
metrics are recorded at their measured values rather than hidden by the older
identity-collapsing comparison.

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
