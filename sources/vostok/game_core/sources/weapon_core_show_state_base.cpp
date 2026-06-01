////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_show_state_base.h>
#include <vostok/game_core/weapon_core.h>		// m_weapon.instant_toggle_start/end

namespace survarium {

// STATE[100%|DONE]
// survarium::weapon_core_show_state_base::weapon_core_show_state_base(survarium::weapon_core&, bool&)
weapon_core_show_state_base::weapon_core_show_state_base( weapon_core& weapon, bool& is_shown ) :
	weapon_core_animation_end_aware_state	( weapon, true ),
	m_is_shown								( is_shown )
{
	// The base ctor stored body_part_whole_body (-1); show overrides it to
	// body_part_whole_body_but_hands (-3) -> the [this+130h]=0FFFFFFFDh store.
	m_body_part_mask_for_user = animation::body_part_whole_body_but_hands;

	// FUNCTION BODY
	// <0x6fd3d5>|0x055|+0x00d:'22'
	// ******
}

// STATE[100%|DONE]: the empty_stub between base init and instant_toggle_start was a
// compiled-out ASSERT (recovered below), not an LTCG-folded call.
void weapon_core_show_state_base::initialize( )
{
	weapon_core_animation_end_aware_state::initialize( );
	ASSERT( UNKNOWN_EXPRESSION );	// compiled-out ASSERT (the target's `call empty_stub`)
	m_weapon.instant_toggle_start( );
}

// STATE[100%|DONE]
// void survarium::weapon_core_show_state_base::finalize()
void weapon_core_show_state_base::finalize( )
{
	weapon_core_animation_end_aware_state::finalize( );
	m_weapon.instant_toggle_end( );

	// FUNCTION BODY
	// <0x6fd357>|0x007|+0x008:'35'
	// <0>
	// <0x6fd35f>|0x00f|+0x00e:'37'
	// ******
}

// STATE[100%|DONE]: opens with the same compiled-out ASSERT as initialize (recovered).
void weapon_core_show_state_base::on_animation_end_impl( bool& animation_player_tick_result )
{
	ASSERT( UNKNOWN_EXPRESSION );	// compiled-out ASSERT (the target's `call empty_stub`)
	m_is_shown = true;
	animation_player_tick_result = true;
}

} // namespace survarium
