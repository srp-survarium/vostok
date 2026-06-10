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
	// FUNCTION BODY
	// <0x5a1020>|0x000|+0x046:'16'	{
	// <0x5a1066>|0x046|      :'17'	}
	// ******
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


	// FUNCTION BODY
	// <0x5a1079>|0x009|+0x014:'21'
	// <0x5a108d>|0x01d|+0x019:'22'
	// <0>
	// <0x5a10a6>|0x036|+0x014:'24'
	// <0x5a10ba>|0x04a|+0x01a:'25'
	// <0>
	// <0x5a10d4>|0x064|+0x014:'27'
	// <0x5a10e8>|0x078|+0x01a:'28'
	// <0>
	// <0x5a1102>|0x092|+0x014:'30'
	// <0x5a1116>|0x0a6|+0x01a:'31'
	// <0>
	// ******
}

} // namespace survarium
