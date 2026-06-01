////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/player_state.h>

namespace survarium {

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::player_state::deserialize(vostok::network_core::packet_reader&)
void player_state::deserialize( network_core::packet_reader& packet )
{
	// LOCALS
	// float 						yaw
	// float3 const& 				position
	// ******

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7765e1>|0x011|+0x011:'31'
	// <0x7765f2>|0x022|+0x00d:'32'
	// <0x7765ff>|0x02f|+0x013:'33'
	// <0x776612>|0x042|+0x021:'34'
	// <0x776633>|0x063|+0x021:'35'
	// <0>
	// ******
}

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::player_state::serialize(vostok::network_core::udp_match_packet&) const
void player_state::serialize( network_core::udp_match_packet& packet ) const
{
	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x77657b>|0x00b|+0x013:'45'
	// <0x77658e>|0x01e|+0x01a:'46'
	// <0x7765a8>|0x038|+0x012:'47'
	// <0>
	// ******
}

} // namespace survarium
