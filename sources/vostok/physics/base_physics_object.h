////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.08.2025
////////////////////////////////////////////////////////////////////////////

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

class base_physics_object : loose_ptr_base {
public:
	inline base_physics_object() : user_data(NULL) { }

	virtual u16					get_collision_group		( ) const	= 0;
	virtual btCollisionObject*	get_bt_collision_obect	( )			= 0;
public:
	/* offset 0x0004 */ /* fields for loose_ptr_base */
	/* offset 0x0008 */ collision::game_object*             user_data; // sushi@NOTE: Most likely needs to be private, though there are no getters/setters in the header
}; // class base_physics_object

namespace {
	typedef char size_assert[
		sizeof(base_physics_object) == 0xC ? 1 : -1
	];
}

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_BASE_PHYSICS_OBJECT_H_INCLUDED
