<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Editor decomp handoff

The shipped client PDB declares editor-facing interfaces whose implementations
are absent from `survarium.exe`. These declarations must remain in the source
for PDB topology, but their `STATE[UNMATCHABLE]` bodies cannot be reconstructed
or byte-verified from the client alone.

## Animation

- `fixed_joint_camera_effector::{process_camera,on_attach}` in
  `animation/i_editor_mixer.h` have no client procedure or inline expansion.
- The public 2011 X-Ray source does not contain
  `fixed_joint_camera_effector`, so it provides no earlier implementation.

## Physics

- `bt_soft_body_rope::{is_active,get_fragments_count,get_fragment,
  get_nodes_count,get_node}`;
- `bullet_physics_world::{create_soft_body_rope,destroy_soft_body_rope}`;
- `bt_ghost_object::{dbg_render,non_compound_shapes_count}`.

GSC commit `47633d6472d17c88d3e286082a171c1920dfc81c` and the public 2011
X-Ray source contain older out-of-line rope accessors and world create/destroy
helpers. They are useful source priors, but predate the retail client and do not
prove that the bodies, signatures, or inline placement survived unchanged.

## Oracle needed

Close these placeholders only from original source at the retail revision or
from an editor executable/PDB that actually emits their procedures or caller
expansions. When that oracle is available, preserve each named inline seam in
its consumers; reproducing the field logic directly is not source-equivalent.
