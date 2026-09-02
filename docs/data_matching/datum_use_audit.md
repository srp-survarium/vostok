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

The terminal function-row breakdown is:

- 16,760 direct `EXACT` rows;
- 169 `CURRENT_EXACT`, 17 `HASH_MAX_EXACT`, and 33 `CALL_CONE_EXACT` rows;
- 70 source- and datum-diff-hash-bound `REVIEWED_WALL` rows.

The 70 reviews consist of 47 inline-boundary, 13 register-allocation, four
other compiler materialization, three masked/generated-function, two folded
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

The same build reports 98.57% gross PDB-extent coverage and 86.42% fidelity
among paired definitions (10,671 of 12,185). The projection also retains 31,430
blockers and 1,462 otherwise-exact functions with strict referent debt. Those
are definition/ownership and compiler-context work for later passes; they are
not unreviewed datum uses in the completed function audit.

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
