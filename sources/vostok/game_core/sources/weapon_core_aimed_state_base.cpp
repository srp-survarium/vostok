////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_aimed_state_base.h>
#include <vostok/game_core/weapon_core.h>

namespace survarium {

// STATE[100%|DONE]
// survarium::weapon_core_aimed_state_base::weapon_core_aimed_state_base(survarium::weapon_core&)
weapon_core_aimed_state_base::weapon_core_aimed_state_base( weapon_core& weapon ) :
	weapon_core_base_state( weapon, false )
{
	// Body is just the base-ctor delegation + the two compiler-emitted vtable
	// stores (fsm_state @0x00, unmanaged_resource @0x18) - same as idle_state_base.

	// FUNCTION BODY
	// <0x6ea600>|0x000|+0x028:'15'	{
	// <0x6ea628>|0x028|      :'16'	}
	// ******
}

// STATE[100%|DONE]
// void survarium::weapon_core_aimed_state_base::initialize()
void weapon_core_aimed_state_base::initialize( )
{
	// instant_aim_start is virtual on weapon_core, so the call through the
	// weapon_core& reference dispatches via the vtable (slot 0x8c) - unlike
	// idle's non-virtual instant_idle_start.
	m_weapon.instant_aim_start( );

	// FUNCTION BODY
	// <0>
	// <0x6ea597>|0x007|+0x01e:'21'
	// ******
}

// STATE[83.33%|PARTIAL]: LTCG inline-vs-call of animation_playback_state::reset()
// void survarium::weapon_core_aimed_state_base::finalize()
void weapon_core_aimed_state_base::finalize( )
{
	// claude@MATCH: the target emits `add eax,120h; call animation_playback_state::reset`
	// here (reset is kept out-of-line @ 0x087f60). Our /GL LTCG instead inlines reset's
	// real body (-> 54%) or, with the empty {} header stub, elides the no-op call (-> 83%,
	// only the 3 call instrs differ). reset() is kept empty so this is the closer, cleaner
	// match - an uncontrollable cross-module inline-vs-call (MATCHING.md LTCG), not a source bug.
	m_animation_playback_state.reset( );
	m_weapon.instant_aim_end( );

	// FUNCTION BODY
	// <0x6ea5c7>|0x007|+0x00d:'28'
	// <0x6ea5d4>|0x014|+0x01e:'29'
	// ******
}

} // namespace survarium
