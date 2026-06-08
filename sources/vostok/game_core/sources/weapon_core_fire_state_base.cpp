////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_fire_state_base.h>
#include <vostok/game_core/weapon_core.h>		// m_weapon.* + get_bullets_in_queue

namespace survarium {

// STATE[100%|DONE]
weapon_core_fire_state_base::weapon_core_fire_state_base( weapon_core& weapon, float animation_timescale ) :
	weapon_core_animation_end_aware_state( weapon, true )
{
	m_animation_timescale = animation_timescale;
	m_playback_type = animation::mixing::play_cyclically;
}

// STATE[99.71%|DONE]: every instruction matches; the sole residual is a 4-byte /Od
// frame-size diff (base `sub esp,5Ch` vs target `58h`) that cascades into the [ebp-N]
// slot numbering - stack-slot packing, not a logic/structure divergence.
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

// STATE[80.91%|PARTIAL]: inline-vs-call of the empty weapon_core_base_state::execute() (not yet steered).
// Target keeps it out-of-line and emits `call execute` (standalone @0x087f80); our /GL
// inlines its empty `{}` body at this call site (execute standalone in BOTH indexes, so it is a
// per-call-site inline decision that is source-steerable in principle - force the out-of-line call -
// NOT non-steerable LTCG). m_animation_has_been_ended = false matches.
void weapon_core_fire_state_base::execute( )
{
	weapon_core_base_state::execute( );
	m_animation_has_been_ended = false;

	// FUNCTION BODY (kept: PARTIAL - empty-callee inline-vs-call)
	// <0x58ec07>|0x007 weapon_core_base_state::execute( );   (target `call`, base inlines empty body)
	// <0x58ec0f>|0x00f m_animation_has_been_ended = false;
	// ******
	// TARGET @0x07: mov ecx,[ebp-4]; call survarium::weapon_core_base_state::execute
	// BASE   @0x07: (call elided - empty body inlined to nothing)
}

// STATE[100%|DONE]
void weapon_core_fire_state_base::finalize( )
{
	weapon_core_animation_end_aware_state::finalize( );

	m_weapon.remove_animation_callback( "shoot", this );

	ASSERT( UNKNOWN_EXPRESSION );	// compiled-out ASSERT (target's `call finalize_impl` @ +0x28)

	*m_is_firing_ptr = false;
}

// STATE[100%|DONE]
void weapon_core_fire_state_base::on_animation_end_impl( bool& animation_player_tick_result )
{
	animation_player_tick_result = true;
}

// STATE[95.26%|PARTIAL]: logging residual (same class as animation_analysis_result_cook::translate_query).
// All control flow, the two leading + two trailing compiled-out ASSERTs, the if/else and instant_fire
// match the target byte-for-byte. The residual is entirely inside the LOG_ERROR expansion: the __LINE__
// immediate (base `push 4Bh`=75 vs target `push 52h`=82, our file is shorter), the __FILE__/__FUNCSIG__
// string relocs (content differs build-to-build), and a one-instruction reorder of the boost::function
// log_callback temp ctor. None steerable to a clean byte match.
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

	// FUNCTION BODY (kept: PARTIAL - logging string residual)
	// <0x58ed80> params.interrupt_animation_player_tick = true;
	// <0x58ed98> ASSERT( UNKNOWN_EXPRESSION );  (+0xc)
	// <0x58eda4> ASSERT( UNKNOWN_EXPRESSION );  (+0xc)
	// <0x58edb0> if ( !m_weapon.get_bullets_in_queue() ) {
	// <0x58edcc>   LOG_ERROR( "!m_weapon.get_bullets_in_queue()" );  (+0x77)
	// <0x58ee43>   return callback_return_type_call_me_again;
	// <0x58ee47> ASSERT( UNKNOWN_EXPRESSION );  (+0xc)
	// <0x58ee53> m_weapon.instant_fire( params.callback_time_in_ms );
	// <0x58ee68> ASSERT( UNKNOWN_EXPRESSION );  (+0xc)
	// <0x58ee74> return callback_return_type_call_me_again;
	// ******
}

} // namespace survarium
