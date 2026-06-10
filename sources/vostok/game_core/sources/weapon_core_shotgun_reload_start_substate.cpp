////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_shotgun_reload_start_substate.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/animation/animation_callback.h>

namespace survarium {

// STATE[100%|DONE]
weapon_core_shotgun_reload_start_substate::weapon_core_shotgun_reload_start_substate(
	weapon_core&							weapon,
	float									animation_time_scale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
) : weapon_core_shotgun_reload_base_substate(
		weapon, animation_time_scale,
		animations,
		animations_count,
		animation::mixing::play_once_and_freeze_at_end,
		3,
		"shotgun-start_reload",
		"reload_start(stand)",
		"reload_start(crouch)",
		"reload_start(jump)"
	)
{
}

// STATE[76.17%|PARTIAL]: boost::function1::assign_to inline level differs (target inlines the
// wrapper, exposing basic_vtable1::assign_to + the vtable-tag dance), whole-program LTCG.
void weapon_core_shotgun_reload_start_substate::initialize( )
{
	m_animation_ended = false;
	m_weapon.set_animation_callback(
		animation::channel_id_on_animation_end,
		this,
		boost::bind( &weapon_core_shotgun_reload_start_substate::on_animation_end, this, _1 )
	);

	if ( !m_weapon.deserializing( ) )
	{
		if ( m_weapon.chamber_a_round_on_reload( ) && m_weapon.round_is_chambered( ) )
			m_weapon.unload_chambered_round( );
	}

	// STRUCTURE DIFF: target 5 stmts / base 5 stmts
	// SIZE -0x35 | 42 | );
	// VERDICT: STRUCTURE MATCH (shape ok) - nested if reproduces the target's split condition
	// records (0x1a + 0x2f, identical bytes); sole SIZE is function1::assign_to kept
	// out-of-line in base vs inlined in target, non-steerable boost-internal LTCG.
}

// STATE[61.84%|PARTIAL]: animation_playback_state::reset() inlined in base vs out-of-line
// promoted call in target, whole-program LTCG, non-steerable.
void weapon_core_shotgun_reload_start_substate::finalize( )
{
	ASSERT( UNKNOWN_EXPRESSION ); m_animation_playback_state->reset( );
	m_weapon.remove_animation_callback( animation::channel_id_on_animation_end, this );

	// STRUCTURE DIFF: target 2 stmts / base 2 stmts
	// SIZE +0x17 | 61 | ASSERT( UNKNOWN_EXPRESSION ); m_animation_playback_state->reset( );
	// VERDICT: STRUCTURE MATCH (shape ok) - target's line record merges the assert eater and the
	// reset on one line (0x1a = 0xc eater + 0xe promoted reset call); base inlines reset's body
	// (0x25), non-steerable LTCG inline choice on the header-inline reset.
}

// STATE[100%|DONE]
bool weapon_core_shotgun_reload_start_substate::is_ready_for_transition( ) const
{
	return m_animation_ended;
}

// STATE[83.00%|PARTIAL]: intrusive_ptr::operator== operand scheduling (+0x1) and the ASSERT
// eater's ICF fold-name reloc; structure exact.
animation::callback_return_type_enum weapon_core_shotgun_reload_start_substate::on_animation_end( animation::animation_callback_params& params )
{
	params.interrupt_animation_player_tick = false;
	if ( params.animated_object == &m_weapon )
	{
		ASSERT( UNKNOWN_EXPRESSION );
		if ( m_animation_to_wait_for == params.animation )
		{
			m_animation_ended = true;
			params.interrupt_animation_player_tick = true;
		}
	}

	return animation::callback_return_type_call_me_again;

	// STRUCTURE DIFF: target 7 stmts / base 7 stmts
	// SIZE +0x1 | 97 | if ( m_animation_to_wait_for == params.animation )
	// VERDICT: STRUCTURE MATCH (shape ok) - the outer-if BASE_ONLY/TRGT_ONLY pair is an aligner
	// mispair (identical 0x10 bytes at offset 0x10 both sides); sole SIZE is the operator==
	// operand scheduling, non-steerable.
}

} // namespace survarium
