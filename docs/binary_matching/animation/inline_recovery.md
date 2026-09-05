<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Animation missing-body audit

The retail PDB declares every method below, but it has no VS2008 inline-site
records. A method is therefore recoverable only when the client emits a
procedure or when one consumer contains an attributable expansion. Names and
plausible behavior are not sufficient on their own.

## Recovered inline seam

- `n_ary_tree_comparer::advance_buffer` increments
  `m_needed_buffer_size`. `n_ary_tree_size_calculator::advance_buffer` and the
  three `interpolator_size_calculator::visit` overloads now call that named
  helper when given a comparer. Reaching through the friendship and performing
  the same field update would erase a retail source seam and could change the
  callers after inlining.

The method has no standalone target procedure. Its body and all four caller
seams are reconstructed together from the private method declaration, the
friend graph, and the repeated target field update.

## Not observable in the client

Thirty declarations remain `STATE[UNMATCHABLE]`:

- editor-only `fixed_joint_camera_effector::{process_camera,on_attach}`;
- `bone_matrices_computer::overweighting_detected`;
- `animation_states_dumper::{animation_states_dumper,
  ~animation_states_dumper,current_time_in_ms,on_started_dump,
  on_finished_dump}`;
- `animation_player::{has_object,dump_animation_states,
  serialize_empty_state,serialize_state_impl}`;
- `n_ary_tree::{tick_to_nearest_event,has_object,is_consistent,
  dump_animation_states,computed_local_bone_matrix,compute_skeleton_branch,
  update_animation_interval_time,accumulate_object_movement}` (the four-argument
  overload), and `update_weight`;
- the constructor and eight visitor overrides of
  `n_ary_tree_null_weight_detector`.

None has a target or base rich-index procedure. No current client source calls
the public convenience methods or instantiates either editor helper. The large
`n_ary_tree` consumers contain related bone, weight, interval, and serialization
logic, but the client provides no boundary that assigns an exact subset to one
of these declarations. For example, `update_animation_state` already has a
13-statement target/base partition with only a two-byte size residual; that
does not prove how its logic was divided between the two declared inline
helpers. Those bodies stay unfilled until an original source or another target
provides the boundary.

`bone_matrices_computer::overweighting_detected` has no client consumer, and the
public 2011 X-Ray source predates both the accessor and its backing field. A
direct field return is plausible, but the client cannot distinguish it from
another implementation, so the placeholder remains.

Editor-only gaps are tracked centrally in the
[editor decomp handoff](../../todos/editor_decomp.md).

The commented-out `binary_tree_animation_node` destructor was not a declaration
and was removed from the inventory.

## PDB structure restored

- `fixed_joint_camera_effector` is a class rather than a struct.
- `animation_states_dumper` privately inherits `core::noncopyable`; its explicit
  default constructor and `m_current_time_in_ms` are protected.
- all `n_ary_tree_null_weight_detector::visit` overrides are private.
- `animation_player::serialize_empty_state`, `destroy_state`, and
  `destroy_subscriptions` now occupy their retail declaration positions, and
  its anonymous enum includes `estimated_channel_id_length = 12`.
- `n_ary_tree` no longer declares the base-only
  `process_event_result_enum`; `dispatch_callbacks` and
  `update_animation_time` now occupy retail positions 43 and 45.

`n_ary_tree_comparer` and `bone_matrices_computer` are topology-identical to
retail independently of whether their un-emitted inline bodies are filled.
