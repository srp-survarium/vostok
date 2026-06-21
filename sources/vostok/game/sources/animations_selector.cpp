////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "animations_selector.h"
#include "game_world.h"				// m_game_world.get_game() ...
#include "game.h"					// ... .game_time_ms() inlines the game clock
#include <vostok/animation/animation_player.h>	// subscribe( channel_id_on_animation_interval_end, ... )
#include <vostok/ai/animation_item.h>
#include <vostok/ai/movement_target.h>

namespace survarium {

// claude@NOTE: init list + the on_animation_interval_end subscribe match in shape (1 stmt,
// member set + the boost::function build). In our link env the subscribe worker is inlined
// and DCE'd (callback + empty animation temp built then destroyed, no `call subscribe`)
// while the target keeps the out-of-line subscribe call - an LTCG inlining residual.
 animations_selector::animations_selector(
	animation::animation_player&		player,
	animation_space_graph_ptr const&	space_graph,
	resources::managed_resource_ptr const&	default_animation,
	ai::navigation::world const&		ai_navigation_world,
	game_world const&					world,
	human_npc&							owner
) :
	m_single_position_animation_controller( space_graph, ai_navigation_world, owner ),
	m_simple_animation_controller( owner ),
	m_animation_player( player ),
	m_current_controller( NULL ),
	m_target_controller( NULL ),
	m_target_controller_parameters( NULL ),
	m_game_world( world ),
	m_owner( owner ),
	m_default_animation( default_animation )
{
	m_animation_player.subscribe(
		animation::channel_id_on_animation_interval_end,
		boost::bind( &animations_selector::on_animation_interval_end, this, _1 ),
		NULL,
		resources::managed_resource_ptr( ),
		this
	);
}

// STATE[STUB]
// claude@NOTE: lexeme wall - builds an animation_lexeme over a mutable_buffer from the
// target expression, clones it into the buffer, then animation_player::set_target_and_tick.
// Needs the mixing-lexeme machinery (create_animation_intervals / animation_lexeme /
// binary_tree_animation_node). Parked behind the lexeme wall.
void animations_selector::set_animation_player_target( animation::mixing::expression const& target_expression, const u32 time_in_ms )
{
}

// STATE[STUB]
// claude@NOTE: builds a mixing::expression from the current controller over a 0x4000
// mutable_buffer, swaps m_current_controller -> m_target_controller, sets the target
// params, then set_animation_player_target. Depends on set_animation_player_target
// (lexeme wall) and the controller make_expression chain. Parked behind the lexeme wall.
void animations_selector::reset_animation_controller( const u32 time_in_ms )
{
}

animation::callback_return_type_enum animations_selector::on_animation_interval_end( animation::animation_callback_params& params )
{
	params.interrupt_animation_player_tick	= true;
	m_current_controller->query_new_target_if_needed( );
	reset_animation_controller( params.callback_time_in_ms );
	return animation::callback_return_type_call_me_again;
}

void animations_selector::on_set_target( )
{
	if ( !m_current_controller )
		reset_animation_controller( m_game_world.get_game().game_time_ms() );
}

void animations_selector::set_target( ai::animation_item const& animation_emitter )
{
	m_simple_animation_parameters.emitter	= static_cast_resource_ptr< animation::animation_expression_emitter_ptr >( animation_emitter.animation );

	m_target_controller						= &m_simple_animation_controller;
	m_target_controller_parameters			= &m_simple_animation_parameters;

	if ( !m_current_controller )
		reset_animation_controller( m_game_world.get_game().game_time_ms() );
}

void animations_selector::set_target( ai::movement_target const& target_position )
{
	m_movement_animation_parameters.position		= target_position.target_position;
	m_movement_animation_parameters.eyes_direction	= target_position.direction;
	m_movement_animation_parameters.velocity		= target_position.velocity;
	m_movement_animation_parameters.animation		= static_cast_resource_ptr< animation::animation_expression_emitter_ptr >( target_position.preferable_animation->animation );

	m_target_controller								= &m_single_position_animation_controller;
	m_target_controller_parameters					= &m_movement_animation_parameters;

	if ( !m_current_controller )
		reset_animation_controller( m_game_world.get_game().game_time_ms() );
}

void animations_selector::debug_draw( render::game::renderer& render, render::scene_ptr const& scene ) const
{
	if ( m_current_controller )
		m_current_controller->debug_draw( render, scene );
}

} // namespace survarium
