<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Human NPC inline interface and AI collision wrapper

Source-only continuation above `5ad329f37` in PR 571. No build, tool rebuild,
ledger regeneration or new score claim. Target/base comparisons below use the
existing frozen executables, not a binary containing this batch.

This register adds **54 entries**: 30 remaining NPC in-class bodies, four
implicit NPC/attribute operations, and 20 collision-wrapper operations
(17 bodies and three implicit methods). The three NPC transform helpers
already registered in [the world audit](game_world_inline_recovery.md) are
not counted twice. Together these cover all 33 NPC root in-class bodies.

`Verified` means the retained retail operation was inspected, including its
installed virtual slot where applicable. `Model` means a source-prior or
semantic hypothesis with an explicit remaining evidence gap. Replacing a
placeholder with a model does not prove the original named inline boundary.

## Individual register

All addresses are RVAs. Offsets in the first table refer to the complete NPC
unless a base-interface adjustment is explicitly stated.

| # | Function | Evidence and disposition |
|---:|---|---|
| 1 | `human_npc::get_color() const` | Verified `0x9ba60`: copy packed color at `+0x1bc` to the return buffer. Keep value return. |
| 2 | `set_eyes_direction(float3 const&)` | Verified `0x12c50`: `ret 4`; installed NPC slot `+0x20`. Empty override is real, not a missing write. |
| 3 | `get_name() const` | Verified `0x9ba70`: load the fixed-string buffer pointer; AI-game-object `this +0x1bc` is complete-object `+0x1c0`. Existing `fill_stats` also uses the getter. |
| 4 | `get_id() const` | Verified `0x9ba80`: AI-game-object `this +0x21c`, complete-object `+0x220`. |
| 5 | `get_group_id() const` | Verified `0x9ba90`: load `+0x224`; installed primary slot `+0x28`. |
| 6 | `get_class_id() const` | Verified `0x9baa0`: load `+0x228`; installed primary slot `+0x2c`. |
| 7 | `get_outfit_id() const` | Verified `0x9bab0`: load `+0x22c`; installed primary slot `+0x30`. |
| 8 | `is_patrolling() const` | Verified `0x9bac0`: load byte `+0x290`, not an AI-world query. |
| 9 | `is_at_cover() const` | Verified `0xa2740`: false; installed primary slot `+0x38`. Keep this independently of the neighboring nontrivial `is_safe`. |
| 10 | `is_invisible() const` | Verified same folded false leaf, installed primary slot `+0x40`. `brain_unit::is_invisible` actually dispatches through this slot. Remove stale marker. |
| 11 | `get_description() const` | Verified `0x9bad0`: sound-producer `this +0x1e0` is complete-object fixed string `+0x1ec`. |
| 12 | `get_source_position(float3 const&) const` | Verified `0x9bae0`: adjust sound-producer `this` by `-0xc`, then virtual primary slot `+0x8`, forwarding requester/return buffer. Preserve `get_position(requester)`, not private zero-argument `get_position()`. |
| 13 | `cast_npc()` | Verified `0x9bb00`: return adjusted `this -4` from AI-game-object interface. |
| 14 | `cast_npc() const` | Same installed folded body, separate const declaration; remove stale marker. |
| 15 | `cast_weapon()` | Verified `0x327c0`: null; installed AI-game-object slot `+0x10`. |
| 16 | `cast_weapon() const` | Same null leaf at slot `+0x14`; remove stale marker. |
| 17 | `get_velocity() const` | Verified `0x9bb10`: load AI-game-object `this +0x214`, complete-object `+0x218` (`initial_velocity`). |
| 18 | `get_luminosity() const` | Verified `0x9bb20`: corresponding load at complete-object `+0x21c` (`initial_luminosity`). |
| 19 | `cast_game_object() const` | Verified `0x9beb0`: ordinary null-preserving base conversion by `+4`. Existing `return this` supplies it; no hand-written pointer arithmetic/guard needed. |
| 20 | `load(config, path, callback&)` | Verified `0x9bb30`: `ret 0xc`; installed game-object slot `+0x1c`. Does not invoke the callback or load another resource. |
| 21 | `set_filter(begin,end)` | Model restored from legacy direct `m_ai_world.set_ignore_filter(m_brain_unit,begin,end)`. Existing AI-world/brain/pre-perceptor implementations establish the destination pipeline. Original NPC invocation and brain-readiness conditions remain open. |
| 22 | `clear_filter()` | Legacy model forwarding to `clear_ignore_filter(m_brain_unit)`. That existing API forwards an empty range. Original reset invocation remains open; no invented filter traversal. |
| 23 | `add_weapon(object_weapon*)` | Legacy model: `contains_object` guard then `push_back`. Original inventory caller and any surrounding synchronization remain unbound. Do not silently substitute the differently shaped `push_back_unique`. |
| 24 | `remove_weapon(object_weapon*)` | Legacy `weapons.erase(weapon)` model, no delete. Recover original caller/destruction policy; do not conflate unlinking with `pop_weapon`/world cleanup. |
| 25 | `get_dbg_render() const` | Legacy reference getter for renderer `+0x158`; older sound-player drawing mentions it in commented code. Original retail consumer remains open. |
| 26 | `get_dbg_scene() const` | Legacy const-reference getter for scene resource `+0x274`; same consumer gap. Do not introduce a by-value resource copy. |
| 27 | `set_sound_dbg_mode(bool)` | Legacy assignment to `m_dbg_sound` (`+0x2c2`); original control invocation/build guards remain open. |
| 28 | `get_sound_dbg_mode() const` | Legacy flag read, also used by the older sound player. This is source-prior evidence, not proof of retail use. Do not add a new guard around live NPC drawing. |
| 29 | `get_speed() const` | Verified `0x9bb40`: `fld1; ret`, installed hit-receiver slot `+0x18`. Preserve private virtual override and literal `1.0f`; this is not `get_velocity()`. |
| 30 | `return_this()` | Model: return complete-object `this`. Existing affect-subscription constructor now uses the declared private helper in its existing bind. Retail captures the same NPC pointer, but the named helper boundary remains open. |
| 31 | Implicit `human_npc` copy constructor | Raw constructor-list entry `0xa505`, attributes `0x103`: compiler-generated public declaration. Keep implicit; the record alone does not prove copying is valid through noncopyable bases. |
| 32 | Implicit `human_npc::operator=` | Method `0xa55b`, attributes `0x103`; keep implicit, do not invent a memberwise implementation. |
| 33 | Implicit `npc_game_attributes` copy constructor | Method `0xa5f0`, attributes `0x103`; keep implicit. Its intrusive-list member has an explicit empty-list copy contract, not a weapon-cloning loop. |
| 34 | Implicit `npc_game_attributes` destructor | Method `0xa5f4`, attributes `0x103`; ordinary member cleanup. Intrusive list has no element-deleting destructor. Do not insert weapon destruction here. |
| 35 | `ai_collision_object(geometry*,matrix,AI object&,type,data)` | Legacy model allocates composite geometry with game `g_allocator`, binds references, writes inherited user data and calls protected `set_type`. Replaces the dereferenced-null placeholder. Original creation site and inner/outer allocator relationship remain open. |
| 36 | `ai_collision_object(instance*,AI object&,type,data)` | Legacy model binds supplied instance/object and assigns data/type. Recover whether instance ownership transfers; do not manufacture a null fallback. |
| 37 | `render(scene,renderer) const` | Legacy geometry-instance forwarding model; original wrapper/render consumer remains unbound. |
| 38 | `aabb_query(aabb,triangles) const` | Legacy forwarding model passing `this` as collision owner, then bounds/output. Original query consumer remains unbound. |
| 39 | `cuboid_query(cuboid,triangles) const` | Same owner-preserving legacy model for cuboid query; original consumer remains unbound. |
| 40 | `ray_query(origin,direction,max,distance&,triangles,predicate) const` | Legacy forwarding model preserving all outputs/predicate and `this` owner. No added early exit/filter. Original consumer remains unbound. |
| 41 | `aabb_test(aabb) const` | Legacy geometry-instance test model, no owner/output vector argument; original consumer remains unbound. |
| 42 | `cuboid_test(cuboid) const` | Corresponding legacy test model; original consumer remains unbound. |
| 43 | `ray_test(origin,direction,max,distance&) const` | Legacy test forwarding with distance output; original consumer remains unbound. |
| 44 | `add_triangles(triangles) const` | Legacy instance forwarding model; preserve output append behavior, no invented clearing. Original consumer remains unbound. |
| 45 | `update_aabb(matrix)` | Legacy model sets instance matrix then returns instance AABB. Existing octree owns assignment to the object's cached AABB; do not duplicate it here. Original wrapper insertion/move remains unbound. |
| 46 | `get_game_object() const` | Legacy reference getter; the previously restored world frustum callback is a model consumer, not independent proof of retail use. |
| 47 | `get_origin() const` | Legacy cached-object `get_aabb().center()` model, not matrix translation or geometry-instance AABB. Its validity depends on the original tree update path. |
| 48 | `new_ai_geometry_object(...,matrix,geometry*,...)` | Legacy factory model uses supplied allocator for outer wrapper through `VOSTOK_NEW_IMPL`; constructor uses global allocator internally. Original caller/allocator invariant remains open. |
| 49 | `new_ai_geometry_object(...,instance*,...)` | Legacy factory model allocates outer wrapper and passes supplied instance without copying it. Original caller/ownership remains open. |
| 50 | `delete_ai_collision_object(allocator,object)` | Legacy model guards null allocator/object, destroys referenced geometry with supplied allocator, then deletes wrapper. Original ownership/allocator pairing remains open; do not move reference cleanup into implicit destructor. |
| 51 | `get_geom_instance() const` | Legacy const-reference getter, no copy; original consumer remains unbound. |
| 52 | Implicit collision-wrapper copy constructor | Raw method `0xa33b`, attributes `0x103`; leave implicit. Noncopyable base prevents treating declaration as proof of a callable copy implementation. |
| 53 | Implicit collision-wrapper assignment | Method `0xa359`, attributes `0x103`; leave implicit. Reference members/noncopyable base are not a license to write an invented assignment policy. |
| 54 | Implicit collision-wrapper virtual destructor | Method `0xa357`, attributes `0x107`; leave generated. Reference members do not destroy their referents; explicit factory cleanup is a separate modeled operation. |

## PDB structure and installed dispatch

`pdb_topology --classes --class survarium::human_npc --show-identical`
finds target records `0xa55f/0x3fc8f`, base `0x8b3c`: one shared semantic
variant, size `0x2e0`, 123 declarations, but **two versus one complete records**.
This is not whole-PDB equality. `ai_collision_object` is identical in the
frozen pair (one record/variant each, size `0x3c`). Its raw `LF_BCLASS` entries
are public collision object at zero, public AI interface at `+0x30`, and
**private** noncopyable at `+0x34`; the generated header's public spelling of
that last base is the already documented renderer defect.

The NPC's private `human_npc_cook` seam is supported by the older friend
declaration and current cook calls to `set_brain_unit`, `set_model`,
`set_default_animation`, and `set_animation_space_graph`. Keep that friend.
Do not widen private helpers or add new world/debug friends to make models used.

Raw TPI method lists `0xa508` (NPC), `0xa5f2` (attributes), and `0xa340`
(collision wrapper) distinguish generated `0x103` copy declarations from
explicit `0x003` constructors. NPC destructor `0xa509` has `0x007`, so its
existing user-written destructor remains. Attribute assignment `0xa5f3`
has `0x003`, so it is not compiler-generated assignment despite the implicit
copy constructor. No new `VOSTOK_NOVTABLE` annotation is justified here.

Read the actual `C7` vtable-pointer immediates in retained NPC constructor
`0x5b0160` (base `0x5b1ca0`), then the corresponding PE table entries:

| Base interface / offset | Target table RVA | Frozen base table RVA | Relevant slot examples |
|---|---:|---:|---|
| `ai::npc`, `+0` | `0x9632bc` | `0x95635c` | `+0x38` cover, `+0x40` invisible, `+0x18` game-object cast |
| `ai::game_object`, `+4` | `0x96333c` | `0x9563dc` | name/id, both NPC/weapon casts, velocity/luminosity |
| `sound_producer`, `+0xc` | `0x96336c` | `0x95640c` | description and source-position delegation |
| `sound_receiver`, `+0x1c` | `0x96337c` | `0x95641c` | retained sound event |
| `hit_receiver`, `+0x24` | `0x963388` | `0x956428` | `+0x18` constant speed |
| `game_object_`, `+0x30` | `0x9633b0` | `0x956450` | `+0x1c` empty load |

These tables bind folded leaves to actual methods. A zero-return alias named
`cast_weapon` also appears at inherited hit-receiver cast slots; that does
not change those slots into AI weapon casts. Likewise an empty-method alias
does not identify the semantic owner without the interface's slot order.

The 21 retained NPC inline bodies were independently selected in both rich
indexes and their instruction streams agree. In particular,
`get_source_position` has a frozen `STRUCTURE MATCH` of zero statements/
`0x1c` bytes on both sides; the constructor has zero/`0x254` on both sides.
These checks do not verify code generation after the new helper substitution.
Raw line-table entry counts were not used to tune whitespace.

## Consumers, prior source and ownership limits

The older NPC implementation is available at
`git show ce6adb983^:temp/game_legacy/human_npc.cpp`, not the later parked
snapshot's missing `.cpp`. It directly supplies filter set/clear, guarded
weapon insertion, unlink-only removal and rotation replacement. The rotation
body uses a **const** `new_transform` and composes scale × new rotation ×
translation, then calls `set_transform`; that const is restored in the
previously counted helper. Its original retail consumer is still unbound.
The older header is at `885e1d4a4:temp/game_legacy/human_npc.h`.

The filter alias is important: `ai::ignorable_game_object` uses
`loose<game_object const>::ptr`, which is a raw pointer with debug pointers
disabled. Thus it reproduces retail's pair type in shipping while staying
compatible with the existing AI-world API in debug configurations. No new
conversion adapter or change to AI's interface is needed. The retained
`ai_world::set_ignore_filter` forwards into the brain's filter; the existing
pre-perceptor implementation clears its old vector and evaluates incoming
object/filter pairs. That does not prove the original NPC wrapper's guards.

`npc_game_attributes::operator=` (`0x5aef40`, frozen base `0x5b0d10`)
copies the scalar/vector/string attributes and calls intrusive-list `swap`
at target `+0xf8`. Current source already has this operation; it is preserved.
`get_available_weapons` (`0x5ae670`) traverses list links and returns AI weapon
pointers, while `pop_weapon` (`0x5aef30`) delegates to list removal. Neither
establishes the missing add/remove helper's original call boundary. The list
copy constructor initializes an empty list and asserts the source is empty;
its implicit destructor does not delete pointed-to weapons. Existing world
cleanup separately pops and deletes weapons. NPC's explicit destructor deletes
the animation selector; `clear_resources` owns the explicit unsubscription/
world-removal operations. Do not move those into generated member cleanup.

`ce6adb983^:temp/game_legacy/ai_sound_player.cpp` reads the NPC debug flag;
its renderer/scene getter uses are commented. Current direct drawing/member
accesses are not rewritten merely to make debug helpers reachable. The private
`return_this` helper is used only at the already-existing subscription bind;
retail constructor pointer capture corroborates its value, not its spelling.

All 17 collision-wrapper bodies come from
`885e1d4a4:temp/game_legacy/ai_collision_object.cpp` and its header. Their types/access and
the current collision API allow these operations, but neither retained rich
procedures nor an independent current creation path were found for this
wrapper. The world frustum callback alone is not such proof. This is **not**
an editor-only/server-only classification and does not close retail use.

Two ownership details remain deliberately visible: geometry construction uses
global game allocator whereas the outer factory takes an allocator; deletion
uses the supplied allocator for both objects. Also the instance-taking overload
binds a reference that the explicit deletion helper subsequently destroys.
Do not repair either policy without recovering the original callers.

The octree's existing insert/move paths assign the result of `update_aabb` to
the object's cached AABB (`loose_oct_tree.cpp`, including the insertion at
line 312). The wrapper returns the geometry extent; its origin getter reads
the cached object's center. Writing that cache inside the wrapper, or returning
matrix translation, would silently change the recovered legacy contract.

Static review checked complete AI-world/geometry-instance declarations,
allocator declaration, existing query/factory argument types, protected base
access, and generated-method ownership. No compilation was run. Thirteen new
source questions are paired in `review_todos.md`; the earlier rotation question
is strengthened, not removed. Thirty literal markers are removed (13 NPC,
17 wrapper), but unresolved models remain explicit in this register.
