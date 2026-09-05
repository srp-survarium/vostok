<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# Editor decomp handoff

The shipped client PDB declares editor-facing interfaces whose implementations
are absent from `survarium.exe`. These declarations must remain in the source
for PDB topology. Some now have historical-source bodies or explicit semantic
models; none should be mistaken for byte verification from the client alone.
The current per-function evidence and open alternatives are maintained in the
[PR 569 reconstruction register](../binary_matching/pr569_semantic_reconstruction.md).

## Animation

- `fixed_joint_camera_effector::{process_camera,on_attach}` in
  `animation/i_editor_mixer.h` have no client procedure or inline expansion.
- The public 2011 X-Ray source does not contain
  `fixed_joint_camera_effector`, so it provides no earlier implementation.
- Its current model uses the joint matrix as the inverse view and has an empty
  attachment hook; verify coordinate conventions against an editor consumer.
- `animation_states_dumper` has timestamp/default-hook models, but the tree's
  19-argument dump callback still needs a concrete consumer to map every value.
- The null-weight detector has conservative structural-zero rules. Transition
  and compound-operator semantics are explicitly unproven.

## Physics

- `bt_soft_body_rope::{is_active,get_fragments_count,get_fragment,
  get_nodes_count,get_node}`;
- `bullet_physics_world::{create_soft_body_rope,destroy_soft_body_rope}`;
- `bt_ghost_object::{dbg_render,non_compound_shapes_count}`.

GSC commit `47633d6472d17c88d3e286082a171c1920dfc81c` and the public 2011
X-Ray source contain older out-of-line rope accessors and world create/destroy
helpers. They are useful source priors, but predate the retail client and do not
prove that the bodies, signatures, or inline placement survived unchanged.
The seven rope methods are now restored from that source. Both ghost methods
have models using the retained shape/transform and recursive centres traversal;
the original leaf-count implementation strategy remains open.

## Render

World facade methods now forward lighting picking, probe generation and sky-AO
generation to their declared renderer counterparts. **Those renderer bodies
are still absent**: the wrappers do not make an editor configuration functional
or linkable. `scene_renderer::draw_present_impl` also remains unresolved; its
class has no output-window state from which to infer the presentation target.

## Oracle needed

Promote these models to verified retail reconstructions only from original
source at the retail revision or
from an editor executable/PDB that actually emits their procedures or caller
expansions. When that oracle is available, preserve each named inline seam in
its consumers; reproducing the field logic directly is not source-equivalent.
