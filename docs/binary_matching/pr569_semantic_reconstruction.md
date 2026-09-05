<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# PR 569: individual non-network reconstruction register

Baseline: `96d18a39b`, after the network/network-core reconstruction. This
register accounts for all **61 inline bodies and three additional declarations**
left by that audit. Each overload and visitor gets its own row.

The source-only follow-up fills **50 inline bodies and two out-of-line bodies**.
**Eleven inline bodies and `can_prone` remain unresolved.** These are not
silently accepted no-ops, and removing a marker is not evidence of recovery.
The network's 74-body register remains in its module reports.

No build, ledger refresh, or byte-match claim accompanies this batch. The user
explicitly requested reconstruction before another expensive build.

Evidence labels:

- **Prior**: a body exists in authentic GSC repository history; survival to
  v0.100b is not established for an un-emitted function.
- **Caller**: executable operations already exist in a retained caller; their
  extraction into this declared helper is a source-boundary hypothesis pending
  the batched structure comparison.
- **Model**: semantic reconstruction from fields, signatures, siblings, and
  usage. It is not proven retail source.
- **Open**: alternatives remain that the available evidence does not resolve.

## AI and game-core

| ID | Function | Disposition and evidence |
|---|---|---|
| F01 | `fsm::remove_state` | Model: erase from `m_states`, without deleting an externally owned state. `add_state`, `pop_state`, and the separate `clear_transitions` establish the ownership split. Whether removal also clears incoming transitions or `m_current_state` remains open; no consumer was found. |
| F02 | `fsm::remove_transition` | Model: unlink and free matching transition records with AI's allocator, preserving iteration across erase. Inverse of `add_transition` and the allocation policy in `clear_transitions`; whether retail removed the first or every parallel edge is unproven. |
| G01 | `jump_logic::get_animated_object` | Model: return `m_animated_object`. The const pointer is initialized by the constructor; no observed consumer distinguishes an alternative getter. |

## Animation

| ID | Function | Disposition and evidence |
|---|---|---|
| A01 | `bone_matrices_computer::overweighting_detected` | Model: return the flag set by `computed_local_bone_transform` after weight normalization. No consumer found. |
| A02 | `fixed_joint_camera_effector::process_camera` | Model: assign the referenced joint matrix to the inverse view. Its only state is that matrix reference; an editor target is needed to resolve any coordinate-convention adjustment. |
| A03 | `fixed_joint_camera_effector::on_attach` | Model: empty attachment hook; there is no owned temporal state. This is not proof that the original hook had no side effects. |
| A04 | `animation_states_dumper::animation_states_dumper` | Model: initialize the sole timestamp to zero. Constructor access remains protected. Initial sentinel unproven. |
| A05 | `animation_states_dumper::~animation_states_dumper` | Model: empty virtual destructor; no owned resources in the recorded layout. |
| A06 | `animation_states_dumper::current_time_in_ms` | Model: return the recorded timestamp field. |
| A07 | `animation_states_dumper::on_started_dump` | Model: store the supplied timestamp. |
| A08 | `animation_states_dumper::on_finished_dump` | Model: empty default hook. No derived dumper consumer found. |
| A09 | `animation_player::has_object` | Model: forward to its mixing tree's named lookup. |
| A10 | `animation_player::dump_animation_states` | Model: forward to the mixing tree. **Callee A16 remains unresolved**, so this is not a working dump pipeline yet. |
| A11 | `animation_player::serialize_empty_state` | Model: invoke A12 with an empty tree, zero tree size and null subscription head. The retained serializer/deserializer establish the cookie, size word and placement-tree format. |
| A12 | `animation_player::serialize_state_impl` | Caller: the existing serializer's cookie/size/buffer/tree-copy region, parameterized by the exact PDB signature. The const tree parameter is compatible with copying; non-const event adjustment and time inversion stay in `serialize_state`. Caller now uses the helper. |
| A13 | `n_ary_tree::tick_to_nearest_event` | Model: when animations exist, call `tick` with `nearest_event_time_in_ms`. Do not advance an empty tree to the `u32(-1)` sentinel. Return/callback handling follows the existing `tick` interface. |
| A14 | `n_ary_tree::has_object` | Model: scan the recorded holders for their `animated_object` identity. The same identity comparison is used in `get_object_transform`. |
| A15 | `n_ary_tree::is_consistent` | Open: count/list agreement, event ordering and iterator ownership are candidate invariants, not an established original predicate. See the open-work table. |
| A16 | `n_ary_tree::dump_animation_states` | Open: the callback has 19 parameters whose PDB names are absent. The earlier text logger establishes some values but not the complete argument mapping. |
| A17 | `n_ary_tree::computed_local_bone_matrix` | Open: an older single-object helper predates the current multi-object tree. The signature lacks the object identity required by the retained bone computer. |
| A18 | `n_ary_tree::compute_skeleton_branch` | Open: child-pointer/output offsets and parent-matrix multiplication are present in `bone_matrices_computer`; object selection still depends on A17. |
| A19 | `n_ary_tree::update_animation_interval_time` | Caller: extract the freeze check, independent/driven interval calculation and absolute-time update from `update_animation_state`. The historic declaration names the two integers start/target time. Caller now uses the helper. |
| A20 | `n_ary_tree::accumulate_object_movement(node,u32,float,u32)` | Open: historic parameter names are `start_time_in_ms`, `start_animation_time`, `target_time_in_ms`. In particular, the first integer is **not** an interval ID. Integration over the interval is plausible; sampling and subdivision rules remain unknown. |
| A21 | `n_ary_tree::update_weight` | Caller: existing `n_ary_tree_weight_calculator(target_time,&node)` evaluation and state-weight store. Start time is unused in this model. Caller now uses it; interval-dependent side effects in the original helper remain a question. |
| A22 | `n_ary_tree_null_weight_detector` constructor | Model: initialize false and visit the supplied animation. The PDB layout has only a boolean result, no evaluation-time input. |
| A23 | detector `visit(animation_node&)` | Model: follow the weight driver, otherwise detect a zero weight operand and skip time-scale operands. |
| A24 | detector `visit(weight_transition_node&)` | Model: prove zero only when both endpoints are zero. This is a conservative structural predicate, not a time-evaluated weight. |
| A25 | detector `visit(time_scale_transition_node&)` | Model: false, since this operand does not supply a weight. |
| A26 | detector `visit(weight_node&)` | Model: exact comparison of the stored weight with zero. |
| A27 | detector `visit(time_scale_node&)` | Model: false for the non-weight operand. |
| A28 | detector `visit(addition_node&)` | Model: prove zero when every operand is structurally zero. Does not discover cancellation of nonzero terms. |
| A29 | detector `visit(subtraction_node&)` | Model: same conservative all-zero test; cancellation remains undetected. |
| A30 | detector `visit(multiplication_node&)` | Model: any structurally zero operand suffices. The empty product is not zero. |

The detector's boolean layout does not prove these operator rules: a different
visitor implementation could use local state. Its transition and compound-node
semantics remain explicitly tracked for an editor target.

## Physics

| ID | Function | Disposition and evidence |
|---|---|---|
| P01 | `bt_static_rigid_body::is_active` | Prior/model: the older rigid-body wrapper delegates to Bullet `isActive`; apply the same operation to the recorded static-body pointer. |
| P02 | `bt_static_rigid_body::set_ccd_motion_thresholds` | Model: delegate the two floats to Bullet's motion-threshold and swept-sphere-radius setters, in that order. Parameter roles require a consuming oracle. |
| P03 | `bt_static_rigid_body::get_collision_shape` | Model: return the recorded intrusive shape pointer, preserving the PDB return type. |
| P04 | `bt_static_rigid_body::predict_integrated_transform` | Model: Bullet prediction to a local `btTransform`, then `from_bullet` into the output matrix. |
| P05 | `bt_rigid_body_construction_info::load` | Prior: GSC `47633d6472d17c88d3e286082a171c1920dfc81c`, `physics/sources/rigid_body_construction_info.cpp`. Preserve all surviving fields and keys, including `m_linearDamping`. Omit only the ancestor's `mass_center` assignment to `m_render_model_offset`, a member absent from retail's layout. |
| P06 | `bt_ghost_object::dbg_render` | Model, out-of-line: forward the owned shape, Bullet world transform, and RGB color to `world::draw_object`. |
| P07 | `bt_ghost_object::non_compound_shapes_count` | Model, out-of-line: use the retained recursive centres traversal and count its leaves. This establishes the same leaf definition but not retail's allocation/traversal strategy. |
| P08 | `bt_character_controller::adjust_foot_transform(5 args) const` | Open: a hit-point/normal sweep interface is plausible, but the two output vectors have no PDB names. The live six-argument matrix overload is distinct. |
| P09 | `bt_character_controller::can_prone` | Open, out-of-line: the wrapper's layout has no prone dimensions or prone state; neither `can_crouch`'s unconditional true nor `on_ground` proves this predicate. |
| P10 | `bt_soft_body_rope::is_active` | Prior: Bullet `isActive`, from the GSC commit above, `physics/sources/soft_body.cpp`. |
| P11 | `bt_soft_body_rope::get_fragments_count` | Prior: `m_links.size()`. |
| P12 | `bt_soft_body_rope::get_fragment` | Prior: convert the selected link's two node positions from Bullet. |
| P13 | `bt_soft_body_rope::get_nodes_count` | Prior: `m_nodes.size()`. |
| P14 | `bt_soft_body_rope::get_node` | Prior: convert the selected node's position from Bullet. |
| P15 | `bullet_physics_world::create_soft_body_rope` | Prior: same GSC commit, `physics/sources/bullet_physics_world.cpp`. CreateRope with both ends fixed; copy iterations, configuration coefficients, margin and stiffness; allocate the wrapper through the world's allocator. |
| P16 | `bullet_physics_world::destroy_soft_body_rope` | Prior: delete the wrapper with the world's allocator, as the ancestor does. Do not invent an additional Bullet-body deletion. |
| P17 | `collision_shape_cook::load_collision_resources` | Caller: extract the retained exported-primitives/settings/vertices/indices/face-data request setup, callback and parent relationship. `translate_query` now calls the private helper after preparing `cook_data`. |
| P18 | `bullet_character_controller::get_gravity` | Model: return the constructor-set constant member. |
| P19 | `bullet_character_controller::is_inserted` | Model: non-null collision-world pointer, set/cleared by insert/remove. |
| P20 | `bullet_character_controller::prevent_step_bouncing` | Open: `m_positions` and `s_cc_smooth_positions_count_value` suggest smoothing, but the retained path only clears the history. Axis selection, sampling point and reset policy are unresolved. |
| P21 | `bullet_character_controller::can_overstep_obstacle` | Open: raised capsule sweep between the two vector arguments is plausible; the callback/filter/normal rules and interpretation of the endpoints are not established. |
| P22 | `bullet_character_controller::has_support_to_overstep_obstacle` | Open: a downward support sweep at a candidate step destination is plausible. The no-argument signature leaves the source of that destination unresolved. |
| P23 | `bullet_character_controller::updata_slide_vector` | Open: slope-tangent projection is plausible. Whether the float is dt, speed or distance, and whether the result updates walk displacement or vertical velocity, is not established. Keep retail's spelling. |
| P24 | `bullet_character_controller::get_contacts_count` | Model: enumerate cached overlapping-pair manifolds and sum `getNumContacts`, using the same data path as `recover_from_penetration`. Whether retail filtered positive-distance cached contacts remains open. |

## Render

| ID | Function | Disposition and evidence |
|---|---|---|
| R01 | `engine::world::pick_lighting_luminance` | Model: guarded forwarding to the renderer's identically typed declaration. The renderer callee itself still needs recovery. |
| R02 | `engine::world::set_picking_lighting_luminance_mode` | Model: same renderer forwarding. Null-renderer behavior follows the existing world facade convention, not a retained call to this method. |
| R03 | `engine::world::generate_environment_probe` | Model: forward all four arguments; renderer implementation remains absent. |
| R04 | `engine::world::generate_sky_ao_map` | Model: forward all four arguments; renderer implementation remains absent. This is not recovery of the AO generation algorithm. |
| R05 | `scene_renderer::set_fixed_lod` | Model: interlocked store to `base_scene::fixed_lod_value`. Retail records a volatile long; its reader in `scene.cpp` uses the value, and initialization supplies the automatic-LOD sentinel. Atomic versus plain store is unproven. |
| R06 | `scene_renderer::get_fixed_lod` | Model: return the same volatile field through the declared unsigned interface. |
| R07 | `scene_renderer::draw_present_impl` | Open: presenting normally requires a window and viewport, neither of which is stored by this class. `engine::world::end_frame` only increments a counter, so treating it as presentation would be an unsupported substitution. |

## Remaining evidence work, not a no-op classification

| IDs | Next discriminating evidence |
|---|---|
| A15 | Find an assertion/call consumer or older definition of the consistency predicate; distinguish boolean validation from assert-only diagnostics. |
| A16 | Recover a concrete dumper subclass or caller to assign all 19 callback argument roles. The old seven-value text log cannot supply the missing ordering. |
| A17–A18 | Find a consumer establishing the single-object precondition or object-selection rule before reusing the current bone computer. |
| A20 | Locate the old four-argument definition, or compare a consuming integration loop's sampling times and root-motion accumulation. Historic parameter names already rule out the interval-ID guess. |
| P08 | Find the older output-vector overload or a caller consuming both outputs, then establish capsule orientation and collision filters. |
| P09 | Search an editor/server stance caller and its decision branch; no public/private adjustment in the wrapper alone resolves the predicate. |
| P20–P23 | Locate producers of position history/slide state or an earlier controller implementation. Present client step routines supply related operations, not unique implementations for these helpers. |
| R07 | Find the corresponding draw/present command consumer and where its output window originates. |

## Deferred batch validation

The three caller extractions affect `animation_player::serialize_state`,
`n_ary_tree::update_animation_state` (two helpers), and
`collision_shape_cook::translate_query`. Their pre-edit target statement counts
are 8, 13 and 24 respectively. Existing line programs alone do not prove that
an un-emitted inline helper was absent: they are constraints to test against the
new candidate, not an automatic veto. Re-run structure diff before inspecting
byte regressions when the user permits the batched build. Do not claim the
unbuilt source has reproduced those partitions.

Also check that the unconsumed editor forwarding methods remain discarded in
the client link; their missing renderer callees must be implemented before an
editor configuration can link. Keep this distinct from client build safety.
