# Report-unpaired function with a PLAIN (demangled-style) target name = a file STATIC
tags: cpp:static | topic:mangling topic:anchoring
symptoms: plain name in report.json (no ?@@ mangle), fuzzy None, survarium::foo listed undecorated
confidence: 9/10
variants: mangled-join-failure.md, dced-static-helper-anchor.md

The target PDB records STATICS under plain names; defining the function extern gives it a
mangled symbol objdiff never pairs with the plain name. Restore `static` (internal
linkage). The anchor must then live IN-TU - park the volatile-fn-pointer block inside a
same-TU stub the target also has (an extern temp_include_all declaration no longer links).

```cpp
static float distance_from_box_center_to_point_on_shape( ... );   // internal linkage pairs
```
Evidence: damage_zone_core shape helpers (sphere/box/capsule/cylinder) None -> 100/86.5/82.5/76.6, +8 report improvements, 0 regressions; call_item_serialize precedent. Same shape pending on booby_trap_set_core trap_is_active / find_free_trap_predicate / create_place_matrix_for_looking_point.
