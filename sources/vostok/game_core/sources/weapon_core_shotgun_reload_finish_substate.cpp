////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_shotgun_reload_finish_substate.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/animation/animation_callback.h>

namespace survarium {

// STATE[100%|DONE]
weapon_core_shotgun_reload_finish_substate::weapon_core_shotgun_reload_finish_substate(
	weapon_core&							weapon,
	float									animation_time_scale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
) : weapon_core_shotgun_reload_base_substate(
		weapon, animation_time_scale,
		animations,
		animations_count,
		animation::mixing::play_once_and_freeze_at_end,
		5,
		"shotgun-finish_reload",
		"reload_finish(stand)",
		"reload_finish(crouch)",
		"reload_finish(jump)"
	)
	, m_owner_ready_for_transition( NULL )
{
}

// STATE[100%|DONE]
bool weapon_core_shotgun_reload_finish_substate::is_ready_for_transition( ) const
{
	return true;
}

// STATE[100%|DONE]
void weapon_core_shotgun_reload_finish_substate::initialize( )
{
	m_weapon.set_animation_callback(
		animation::channel_id_on_animation_end,
		this,
		boost::bind( &weapon_core_shotgun_reload_finish_substate::on_animation_end, this, _1 )
	);
}

// STATE[82.43%|PARTIAL]: animation_playback_state::reset() inlined in base vs out-of-line
// promoted call in target (whole-program LTCG); the extra inline bytes also shift downstream
// register numbers in the chamber-refill if. Non-steerable.
void weapon_core_shotgun_reload_finish_substate::finalize( )
{
	ASSERT( UNKNOWN_EXPRESSION ); m_animation_playback_state->reset( );
	m_weapon.remove_animation_callback( animation::channel_id_on_animation_end, this );
	if ( !m_weapon.deserializing( ) )
	{
		if ( m_weapon.chamber_a_round_on_reload( ) && m_weapon.ammo_in_magazine( ) )
		{
			m_weapon.instant_chamber_a_round( );
		}
	}

	// STRUCTURE DIFF: target 5 stmts / base 5 stmts
	// SIZE +0x17 | 56 | ASSERT( UNKNOWN_EXPRESSION ); m_animation_playback_state->reset( );
	// VERDICT: STRUCTURE MATCH (shape ok) - assert eater + reset merged on one line (target
	// 0x1a = 0xc eater + 0xe promoted reset call vs base inlining reset's body); the nested if
	// reproduces the target's split condition records (0x1a + 0x2f at lines 38/40, body at 42).
}

// STATE[83.55%|PARTIAL]: dummy::nonnull/finalize_impl ICF fold + intrusive_ptr::operator== operand
// scheduling (LTCG call-boundary). Structure matches statement-for-statement.
animation::callback_return_type_enum weapon_core_shotgun_reload_finish_substate::on_animation_end( animation::animation_callback_params& params )
{
	params.interrupt_animation_player_tick = false;
	if ( params.animated_object == &m_weapon )
	{
		ASSERT( UNKNOWN_EXPRESSION );
		if ( m_animation_to_wait_for == params.animation )
		{
			*m_owner_ready_for_transition = true;
			params.interrupt_animation_player_tick = true;
		}
	}

	return animation::callback_return_type_call_me_again;

	// STRUCTURE DIFF: target 7 stmts / base 7 stmts
	// SIZE +0x1 | 86 | if ( m_animation_to_wait_for == params.animation )
	// VERDICT: STRUCTURE MATCH (shape ok) - the outer-if BASE_ONLY/TRGT_ONLY pair is an aligner
	// mispair (identical 0x10 bytes at offset 0x10 both sides); sole SIZE is the operator==
	// operand scheduling, non-steerable.
}

} // namespace survarium
