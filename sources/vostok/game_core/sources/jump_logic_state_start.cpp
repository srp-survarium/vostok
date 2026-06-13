////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "jump_logic_state_start.h"
#include <vostok/game_core/weapon_user_animations_selector.h>
#include <vostok/game_core/base_player.h>
#include <vostok/game_core/player_stamina.h>
#include <vostok/animation/mixing_animation_lexeme_parameters.h>

namespace survarium {

jump_logic_state_start::jump_logic_state_start( jump_logic& owner )
	: jump_logic_base_state( owner ),
	  m_preface_interval_ended( false ),
	  m_jump_interval_ended( false )
{
}

// claude@NOTE: PARKED at the mixing expression/operator+ wall. Recovered structure
// (target @0x6e9210, 3 statements, locals main_lexeme/look_lexeme - both
// animation_lexeme, which has NO default ctor so they are constructed in the return):
//   main_lexeme = get_main_lexeme( buffer, is_third_view,
//       weapon_parameters.is_aimed ? animation::body_part_whole_body : weapon_parameters.body_part_mask );
//   look_lexeme = get_look_lexeme( buffer, is_third_view, look_calculator, main_lexeme );
//   return std::make_pair( animation::mixing::expression( main_lexeme ) + look_lexeme, main_lexeme );
// Blocked: the expression operator+ emits an out-of-line addition_lexeme builder
// (mixing_addition_lexeme) that cannot be steered from this TU; same wall as the sibling
// player_logic_*_state lexeme fns. Needs get_main_lexeme landed first (see below).
std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> jump_logic_state_start::selected_animations(
	mutable_buffer&						buffer,
	bool								is_third_view,
	fastdelegate::FastDelegate<float(float,float,u32,u32,u32,float)> const&	look_calculator,
	weapon_animation_parameters const&	weapon_parameters
)
{
	UNREACHABLE_CODE( );
}

// claude@NOTE: PARKED. Full structure recovered from target @0x6e8f00 (19 statements,
// locals: pcstr const caption / fixed_vector<animation::mixing::animation_interval,2>
// intervals / animation::linear_interpolator interpolator). Recovered body:
//   typedef fixed_vector<animation::mixing::animation_interval,2> two_anim_intervals_type;
//   m_animation = m_jump_logic.get_animation( jump_animations_part_start, is_third_view );
//   pcstr const caption = m_jump_logic.get_animation_caption( jump_animations_part_start );
//   if ( m_jump_logic.get_jump_direction() != move_direction_on_site ) {
//       m_preface_animation = m_jump_logic.get_move_animation( is_third_view );
//       ASSERT( UNKNOWN_EXPRESSION );
//       two_anim_intervals_type intervals;
//       if ( m_jump_logic.is_jump_from_right_leg() ) {
//           intervals.push_back( animation_lexeme_parameters::create_animation_interval( m_animation, 0 ) );
//           intervals.push_back( animation_lexeme_parameters::create_animation_interval( m_preface_animation, 1 ) );
//           m_interval_id_to_wait_for = 0;
//       } else {
//           intervals.push_back( animation_lexeme_parameters::create_animation_interval( m_preface_animation, 0 ) );
//           intervals.push_back( animation_lexeme_parameters::create_animation_interval( m_animation, 0 ) );
//           m_interval_id_to_wait_for = 1;
//       }
//       return animation_lexeme( animation_lexeme_parameters( buffer, caption, intervals.begin(), intervals.end(), 0, 0 )
//           .weight_synchronization_group_id(0).weight_interpolator( linear_interpolator(s_aim_transition_time) )
//           .time_synchronization_group_id(0).time_scale_interpolator( linear_interpolator(s_aim_transition_time) )
//           .animated_object( m_user ).bones_mask( bones_mask ) );
//   }
//   m_interval_id_to_wait_for = 0;
//   linear_interpolator interpolator( s_aim_transition_time );
//   return animation_lexeme( animation_lexeme_parameters( buffer, caption, m_animation, 0, 0 )
//       .weight_synchronization_group_id(0).weight_interpolator( interpolator )
//       .animated_object( m_user ).bones_mask( bones_mask ) );
// Blocked: animation_lexeme_parameters::create_animation_interval (private static) is only
// DECLARED, never defined - it is unimplemented in the animation module's
// mixing_animation_lexeme_parameters.cpp (only create_animation_intervals(plural) exists), so
// the friend + body LNK2001 unresolved. NEXT STEP: implement create_animation_interval in the
// animation module first (cross-module), then friend jump_logic_state_start here and land this body.
animation::mixing::animation_lexeme jump_logic_state_start::get_main_lexeme( mutable_buffer& buffer, bool is_third_view, animation::body_part_masks_enum bones_mask )
{
	UNREACHABLE_CODE( );
}

animation::mixing::animation_lexeme jump_logic_state_start::get_look_lexeme(
	mutable_buffer&						buffer,
	bool								is_third_view,
	fastdelegate::FastDelegate<float(float,float,u32,u32,u32,float)> const&	look_calculator,
	animation::mixing::animation_lexeme&	weight_driving_animation
)
{
	resources::managed_resource_ptr	look_animation( 0 );
	pcstr							look_animation_id	= 0;
	if ( m_preface_interval_ended )
	{
		look_animation		= m_jump_logic.get_animation( jump_animations_part_start_look, is_third_view );
		look_animation_id	= m_jump_logic.get_animation_caption( jump_animations_part_start_look );
	}
	else
	{
		look_animation		= m_jump_logic.get_move_look_animation( is_third_view );
		look_animation_id	= m_jump_logic.get_move_look_caption( );
	}

	animation::mixing::animation_lexeme_parameters	parameters( buffer, look_animation_id, look_animation, 0, &weight_driving_animation );

	float const	start_animation_interval_time	= parameters.animation_intervals( )[ 0 ].length( ) * m_jump_logic.look_time_factor( );

	return animation::mixing::animation_lexeme(
		parameters
			.start_animation_interval_time( start_animation_interval_time )
			.animated_object( m_user )
			.additivity_priority( 4 )
			.time_calculator( look_calculator )
	);
}

void jump_logic_state_start::initialize( )
{
	m_animation					= 0;
	m_preface_animation			= 0;
	m_preface_interval_ended	= false;
	m_jump_interval_ended		= false;

	m_jump_logic.owner().set_animation_callback( animation::channel_id_on_animation_interval_end, this, boost::bind( &jump_logic_state_start::on_interval_end, this, _1 ) );
	m_jump_logic.owner().set_animation_callback( "jump", this, boost::bind( &jump_logic_state_start::on_jump_event, this, _1 ) );
}

void jump_logic_state_start::finalize( )
{
	m_jump_logic.owner().remove_animation_callback( "jump", this );
	m_jump_logic.owner().remove_animation_callback( animation::channel_id_on_animation_interval_end, this );
}

animation::callback_return_type_enum jump_logic_state_start::on_interval_end( animation::animation_callback_params& params )
{
	if ( params.animated_object == &m_jump_logic.owner().user() )
	{
		m_jump_interval_ended		= m_animation == params.animation && params.animation_interval_id == m_interval_id_to_wait_for;
		m_preface_interval_ended	= m_preface_animation == params.animation;
		params.interrupt_animation_player_tick = true;
	}

	return animation::callback_return_type_call_me_again;
}

animation::callback_return_type_enum jump_logic_state_start::on_jump_event( animation::animation_callback_params& params )
{
	if ( params.animation_interval_id == m_interval_id_to_wait_for && params.animated_object == &m_jump_logic.owner().user() )
	{
		params.interrupt_animation_player_tick = false;
		m_user->stamina().spend( m_user->stamina().amount_to_jump() );
		m_user->jump();
	}

	return animation::callback_return_type_call_me_again;
}

} // namespace survarium
