////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_hide_state_base.h>
#include <vostok/game_core/weapon_core.h>		// m_weapon.instant_toggle_start/end

namespace survarium {

// STATE[100%|DONE]
// survarium::weapon_core_hide_state_base::weapon_core_hide_state_base(survarium::weapon_core&, bool&)
weapon_core_hide_state_base::weapon_core_hide_state_base( weapon_core& weapon, bool& is_shown ) :
	weapon_core_animation_end_aware_state	( weapon, true ),
	m_is_shown								( is_shown )
{
	// The base ctor stored body_part_whole_body (-1); hide overrides it to
	// body_part_whole_body_but_hands (-3) -> the [this+130h]=0FFFFFFFDh store.
	m_body_part_mask_for_user = animation::body_part_whole_body_but_hands;

	// FUNCTION BODY
	// <0x6fd2d5>|0x055|+0x00d:'21'
	// ******
}

// STATE[100%|DONE]: mirror of show - the empty_stub between base init and
// instant_toggle_start was a compiled-out ASSERT (recovered below), not an LTCG call.
void weapon_core_hide_state_base::initialize( )
{
	weapon_core_animation_end_aware_state::initialize( );
	ASSERT( UNKNOWN_EXPRESSION );	// compiled-out ASSERT (the target's `call empty_stub`)
	m_weapon.instant_toggle_start( );
}

// STATE[100%|DONE]
// void survarium::weapon_core_hide_state_base::finalize()
void weapon_core_hide_state_base::finalize( )
{
	weapon_core_animation_end_aware_state::finalize( );
	m_weapon.instant_toggle_end( );

	// FUNCTION BODY
	// <0x6fd257>|0x007|+0x008:'34'
	// <0>
	// <0x6fd25f>|0x00f|+0x00e:'36'
	// ******
}

// STATE[100%|DONE]: opens with the same compiled-out ASSERT as initialize (recovered).
// NOTE: hide CLEARS m_is_shown (`mov byte[edx],0` @ this+140h) where show set 1.
void weapon_core_hide_state_base::on_animation_end_impl( bool& animation_player_tick_result )
{
	ASSERT( UNKNOWN_EXPRESSION );	// compiled-out ASSERT (the target's `call empty_stub`)
	m_is_shown = false;
	animation_player_tick_result = true;
}

} // namespace survarium
