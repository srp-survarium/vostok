////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_shotgun_reload_one_round_substate.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/animation/animation_callback.h>

namespace survarium {

// STATE[100%|DONE]
weapon_core_shotgun_reload_one_round_substate::weapon_core_shotgun_reload_one_round_substate(
	weapon_core&							weapon,
	float									animation_time_scale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
) : weapon_core_shotgun_reload_base_substate(
		weapon, animation_time_scale,
		animations,
		animations_count,
		animation::mixing::play_cyclically,
		4,
		"shotgun-reload_one",
		"reload_cycle(stand)",
		"reload_cycle(crouch)",
		"reload_cycle(jump)"
	)
{
}

// STATE[100%|DONE]
bool weapon_core_shotgun_reload_one_round_substate::is_ready_for_transition( ) const
{
	return true;
}

// STATE[100%|DONE]
void weapon_core_shotgun_reload_one_round_substate::initialize( )
{
	m_weapon.set_animation_callback(
		animation::channel_id_on_animation_end,
		this,
		boost::bind( &weapon_core_shotgun_reload_one_round_substate::on_animation_end, this, _1 )
	);
}

// STATE[61.84%|PARTIAL]: animation_playback_state::reset() inlined in base vs out-of-line
// promoted call in target, whole-program LTCG, non-steerable.
void weapon_core_shotgun_reload_one_round_substate::finalize( )
{
	ASSERT( UNKNOWN_EXPRESSION ); m_animation_playback_state->reset( );
	m_weapon.remove_animation_callback( animation::channel_id_on_animation_end, this );

	// STRUCTURE DIFF: target 2 stmts / base 2 stmts
	// SIZE +0x17 | 53 | ASSERT( UNKNOWN_EXPRESSION ); m_animation_playback_state->reset( );
	// VERDICT: STRUCTURE MATCH (shape ok) - target's line record merges the assert eater and the
	// reset on one line (0x1a = 0xc eater + 0xe promoted reset call); base inlines reset's body
	// (0x25), non-steerable LTCG inline choice on the header-inline reset.
}

// STATE[83.55%|PARTIAL]: dummy::nonnull/finalize_impl ICF fold + intrusive_ptr::operator== operand
// scheduling (LTCG call-boundary). Structure matches statement-for-statement.
animation::callback_return_type_enum weapon_core_shotgun_reload_one_round_substate::on_animation_end( animation::animation_callback_params& params )
{
	params.interrupt_animation_player_tick = false;
	if ( params.animated_object == &m_weapon )
	{
		ASSERT( UNKNOWN_EXPRESSION );
		if ( m_animation_to_wait_for == params.animation )
		{
			m_weapon.reload_one_round( );
			params.interrupt_animation_player_tick = true;
		}
	}

	return animation::callback_return_type_call_me_again;

	// STRUCTURE DIFF: target 7 stmts / base 7 stmts
	// SIZE +0x1 | 71 | if ( m_animation_to_wait_for == params.animation )
	// VERDICT: STRUCTURE MATCH (shape ok) - the outer-if BASE_ONLY/TRGT_ONLY pair is an aligner
	// mispair (identical 0x10 bytes at offset 0x10 both sides); sole SIZE is the operator==
	// operand scheduling, non-steerable.
}

} // namespace survarium
