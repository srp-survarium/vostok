<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Animation missing-body audit

The retail PDB declares every method below, but it has no VS2008 inline-site
records. A method is therefore recoverable only when the client emits a
procedure or when one consumer contains an attributable expansion. Names and
plausible behavior are not sufficient on their own.

## Recovered field and hook bodies

- `animation_states_dumper::current_time_in_ms` returns
  `m_current_time_in_ms`, and `on_started_dump` stores its argument there.
  Its explicit destructor and `on_finished_dump` are empty base hooks. The
  protected default constructor remains unobservable.
- `n_ary_tree_comparer::advance_buffer` increments
  `m_needed_buffer_size`. The same operation is used throughout the comparer
  and by `n_ary_tree_size_calculator::advance_buffer` when it is given a
  comparer.
- `bone_matrices_computer::overweighting_detected` returns
  `m_overweighting_detected`.

These methods do not have standalone target procedures. Their bodies restore
direct field/interface semantics, but remain outside the byte denominator for
this client.

## Not observable in the client

Twenty-five declarations remain `STATE[UNMATCHABLE]`:

- editor-only `fixed_joint_camera_effector::{process_camera,on_attach}`;
- `animation_player::{has_object,dump_animation_states,
  serialize_empty_state,serialize_state_impl}`;
- the protected `animation_states_dumper` default constructor;
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

`n_ary_tree_comparer` and `bone_matrices_computer` were already
topology-identical before their direct getters were filled.
