////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_aimed_state_base.h>
#include <vostok/game_core/weapon_core.h>

namespace survarium {

// STATE[100%|DONE]
weapon_core_aimed_state_base::weapon_core_aimed_state_base( weapon_core& weapon ) :
	weapon_core_base_state( weapon, false )
{
}

// STATE[100%|DONE]
void weapon_core_aimed_state_base::initialize( )
{
	m_weapon.instant_aim_start( );
}

// STATE[83.33%|PARTIAL]: residual is the inline-vs-call of animation_playback_state::reset()
// (target calls it out-of-line @0x087f60; base inlines/elides it). Per the new guidelines this
// must be re-diffed against source, not banked as LTCG - see the .md for the 5-iteration analysis.
void weapon_core_aimed_state_base::finalize( )
{
	m_animation_playback_state.reset( );
	m_weapon.instant_aim_end( );

	// FUNCTION BODY (kept: this is a PARTIAL match, not 100%)
	// <0x6ea5c7>|0x007|+0x00d:'28'
	// <0x6ea5d4>|0x014|+0x01e:'29'
	// ******
}

} // namespace survarium
