////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/player_state.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

// STATE[PARTIAL]: read position(float3) + yaw(float) + look_pitch(float), then
// rebuild transform = create_rotation_y(yaw) with translation = position.
void player_state::deserialize( network_core::packet_reader& packet )
{
	float3 const&	position	= packet.r< math::float3 >( );
	float			yaw			= packet.r< float >( );
	look_pitch					= packet.r< float >( );
	transform					= math::create_rotation_y( yaw );
	transform.c.xyz( )			= position;
}

// STATE[PARTIAL]: append translation(float3), yaw from get_angles().y, look_pitch.
void player_state::serialize( network_core::udp_match_packet& packet ) const
{
	packet.append		( transform.c.xyz( ) );
	packet.append		( transform.get_angles( math::rotation_zxy ).y );
	packet.append		( look_pitch );
}

} // namespace survarium
