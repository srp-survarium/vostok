// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "animations_selector.h"
#include "game_world.h"				// m_game_world.get_game() ...
#include "game.h"					// ... .game_time_ms() inlines the game clock
#include "human_npc.h"				// m_owner.get_transform( )
#include <vostok/animation/animation_player.h>	// subscribe( channel_id_on_animation_interval_end, ... )
#include <vostok/animation/mixing_expression.h>
#include <vostok/animation/mixing_animation_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme_parameters.h>
#include <vostok/animation/linear_interpolator.h>
#include <vostok/memory_extensions.h>	// ALLOCA
#include <vostok/ai/animation_item.h>
#include <vostok/ai/movement_target.h>

namespace survarium {

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

// retail calls the two-store buffer ctor out-of-line here (ICF ctor group)
static __declspec( noinline ) mutable_buffer make_stack_buffer( pvoid data, u32 size )
{
	return mutable_buffer( data, size );
}

void animations_selector::set_animation_player_target( animation::mixing::expression const& target_expression, const u32 time_in_ms )
{
	if ( !target_expression.is_empty( ) )
		m_animation_player.set_target_and_tick( target_expression, time_in_ms, m_owner.get_transform( ) );
	else
	{
		mutable_buffer buffer = make_stack_buffer( ALLOCA( animation::animation_player::stack_buffer_size ), animation::animation_player::stack_buffer_size );
		animation::mixing::animation_lexeme lexeme( animation::mixing::animation_lexeme_parameters( buffer, "default", m_default_animation, NULL, NULL ).weight_interpolator( animation::linear_interpolator( 0.25f ) ) );
		m_animation_player.set_target_and_tick( lexeme, time_in_ms, m_owner.get_transform( ) );
	}
}

void animations_selector::reset_animation_controller( const u32 time_in_ms )
{
	mutable_buffer buffer = make_stack_buffer( ALLOCA( animation::animation_player::stack_buffer_size ), animation::animation_player::stack_buffer_size );
	if ( m_current_controller != m_target_controller )
	{
		if ( m_current_controller )
		{
			animation::mixing::expression expression = m_current_controller->try_finalize( *m_target_controller, buffer );
			if ( !expression.is_empty( ) )
			{
				set_animation_player_target( expression, time_in_ms );
				return;
			}
		}

		m_current_controller = m_target_controller;
		m_current_controller->initialize( );
		m_current_controller->set_target( *m_target_controller_parameters );
		m_target_controller_parameters->reset( );
	}

	animation::mixing::expression expression = m_current_controller->selected_animations( buffer );
	set_animation_player_target( expression, time_in_ms );
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

	on_set_target( );
}

void animations_selector::set_target( ai::movement_target const& target_position )
{
	m_movement_animation_parameters.position		= target_position.target_position;
	m_movement_animation_parameters.eyes_direction	= target_position.direction;
	m_movement_animation_parameters.velocity		= target_position.velocity;
	m_movement_animation_parameters.animation		= static_cast_resource_ptr< animation::animation_expression_emitter_ptr >( target_position.preferable_animation->animation );

	m_target_controller								= &m_single_position_animation_controller;
	m_target_controller_parameters					= &m_movement_animation_parameters;

	on_set_target( );
}

void animations_selector::debug_draw( render::game::renderer& render, render::scene_ptr const& scene ) const
{
	if ( m_current_controller )
		m_current_controller->debug_draw( render, scene );
}

} // namespace survarium
