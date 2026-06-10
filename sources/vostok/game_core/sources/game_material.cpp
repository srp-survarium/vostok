////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/game_material.h>

static float s_aim_transition_time = 0.3f;

namespace survarium {

// game_id -> physics_group
u16 g_material_physics_group[64] = {};

// STATE[97.78%|DONE]: fixed_string m_name ctor inline-vs-call frame-slot (LTCG), shape matches
// STRUCTURE DIFF:
// target: 0x750310            base: 0x5657b0
// ; survarium::game_material::game_material() ; target 0 stmts / base 0 stmts
// ; aligned 0, size-diffs 0, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - 0-stmt member-init, member stores byte-exact; residual is m_name fixed_string("default") ctor materialization frame-slot ([ebp-4] vs [ebp-0Ch]), non-steerable. trail: game_material.md
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

// STATE[96.23%|PARTIAL]: binary_config_value access + buffer_string inline-vs-call (LTCG), shape matches
// STRUCTURE DIFF:
// target: 0x750390            base: 0x565830
// ; void survarium::game_material::load_from_config(vostok::configs::binary_config_value const&) ; target 21 stmts / base 21 stmts
// ; aligned 21, size-diffs 0, quantity-diffs 0
// VERDICT: STRUCTURE MATCH - 21/21 stmts byte-aligned; residual is binary_config_value operator[]/cast + m_name buffer_string assign inline-vs-call, non-steerable. trail: game_material.md
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
