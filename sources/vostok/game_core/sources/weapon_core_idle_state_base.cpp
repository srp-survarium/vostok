////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_idle_state_base.h>
#include <vostok/game_core/weapon_core.h>

namespace survarium {

// STATE[100%|DONE]
weapon_core_idle_state_base::weapon_core_idle_state_base( weapon_core& weapon ) :
	weapon_core_base_state( weapon, false )
{
}

// STATE[100%|DONE]
void weapon_core_idle_state_base::initialize( )
{
	m_weapon.instant_idle_start( );
}

// STATE[100%|DONE]
void weapon_core_idle_state_base::finalize( )
{
	m_weapon.instant_idle_end( );
}

} // namespace survarium
