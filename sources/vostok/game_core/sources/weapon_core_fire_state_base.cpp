// SPDX-License-Identifier: GPL-3.0-or-later

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

	set_is_firing( true );
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

	set_is_firing( false );
}

// claude@NOTE: paired, structure matches (10/10 stmts); sole residual is a 3-byte size
// diff in the LOG_ERROR statement - the macro bakes __LINE__ into the pushed string length
// (target pushes 0x52, base 0x42), which depends on this file's line layout matching the
// original's and is not faithfully reconstructable.
animation::callback_return_type_enum weapon_core_fire_state_base::on_shot_event( animation::animation_callback_params& params )
{
	params.interrupt_animation_player_tick = true;

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	if ( !m_weapon.get_bullets_in_queue( ) )
	{
		LOG_ERROR( "!m_weapon.get_bullets_in_queue()" );
		return animation::callback_return_type_call_me_again;
	}

	ASSERT( UNKNOWN_EXPRESSION );

	m_weapon.instant_fire( params.callback_time_in_ms );

	ASSERT( UNKNOWN_EXPRESSION );

	return animation::callback_return_type_call_me_again;
}

void weapon_core_fire_state_base::on_animation_end_impl( bool& animation_player_tick_result )
{
	animation_player_tick_result = true;
}

} // namespace survarium
