// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_material.h>

namespace survarium {

// game_id -> physics_group
u16 g_material_physics_group[64] = {};

game_material::game_material( )	:
	m_name							( "default" ),
	m_material_resistance			( 50.0f ),
	m_bullet_reflection_speed_down	( 1.0f ),
	m_width							( 0.1f ),
	m_ricochet_koef					( 0.3f ),
	m_id							( u16(-1) ),
	m_mine_can_place				( false ),
	m_mine_can_stick				( false )
{
}

void game_material::load_from_config( configs::binary_config_value const& val )
{
	m_id							= (u16)val["id"];
	m_name							= (pcstr)val["name"];	// sushi@MATCH: Target uses stack for whatever reason
	m_material_resistance			= (float)val["physic"]["resistance"];
	m_bullet_reflection_speed_down	= (float)val["physic"]["reflection_speed_down"];
	m_width							= (float)val["physic"]["width"];
	m_ricochet_koef					= (float)val["physic"]["k_ricochet"];

	u16 physics_group = 0;

	m_mine_can_place				= (bool)val["mine"]["can_place"];
	m_mine_can_stick				= (bool)val["mine"]["can_stick"];

	if ( (bool)val["mine"]["hittable"] )
		physics_group |= 0x8;

	if ( (bool)val["mine"]["walkable"] )
		physics_group |= 0x2;

	g_material_physics_group[m_id]	= physics_group;

	ASSERT( UNKNOWN_EXPRESSION );
}

} // namespace survarium
