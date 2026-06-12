////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/player_input.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

player_input::player_input( ) :
	angular_velocity	( 0.0f, 0.0f ),
	angular_acceleration( 0.0f, 0.0f ),
	actions_mask		( 0 )
{
}

void player_input::serialize( network_core::udp_match_packet& packet ) const
{
	packet.append		( angular_velocity );
	packet.append		( angular_acceleration );
	packet.append		( actions_mask );
}

void player_input::deserialize( network_core::packet_reader& reader )
{
	angular_velocity		= reader.r< math::float2 >( );
	angular_acceleration	= reader.r< math::float2 >( );
	actions_mask			= reader.r< u32 >( );
}

bool player_input::is_empty( ) const
{
	return math::is_zero( angular_velocity.x )
		&& math::is_zero( angular_velocity.y )
		&& math::is_zero( angular_acceleration.x )
		&& math::is_zero( angular_acceleration.y )
		&& !actions_mask
		&& true; // sushi@NOTE: master_gold check
}

} // namespace survarium
