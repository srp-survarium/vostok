// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/weapon_core_shotgun_reload_one_round_substate.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/animation/animation_callback.h>

namespace survarium {

weapon_core_shotgun_reload_one_round_substate::weapon_core_shotgun_reload_one_round_substate(
	weapon_core&							weapon,
	const float									animation_time_scale,
	resources::managed_resource_ptr const*	animations,
	const u32										animations_count
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

bool weapon_core_shotgun_reload_one_round_substate::is_ready_for_transition( ) const
{
	return true;
}

void weapon_core_shotgun_reload_one_round_substate::initialize( )
{
	m_weapon.set_animation_callback(
		animation::channel_id_on_animation_end,
		this,
		boost::bind( &weapon_core_shotgun_reload_one_round_substate::on_animation_end, this, _1 )
	);
}

void weapon_core_shotgun_reload_one_round_substate::finalize( )
{
	ASSERT( UNKNOWN_EXPRESSION ); m_animation_playback_state->reset( );
	m_weapon.remove_animation_callback( animation::channel_id_on_animation_end, this );
}

// claude@NOTE: structure-correct; residual is the intrusive_ptr operator== operand-evaluation
// order (target loads `this` before the arg; /Od reverses it). Non-steerable from source.
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
}

} // namespace survarium
