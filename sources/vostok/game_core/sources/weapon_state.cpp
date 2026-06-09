////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_state.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

// STATE[100%|DONE]
weapon_state::weapon_state( )
{
}

// STATE[100%|DONE]
weapon_state& weapon_state::operator=( weapon_state const& other )
{
	// claude@NOTE: target does a plain member-wise copy with NO self-guard.
	slot_id			= other.slot_id;
	ammo_slot_id	= other.ammo_slot_id;
	state			= other.state;
	return *this;
}

// STATE[PARTIAL]: reads slot_id, ammo_slot_id, state - each as r< bool > (single-byte
// stores into the three u8 members, matching the target's r<bool> instantiation).
void weapon_state::deserialize( network_core::packet_reader& packet )
{
	slot_id			= packet.r< bool >( );
	ammo_slot_id	= packet.r< bool >( );
	state			= packet.r< bool >( );
}

} // namespace survarium
