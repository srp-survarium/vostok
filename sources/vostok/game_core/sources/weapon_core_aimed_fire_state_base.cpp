// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/weapon_core_aimed_fire_state_base.h>
#include <vostok/game_core/weapon_core.h>

namespace survarium {

weapon_core_aimed_fire_state_base::weapon_core_aimed_fire_state_base( weapon_core& weapon, float animation_timescale ) :
	weapon_core_animation_end_aware_state( weapon, false ),
	m_animation_timescale				( animation_timescale )
{
}

// claude@NOTE: paired (@@MAE), structure matches (6/6 stmts after folding ASSERT +
// *m_is_firing_ptr into one statement). Residual is a boost::function inlining divergence
// on the SECOND set_animation_callback: the target inlines function1<>::assign_to (exposing
// basic_vtable1::assign_to + the conditional flag path, like the first bind) but MSVC here
// emits an out-of-line function1<>::assign_to for the second, shifting the frame by 8 bytes.
// /Od inline-heuristic ceiling on the shared boost::function machinery, not steerable.
void weapon_core_aimed_fire_state_base::initialize( )
{
	ASSERT( UNKNOWN_EXPRESSION ); *m_is_firing_ptr = true;

	weapon_core_animation_end_aware_state::initialize( );

	m_weapon.set_animation_callback(
		"shoot",
		this,
		boost::bind( &weapon_core_aimed_fire_state_base::on_shot_event, this, _1 )
	);

	m_weapon.instant_aim_start( );

	m_weapon.set_animation_callback(
		"aiming",
		this,
		boost::bind( &weapon_core_aimed_fire_state_base::on_aiming_event, this, _1 )
	);

	m_playback_type = animation::mixing::playback_enum( m_weapon.get_bullets_in_queue( ) <= 1 );
}

// claude@NOTE: paired (@@MAE), walled by a cross-unit inline-vs-call boundary. The target
// emits a `call weapon_core_base_state::execute` (out-of-line 0x16-byte function there); here
// that base method is an inline `{}` in weapon_core_base_state.h, so MSVC inlines it away and
// drops the call statement. Out-lining it belongs to weapon_core_base_state.cpp's match.
void weapon_core_aimed_fire_state_base::execute( )
{
	weapon_core_base_state::execute( );
	m_animation_has_been_ended = false;
}

void weapon_core_aimed_fire_state_base::finalize( )
{
	weapon_core_animation_end_aware_state::finalize( );

	m_weapon.remove_animation_callback( "shoot", this );

	m_weapon.instant_aim_end( );

	m_weapon.remove_animation_callback( "aiming", this );

	ASSERT( UNKNOWN_EXPRESSION ); *m_is_firing_ptr = false;
}

// claude@NOTE: paired (@@MAE), structure matches (10/10 stmts); sole residual is a 3-byte
// size diff in the LOG_WARNING statement - the macro bakes __LINE__ into the pushed string
// length (target pushes 0x53, base 0x56) and that depends on this file's line layout matching
// the original's, which is not faithfully reconstructable.
animation::callback_return_type_enum weapon_core_aimed_fire_state_base::on_shot_event( animation::animation_callback_params& params )
{
	params.interrupt_animation_player_tick = true;

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	if ( !m_weapon.get_bullets_in_queue( ) )
	{
		LOG_WARNING( "!m_weapon.get_bullets_in_queue()" );
		return animation::callback_return_type_call_me_again;
	}

	ASSERT( UNKNOWN_EXPRESSION );

	m_weapon.instant_fire( params.callback_time_in_ms );

	ASSERT( UNKNOWN_EXPRESSION );

	return animation::callback_return_type_call_me_again;
}

void weapon_core_aimed_fire_state_base::on_animation_end_impl( bool& animation_player_tick_result )
{
	animation_player_tick_result = true;
}

animation::callback_return_type_enum weapon_core_aimed_fire_state_base::on_aiming_event( animation::animation_callback_params& params )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	if ( params.domain_data == 9 )
		m_weapon.instant_aim_start( );
	else
		m_weapon.instant_aim_end( );

	return animation::callback_return_type_call_me_again;
}

} // namespace survarium
