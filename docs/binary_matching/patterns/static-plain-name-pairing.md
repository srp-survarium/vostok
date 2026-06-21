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
Confirm it cheaply: `llvm-nm` the delinked target `.obj` - the static prints as the bare
`namespace::foo` text symbol while every sibling is `?foo@@...` mangled.

When the TU has NO organic in-TU caller yet (the real one is a still-STUB sibling), give the
static a same-TU keep-alive: a `pin_*_statics()` fn that address-takes it behind a never-true
`if(s_run)` volatile guard, and call that pin fn from the module's /OPT:REF anchor (the
address-take stays in-TU; only the keep-alive call crosses the TU boundary, which is fine).

Evidence: damage_zone_core shape helpers (sphere/box/capsule/cylinder) None -> 100/86.5/82.5/76.6, +8 report improvements, 0 regressions; call_item_serialize precedent. booby_trap_set_core: find_free_trap_predicate / create_place_matrix_for_looking_point paired (100 / 89.8) once made static + organically called in-TU (try_place_trap / get_visible_place_transform). trap_is_active stayed unpaired: its only caller (header-inline count_active_traps) is LTCG-inlined into a root in ANOTHER TU, so this TU has no in-TU anchor and /OPT:REF drops it (anchoring from temp_include_all would emit it in the wrong obj). lobby_menu_scene::identity_transform_functor (a `float4x4().identity()` scene functor, originally called by the still-STUB query_scene_resources): None -> 100 via `static` + an in-TU `pin_lobby_menu_scene_statics()` called from anchor_game_lobby_scene's use_*().
