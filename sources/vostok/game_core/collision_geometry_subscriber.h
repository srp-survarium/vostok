////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////

#ifndef COLLISION_GEOMETRY_SUBSCRIBER_H_INCLUDED
#define COLLISION_GEOMETRY_SUBSCRIBER_H_INCLUDED

#include <vostok/game_core/base_game_object.h>

namespace survarium {

typedef void usable_object;
class collision_sensor;

class collision_geometry_subscriber : public base_game_object {
public:
	// virtual	inline	~collision_geometry_subscriber	( ) { /* no source */ } sushi@TODO

	virtual	usable_object*			cast_to_usable	( );
	virtual	collision_sensor*		cast_to_sensor	( );
}; // class collision_geometry_subscriber

STATIC_SIZE_ASSERT(collision_geometry_subscriber, 0x4);

} // namespace survarium

#endif // #ifndef COLLISION_GEOMETRY_SUBSCRIBER_H_INCLUDED
