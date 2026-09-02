// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/character_dispersion_params.h>

namespace survarium {

character_dispersion_params::character_dispersion_params( ) :
	idle_multiplier				( 1.0f ),
	idle_aim_multiplier			( 1.0f ),
	walk_multiplier				( 1.0f ),
	walk_aim_multiplier			( 1.0f ),
	run_multiplier				( 1.0f ),
	jump_multiplier				( 1.0f ),
	crouch_multiplier			( 1.0f ),
	crouch_aim_multiplier			( 1.0f ),
	crouch_walk_multiplier			( 1.0f ),
	crouch_walk_aim_multiplier		( 1.0f ),
	prone_multiplier			( 1.0f ),
	prone_aim_multiplier			( 1.0f ),
	injury_penalty_for_double_handed	( 1.0f ),
	injury_penalty_for_one_handed		( 1.0f )
{
}

void character_dispersion_params::load( configs::binary_config_value const& cfg )
{
	if ( cfg.value_exists( "idle_multiplier" ) )
		idle_multiplier = (float)cfg["idle_multiplier"];

	if ( cfg.value_exists( "idle_aim_multiplier" ) )
		idle_aim_multiplier = (float)cfg["idle_aim_multiplier"];

	if ( cfg.value_exists( "walk_multiplier" ) )
		walk_multiplier = (float)cfg["walk_multiplier"];

	if ( cfg.value_exists( "walk_aim_multiplier" ) )
		walk_aim_multiplier = (float)cfg["walk_aim_multiplier"];

	if ( cfg.value_exists( "run_multiplier" ) )
		run_multiplier = (float)cfg["run_multiplier"];

	if ( cfg.value_exists( "jump_multiplier" ) )
		jump_multiplier = (float)cfg["jump_multiplier"];

	if ( cfg.value_exists( "crouch_multiplier" ) )
		crouch_multiplier = (float)cfg["crouch_multiplier"];

	if ( cfg.value_exists( "crouch_aim_multiplier" ) )
		crouch_aim_multiplier = (float)cfg["crouch_aim_multiplier"];

	if ( cfg.value_exists( "crouch_walk_multiplier" ) )
		crouch_walk_multiplier = (float)cfg["crouch_walk_multiplier"];

	if ( cfg.value_exists( "crouch_walk_aim_multiplier" ) )
		crouch_walk_aim_multiplier = (float)cfg["crouch_walk_aim_multiplier"];

	if ( cfg.value_exists( "prone_multiplier" ) )
		prone_multiplier = (float)cfg["prone_multiplier"];

	if ( cfg.value_exists( "prone_aim_multiplier" ) )
		prone_aim_multiplier = (float)cfg["prone_aim_multiplier"];

	if ( cfg.value_exists( "injury_penalty_for_double_handed" ) )
		injury_penalty_for_double_handed = (float)cfg["injury_penalty_for_double_handed"];

	if ( cfg.value_exists( "injury_penalty_for_one_handed" ) )
		injury_penalty_for_one_handed = (float)cfg["injury_penalty_for_one_handed"];
}

} // namespace survarium
