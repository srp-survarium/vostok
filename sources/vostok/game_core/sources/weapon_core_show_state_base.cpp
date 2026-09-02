// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/weapon_core_show_state_base.h>
#include <vostok/game_core/weapon_core.h>		// m_weapon.instant_toggle_start/end

namespace survarium {

weapon_core_show_state_base::weapon_core_show_state_base( weapon_core& weapon, bool& is_shown ) :
	weapon_core_animation_end_aware_state	( weapon, true ),
	m_is_shown								( is_shown )
{
	m_body_part_mask_for_user = animation::body_part_whole_body_but_hands;
}

void weapon_core_show_state_base::initialize( )
{
	weapon_core_animation_end_aware_state::initialize( );
	ASSERT( UNKNOWN_EXPRESSION );	// compiled-out ASSERT (target's `call empty_stub`)
	m_weapon.instant_toggle_start( );
}

void weapon_core_show_state_base::finalize( )
{
	weapon_core_animation_end_aware_state::finalize( );
	m_weapon.instant_toggle_end( );
}

void weapon_core_show_state_base::on_animation_end_impl( bool& animation_player_tick_result )
{
	ASSERT( UNKNOWN_EXPRESSION );	// compiled-out ASSERT (target's `call empty_stub`)
	m_is_shown = true;
	animation_player_tick_result = true;
}

} // namespace survarium
