////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "jump_logic_state_landing.h"
#include <vostok/game_core/weapon_user_animations_selector.h>
#include <vostok/game_core/base_player.h>
#include <vostok/animation/mixing_animation_lexeme_parameters.h>

namespace survarium {

jump_logic_state_landing::jump_logic_state_landing( jump_logic& owner )
	: jump_logic_base_state( owner ), m_landing_type( jump_animations_part_land_run )
{
}

// claude@NOTE: PARKED at the mixing expression/operator+ wall, same as the sibling
// jump_logic_state_start::selected_animations. Recovered structure (target @0x57d3e0,
// 3 statements, locals main_lexeme/look_lexeme - both animation_lexeme, which has no
// default ctor so they are constructed in the make_pair return):
//   main_lexeme = get_main_lexeme( buffer, is_third_view,
//       weapon_parameters.is_aimed ? animation::body_part_whole_body : weapon_parameters.body_part_mask );
//   look_lexeme = get_look_lexeme( buffer, is_third_view, look_calculator, main_lexeme );
//   return std::make_pair( animation::mixing::expression( main_lexeme ) + look_lexeme, main_lexeme );
// Blocked: the expression operator+ emits an out-of-line addition_lexeme builder that
// cannot be steered from this TU, AND get_main_lexeme must land first (see below). Both
// wait on the animation module's create_animation_interval / operator+ PR.
std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> jump_logic_state_landing::selected_animations(
	mutable_buffer&						buffer,
	bool								is_third_view,
	fastdelegate::FastDelegate<float(float,float,u32,u32,u32,float)> const&	look_calculator,
	weapon_animation_parameters const&	weapon_parameters
)
{
	UNREACHABLE_CODE( );
}

// claude@NOTE: PARKED on the cross-module create_animation_interval wall. Full structure
// recovered from target @0x57d110 (19 statements, locals: pcstr const caption /
// resource_ptr move_animation / fixed_vector<animation::mixing::animation_interval,2>
// intervals / bool const landing_to_left_leg). Recovered body:
//   typedef fixed_vector<animation::mixing::animation_interval,2> two_anim_intervals_type;
//   m_animation = m_jump_logic.get_animation( m_landing_type, is_third_view );
//   pcstr const caption = m_jump_logic.get_animation_caption( m_landing_type );
//   if ( m_landing_type == jump_animations_part_land_run ) {
//       move_animation = m_jump_logic.get_move_animation( is_third_view );
//       ASSERT( UNKNOWN_EXPRESSION );
//       two_anim_intervals_type intervals;
//       bool const landing_to_left_leg = m_jump_logic.is_jump_from_right_leg();
//       if ( landing_to_left_leg ) {
//           intervals.push_back( animation_lexeme_parameters::create_animation_interval( move_animation, 0 ) );
//           intervals.push_back( animation_lexeme_parameters::create_animation_interval( m_animation, 0 ) );
//           m_interval_id_to_wait_for = 1;
//       } else {
//           intervals.push_back( animation_lexeme_parameters::create_animation_interval( m_animation, 0 ) );
//           intervals.push_back( animation_lexeme_parameters::create_animation_interval( move_animation, 1 ) );
//           m_interval_id_to_wait_for = 0;
//       }
//       return animation_lexeme( animation_lexeme_parameters( buffer, caption, intervals.begin(), intervals.end(), 0, 0 )
//           .weight_synchronization_group_id(0).weight_interpolator( instant_interpolator() )
//           .time_synchronization_group_id(0).time_scale_interpolator( linear_interpolator(s_aim_transition_time) )
//           .animated_object( m_user ).bones_mask( bones_mask ) );
//   }
//   m_interval_id_to_wait_for = 0;
//   return animation_lexeme( animation_lexeme_parameters( buffer, caption, m_animation, 0, 0 )
//       .weight_synchronization_group_id(0).animated_object( m_user ).bones_mask( bones_mask )
//       .playback_type( play_once_and_freeze_at_end ) );
// Blocked: animation_lexeme_parameters::create_animation_interval (private static) is only
// DECLARED, never defined - the animation module's mixing_animation_lexeme_parameters.cpp
// implements only create_animation_intervals(plural), so the body LNK2001s unresolved (called
// at target offsets 0xd7/0x12a/0x15e/0x180). NEXT STEP: implement create_animation_interval in
// the animation module first (cross-module), then friend + land this body. Also needs a file
// static `static float s_aim_transition_time = 0.3f;`.
animation::mixing::animation_lexeme jump_logic_state_landing::get_main_lexeme( mutable_buffer& buffer, bool is_third_view, animation::body_part_masks_enum bones_mask )
{
	UNREACHABLE_CODE( );
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
