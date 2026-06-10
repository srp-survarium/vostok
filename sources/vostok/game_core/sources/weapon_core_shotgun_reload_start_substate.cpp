////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_shotgun_reload_start_substate.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/animation/animation_callback.h>

namespace survarium {

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
}

void weapon_core_shotgun_reload_start_substate::finalize( )
{
	ASSERT( UNKNOWN_EXPRESSION ); m_animation_playback_state->reset( );
	m_weapon.remove_animation_callback( animation::channel_id_on_animation_end, this );
}

bool weapon_core_shotgun_reload_start_substate::is_ready_for_transition( ) const
{
	return m_animation_ended;
}

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
}

} // namespace survarium
