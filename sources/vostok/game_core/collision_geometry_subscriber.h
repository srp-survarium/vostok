// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef COLLISION_GEOMETRY_SUBSCRIBER_H_INCLUDED
#define COLLISION_GEOMETRY_SUBSCRIBER_H_INCLUDED

#include <vostok/game_core/base_game_object.h>

namespace survarium {

class usable_object;
class collision_sensor;

class collision_geometry_subscriber : public base_game_object {
public:
	virtual						~collision_geometry_subscriber	( ) {}

	virtual	usable_object*			cast_to_usable	( ) { return NULL; }
	virtual	collision_sensor*		cast_to_sensor	( ) { return NULL; }
}; // class collision_geometry_subscriber

STATIC_SIZE_ASSERT(collision_geometry_subscriber, 0x4);

} // namespace survarium

#endif // #ifndef COLLISION_GEOMETRY_SUBSCRIBER_H_INCLUDED
