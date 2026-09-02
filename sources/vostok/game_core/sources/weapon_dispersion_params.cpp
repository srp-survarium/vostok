// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/weapon_dispersion_params.h>

namespace survarium {

weapon_dispersion_params::weapon_dispersion_params( ) :
	base_dispersion				( 0.0f ),
	from_the_hip_multiplier		( 1.0f ),
	aim_multiplier				( 1.0f ),
	speed_of_aiming				( 1.0f ),
	one_shoot_dispersion_amount	( 1.0f ),
	reload_dispersion_amount	( 1.0f ),
	growth_speed				( 1.0f ),
	max_dispersion				( 2.0f )
{
}

weapon_dispersion_params::weapon_dispersion_params( configs::binary_config_value const& cfg ) :
	base_dispersion				( 0.0f ),
	from_the_hip_multiplier		( 1.0f ),
	aim_multiplier				( 1.0f ),
	speed_of_aiming				( 1.0f ),
	one_shoot_dispersion_amount	( 1.0f ),
	reload_dispersion_amount	( 1.0f ),
	growth_speed				( 1.0f ),
	max_dispersion				( 2.0f )
{
	if ( cfg.value_exists( "base_dispersion" ) )
		base_dispersion = (float)cfg["base_dispersion"];

	if ( cfg.value_exists( "from_the_hip_multiplier" ) )
		from_the_hip_multiplier = (float)cfg["from_the_hip_multiplier"];

	if ( cfg.value_exists( "aim_multiplier" ) )
		aim_multiplier = (float)cfg["aim_multiplier"];

	if ( cfg.value_exists( "speed_of_aiming" ) )
		speed_of_aiming = (float)cfg["speed_of_aiming"];

	if ( cfg.value_exists( "one_shoot_dispersion_amount" ) )
		one_shoot_dispersion_amount = (float)cfg["one_shoot_dispersion_amount"];

	if ( cfg.value_exists( "reload_dispersion_amount" ) )
		reload_dispersion_amount = (float)cfg["reload_dispersion_amount"];

	if ( cfg.value_exists( "growth_speed" ) )
		growth_speed = (float)cfg["growth_speed"];

	if ( cfg.value_exists( "max_dispersion" ) )
		max_dispersion = (float)cfg["max_dispersion"];

	// claude@MATCH: the target unconditionally clears this AFTER the if-blocks,
	// discarding any "one_shoot_dispersion_amount" the config just read (matched verbatim,
	// not a fix: movss from .rdata 0.0f at target .text 0x22d -> [this+0x10]).
	one_shoot_dispersion_amount = 0.0f;
}

} // namespace survarium
