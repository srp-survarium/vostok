////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_fire_state_base.h>
#include <vostok/game_core/weapon_core.h>		// m_weapon.* + get_bullets_in_queue

namespace survarium {

weapon_core_fire_state_base::weapon_core_fire_state_base( weapon_core& weapon, float animation_timescale ) :
	weapon_core_animation_end_aware_state	( weapon, true ),
	m_animation_timescale					( animation_timescale ),
	m_playback_type							( animation::mixing::play_cyclically )
{
}

void weapon_core_fire_state_base::initialize( )
{
	weapon_core_animation_end_aware_state::initialize( );

	m_weapon.set_animation_callback(
		"shoot",
		this,
		boost::bind( &weapon_core_fire_state_base::on_shot_event, this, _1 )
	);

	m_playback_type = animation::mixing::playback_enum( m_weapon.get_bullets_in_queue( ) <= 1 );

	ASSERT( UNKNOWN_EXPRESSION );	// compiled-out ASSERT (target's `call finalize_impl` @ +0xcf)

	*m_is_firing_ptr = true;
}

void weapon_core_fire_state_base::execute( )
{
	weapon_core_base_state::execute( );
	m_animation_has_been_ended = false;
}

void weapon_core_fire_state_base::finalize( )
{
	weapon_core_animation_end_aware_state::finalize( );

	m_weapon.remove_animation_callback( "shoot", this );

	ASSERT( UNKNOWN_EXPRESSION );	// compiled-out ASSERT (target's `call finalize_impl` @ +0x28)

	*m_is_firing_ptr = false;
}

void weapon_core_fire_state_base::on_animation_end_impl( bool& animation_player_tick_result )
{
	animation_player_tick_result = true;
}

animation::callback_return_type_enum weapon_core_fire_state_base::on_shot_event( animation::animation_callback_params& params )
{
	params.interrupt_animation_player_tick = true;

	ASSERT( UNKNOWN_EXPRESSION );	// compiled-out ASSERT (target's `call finalize_impl` @ +0x18)
	ASSERT( UNKNOWN_EXPRESSION );	// compiled-out ASSERT (target's `call finalize_impl` @ +0x24)

	if ( !m_weapon.get_bullets_in_queue( ) )
	{
		LOG_ERROR( "!m_weapon.get_bullets_in_queue()" );
		return animation::callback_return_type_call_me_again;
	}

	ASSERT( UNKNOWN_EXPRESSION );	// compiled-out ASSERT (target's `call finalize_impl` @ +0xc7)

	m_weapon.instant_fire( params.callback_time_in_ms );

	ASSERT( UNKNOWN_EXPRESSION );	// compiled-out ASSERT (target's `call finalize_impl` @ +0xe8)

	return animation::callback_return_type_call_me_again;
}

} // namespace survarium
