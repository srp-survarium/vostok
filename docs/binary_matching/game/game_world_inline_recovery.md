<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# World inline interface, tracer ownership and NPC predicates

Source-only continuation above `c953bc461` in PR 571. No build, tool rebuild,
ledger regeneration or new byte-score claim. Evidence comes from retail and
the existing frozen base, not a binary containing these edits.

The register adds **50 entries**: 26 world in-class bodies, four nested tracer
operations, five operations of its existing render-owned resource, ten local
NPC predicate operations, three human-NPC transform callees and two visibility
parameter methods. The world portal forwarder was already counted in
[the game register](game_inline_recovery.md), and is not counted again.
The rest of `human_npc` and `ai_collision_object` still need their own audits.

`verified operation` means a retained procedure or consumer establishes the
operation, not necessarily the original inlined helper's spelling. `model`
means a documented reconstruction hypothesis; `open` retains an unresolved
body. An absent standalone procedure does not establish absence of use.

## Individual register

| # | Function | Result and consumer evidence / remaining question |
|---:|---|---|
| 1 | `game_world::input_priority()` | Verified retained RVA `0x868f0`: return 10. Existing unmarked body stays. |
| 2 | `get_ai_world() const` | Verified operation: `human_npc` constructor `0x5b0160 +0xd2` loads world `+0x248`, stores NPC `+0x144`; keep dereferenced pointer return. |
| 3 | `get_ai_navigation_world() const` | Verified operation: `human_npc::enable +0x114` loads world `+0x24c` for the animation graph. |
| 4 | `is_loading() const` | Verified operation: `game::is_loading` reads embedded world flag at game `+0x35a` = world `+0x98 +0x2c2`. Lobby UI and load waiting also consume it; preserve world load/reset writers. |
| 5 | `is_loading_or_unloading() const` | Existing loading-flag model retained, now explicitly marked. No separate unloading flag in this layout; that does not prove that resource/scene state cannot contribute. Find its consumer before extending the expression. |
| 6 | `selected_npc() const` | Const-reference getter model for resource pointer `+0x2b8`. Stats and behaviour callback read that selection, but do not identify a call to this getter. No by-value copy is introduced. |
| 7 | `get_current_input_mode() const` | Verified operation: `step_manager::on_step +0xb0` reads world `+0x2bc` in its local-player branch. |
| 8 | `get_free_fly_camera() const` | Verified operation: warmup camera consumer reads game `+0x2cc` = embedded world `+0x98 +0x234`, then calls `game_camera::set_position_direction`. |
| 9 | `get_project() const` | Verified **const by-value** resource return. `process_victory_item_take_or_put`, RVA `0x5b53d0 +0x6f..0xd5`, copies game `+0x2c4` = world `+0x22c`, increments before container lookup and decrements afterwards. Do not change to a reference to avoid copies. |
| 10 | `get_bullet_manager() const` | Retained RVA `0x7f770` returns `[ecx+0x230]`. This is the inherited `survarium::engine` interface at world `+0xc`; adjusted address is the real `m_bullet_manager +0x23c`, not the portal pointer. Existing body stays. |
| 11 | `get_game_material_manager() const` | Verified operation in `booby_trap_set::get_game_material_manager` and `step_manager::on_step +0xea`: world `+0x244`. Keep borrowed const reference. |
| 12 | `get_step_manager() const` | Verified operation: `weapon::on_foot_step +0x40` loads world `+0x240` before `on_step`. |
| 13 | `check_selected_npc()` | Restore legacy toggle/select/clear model. If the picker returns null, clear active-selection flag again. Original invocation and physics picker remain open; this is not a working selection pipeline yet. |
| 14 | `rotate_selected_npc(float)` | Legacy degree-to-radian yaw, current rotation multiplied by new yaw, public NPC setter. Degree convention, composition and original retail consumer remain model questions. |
| 15 | `move_selected_npc(float3 const&)` | Relative-translation model: copy public transform, add offset, delegate to `set_transform`. Absolute-position versus delta argument is unresolved. |
| 16 | `delete_selected_npc()` | Adapt legacy erase/kill/clear-selection/clear-flag sequence. Do not add legacy `m_npc_queries_count`: retail has no such member. Notification/order/resource ownership at the original invocation remain open. |
| 17 | `assign_behaviour()` | Guarded legacy `"test"` behaviour-resource query model with existing `behaviour_cook_params` and `on_behaviour_created` bind. Path, parameters and original invocation unverified; no new console/key binding. |
| 18 | `hit_player(hit_info const&)` | **Open.** Candidate contracts are target-player lookup then local `on_hit` application, or delegation to the network authority. Packet hit processing already has a separate retained path; do not guess which owns this hook or apply damage twice. |
| 19 | `take_victory_item(u8)` | Indexed `m_victory_items[id]->take()` model. Existing packet code has an insertion guard, resource copy and container operation; none are replaced. Original helper guard/caller still needed. |
| 20 | `get_victory_items()` | Verified mutable vector reference: `process_victory_item_take_or_put +0x48` uses game `+0x35c` = world `+0x2c4`, then indexes four-byte owning resource pointers. |
| 21 | `initialize_physics()` | Delegate to existing `base_game_scene::init_physics`; world constructor now uses this declared wrapper. Retail ctor `0x5d0350 +0x318` allocates/constructs the world and invokes its initialization. The original named wrapper boundary awaits deferred comparison. |
| 22 | `get_frustum_objects_callback(...)` | Legacy checked collision-to-`ai_collision_object` cast, then invoke callback with its public `get_game_object()`. Query binding and concrete payload provenance remain open. |
| 23 | `find_npc_in_camera_direction() const` | **Open.** Old picker uses collision triangles; retail predicate signature uses physics results. Need hit-to-NPC identity, nearest/first and occlusion ordering, masks and ray length. No unsupported old 50000-unit ray or collision mask is installed. |
| 24 | `on_npc_dictionary_created(...)` | Restore legacy success guard/assert, binary config resource cast, `fill_objects_dictionary`, then flag true. Retail `query_npc_dictionary` is a bare return, so this model is not wired there. Recover original query path/bind separately. |
| 25 | `set_navmesh_info(pcstr) const` | Retained RVA `0x12c50`: `ret 4`. Keep empty body; older source's stats forwarding is not evidence for adding it to the shipped client. |
| 26 | `draw_respawn_debug()` | **Open.** Enemy/spawn lists exist, but primitive selection, colors, conditions and original draw invocation are unknown. Do not substitute a generic renderer loop. |
| 27 | `bullet_tracer(bullet*, tracer_ptr)` | Existing initializer list verified by world project-load insertion and resource-pointer increments. Parameter is by value; raw bullet pointer is borrowed, tracer pointer owns a reference. Remove stale annotation. |
| 28 | `bullet_tracer(bullet_tracer const&)` | Preserve implicit generation: raw method attributes `0x103`. Retail `__ucopy` `0x6e180` copies the raw bullet and increments the resource at member `+4`. |
| 29 | `bullet_tracer::~bullet_tracer()` | Preserve implicit generation, attributes `0x103`; retail reverse destruction `0x6df50` decrements tracer at element `+4`, conditionally destroys its resource, never deletes the raw bullet. |
| 30 | `bullet_tracer::operator=` | Preserve implicit memberwise assignment, raw attributes `0x103`. No explicit method or memcpy substitute. Exact instantiated assignment consumer not identified in this pass. |
| 31 | `render::tracer_model_instance::set_color` | Use existing render-owned body, not game's duplicate empty body. Retail cook `on_model_ready`, `0x75c070 +0xf9`, stores color at `+0x14c`. |
| 32 | `tracer_model_instance()` | Retail marks it generated; existing render declaration does too. Cook constructs unmanaged base, null model and default color `0xffffffff`. Matrix identity belongs to the later cook assignment, not default construction. |
| 33 | `tracer_model_instance(tracer_model_instance const&)` | Keep generated method, raw attributes `0x103`; shared existing owner supplies normal base/member semantics. No new explicit copy body or invented consumer. |
| 34 | `tracer_model_instance::operator=` | Keep generated method, attributes `0x103`, existing owner; preserve member resource ownership. Not claimed as a separately observed copy path. |
| 35 | `tracer_model_instance::~tracer_model_instance()` | Retail generated virtual method attributes `0x107`, versus duplicate game's explicit `0x007`. Existing owner supplies automatic resource/base destruction; this is not removal of virtual destruction. |
| 36 | `find_closest_collision_predicate()` | Existing null-result / unit-fraction initializer model explicitly recorded. PDB fields alone do not establish initial values; find original traversal. |
| 37 | `find_closest_collision_predicate::operator()` | Previously declaration-only. Model strict smaller `fraction`, storing result address and new fraction. Tie policy, endpoint at fraction 1, temporary-result lifetime and caller remain open. |
| 38 | `test_objects_in_shape_predicate()` | Existing false-until-contact initializer model. No current construction site found; original query binding still required. |
| 39 | `test_objects_in_shape_predicate::add_single_result` | Previously declaration-only override. Model sets `m_in_shape`, returns zero. Existing physics callback forwards user data, shape types/transforms/dimensions; whether original applies additional geometry filtering or a different return contract is open. |
| 40 | `test_objects_in_shape_predicate` copy constructor | Raw overloaded method record identifies generated copy, attributes `0x103`, next to explicit default ctor `0x003`. Leave implicit; no handwritten vptr copying. |
| 41 | `test_objects_in_shape_predicate::operator=` | Raw field list records generated assignment `0x103`. Keep implicit base/member operations; not a separately verified active copy site. |
| 42 | `get_first_npc_in_camera_direction_predicate()` | Restore legacy null `first_npc` model. Current physics-based producer is unresolved. |
| 43 | Its `operator()(physics::closest_ray_result const&)` | **Open.** The physics result owns `base_physics_object*`, whose game-object interface does not expose an NPC cast. Old `ai_collision_object` triangle cast is not type-compatible evidence. |
| 44 | `ray_query_predicate(...)` | Restore legacy `requested_object_was_found(false)` alongside existing reference/pointer/threshold initializers. Original collision-query producer is open. |
| 45 | `ray_query_predicate::predicate(...)` | Restore legacy material-payload check, ignored-object check, transparency lookup, requested-object flag, subtractive attenuation and stopping tests. Payload type/binding remains a model; do not turn it into multiplicative attenuation or wire it into retail's unrelated physics-result query. |
| 46 | `human_npc::set_rotation(float4x4 const&)` | Scale × supplied rotation × current translation model, delegated to existing `set_transform`, mirroring retained `set_translation`. Original composition/source boundary remains open. |
| 47 | `human_npc::get_rotation_angles() const` | Legacy `m_transform.get_angles_xyz()` model. Retained translation setter also extracts those angles; original named getter consumer remains unproven. |
| 48 | `human_npc::get_transform()` | Existing const-reference return model for matrix `+0x230`. Method itself is non-const in the PDB; do not change it. Added model consumer is not proof of original invocation. |
| 49 | `game_material_visibility_parameters(float)` | Constructor operation corroborated at `human_npc` constructor `+0x126`: zero float stored at NPC `+0x160`, this value member. Preserve argument-copy form; do not hardcode zero for other callers. |
| 50 | `get_transparency_value() const` | Existing single-field getter has exact older source support; original retail material-query consumer unbound. Mark as model, not an independently recovered expansion. |

## Declaration and ownership evidence

`pdb_topology --classes --class survarium::game_world --show-identical` finds
one semantic shape, size `0x2d8`, 119 declarations: target equal records
`0xa5cc/0xf4f5`, base `0x7776`. This is **record multiplicity**, not whole-PDB
equality. The nested tracer, four predicates and visibility-parameter class
have identical target/base semantic sets. Raw base access remains the
[separately recorded renderer issue](../pdb_base_access_rendering.md);
source private AI/navigation/bullet-engine inheritance is not changed.

For `vostok::render::tracer_model_instance`, target record `0x7cf2` has one
shape; base `0x1191b` matches it and `0x9223` is an extra shape. Reading raw
field/method lists establishes the distinction, not a guess from the dump:

- Retail constructor list `0x7cec`: generated copy and default, both `0x103`.
- Retail destructor `0x7ced`: generated virtual `0x107`.
- Extra base game's explicit default/destructor use `0x003/0x007`; its
  declaration ordering also differs.
- Existing render header has the retail shape. Game now includes that owner;
  no render implementation was edited and no class method was fabricated.

Nested `bullet_tracer` target record `0xb07c`, field list `0xb07b`, has
constructor list `0xb076`: generated copy `0xb073/0x103`, explicit two-arg
constructor `0xb075/0x003`; destructor and assignment are generated.
The eight-byte vector element is not a trivially copyable allocation: retained
copy/destruction loops perform reference-count operations. Compiler-generated
closure/deleting helpers are not additional handwritten functions.

## Public access and real callers

Game's duplicate friends `network_client`, `base_network_client` and `player`
were reconstruction additions. The network callers use `game_ui`, which is
already **public** at `+0x1d4`. Player's detachment used private
`switch_to_free_fly_camera`; it now calls public
`switch_camera_mode(free_fly_mode)`, as other player camera transitions do.

Retail detachment `0x5d3220` and frozen base independently have four statements,
`0x6f` bytes, and the same operations. The old comment asserting a separate
base-only null-check statement was stale and removed. Retail free-fly sequence
loads `+0x234`, passes `"Free Fly View"` to the camera director, and stores mode
1; public `switch_camera_mode`, RVA `0x5cff20 +0x79`, has that same branch.
The source-interface correction is supported; its new caller codegen remains
unmeasured until the deferred build.

Getter offsets must account for both embedded worlds and adjusted base `this`:
game's world starts at `+0x98`; `survarium::engine` inside the world starts at
`+0xc`. Thus the apparently suspicious bullet-manager `+0x230` read is correct
for the adjusted interface, while game `+0x2c4` is the project resource, not the
world's victory-items vector. No offset cast is needed in source.

## Legacy evidence and preserved retail behavior

Prior source was read locally through
`git show 885e1d4a4:temp/game_legacy/game_unused.cpp` (selection, rotation,
deletion, dictionary, behaviour and frustum helpers), `game.cpp:774..813`
(visibility predicate), and `human_npc.h` (angles getter). Much of this was
already commented-out old code. It supplies semantics to test, **not proof of
the retail helper's use or build configuration**.

Retail `get_visible_objects` (`0x5be4e0`) returns without invoking a callback;
`query_npc_dictionary` (`0x5be4d0`) returns without a resource query.
`ray_query` (`0x5be510`) constructs a physics-result vector, invokes the physics
query with masks `0xffff/0x40`, destroys the vector and returns false. These
retained entry points stay unchanged. No invented reference is added to force
models into the executable. Direct xrefs alone cannot exclude a callback bind.

The picker remains incomplete because the new `closest_ray_result::object`
is a physics object, not the collision object used in the older selection
predicate. Recover its user-data producer and NPC identity path first, then
the consumer's sorting/occlusion and query parameters. In contrast the older
visibility predicate still takes a collision triangle and can be expressed
without an invalid physics cast, but its payload ownership still needs binding.

NPC transform models delegate through `set_transform`: that retained method
updates the stored/feet-target transform, renderer and animation player.
Directly writing only the matrix would miss those effects. Physics updates
belong to the existing later path; no new immediate physics move is invented.

Static review checked the query API's argument defaults, the existing behaviour
parameter type, complete config/query/collision declarations, public callee
access and unchanged method signatures. This does not replace compilation.
Next: the complete `human_npc`/collision-object families and the open selection,
hit, debug, visibility and query-producer contracts above.
