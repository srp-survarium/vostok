// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VOSTOK_PHYSICS_BASE_PHYSICS_OBJECT_H_INCLUDED
#define VOSTOK_PHYSICS_BASE_PHYSICS_OBJECT_H_INCLUDED

#include <vostok/loose_ptr_base.h>

class btCollisionObject;

namespace vostok {
namespace collision {
	struct game_object;
}
}

namespace vostok {
namespace physics {

class VOSTOK_NOVTABLE base_physics_object : public loose_ptr_base {
public:
	inline						base_physics_object		( ) : user_data( NULL ) { }

	virtual u16					get_collision_group		( ) const	= 0;
	virtual btCollisionObject*	get_bt_collision_obect	( )			= 0;

	inline						~base_physics_object	( ) { }
public:
	/* 0x0004 */	/* loose_ptr_base */
	/* 0x0008 */	collision::game_object*		user_data;
}; // class base_physics_object

STATIC_SIZE_ASSERT(base_physics_object, 0xC);

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_BASE_PHYSICS_OBJECT_H_INCLUDED
