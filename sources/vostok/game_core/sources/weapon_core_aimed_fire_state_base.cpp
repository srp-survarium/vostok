////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_aimed_fire_state_base.h>
#include <vostok/game_core/weapon_core.h>

namespace survarium {

// STATE[100%|DONE]
weapon_core_aimed_fire_state_base::weapon_core_aimed_fire_state_base( weapon_core& weapon, float animation_timescale ) :
	weapon_core_animation_end_aware_state( weapon, false ),
	m_animation_timescale				( animation_timescale )
{
}

// STATE[PARTIAL]: structure and logic match target; residual is LTCG frame-size diff
// (base sub esp,0ACh vs target 0A4h) cascading into [ebp-N] slot numbering + COMDAT
// boost::bind symbol pairing mismatch. Same class as weapon_core_fire_state_base::initialize.
void weapon_core_aimed_fire_state_base::initialize( )
{
	ASSERT( UNKNOWN_EXPRESSION );

	*m_is_firing_ptr = true;

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

// STATE[PARTIAL]: structure correct (2 stmts); target keeps weapon_core_base_state::execute()
// out-of-line, base inlines the empty `{}` body. Same wall as weapon_core_fire_state_base::execute.
void weapon_core_aimed_fire_state_base::execute( )
{
	weapon_core_base_state::execute( );
	m_animation_has_been_ended = false;
}

// STATE[PARTIAL]: structure and logic match; residual is ASSERT slot drift from the
// 8-byte frame gap (same LTCG class as initialize). `*m_is_firing_ptr = false` folds
// into the ASSERT statement boundary in target PDB.
void weapon_core_aimed_fire_state_base::finalize( )
{
	weapon_core_animation_end_aware_state::finalize( );

	m_weapon.remove_animation_callback( "shoot", this );

	m_weapon.instant_aim_end( );

	m_weapon.remove_animation_callback( "aiming", this );

	ASSERT( UNKNOWN_EXPRESSION );

	*m_is_firing_ptr = false;
}

// STATE[PARTIAL]: 10/10 stmts matched; SIZE -0x3 on LOG_WARNING is LTCG inline-vs-call
// of the logging infrastructure, non-steerable.
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

// STATE[100%|DONE]
void weapon_core_aimed_fire_state_base::on_animation_end_impl( bool& animation_player_tick_result )
{
	animation_player_tick_result = true;
}

// STATE[100%|DONE]
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
