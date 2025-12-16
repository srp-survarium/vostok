////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/player_input.h>

namespace survarium {

// STATE[100%|DONE]
player_input::player_input( ) :
	angular_velocity	( 0.0f, 0.0f ),
	angular_acceleration( 0.0f, 0.0f ),
	actions_mask		( 0 )
{
}

// STATE[BLOCKED]
void player_input::serialize( network_core::udp_match_packet& packet ) const
{
	// FUNCTION BODY
	// <0x700e80>|0x000|+0x009:'22'	{
	// <0x700e89>|0x009|+0x00b:'23'
	// <0x700e94>|0x014|+0x00e:'24'
	// <0x700ea2>|0x022|+0x00f:'25'
	// <0x700eb1>|0x031|      :'26'	}
	// ******
}

// STATE[BLOCKED]
void player_input::deserialize( network_core::packet_reader& reader )
{
	// FUNCTION BODY
	// <0x700df0>|0x000|+0x00b:'29'	{
	// <0x700dfb>|0x00b|+0x032:'30'
	// <0x700e2d>|0x03d|+0x033:'31'
	// <0x700e60>|0x070|+0x00e:'32'
	// <0x700e6e>|0x07e|      :'33'	}
	// ******
}

// STATE[100%|DONE]
bool player_input::is_empty( ) const
{
	return math::is_zero( angular_velocity.x )
		&& math::is_zero( angular_velocity.y )
		&& math::is_zero( angular_acceleration.x )
		&& math::is_zero( angular_acceleration.y )
		&& !actions_mask
		&& true; // sushi@NOTE: master_gold check

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x700d69>|0x009|+0x07e:'56'
	// ******
}

} // namespace survarium
