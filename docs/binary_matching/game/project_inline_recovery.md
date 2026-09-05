<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Project-loading inline recovery

Source-only continuation above `544425bdc` in PR 571. **34 additional entries**
cover project and nested-record inlines, generated methods, retained ownership
operations and six consumer anchors. No build or new match scores. Target RVAs
refer to existing retail artifacts; base observations use the frozen candidate.

The substantive recovery is `project_name()` and its minimap consumer: the
retail caller performs the lookup through one acquired project handle, whereas
our expanded ternary acquired a second handle in the successful branch. The
declared helper was still returning null. Restore its observed lookup semantics
and use the declared seam; its original named boundary remains an inference.

## Individual register

| # | Function | Evidence and disposition |
|---:|---|---|
| 1 | `simple_game_project::is_inserted()` | Verified in world unload `0x5d0b30 +0x201`: project `+0x1b4` gates removal. Preserve field getter and current consumer; remove stale marker. |
| 2 | `project_name()` | Restore `value_exists("project_name") ? root["project_name"] : ""` from minimap expansion `0x5c5250 +8`. Null was not the observed fallback. Original helper boundary remains indexed. |
| 3 | `loading::loading()` | Verified project ctor `0x5bc560 +0xed`: zero count at `+0x1b8`, then all-queried/visuals/collision bytes `+0x1bc/+0x1bd/+0x1be`. Keep existing initializer list; no synthetic zeroed padding. |
| 4 | Implicit project copy constructor | Raw `0x988c`, attributes `0x103`, constructor list `0x988e`. Keep generated; noncopyable inherited state does not become a custom copy. |
| 5 | Implicit project assignment | Raw `0x989c`, attributes `0x103`. Keep implicit; do not rebind scene reference or invent pointer ownership. |
| 6 | Explicit project constructor | Verified `0x5bc560`: resource base argument 1, base-project construction, borrowed scene reference, null config/visual array, empty containers, false inserted and loading state. Retail does not initialize render-visual count here; its cook supplies it. |
| 7 | Explicit virtual project destructor | Verified `0x5bc3c0`, raw method `0x988f/0x007`: delete visual array, then generated member/base cleanup. Do not add deletion of every raw pointer in its vectors; cook cleanup owns those objects separately. |
| 8 | `all_loaded()` | Verified retained `0x5bc300`: visuals, collision, loaded-count equals object-vector size, all-queried, in that order. Restore five equivalent cook predicates through this existing helper; original call/inline boundaries remain open. |
| 9 | `insert(scheduler&)` | Retained `0x5bc6a0`; existing visual/static-collision/object/zone/anomaly/container/ladder operations and final inserted flag are preserved. This review does not claim every loop's byte closure. |
| 10 | `remove()` | Retained `0x5bc870`; existing removal/deactivation ordering and final false inserted flag remain unchanged. No destructor substitute or implicit object deletion introduced. |
| 11 | `get_items_container(u8)` | Verified `0x5bc340`: linear pointer-vector search, byte ID at object `+0x34`, first match or null. Keep byte parameter and null result for absent ID. |
| 12 | `on_npc_loaded(queries_result&)` | Verified `0x5bc370`: acquire/cast/release sound-emitter pointer. No NPC insertion or loaded-count update is performed merely because of the callback's name. |
| 13 | `base_project::register_named_object` | Verified `0x84960`: map at `+4`, subscript by name, assign object pointer. Frozen base has the same instruction shape and zero line records; keep existing inline definition. |
| 14 | `register_object_to_resolve` | Verified `0x84980`: copy by-value 24-byte config and object pointer into record, append to vector `+0x1c`, growing if needed. Frozen instructions agree; retail zero versus base three body line records remains an indexed partition question. |
| 15 | `resolve_link_object(link_resolver*,config)` | Explicit raw `0x9544/0x003`; constructor operations are visible in registration above: config at zero, borrowed resolver at `+0x18`. Keep by-value config and existing initializer order. |
| 16 | Implicit resolve-record assignment | Raw `0x9549/0x103`; keep generated. Four-byte tail padding in the `0x20`-byte vector stride is not another source field. |
| 17 | Implicit base-project copy constructor | Raw `0x94f5/0x103`; keep implicit through private noncopyable base. No new copying API. |
| 18 | Implicit base-project assignment | Raw `0x9508/0x103`; preserve generated declaration, not a manual map/vector/array clone. |
| 19 | Explicit base-project constructor | Verified `0x6ef360`: empty registry/vector, static-collision pointer and count zero, real vptr store. No novtable added. |
| 20 | Explicit virtual base-project destructor | Verified `0x6ef420`, raw `0x94f8/0x013`: derived vptr, static-collision array deletion, vector/map cleanup. Introducing-virtual attribute is not the compiler-generated bit. |
| 21 | `get_object_by_name(pcstr)` | Verified `0x6ef3e0`: surviving assertion scaffold then map subscript/load. Existing `UNKNOWN_EXPRESSION_T(find != end)` predicate remains a model; original condition is explicitly indexed, not recovered from elided code. |
| 22 | `resolve_links()` | Verified `0x6ef2c0`: stride-`0x20` walk, pass each config by value and resolver pointer, then clear named-object registry. Does not clear the resolver vector here; do not add it. |
| 23 | Implicit render-visual default constructor | Raw `0x9d2f/0x103`; real array helper initializes only model `+0x40` to null at stride `0x44`. Matrix is later written by `read_transform`; no invented identity matrix. |
| 24 | Implicit render-visual copy constructor | Raw `0x9d2e/0x103`; keep generated matrix/resource-pointer copy. |
| 25 | Implicit render-visual assignment | Raw `0x9d33/0x103`; preserve resource handle semantics instead of raw-copy replacement. |
| 26 | Implicit render-visual destructor | Raw `0x9d31/0x103`; array deletion supplies element destruction. No custom scene removal, delete of model pointer, or virtual destructor introduced. |
| 27 | `render_visual::insert` | Retained leaf checks model `+0x40`, then supplies model's render resource `+0x108`, matrix and scene handle to scene-renderer add. Existing implementation retained. |
| 28 | `render_visual::remove` | Retained leaf checks model `+0x40`, then calls renderer removal using same model/scene ownership. Existing implementation retained. |
| 29 | `game_world_ui::initialize_minimap` | Restore `get_project()->project_name()`. Retail lookup `+8..+0x7e` acquires/releases one project resource; frozen expanded ternary performs two successful-branch acquisitions and two releases. The scene keeps the project alive after the temporary expires. |
| 30 | `project_cooker_simple::on_object_loaded` | `0x5c7e30 +0xe`: exact four-condition completion predicate after count increment. Use `all_loaded`; preserve link resolution, resource registration and finish order. |
| 31 | `create_game_objects` completion guard | `0x5c7ed0 +0x17a3`: same four-condition predicate after query scheduling and its existing flag writes. Use `all_loaded`; do not repair or reorder asynchronous flag policy in this batch. |
| 32 | `on_damage_zones_loaded` completion guard | `0x5c7c00 +0x1ab`: set all-queried true, then test the other three terms. Use full helper; the known-true term accounts for its absence from optimized retail. |
| 33 | `on_ladders_loaded` completion guard | `0x5c79d0 +0x1ab`: same flag-setting/three-term pattern as damage zones. Use full helper, preserving actual callback and resource operations. |
| 34 | `on_collision_and_visuals_loaded` completion guard | `0x5c7730 +0x21a`: set collision/visuals true, then compare count and all-queried. Use full helper; first two terms are already true. No extra scene insertion or ownership transfer. |

## Visibility, generated records and source priors

Frozen target/base topology agrees for project (`0x1c0`) and base project
(`0x30`), one semantic variant and complete record each. Raw target records:

| Type | Class TI / fields | Important evidence |
|---|---|---|
| `simple_game_project` | `0x989f / 0x989e` | Public resource/base-project bases; every stored field, including config and loading state, public (`LF_MEMBER` access 3). |
| `loading` | `0x9d3a / 0x9d39` | Explicit ctor `0x9d38/0x003`; all four fields public. No generated-copy record is fabricated from a constructor's absence. |
| `base_project` | `0x950b / 0x950a` | Raw noncopyable-base access is private (1), despite generated header saying public; all four stored fields are public (3). |
| `resolve_link_object` | `0x954b / 0x954a` | Explicit two-argument constructor, generated assignment, public config/resolver fields. |
| `render_visual` | `0x9d36 / 0x9d35` | Default/copy ctor list `0x9d30` and destructor/assignment all carry generated bit `0x100`. No hand-written empty constructor/destructor needed. |

Remove five reconstructed project friendships and three base-project friendships:
they were introduced to justify access to fields that retail already declares
public. Keep access and field/declaration order; absence of friendship records
alone would not justify removing a necessary friend. Here no private access
requires them. The actual private noncopyable inheritance stays unchanged.

Older tracked `temp/game_legacy/project_cooker_simple.cpp` is a different
Lua/editor conversion pipeline, not evidence for transplanting the current
binary-project lookup or its callback bodies. Current `project_name` source
search/clangd references found only its declaration before recovery; that did
not prove it unused, as the retained minimap expansion demonstrates.

## One resource lifetime, and the completion predicate

Retail minimap code loads world project `+0x22c`, increments its resource count,
tests the config key, fetches it or selects the empty literal, then decrements
that same resource once. Frozen base reacquires project in the successful
branch and must conditionally release both temporaries. Moving the config
expression into the already-declared `project_name` helper restores one
`get_project` full-expression temporary. The getter returns borrowed config
string storage, not an allocated string or a temporary local buffer.

There is still no standalone retail `project_name` procedure proving its named
inline boundary. Record that inference without discarding the strong ownership
evidence. The later build should inspect both refcount operations and PDB
partition, not only fuzzy score.

All five completion sites have the same logical contract as retained
`all_loaded`. Two carry all four checks; two just stored all-queried true; one
just stored both visuals and collision true. Those optimized omissions are
consistent with the full helper, not evidence for five separate source
implementations. Restore the declared seam without `noinline`, forced emission
or changed callbacks. Original named boundaries and the post-change compiler
projection remain open until the deferred comparison.

`register_object_to_resolve` is a separate structural caveat: independently
fetched instruction streams agree, size `0x6c`, but retail has zero body line
records and frozen base has three. Do not claim structural closure or alter
the semantics to erase records. Its by-value config and nested-record copy
operations are verified. `get_object_by_name` likewise retains its unresolved
assertion expression rather than promoting a plausible `find != end` to fact.

Static review checked complete binary-config includes, unchanged access/order,
five helper consumers, ownership, four synchronized TODOs and diff integrity.
Two literal markers are removed. No build, tool rebuild, ledger or README score
refresh was run; generated-method and callback verification awaits the eventual
batched build.
