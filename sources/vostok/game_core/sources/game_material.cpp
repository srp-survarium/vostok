////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/game_material.h>

static float s_aim_transition_time = 0.3f;

namespace survarium {

// game_id -> physics_group
u16 g_material_physics_group[64] = {};

// STATE[83.44%|DONE]: LTCG for fixed_string constructor
game_material::game_material( )	:
	m_name							( "default" ),
	m_material_resistance			( 50.0f ),
	m_bullet_reflection_speed_down	( 1.0f ),
	m_width							( 0.1f ),
	m_ricochet_koef					( s_aim_transition_time ),
	m_id							( u16(-1) ),
	m_mine_can_place				( false ),
	m_mine_can_stick				( false )
{
}

// STATE[88.10%|PARTIAL]: LTCG for binary_config_value access, also buffer_string
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

	// FUNCTION BODY
	// <0x760399>|0x009|+0x019:'28'
	// <0x7603b2>|0x022|+0x021:'29'
	// <0x7603d3>|0x043|+0x026:'30'
	// <0x7603f9>|0x069|+0x026:'31'
	// <0x76041f>|0x08f|+0x026:'32'
	// <0x760445>|0x0b5|+0x026:'33'
	// <0>
	// <0x76046b>|0x0db|+0x006:'35'
	// <0>
	// <0x760471>|0x0e1|+0x026:'37'
	// <0x760497>|0x107|+0x026:'38'
	// <0>
	// <0x7604bd>|0x12d|+0x027:'40'
	// <0x7604e4>|0x154|+0x00b:'41'
	// <0>
	// <0x7604ef>|0x15f|+0x027:'43'
	// <0x760516>|0x186|+0x00b:'44'
	// <0>
	// <0x760521>|0x191|+0x013:'46'
	// <0>
	// <0x760534>|0x1a4|+0x00c:'48'
	// ******
}

} // namespace survarium
