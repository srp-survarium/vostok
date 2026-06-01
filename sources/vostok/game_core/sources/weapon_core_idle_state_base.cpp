////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_idle_state_base.h>
#include <vostok/game_core/weapon_core.h>

namespace survarium {

// STATE[100%|DONE]
// survarium::weapon_core_idle_state_base::weapon_core_idle_state_base(survarium::weapon_core&)
weapon_core_idle_state_base::weapon_core_idle_state_base( weapon_core& weapon ) :
	weapon_core_base_state( weapon, false )
{
	// Body is just the base-ctor delegation + the two compiler-emitted vtable
	// stores (fsm_state @0x00, unmanaged_resource @0x18).

	// FUNCTION BODY
	// <0x6fa680>|0x000|+0x028:'15'	{
	// <0x6fa6a8>|0x028|      :'16'	}
	// ******
}

// STATE[100%|DONE]
// void survarium::weapon_core_idle_state_base::initialize()
void weapon_core_idle_state_base::initialize( )
{
	m_weapon.instant_idle_start( );

	// FUNCTION BODY
	// <0>
	// <0x6fa667>|0x007|+0x00e:'21'
	// ******
}

// STATE[100%|DONE]
// void survarium::weapon_core_idle_state_base::finalize()
void weapon_core_idle_state_base::finalize( )
{
	m_weapon.instant_idle_end( );

	// FUNCTION BODY
	// <0>
	// <0x6fa647>|0x007|+0x00e:'27'
	// ******
}

} // namespace survarium
