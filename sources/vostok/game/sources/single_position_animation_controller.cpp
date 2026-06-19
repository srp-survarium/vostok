////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "single_position_animation_controller.h"
// mixing::expression returned by value -> needs the complete type at the definition
#include <vostok/animation/mixing_expression.h>
// resource_ptr<animation_space_graph> dtor instantiates ->destroy() -> needs complete type
#include "animation_space_graph.h"

namespace survarium {

// STATE[STUB]
 single_position_animation_controller::single_position_animation_controller(
	animation_space_graph_ptr const&	graph,
	ai::navigation::world const&		ai_navigation_world,
	human_npc&							owner
) :
	// the ref members + allocator-taking vectora force the init-list; the
	// same-named params / module allocator are the obvious sources - a matcher
	// confirms when this TU is enabled
	m_ai_navigation_world( ai_navigation_world ),
	m_owner( owner ),
	m_navigation_path( g_allocator )
{
	// FUNCTION BODY[0x7769b0]: 0
	// <0x7769b0>|0x000|+0x085:'37'	{
	// <0x776a35>|0x085|      :'38'	}
	// ******
}

// STATE[STUB]
 single_position_animation_controller::~single_position_animation_controller( )
{
	// FUNCTION BODY[0x7768d0]: 2
	// <0x7768d1>|0x001|+0x03c:'42'
	// <0x77690d>|0x03d|+0x015:'43'
	// ******
}

// STATE[STUB]
void single_position_animation_controller::initialize( )
{
	// CALL SITE INFO
	// <0x77683d> -> float3 < unknown >( float3 const& ) const
	// ******

	// FUNCTION BODY[0x7767c0]: 2
	// <0x7767c9>|0x009|+0x04d:'48'
	// <0x776816>|0x056|+0x03a:'49'
	// ******
}

// STATE[STUB]
animation::mixing::expression single_position_animation_controller::try_finalize( base_animation_controller& next_controller, mutable_buffer& buffer )
{
	// claude@NOTE: buildability return only - force-codegen'd by anchor_game_npc.cpp.
	return *( animation::mixing::expression* )0;

	// FUNCTION BODY[0x776860]: 2
	// <0>
	// <0x776860>|0x000|+0x011:'55'
	// ******
}

// STATE[STUB]
void single_position_animation_controller::query_new_target_if_needed( )
{
	// CALL SITE INFO
	// <0x7768b4> -> void < unknown >()
	// <0x7768c0> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x776880]: 6
	// <0x776883>|0x003|+0x021:'60'
	// <0>
	// <0x7768a4>|0x024|+0x008:'62'
	// <0x7768ac>|0x02c|+0x00a:'63'
	// <0x7768b6>|0x036|+0x00d:'64'
	// <0>
	// ******
}

// STATE[STUB]
animation::mixing::expression single_position_animation_controller::selected_animations( mutable_buffer& buffer )
{
	// claude@NOTE: buildability return only - force-codegen'd by anchor_game_npc.cpp;
	// body is the create_animation_interval lexeme wall (a matcher recovers it).
	return *( animation::mixing::expression* )0;

	// LOCALS
	// float3 							target_position
	// float3 const& 					movement_position
	// vector< u32 > 					path
	// const float 						time_scale
	// animation::mixing::animation_lexeme right_animation
	// animation_space_vertex_id 		start_vertex_id
	// animation::mixing::weight_lexeme left_weight
	// animation::mixing::animation_lexeme left_animation
	// const u32 						target_node_id
	// u32 								i
	// const float 						previous_to_current_length
	// animation::mixing::animation_lexeme lexeme
	// ******

	// CALL SITE INFO
	// <0x776c62> -> float3 < unknown >( float3 const& ) const
	// <0x776ca7> -> u32 < unknown >( float3 const& ) const
	// <0x776cba> -> u32 < unknown >( float3 const& ) const
	// <0x776cf5> -> bool < unknown >( u32, float3 const&, u32, float3 const&, float, vectora< float3 >& ) const
	// ******

	// FUNCTION BODY[0x776bc0]: 133
	// <0>
	// <1>
	// <2>
	// <0x776bd3>|0x013|+0x01d:'73'
	// <0x776bf0>|0x030|+0x009:'74'
	// <0>
	// <0x776bf9>|0x039|+0x02c:'76'
	// <0>
	// <1>
	// <0x776c25>|0x065|+0x03f:'79'
	// <0>
	// <0x776c64>|0x0a4|+0x024:'81'
	// <0>
	// <0x776c88>|0x0c8|+0x012:'83'
	// <0x776c9a>|0x0da|+0x00f:'84'
	// <0x776ca9>|0x0e9|+0x017:'85'
	// <0x776cc0>|0x100|+0x05b:'86'
	// <0x776d1b>|0x15b|-0x051:'87'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x776cca>|0x10a|+0x02d:'97'
	// <0x776cf7>|0x137|+0x02e:'98'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x776d25>|0x165|+0x00b:'104'
	// <0x776d30>|0x170|+0x00e:'105'
	// <0x776d3e>|0x17e|+0x005:'106'
	// <0>
	// <1>
	// <2>
	// <0x776d43>|0x183|+0x1d2:'110'
	// <0x776f15>|0x355|-0x1a5:'110'
	// <0>
	// <0x776d70>|0x1b0|+0x00c:'112'
	// <0>
	// <0x776d7c>|0x1bc|+0x063:'114'
	// <0x776ddf>|0x21f|+0x04a:'115'
	// <0x776e29>|0x269|+0x023:'116'
	// <0x776e4c>|0x28c|+0x06b:'117'
	// <0>
	// <0x776eb7>|0x2f7|+0x07c:'119'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x776f33>|0x373|+0x024:'126'
	// <0>
	// <0x776f57>|0x397|+0x0e2:'128'
	// <0>
	// <0x777039>|0x479|+0x018:'130'
	// <0>
	// <0x777051>|0x491|+0x002:'132'
	// <0x777053>|0x493|+0x016:'133'
	// <0>
	// <1>
	// <0x777069>|0x4a9|+0x011:'136'
	// <0>
	// <1>
	// <0x77707a>|0x4ba|+0x04c:'139'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7770c6>|0x506|+0x0aa:'144'
	// <0>
	// <0x777170>|0x5b0|+0x0cd:'146'
	// <0x77723d>|0x67d|-0x09d:'146'
	// <0>
	// <0x7771a0>|0x5e0|+0x006:'148'
	// <0x7771a6>|0x5e6|+0x018:'149'
	// <0x7771be>|0x5fe|+0x0a3:'150'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x777261>|0x6a1|+0x024:'156'
	// <0>
	// <1>
	// <0x777285>|0x6c5|+0x01d:'159'
	// <0>
	// <0x7772a2>|0x6e2|+0x12c:'161'
	// <0x7773ce>|0x80e|+0x01b:'162'
	// <0>
	// <1>
	// <2>
	// <0x7773e9>|0x829|+0x016:'166'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7773ff>|0x83f|+0x054:'171'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <0x777453>|0x893|+0x1bb:'184'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x77760e>|0xa4e|+0x067:'189'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x777675>|0xab5|+0x1e9:'201'
	// <0x77785e>|0xc9e|+0x0da:'202'
	// ******
}

// STATE[STUB]
void single_position_animation_controller::set_target( animation_controller_parameters const& target )
{
	m_target_parameters												= static_cast_checked< movement_animation_controller_parameters const& >( target );

	// FUNCTION BODY[0x776a50]: 1
	// <0x776a50>|0x000|+0x00e:'207'
	// ******
}

// STATE[STUB]
void single_position_animation_controller::debug_draw( render::game::renderer& render, render::scene_ptr const& scene ) const
{
	// LOCALS
	// float3 const* 					e
	// ******

	// FUNCTION BODY[0x776a70]: 13
	// <0x776a80>|0x010|+0x00c:'212'
	// <0>
	// <1>
	// <0x776a8c>|0x01c|+0x091:'215'
	// <0>
	// <1>
	// <2>
	// <0x776b1d>|0x0ad|+0x014:'219'
	// <0>
	// <0x776b31>|0x0c1|+0x04e:'221'
	// <0x776b7f>|0x10f|+0x004:'222'
	// <0x776b83>|0x113|+0x02e:'223'
	// <0>
	// ******
}

} // namespace survarium
