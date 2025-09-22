////////////////////////////////////////////////////////////////////////////
//	Created 	: 20.09.2025
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_COLLISION_GAME_OBJECT_H_INCLUDED
#define VOSTOK_COLLISION_GAME_OBJECT_H_INCLUDED

namespace survarium {
	typedef void hit_receiver;			// sushi@TODO: Interesting that `vostok` now references `survarium`
	typedef void collision_geometry;	// sushi@TODO
	typedef void base_player;			// sushi@TODO
}

namespace vostok {
namespace collision {

struct game_object  {
public:
	virtual hit_receiver*		cast_to_hit_receiver()			= 0;
	virtual collision_geometry* cast_to_collision_geometry()	= 0;
	virtual base_player*		cast_to_base_player()			= 0;
}; // struct game_object

namespace {
	typedef char size_assert[
		sizeof(game_object) == 0x4 ? 1 : -1
	];
}

} // namespace collision
} // namespace vostok

#endif // #ifndef VOSTOK_COLLISION_GAME_OBJECT_H_INCLUDE