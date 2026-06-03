////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/respawn_point_core.h>

namespace survarium {

// STATE[UNCHECKED]
respawn_point_core::respawn_point_core( ) :
	point_id				( u32(-1) ),
	orientation				( 0.0f ),
	point_priority			( 0 ),
	team_owner				( team_1 ),
	selected_for_respawn	( 0 )
{
}

// STATE[99.60%|DONE]
void respawn_point_core::load( configs::binary_config_value const& config )
{
	point_id		= (u32)config["point_id"];
	point_priority	= (u32)config["priority"];
	position		= config["position"];
	orientation		= math::create_rotation( (float3)config["rotation"] ).get_angles( math::rotation_zxy ).y;;
	team_owner		= (game_team_id)(u32)config["team"];


	// FUNCTION BODY[0x6fea60]: 5
	// <0x6fea71>|0x011|+0x01b:'23'
	// <0x6fea8c>|0x02c|+0x01b:'24'
	// <0x6feaa7>|0x047|+0x02b:'25'
	// <0x6fead2>|0x072|+0x049:'26'
	// <0x6feb1b>|0x0bb|+0x01b:'27'
	// ******
}

} // namespace survarium
