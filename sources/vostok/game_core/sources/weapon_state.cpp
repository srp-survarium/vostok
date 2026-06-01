////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_state.h>

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

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::weapon_state::deserialize(vostok::network_core::packet_reader&)
void weapon_state::deserialize( network_core::packet_reader& packet )
{
	// FUNCTION BODY
	// <0x5a0c59>|0x009|+0x00d:'36'
	// <0x5a0c66>|0x016|+0x00e:'37'
	// <0x5a0c74>|0x024|+0x00e:'38'
	// ******
}

} // namespace survarium
