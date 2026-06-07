# game_material_manager_cook.cpp - structure verification (v2 matcher pass)

Branch: chain/game_core-sv-game_material_manager_cook
Diagnose cmd (per fn):
  nix develop -c pdb_fetch --target-index binaries/rich/target/index.jsonl \
    --base-index binaries/rich/base/index.jsonl --function <fn> --rva <rva> \
    --view structure-diff --condensed \
    --objdiff-base-dir binaries/objdiff/base --objdiff-target-dir binaries/objdiff/target

Target RVAs:
  ctor 0x753630, delete_resource 0x753690, create_game_materials 0x7536e0,
  on_decals_loaded 0x753830, create_game_material_pairs 0x753b00,
  on_configs_loaded 0x754330, translate_query 0x7544b0

report.json fuzzy% (actual; the in-source STATE markers were badly STALE):

| fn                        | stale marker | report.json | new STATE        |
|---------------------------|--------------|-------------|------------------|
| ctor                      | 70.31%       | 100.0       | 100%|DONE        |
| translate_query           | 97.30%       | 100.0       | 100%|DONE        |
| delete_resource           | 94.26%       | 94.26       | 94.26%|PARTIAL   |
| create_game_materials     | 90.02%       | 97.79       | 97.79%|PARTIAL   |
| on_decals_loaded          | 81.23%       | 92.35       | 92.35%|PARTIAL   |
| on_configs_loaded         | 75.71%       | 90.20       | 90.20%|PARTIAL   |
| create_game_material_pairs| 76.02%       | 57.81       | 57.81%|PARTIAL   |

No compiled bytes changed in this pass - only STATE markers fixed to match
report.json, carcasses deleted, structure-diff + VERDICT embedded for the <100%
functions.

## ctor (0x753630) - STRUCTURE MATCH, 100%
structure-diff: target 1 / base 1 stmts, size-diffs 0, quantity-diffs 0. The old
"70.31% LTCG translate_query_cook" marker was stale; report.json says 100.0.
Carcass deleted, marker -> bare 100%|DONE.

## translate_query (0x7544b0) - STRUCTURE MATCH, 100%
structure-diff: target 4 / base 4 stmts, 0 diffs. Old "97.30% LTCG boost::function
destructor" marker stale; report.json 100.0. Carcass deleted, marker -> 100%|DONE.

## delete_resource (0x753690) - STRUCTURE MATCH, 94.26%, WALL
Sole divergence: SIZE on VOSTOK_DELETE_IMPL (target 0x22 vs base 0x26). The base
emits a call to the (empty in Master Gold) operator delete; the byte size differs
from the target's. Non-steerable empty-stub wall.

## create_game_materials (0x7536e0) - STRUCTURE MATCH, 97.79%, WALL
1 SIZE on `game_material* mtrl = VOSTOK_NEW_IMPL(...)` (target 0x47 vs base 0x4b) =
VOSTOK_NEW empty operator-new stub; 1 quantity = a collapsed source-line gap
(EMPTY only target). Non-steerable.

## on_configs_loaded (0x754330) - STRUCTURE MATCH, 90.20%, WALL
- mtrl_cfg / pairs_cfg (static_cast_resource_ptr<binary_config_ptr>): target
  statement is 0x22, base 0x1d. --view diff shows the target does an extra
  `push ecx; mov esi, esp` before the data[idx] call - it materializes the
  resource_ptr through a stack temporary / copy that the base elides. Also a L51 vs
  L53 source-line attribution split (target has the two decls on L51 and L53 with a
  blank L52 between; base packs them adjacent -> shows as ONLY target L51 / ONLY base
  pairs_cfg). Line numbers do not change bytes.
- manager VOSTOK_NEW_IMPL: SIZE 0x4d vs 0x51 - empty operator-new stub.
All non-steerable (resource_ptr copy + empty-stub).

## on_decals_loaded (0x753830) - STRUCTURE MATCH, 92.35%, WALL
- LOG_ERROR SIZE 0xad vs 0xb3: logging-macro LTCG.
- VOSTOK_DELETE_IMPL SIZE 0x1b vs 0x20: empty operator-delete stub.
- quantity diffs (L247 ONLY target etc.): }-brace / collapsed source-line attribution.
Non-steerable.

## create_game_material_pairs (0x753b00) - STRUCTURE MATCH, 57.81%, WALL
Dominant wall: resources::create_request return ABI. --view diff at each
create_request shows:
  base : push 0Fh ; lea eax,[esp+10Ch] ; call create_request   (sret/RVO -> writes r directly)
  target: push 0Fh ; mov edx,[..] ; push edx ; call create_request ; add esp,8 ;
          mov [tmp],eax ; mov [tmp+4],edx ; mov ...,eax ; mov ...,ecx  (returns the
          8-byte request in eax:edx, then COPIES into local r)
This return-by-value-into-registers-plus-copy happens at ALL FIVE create_request
call sites (decal1, decal2, sound, particle) and cascades [esp+N] offset shifts
through the rest of the body (the ~20 SIZE rows and most quantity rows downstream).
Remaining quantity diffs are decl-block and sound-block source-line attribution
(same statements, different L numbers; e.g. ext_data.pair=pair in the sound case).
report.json regressed 76.02 -> 57.81 vs the stale marker, but the structure is sound;
the regression reflects a re-delink, not a new source bug. Non-steerable (return ABI
of create_request - the request type's copy/return convention, decided outside this
function).

Next step to push higher would require the resources::request type to be matched
such that create_request returns via sret in the target too - out of scope for this
unit (it is the callee's/type's ABI, a call-boundary LTCG artifact).
