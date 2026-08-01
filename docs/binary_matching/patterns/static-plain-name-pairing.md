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
`namespace::foo` text symbol while every sibling is `?foo@@...` mangled. Equivalently, read
the `mangled` field of both rich indexes side by side:

```
# target index                       # base index
vostok::render::fill_light           ?fill_light@render@vostok@@YAXAAVlight@12@PAUlight_props@12@@Z
```

The two internal-linkage spellings produce DIFFERENT names, so picking the wrong one still
fails to pair: `static void f( )` -> `vostok::render::f`, while `namespace { void f( ); }` ->
`` vostok::render::`anonymous namespace'::f ``. Read the target's spelling off the name (that
is why `custom_config.cpp`'s anonymous-namespace `sort_by_crc` / `convert_type` /
`copy_destroyer` family still reads unpaired against a `static` target).

**Sweep form that actually finds the rows (batch A10, 12 free pairings in `render/engine`).**
Do not stop at "the same short name appears undecorated on one side and decorated on the
other in the same unit" - that INVERTED form is rare (A9 found the one instance in all of
`render`). The productive form is one-sided: *target undecorated, base decorated, same
`file:`, same short name.* Join the two rich indexes on `(file, demangled function
identifier)`:

```python
def undec(o): m = o.get('mangled',''); return not m.startswith('?') and '`' not in m
def ident(name): return name.split('(')[0].split('::')[-1].strip().split(' ')[-1]
# for every target fn with undec() in scope, find base fns in the SAME file whose
# ident() matches and which are NOT undec() -> add `static` to that definition.
```

Two filters keep the output honest: drop `` `dynamic initializer for ... ` `` /
`` `dynamic atexit destructor ...` `` rows (always undecorated, never a linkage bug), and
match on the demangled **identifier**, not a substring - `sh in mangled` reports every
`register_cooks` in the image.

A10 yield on `render/engine` (all were `unpaired` before, so the sweep is pure upside):
`clip_2_screen` / `unregister_cooks` / `get_material_effects_instance_request_path` /
`vertex_input_type_to_speedtree_component_type` **100%**, `mesh_type_to_vertex_input_type`
99.9, `does_os_support_dx11` 98.1, `on_speedtree_material_effects_instance_ready` 95.1,
`get_dx_version_via_dxdiag` 94.9, `on_model_material_effects_instance_ready` 90.3,
`get_format_block_size` 90.1, `fix_view_matrix` 62.8, `register_cooks` 38.0.

**Negative result worth keeping:** a target static with NO base symbol *anywhere* is not a
linkage bug - our body is a stub small enough to inline at every call site, so nothing is
emitted to mis-link (twelve such names in `render/core`, and `decal_instance`'s
`get_world_to_decal_matrix`, `grass_patch`'s `transform_packed_normal`, ... in
`render/engine`). And adding `static` can *cost* an emission: with a single call site
MSVC then inlines the helper away entirely (A10: `calc_pattern`, `compute_gaussian_value`,
`get_gaussain_weights_offsets` went decorated-unpaired -> inlined-away). Keep the `static`
anyway - it is what the target has - but do not count those as wins.

Sweep a whole module for the class of them in one shot - any row printed for ONE side only is
a linkage mismatch, not a matching gap:

```sh
python3 - <<'PY'
import json
for side in ('target','base'):
    for line in open(f'binaries/rich/{side}/index.jsonl'):
        d = json.loads(line)
        m = d.get('mangled','')
        if not m.startswith(('?','_')) and 'dynamic' not in m:
            print(side, d.get('file'), m)
PY
```

When the TU has NO organic in-TU caller yet (the real one is a still-STUB sibling), give the
static a same-TU keep-alive: a `pin_*_statics()` fn that address-takes it behind a never-true
`if(s_run)` volatile guard, and call that pin fn from the module's /OPT:REF anchor (the
address-take stays in-TU; only the keep-alive call crosses the TU boundary, which is fine).

Evidence: damage_zone_core shape helpers (sphere/box/capsule/cylinder) None -> 100/86.5/82.5/76.6, +8 report improvements, 0 regressions; call_item_serialize precedent. booby_trap_set_core: find_free_trap_predicate / create_place_matrix_for_looking_point paired (100 / 89.8) once made static + organically called in-TU (try_place_trap / get_visible_place_transform). trap_is_active stayed unpaired: its only caller (header-inline count_active_traps) is LTCG-inlined into a root in ANOTHER TU, so this TU has no in-TU anchor and /OPT:REF drops it (anchoring from temp_include_all would emit it in the wrong obj). lobby_menu_scene::identity_transform_functor (a `float4x4().identity()` scene functor, originally called by the still-STUB query_scene_resources): None -> 100 via `static` + an in-TU `pin_lobby_menu_scene_statics()` called from anchor_game_lobby_scene's use_*(). render::fill_light (lights_db.cpp, batch A4): unpaired -> 73.9% from adding `static` alone, no body change - 1644 target bytes that had been scoring zero.
