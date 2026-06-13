# A header's FILE PATH (incl. sources/ subdir) decides the objdiff unit - a wrong path leaves it TARGET_ONLY
tags: cpp:inline cpp:member | topic:anchoring topic:tooling topic:scoring-artifact
symptoms: function unpaired/TARGET_ONLY despite correct bytes, base emits it in a DIFFERENT objdiff unit, rich-index path differs base vs target, report.json shows the symbol only under the target's unit
confidence: 9/10
variants: address-taken-anchor-emits-header-comdat.md, mangled-join-failure.md

objdiff units are the TARGET's delinked `.obj`s, named by the PDB source path of each
COMDAT (engine-path prefix stripped). The delinker groups a COMDAT into the `.obj` of the
file its body's debug-lines point to - i.e. the file the inline body is DEFINED in. If our
repo keeps that header at a DIFFERENT path than the original, the base emits the COMDAT in
the wrong pseudo-unit and it can never pair - it shows TARGET_ONLY in the target's unit and
"unpaired" (target-only) in report.json, while the base copy sits invisibly in a unit that
isn't even in objdiff.json (no matching target obj).

Tell it apart from a real miss:
```
# base vs target source attribution disagree:
pdb_rich_query --index .../base/index.jsonl   --function <name> --list   # vostok/game_core/foo.h
pdb_rich_query --index .../target/index.jsonl --function <name> --list   # vostok/game_core/sources/foo.h   <-- sources/ !
# confirm the target's unit obj has it and the base's does NOT:
objdiff-cli diff -p binaries/objdiff -u "<unit>" --format json -o - <sym>   # left(target) has it, right(base) empty
```
Two flavours seen in game_core:
- **wrong directory**: header physically at `sources/vostok/game_core/foo.h` but the target
  PDB records `vostok/game_core/sources/foo.h`. Fix: `git mv` the header into the `sources/`
  subdir, update the `#include` (relative `"foo.h"` from a same-dir `.cpp`) and the
  `.vcproj` RelativePath. weapon_core_inactive_state.h: on_specific_event 0->82%,
  weapon_and_hands_expression 0->64% just from the move.
- **wrong file**: an inline ctor/method defined in its own split header (`usable_object_user_data.h`)
  that the original kept inside a USING header (`collision_user.h`). Fix: declare-only in the
  split header, define the inline body in the using header; make the anchor `#include` the
  using header so the construction site sees the definition. usable_object_user_data ctor
  0->100%. (The target had NO separate structure file for the split header - a tell.)

The byte structure is already correct in both flavours; this only moves WHERE the COMDAT
lands so objdiff pairs it. Always rebuild a clean baseline before/after - the relink also
flips unrelated ICF fold-rep names (zero-sum 100<->0 churn on Scaleform/boost dtors), not real
regressions.
Evidence: game_core header_anchors batch, 2026-06-13.
