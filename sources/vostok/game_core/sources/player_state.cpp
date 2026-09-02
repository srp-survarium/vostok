// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/player_state.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

void player_state::deserialize( network_core::packet_reader& packet )
{
	float3 const&	position	= packet.r< math::float3 >( );
	float const		yaw			= packet.r< float >( );
	look_pitch					= packet.r< float >( );
	transform					= math::create_rotation_y( yaw );
	transform.c.xyz( )			= position;
}

void player_state::serialize( network_core::udp_match_packet& packet ) const
{
	packet.append		( transform.c.xyz( ) );
	packet.append		( transform.get_angles( math::rotation_zxy ).y );
	packet.append		( look_pitch );
}

} // namespace survarium
