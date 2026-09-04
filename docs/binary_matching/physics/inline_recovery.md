<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Physics missing-body audit

This audit separates a missing client procedure from a recovered inline body. The
retail VS2008 PDB has no inline-site records, so a body is recoverable from this
target only when its expansion is visible in a caller. An absent procedure with
no caller proves only that the client does not emit the method.

## Recovered source

- `bullet_character_controller::get_gravity` returns `m_gravity`.
- `bullet_character_controller::is_inserted` tests `m_collision_world`.
- `bt_static_rigid_body::get_collision_shape` returns `m_shape`.
- `bullet_character_controller::debugDraw` is a real empty virtual override. Its
  `ret 4` body is present through the vtable and ICF-folded in both binaries.

The three accessors have direct, type-preserving field semantics and introduce no
new object or allocation behavior. The client has no call site for them, so their
spelling cannot receive a byte-match verdict from this target.

## Recovered class seams

- Retail records `bt_soft_body_rope::m_bt_body` as private. The shipped
  `bullet_physics_world::add/remove` methods access it directly, matching the
  `friend class bullet_physics_world` seam in GSC's 2011 source.
- Retail records `bullet_character_controller::m_collision_world` as private.
  `bt_character_controller::activate/deactivate` access that field directly, so
  `friend class bt_character_controller` is the faithful seam.
- Retail records `bt_constraint::m_bt_typed_constraint` as protected.
  `bullet_physics_world::add/remove` access it directly, establishing
  `friend class bullet_physics_world` rather than a public field.
- The target PDB contains the missing nested names
  `bt_static_rigid_body::super`, `bullet_physics_world::callbacks_container_type`,
  and `collision_shape_cook::super`.
- PDB declaration order puts all thirteen
  `bt_rigid_body_construction_info` fields before its compiler-generated copy
  constructor, explicit default constructor, and `load`. It also puts
  `collision_shape_cook::cook_data` immediately before `super`, and
  `bullet_character_controller::m_up_vector` immediately after
  `m_collision_world`.

These are source-structure results, independent of fuzzy byte percentage.

## Not observable in the client

The following declarations have no procedure, caller, or recoverable expansion
in `survarium.exe` and remain `STATE[UNMATCHABLE]`:

- `bt_static_rigid_body::{is_active,set_ccd_motion_thresholds,
  predict_integrated_transform}` and
  `bt_rigid_body_construction_info::load`;
- `bt_ghost_object::{dbg_render,non_compound_shapes_count}`;
- the five-argument `bt_character_controller::adjust_foot_transform` overload and
  `bt_character_controller::can_prone`;
- the five `bt_soft_body_rope` accessors;
- `bullet_physics_world::{create_soft_body_rope,destroy_soft_body_rope}`;
- `collision_shape_cook::load_collision_resources`;
- `bullet_character_controller::{prevent_max_slope_moving_prestep,
  prevent_step_bouncing,can_overstep_obstacle,
  has_support_to_overstep_obstacle,updata_slide_vector,get_contacts_count}`.

GSC commit `47633d6472d17c88d3e286082a171c1920dfc81c` supplies older out-of-line
implementations for the rope accessors and world create/destroy helpers. It is
useful source-prior evidence, but it predates the retail client by years and does
not prove that those bodies or their inline/out-of-line placement survived
unchanged. An editor PDB/executable from the retail revision is the appropriate
oracle for closing them.
