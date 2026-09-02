// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/weapon_core_idle_state_base.h>
#include <vostok/game_core/weapon_core.h>

namespace survarium {

weapon_core_idle_state_base::weapon_core_idle_state_base( weapon_core& weapon ) :
	weapon_core_base_state( weapon, false )
{
}

void weapon_core_idle_state_base::initialize( )
{
	m_weapon.instant_idle_start( );
}

void weapon_core_idle_state_base::finalize( )
{
	m_weapon.instant_idle_end( );
}

} // namespace survarium
