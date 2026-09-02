// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/weapon_core_animation_end_aware_state.h>
#include <vostok/game_core/weapon_core.h>		// m_weapon.set/remove_animation_callback, get_user
#include <vostok/game_core/base_player.h>		// get_user()->is_replaying_history()

namespace survarium {

void weapon_core_animation_end_aware_state::initialize( )
{
	m_animation_has_been_ended = false;
	m_weapon.set_animation_callback(
		animation::channel_id_on_animation_end,
		this,
		boost::bind( &weapon_core_animation_end_aware_state::on_animation_end, this, _1 )
	);
}

void weapon_core_animation_end_aware_state::finalize( )
{
	m_weapon.remove_animation_callback( animation::channel_id_on_animation_end, this );
}

void weapon_core_animation_end_aware_state::set_animation_to_wait( resources::managed_resource_ptr const& animation ) const
{
	if ( !m_weapon.get_user()->is_replaying_history() )
		m_animation_to_wait_for = animation;
}

animation::callback_return_type_enum weapon_core_animation_end_aware_state::on_animation_end( animation::animation_callback_params& params )
{
	params.interrupt_animation_player_tick = false;
	if ( params.animated_object == &m_weapon )
	{
		ASSERT( UNKNOWN_EXPRESSION );	// compiled-out ASSERT (target's `call empty_stub` @ +0x20)
		if ( m_animation_to_wait_for == params.animation )
		{
			m_animation_has_been_ended = true;
			on_animation_end_impl( params.interrupt_animation_player_tick );
		}
	}

	return animation::callback_return_type_call_me_again;
}

} // namespace survarium
