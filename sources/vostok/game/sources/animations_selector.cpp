////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "animations_selector.h"

namespace survarium {

// STATE[STUB]
 animations_selector::animations_selector(
	animation::animation_player&		player,
	animation_space_graph_ptr const&	space_graph,
	resources::managed_resource_ptr const&	default_animation,
	ai::navigation::world const&		ai_navigation_world,
	game_world const&					world,
	human_npc&							owner
) :
	// ref members + the controllers' non-default ctors force the init-list;
	// the same-named params are the obvious sources - a matcher confirms
	m_single_position_animation_controller( space_graph, ai_navigation_world, owner ),
	m_simple_animation_controller( owner ),
	m_animation_player( player ),
	m_game_world( world ),
	m_owner( owner )
{
	// FUNCTION BODY[0x5bc390]: 5
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5bc433>|0x0a3|+0x071:'44'
	// ******
}

// STATE[STUB]
void animations_selector::set_animation_player_target( animation::mixing::expression const& target_expression, const u32 time_in_ms )
{
	// LOCALS
	// mutable_buffer 					buffer
	// animation::mixing::animation_lexeme lexeme
	// ******

	// FUNCTION BODY[0x5bc0a0]: 10
	// <0x5bc0a0>|0x000|+0x00a:'48'	{
	// <0>
	// <1>
	// <0x5bc0aa>|0x00a|+0x00d:'51'
	// <0x5bc0b7>|0x017|+0x02e:'52'
	// <0>
	// <1>
	// <0x5bc0e5>|0x045|+0x01a:'55'
	// <0x5bc0ff>|0x05f|+0x0df:'56'
	// <0x5bc1de>|0x13e|+0x049:'57'
	// <0x5bc227>|0x187|-0x151:'58'
	// <0x5bc0d6>|0x036|+0x15c:'59'
	// <0x5bc232>|0x192|      :'59'	}
	// ******
}

// STATE[STUB]
void animations_selector::reset_animation_controller( const u32 time_in_ms )
{
	// LOCALS
	// mutable_buffer 					buffer
	// animation::mixing::expression 	expression
	// ******

	// CALL SITE INFO
	// <0x5bc293> -> animation::mixing::expression < unknown >( base_animation_controller&, mutable_buffer& )
	// <0x5bc2c9> -> void < unknown >()
	// <0x5bc2dd> -> void < unknown >( animation_controller_parameters const& )
	// <0x5bc2e9> -> void < unknown >()
	// <0x5bc2ff> -> animation::mixing::expression < unknown >( mutable_buffer& )
	// ******

	// FUNCTION BODY[0x5bc250]: 23
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5bc257>|0x007|+0x01a:'67'
	// <0x5bc271>|0x021|+0x010:'68'
	// <0>
	// <0x5bc281>|0x031|+0x004:'70'
	// <0>
	// <0x5bc285>|0x035|+0x010:'72'
	// <0x5bc295>|0x045|+0x00d:'73'
	// <0>
	// <0x5bc2a2>|0x052|+0x004:'75'
	// <0x5bc2a6>|0x056|+0x002:'76'
	// <0>
	// <0x5bc2a8>|0x058|+0x011:'78'
	// <0>
	// <0x5bc2b9>|0x069|+0x00c:'80'
	// <0x5bc2c5>|0x075|+0x006:'81'
	// <0>
	// <0x5bc2cb>|0x07b|+0x014:'83'
	// <0x5bc2df>|0x08f|+0x00c:'84'
	// <0x5bc2eb>|0x09b|+0x034:'85'
	// ******
}

// STATE[STUB]
animation::callback_return_type_enum animations_selector::on_animation_interval_end( animation::animation_callback_params& params )
{
	// CALL SITE INFO
	// <0x5bc377> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x5bc360]: 4
	// <0x5bc362>|0x002|+0x00a:'90'
	// <0x5bc36c>|0x00c|+0x00d:'91'
	// <0x5bc379>|0x019|+0x008:'92'
	// <0>
	// ******
}

// STATE[STUB]
void animations_selector::on_set_target( )
{
	// FUNCTION BODY[0x5bc330]: 2
	// <0x5bc334>|0x004|+0x00a:'98'
	// <0x5bc33e>|0x00e|+0x017:'99'
	// ******
}

// STATE[STUB]
void animations_selector::set_target( ai::animation_item const& animation_emitter )
{
	// FUNCTION BODY[0x5bc5b0]: 6
	// <0x5bc5b0>|0x000|+0x078:'104'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5bc628>|0x078|+0x038:'109'
	// ******
}

// STATE[STUB]
void animations_selector::set_target( ai::movement_target const& target_position )
{
	// FUNCTION BODY[0x5bc4b0]: 9
	// <0x5bc4b0>|0x000|+0x019:'114'
	// <0x5bc4c9>|0x019|+0x016:'115'
	// <0x5bc4df>|0x02f|+0x016:'116'
	// <0x5bc4f5>|0x045|+0x077:'117'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5bc56c>|0x0bc|+0x032:'122'
	// ******
}

// STATE[STUB]
void animations_selector::debug_draw( render::game::renderer& render, render::scene_ptr const& scene ) const
{
	// CALL SITE INFO
	// <0x5bc09a> -> void < unknown >( render::game::renderer&, render::base_scene_ptr const& ) const
	// ******

	// FUNCTION BODY[0x5bc080]: 2
	// <0x5bc080>|0x000|+0x009:'127'
	// <0x5bc089>|0x009|+0x013:'128'
	// ******
}

} // namespace survarium
