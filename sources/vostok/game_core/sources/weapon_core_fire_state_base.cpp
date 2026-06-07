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
// frame-size diff (target `sub esp,5Ch` vs base `58h`) that cascades into the [ebp-N]
// slot numbering - stack-slot packing, not a logic/structure divergence.
// STRUCTURE DIFF[target 0x58ec90 | base 0x44d1a0]: target 5 / base 9 stmts
// 0x0cf <0x18> | 0x0cf <0xc> | ASSERT( UNKNOWN_EXPRESSION );   SIZE
// .. same ..
// ; aligned 4, size-diffs 1, quantity-diffs 4
// VERDICT: STRUCTURE MATCH (shape ok) - all stmts byte-identical; sole SIZE is the ASSERT slot drift from the 4-byte larger target frame ([ebp-39h] vs [ebp-53h]), /Od slot packing, non-steerable. trail: weapon_core_fire_state_base.md
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

// STATE[80.91%|PARTIAL]: LTCG inline-vs-call of the empty weapon_core_base_state::execute().
// STRUCTURE DIFF[target 0x58ec00 | base 0x44d130]: target 3 / base 1 stmts
// 0x007 <0x8> | --          | L43   ONLY target
// <0>         | --          |    EMPTY only target
// .. same ..
// ; aligned 1, size-diffs 0, quantity-diffs 2
// VERDICT: STRUCTURE MATCH (shape ok) - target emits `call weapon_core_base_state::execute`, our /GL LTCG inlines its empty {} body to nothing; per-call-site inline-vs-call, non-steerable. trail: weapon_core_fire_state_base.md
void weapon_core_fire_state_base::execute( )
{
	weapon_core_base_state::execute( );
	m_animation_has_been_ended = false;
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
// STRUCTURE DIFF[target 0x58ed80 | base 0x44d290]: target 16 / base 17 stmts
// 0x04c <0x77> | 0x04c <0x74> | LOG_ERROR( "!m_weapon.get_bullets_in_queue()" );   SIZE
// .. same ..
// ; aligned 15, size-diffs 1, quantity-diffs 1
// VERDICT: STRUCTURE MATCH (shape ok) - all control flow + 4 ASSERTs + instant_fire byte-exact; sole SIZE is inside the LOG_ERROR expansion (__LINE__/__FILE__/__FUNCSIG__ build-specific immediates+relocs), non-steerable. trail: weapon_core_fire_state_base.md
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
