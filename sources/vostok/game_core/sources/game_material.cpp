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
	// STRUCTURE DIFF: target 0 stmts / base 0 stmts (member-init only; 0x7f vs 0x7d bytes)
	// VERDICT: STRUCTURE MATCH (shape ok) - residual is the m_name fixed_string("default")
	// ctor materialization frame-slot, non-steerable.
}

// STATE[96.23%|PARTIAL]: binary_config_value access + buffer_string inline-vs-call (LTCG), shape matches
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

	// STRUCTURE DIFF: target 15 stmts / base 15 stmts (no diverging rows, 0x1b6 bytes BOTH)
	// VERDICT: STRUCTURE MATCH - clean skeleton, equal byte counts; the % residual is
	// relocation pairing only (binary_config_value operator[]/cast + buffer_string assign
	// fold names), non-steerable.
}

} // namespace survarium
