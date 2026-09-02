// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/weapon_recoil_params.h>

namespace survarium {

// claude@MATCH: additive_back_recoil (offset 0x20) is omitted - the target never
// stores it (no movss [this+20h]); additive_recoil_time is math::epsilon_3 (.001f).
weapon_recoil_params::weapon_recoil_params( ) :
	first_shoot_side_recoil		( 0.0f ),
	shoot_side_recoil		( 0.0f ),
	first_shoot_back_recoil		( 0.0f ),
	shoot_back_recoil		( 0.0f ),
	shoot_recoil_min_angle		( 0.0f ),
	shoot_recoil_angle_range	( 0.0f ),
	additive_recoil_time		( math::epsilon_3 ),
	additive_side_recoil		( 0.0f ),
	additive_recoil_min_angle	( 0.0f ),
	additive_recoil_angle_range	( 0.0f ),
	side_compensation_speed		( 0.0f ),
	back_compensation_speed		( 0.0f )
{
}

// claude@MATCH: the member-init list omits first_shoot_back_recoil (0x08),
// shoot_back_recoil (0x0c), additive_back_recoil (0x20) and back_compensation_speed
// (0x30) - only 9 of the 12 members are initialized here (the default ctor inits all
// but 0x20); reproduced verbatim from the target's prologue stores.
weapon_recoil_params::weapon_recoil_params( configs::binary_config_value const& cfg ) :
	first_shoot_side_recoil		( 0.0f ),
	shoot_side_recoil		( 0.0f ),
	shoot_recoil_min_angle		( 0.0f ),
	shoot_recoil_angle_range	( 0.0f ),
	additive_recoil_time		( math::epsilon_3 ),
	additive_side_recoil		( 0.0f ),
	additive_recoil_min_angle	( 0.0f ),
	additive_recoil_angle_range	( 0.0f ),
	side_compensation_speed		( 0.0f )
{
	if ( cfg.value_exists( "first_shoot_side_recoil" ) )
		first_shoot_side_recoil = (float)cfg["first_shoot_side_recoil"];

	if ( cfg.value_exists( "shoot_side_recoil" ) )
		shoot_side_recoil = (float)cfg["shoot_side_recoil"];

	if ( cfg.value_exists( "first_shoot_back_recoil" ) )
		first_shoot_back_recoil = (float)cfg["first_shoot_back_recoil"];

	if ( cfg.value_exists( "shoot_back_recoil" ) )
		shoot_back_recoil = (float)cfg["shoot_back_recoil"];

	if ( cfg.value_exists( "shoot_recoil_min_angle" ) && cfg.value_exists( "shoot_recoil_max_angle" ) )
	{
		shoot_recoil_min_angle = (float)cfg["shoot_recoil_min_angle"];
		shoot_recoil_angle_range = (float)cfg["shoot_recoil_max_angle"] - shoot_recoil_min_angle;
	}

	if ( cfg.value_exists( "additive_recoil_time" ) )
		additive_recoil_time = (float)cfg["additive_recoil_time"];

	if ( cfg.value_exists( "additive_side_recoil" ) )
		additive_side_recoil = (float)cfg["additive_side_recoil"];

	if ( cfg.value_exists( "additive_back_recoil" ) )
		additive_back_recoil = (float)cfg["additive_back_recoil"];

	if ( cfg.value_exists( "additive_recoil_min_angle" ) && cfg.value_exists( "additive_recoil_max_angle" ) )
	{
		additive_recoil_min_angle = (float)cfg["additive_recoil_min_angle"];
		additive_recoil_angle_range = (float)cfg["additive_recoil_max_angle"] - additive_recoil_min_angle;
	}

	if ( cfg.value_exists( "side_compensation_speed" ) )
		side_compensation_speed = (float)cfg["side_compensation_speed"];

	if ( cfg.value_exists( "back_compensation_speed" ) )
		back_compensation_speed = (float)cfg["back_compensation_speed"];
}

} // namespace survarium
