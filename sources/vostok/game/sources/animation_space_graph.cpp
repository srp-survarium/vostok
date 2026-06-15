////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "animation_space_graph.h"
#include "animation_space_vertex.h"

namespace survarium {

// STATE[STUB]
 animation_space_vertex::animation_space_vertex( resources::managed_resource_ptr const& animation_vertex, pcstr animation_caption )
{
	// FUNCTION BODY[0x7659c0]: 0
	// <0x7659c0>|0x000|+0x0a3:'25'	{
	// <0x765a63>|0x0a3|      :'26'	}
	// ******
}

// STATE[STUB]
 animation_space_graph::animation_space_graph(
	ai::navigation::world&		navigation_world,
	const float					agent_radius,
	const u32					animations_count,
	const u32					mixes_count,
	const u32					edges_count
) :
	// the ref/const members force the init-list; the same-named params are the
	// obvious sources - a matcher confirms when this TU is enabled
	m_navigation_world( navigation_world ),
	m_agent_radius( agent_radius ),
	m_animations_count( animations_count ),
	m_mixes_count( mixes_count ),
	m_edges_count( edges_count )
{
	// FUNCTION BODY[0x765960]: 0
	// <0x765960>|0x000|+0x057:'41'	{
	// <0x7659b7>|0x057|      :'42'	}
	// ******
}

// STATE[STUB]
 animation_space_graph::~animation_space_graph( )
{
	// FUNCTION BODY[0x765950]: 0
	// <0x765950>|0x000|+0x006:'45'	{
	// <0x765956>|0x006|      :'46'	}
	// ******
}

// STATE[STUB]
animation_space_vertex const* animation_space_graph::get_animation_by_path( pcstr animation_path ) const
{
	return NULL;

	// FUNCTION BODY[0x7658f0]: 8
	// <0>
	// <0x7658f2>|0x002|+0x014:'51'
	// <0>
	// <0x765906>|0x016|+0x00a:'53'
	// <0x765910>|0x020|+0x037:'54'
	// <0>
	// <1>
	// <0x765947>|0x057|+0x004:'57'
	// ******
}

// STATE[STUB]
animation_space_vertex_id animation_space_graph::get_movement(
	animation::animation_player&		player,
	animation_space_vertex const*		left_animation,
	animation_space_vertex const*		right_animation,
	const float							left_weight
)
{
	// LOCALS
	// mutable_buffer 					buffer
	// animation::mixing::animation_lexeme right_lexeme
	// animation::mixing::weight_lexeme left_weight_lexeme
	// animation::mixing::animation_lexeme left_lexeme
	// ******

	// CALL SITE INFO
	// <0x765b7c> -> animation::base_interpolator* < unknown >( mutable_buffer& ) const
	// ******

	// FUNCTION BODY[0x765a70]: 19
	// <0>
	// <1>
	// <0x765a79>|0x009|+0x00f:'83'
	// <0>
	// <1>
	// <0x765a88>|0x018|+0x01a:'86'
	// <0>
	// <0x765aa2>|0x032|+0x0cc:'88'
	// <0x765b6e>|0x0fe|+0x019:'89'
	// <0x765b87>|0x117|+0x0a7:'90'
	// <0>
	// <0x765c2e>|0x1be|+0x07e:'92'
	// <0x765cac>|0x23c|+0x04d:'93'
	// <0x765cf9>|0x289|+0x011:'94'
	// <0>
	// <1>
	// <0x765d0a>|0x29a|+0x029:'97'
	// <0>
	// <0x765d33>|0x2c3|+0x03d:'99'
	// ******
}

// STATE[STUB]
animation_space_edge const& animation_space_graph::edge( const u32 index ) const
{
	// FUNCTION BODY[0x7658c0]: 4
	// <0>
	// <1>
	// <2>
	// <0x7658c0>|0x000|+0x020:'107'
	// ******
}

// STATE[STUB]
float animation_space_graph::max_speed( ) const
{
	// LOCALS
	// animation::animation_player 		player
	// ******

	return 0.0f;

	// FUNCTION BODY[0x765d80]: 16
	// <0x765d92>|0x012|+0x011:'158'
	// <0x765da3>|0x023|+0x006:'159'
	// <0>
	// <0x765da9>|0x029|+0x009:'161'
	// <0>
	// <1>
	// <2>
	// <0x765db2>|0x032|+0x01f:'165'
	// <0>
	// <0x765dd1>|0x051|+0x008:'167'
	// <0>
	// <0x765dd9>|0x059|+0x03a:'169'
	// <0x765e13>|0x093|+0x073:'170'
	// <0>
	// <1>
	// <0x765e86>|0x106|+0x039:'173'
	// ******
}

} // namespace survarium
