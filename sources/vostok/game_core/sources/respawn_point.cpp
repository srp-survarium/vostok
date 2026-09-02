// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/respawn_point_core.h>

namespace survarium {

respawn_point_core::respawn_point_core( ) :
	point_id				( u32(-1) ),
	position				( 0.0f, 0.0f, 0.0f ),
	orientation				( 0.0f ),
	point_priority			( 0 ),
	team_owner				( team_1 ),
	selected_for_respawn	( 0 )
{
}

void respawn_point_core::load( configs::binary_config_value const& config )
{
	point_id		= (u32)config["point_id"];
	point_priority	= (u32)config["priority"];
	position		= config["position"];
	orientation		= math::create_rotation( (float3)config["rotation"] ).get_angles( math::rotation_zxy ).y;;
	team_owner		= (game_team_id)(u32)config["team"];
}

} // namespace survarium
