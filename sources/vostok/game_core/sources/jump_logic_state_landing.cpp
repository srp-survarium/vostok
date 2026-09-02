// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "jump_logic_state_landing.h"
#include <vostok/game_core/weapon_user_animations_selector.h>
#include <vostok/game_core/weapon_animation_parameters.h>
#include <vostok/game_core/base_player.h>
#include <vostok/animation/mixing_addition_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme_parameters.h>
#include <vostok/animation/linear_interpolator.h>
#include <vostok/animation/instant_interpolator.h>
#include <vostok/fixed_vector.h>

namespace survarium {

jump_logic_state_landing::jump_logic_state_landing( jump_logic& owner )
	: jump_logic_base_state( owner ), m_landing_type( jump_animations_part_land_run )
{
}

std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> jump_logic_state_landing::selected_animations(
	mutable_buffer&						buffer,
	const bool								is_third_view,
	fastdelegate::FastDelegate<float(float,float,u32,u32,u32,float)> const&	look_calculator,
	weapon_animation_parameters const&	weapon_parameters
)
{
	animation::mixing::animation_lexeme	main_lexeme	= get_main_lexeme(
		buffer,
		is_third_view,
		weapon_parameters.is_aimed ? animation::body_part_whole_body : weapon_parameters.body_part_mask
	);

	animation::mixing::animation_lexeme	look_lexeme	= get_look_lexeme( buffer, is_third_view, look_calculator, main_lexeme );

	return std::make_pair< animation::mixing::expression, animation::mixing::animation_lexeme >(
		animation::mixing::expression( main_lexeme ) + look_lexeme,
		main_lexeme
	);
}

// claude@MATCH: the trailing branch is a real `else` (target emits a back-to-back
// `jmp .epilogue; jmp .epilogue` = the returning if-body's `}` jumping OVER the else;
// patterns/else-block-double-jmp.md). Residual is the builder-chain inline-vs-call ceiling
// (the animation_lexeme_parameters setters go out-of-line in the target), not steerable here.
animation::mixing::animation_lexeme jump_logic_state_landing::get_main_lexeme( mutable_buffer& buffer, const bool is_third_view, const animation::body_part_masks_enum bones_mask )
{
	typedef fixed_vector< animation::mixing::animation_interval, 2 >	two_anim_intervals_type;

	m_animation		= m_jump_logic.get_animation( m_landing_type, is_third_view );

	pcstr const		caption	= m_jump_logic.get_animation_caption( m_landing_type );

	if ( m_landing_type == jump_animations_part_land_run )
	{
		resources::managed_resource_ptr	move_animation	= m_jump_logic.get_move_animation( is_third_view );

		ASSERT( UNKNOWN_EXPRESSION );

		two_anim_intervals_type	intervals;

		bool const	landing_to_left_leg	= m_jump_logic.is_jump_from_right_leg( );

		if ( landing_to_left_leg )
		{
			intervals.push_back( animation::mixing::animation_lexeme_parameters::create_animation_interval( move_animation, 0 ) );
			intervals.push_back( animation::mixing::animation_lexeme_parameters::create_animation_interval( m_animation, 0 ) );

			m_interval_id_to_wait_for	= 1;
		}
		else
		{
			intervals.push_back( animation::mixing::animation_lexeme_parameters::create_animation_interval( m_animation, 0 ) );
			intervals.push_back( animation::mixing::animation_lexeme_parameters::create_animation_interval( move_animation, 1 ) );

			m_interval_id_to_wait_for	= 0;
		}

		return animation::mixing::animation_lexeme(
			animation::mixing::animation_lexeme_parameters( buffer, caption, intervals.begin( ), intervals.end( ), 0, 0 )
			.weight_synchronization_group_id	( 0 )
			.weight_interpolator				( animation::instant_interpolator( ) )
			.time_synchronization_group_id		( 0 )
			.time_scale_interpolator			( animation::linear_interpolator( 0.3f ) )
			.animated_object					( m_user )
			.bones_mask							( bones_mask )
		);
	}
	else
	{
		m_interval_id_to_wait_for	= 0;

		return animation::mixing::animation_lexeme(
			animation::mixing::animation_lexeme_parameters( buffer, caption, m_animation, 0, 0 )
			.weight_synchronization_group_id	( 0 )
			.animated_object					( m_user )
			.bones_mask							( bones_mask )
			.playback_type						( animation::mixing::play_once_and_freeze_at_end )
		);
	}
}

animation::mixing::animation_lexeme jump_logic_state_landing::get_look_lexeme(
	mutable_buffer&						buffer,
	bool const							is_third_view,
	fastdelegate::FastDelegate<float(float,float,u32,u32,u32,float)> const&	look_calculator,
	animation::mixing::animation_lexeme&	weight_driving_animation
)
{
	resources::managed_resource_ptr	look_animation		= m_jump_logic.get_animation( jump_animations_part_land_run_look, is_third_view );
	pcstr const						look_animation_id	= m_jump_logic.get_animation_caption( jump_animations_part_land_run_look );

	animation::mixing::animation_lexeme_parameters	parameters( buffer, look_animation_id, look_animation, 0, &weight_driving_animation );

	float const	start_animation_interval_time	= parameters.animation_intervals( )[ 0 ].length( ) * m_jump_logic.look_time_factor( );

	return animation::mixing::animation_lexeme(
		parameters
			.start_animation_interval_time( start_animation_interval_time )
			.animated_object( m_user )
			.additivity_priority( 4 )
			.weight_synchronization_group_id( 0 )
			.time_calculator( look_calculator )
	);
}

void jump_logic_state_landing::initialize( )
{
	m_user->end_jump( );

	m_jump_logic.owner().set_animation_callback( animation::channel_id_on_animation_interval_end, this, boost::bind( &jump_logic_state_landing::on_interval_end, this, _1 ) );

	m_landing_type		= m_jump_logic.does_need_land_and_run( ) ? jump_animations_part_land_run : jump_animations_part_land;
	m_is_jump_finished	= false;
}

void jump_logic_state_landing::finalize( )
{
}

animation::callback_return_type_enum jump_logic_state_landing::on_interval_end( animation::animation_callback_params& params )
{
	if ( params.animation_interval_id == m_interval_id_to_wait_for && params.animated_object == &m_jump_logic.owner().user() && m_animation == params.animation )
	{
		params.interrupt_animation_player_tick	= true;
		m_jump_logic.owner().remove_animation_callback( animation::channel_id_on_animation_interval_end, this );
		m_is_jump_finished						= true;
	}

	return animation::callback_return_type_call_me_again;
}

} // namespace survarium
