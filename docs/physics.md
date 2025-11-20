## Physics
If you got close in matching, but the loads of vectors are out of order, this most likely means something got inlined.

## Matching dept


### Overview

`btVector3::lerp` vs `btVector3::setInterpolate3` - The second one builds in-place and was used by VG.
Their impl is also slightly different, so you can figure out which one is used based on that also.


### Modules
* animated_model_instance_cook.cpp
* animated_rigid_body.cpp
* bullet_character_controller.cpp
* bullet_physics_world.cpp
* character_controller.cpp
* collision_shape_cook.cpp
* collision_shapes.cpp
* physics_entry_point.cpp

### Modules (Done)
* ghost_object.cpp
* rigid_body_construction_info.cpp
* static_rigid_body.cpp
