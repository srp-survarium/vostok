// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "client_player_state.h"

#include <vostok/physics/character_controller.h>

namespace survarium {

void client_player_state::update_transform( )
{
	if ( physics_controller->has_updates( ) )
		transform = physics_controller->get_transform( );

	previous_transform = transform;
}

} // namespace survarium
