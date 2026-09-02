// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/character_recoil_params.h>

namespace survarium {

character_recoil_params::character_recoil_params( ) :
	crouch_multiplier		( 1.0f ),
	stand_multiplier		( 1.0f ),
	aimed_crouch_multiplier	( 1.0f ),
	aimed_stand_multiplier	( 1.0f )
{
}

void character_recoil_params::load( configs::binary_config_value const& cfg )
{
	if ( cfg.value_exists( "crouch_multiplier" ) )
		crouch_multiplier = (float)cfg["crouch_multiplier"];

	if ( cfg.value_exists( "stand_multiplier" ) )
		stand_multiplier = (float)cfg["stand_multiplier"];

	if ( cfg.value_exists( "aimed_crouch_multiplier" ) )
		aimed_crouch_multiplier = (float)cfg["aimed_crouch_multiplier"];

	if ( cfg.value_exists( "aimed_stand_multiplier" ) )
		aimed_stand_multiplier = (float)cfg["aimed_stand_multiplier"];
}

} // namespace survarium
