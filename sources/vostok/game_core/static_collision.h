// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef STATIC_COLLISION_H_INCLUDED
#define STATIC_COLLISION_H_INCLUDED

#include <vostok/physics/collision_shapes.h>

namespace vostok {
namespace physics {
	struct world;
	class bt_static_rigid_body;
}
}

namespace survarium {

struct static_collision {
	void	insert	( physics::world* w );
	void	remove	( physics::world* w );

public:
	/* 0x0000 */	float4x4						matrix_;
	/* 0x0040 */	u16								filter_group_;
	/* 0x0042 */	u16								filter_mask_;
	/* 0x0044 */	physics::bt_collision_shape_ptr	shape_;
	/* 0x0048 */	physics::bt_static_rigid_body*	physics_rigid_body_;
}; // struct static_collision

STATIC_SIZE_ASSERT(static_collision, 0x4C);

} // namespace survarium

#endif // #ifndef STATIC_COLLISION_H_INCLUDED
