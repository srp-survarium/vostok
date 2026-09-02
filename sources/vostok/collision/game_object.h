// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 20.09.2025
////////////////////////////////////////////////////////////////////////////
#ifndef VOSTOK_COLLISION_GAME_OBJECT_H_INCLUDED
#define VOSTOK_COLLISION_GAME_OBJECT_H_INCLUDED

// sushi@TODO: Interesting that `vostok` now references `survarium`
namespace survarium {
	struct hit_receiver;
	class collision_geometry;

	struct base_player;
}

namespace vostok {
namespace collision {

struct game_object  { // sushi@TODO: The whole thing needs to be moved to API
	virtual survarium::hit_receiver*		cast_to_hit_receiver		( ) { return NULL; }
	virtual survarium::collision_geometry*	cast_to_collision_geometry	( ) { return NULL; }
	virtual survarium::base_player*			cast_to_base_player			( )	{ return NULL; }
}; // struct game_object

STATIC_SIZE_ASSERT(game_object, 0x4);

} // namespace collision
} // namespace vostok

#endif // #ifndef VOSTOK_COLLISION_GAME_OBJECT_H_INCLUDE
