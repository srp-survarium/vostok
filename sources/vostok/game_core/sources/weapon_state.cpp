// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/weapon_state.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

weapon_state::weapon_state( )
{
}

weapon_state& weapon_state::operator=( weapon_state const& other )
{
	// no self-assignment guard, as retail
	slot_id			= other.slot_id;
	ammo_slot_id	= other.ammo_slot_id;
	state			= other.state;
	return *this;
}






void weapon_state::deserialize( network_core::packet_reader& packet )
{
	slot_id			= packet.r< bool >( );
	ammo_slot_id	= packet.r< bool >( );
	state			= packet.r< bool >( );
}

} // namespace survarium
