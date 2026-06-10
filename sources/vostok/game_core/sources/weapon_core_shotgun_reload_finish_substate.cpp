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

// STATE[89.40%|PARTIAL]: residual is animation_playback_state::reset() inline-vs-call elision +
// dummy::nonnull/finalize_impl ICF fold; the missing reset() call also shifts downstream register
// numbers in the chamber-refill if. Structure (deserializing/chamber_a_round_on_reload/ammo_in_magazine
// -> instant_chamber_a_round) matches. Unsteerable, see md.
void weapon_core_shotgun_reload_finish_substate::finalize( )
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_animation_playback_state->reset( );
	m_weapon.remove_animation_callback( animation::channel_id_on_animation_end, this );

	if ( !m_weapon.deserializing( ) &&
		 m_weapon.chamber_a_round_on_reload( ) &&
		 m_weapon.ammo_in_magazine( ) )
		m_weapon.instant_chamber_a_round( );

	// FUNCTION BODY
	// <0x59e039>|0x009|+0x01a:'36'		ASSERT + reset();
	// <0x59e053>|0x023|+0x014:'37'		remove_animation_callback(...);
	// <0x59e067>|0x037|+0x01a:'38'		if ( !deserializing() &&
	// <0>
	// <0x59e081>|0x051|+0x02f:'40'		chamber_a_round_on_reload() && ammo_in_magazine() )
	// <0>
	// <0x59e0b0>|0x080|+0x00e:'42'		instant_chamber_a_round();
	// <0>
	// <1>
	// ******
}

// STATE[83.55%|PARTIAL]: dummy::nonnull/finalize_impl ICF fold + intrusive_ptr::operator== operand
// scheduling (LTCG call-boundary). Structure matches statement-for-statement. See md.
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
}

} // namespace survarium
