////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_state.h"

namespace survarium {

// STATE[STUB]
// void survarium::player_state::deserialize(vostok::network_core::packet_reader&)
void player_state::deserialize( vostok::network_core::packet_reader& packet )
{
	// LOCALS
	// float 						yaw
	// vostok::math::float3 const& 	position
	// ******

	// FUNCTION BODY
	// 1
	// 2
	// 3
	// 4
	// <0x7765e1>|0x000|0x000:'31'
	// <0x7765f2>|0x011|0x011:'32'
	// <0x7765ff>|0x01e|0x00d:'33'
	// <0x776612>|0x031|0x013:'34'
	// <0x776633>|0x052|0x021:'35'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::player_state::serialize(vostok::network_core::udp_match_packet&) const
void player_state::serialize( vostok::network_core::udp_match_packet& packet ) const
{
	// FUNCTION BODY
	// 1
	// 2
	// 3
	// 4
	// <0x77657b>|0x000|0x000:'45'
	// <0x77658e>|0x013|0x013:'46'
	// <0x7765a8>|0x02d|0x01a:'47'
	// 1
	// ******
}

} // namespace survarium
