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

// STATE[75%|PARTIAL]: LTCG-folded empty out-of-line call (source line 28) unreproducible
// void survarium::weapon_core_hide_state_base::initialize()
void weapon_core_hide_state_base::initialize( )
{
	// claude@MATCH: mirror of weapon_core_show_state_base::initialize - between the base
	// initialize() and instant_toggle_start() the target emits `mov byte[ebp-1],0;
	// lea eax,[ebp-1]; call <empty>` (source line 28), the documented stripped/folded
	// empty out-of-line call (delinker-misnamed `fixed_size_allocator::finalize_impl`,
	// arg in eax with no push/cleanup, no nameable symbol in either rich index - see
	// assembly_patterns.md). Not reproducible from this function's source; everything
	// else (base init call + instant_toggle_start) matches byte-for-byte.
	weapon_core_animation_end_aware_state::initialize( );
	m_weapon.instant_toggle_start( );

	// FUNCTION BODY
	// <0x6fd2f9>|0x009|+0x008:'26'
	// <0x6fd301>|0x011|+0x00c:'27'	<- LTCG-folded empty call, see claude@MATCH
	// <0>
	// <0x6fd30d>|0x01d|+0x00e:'29'
	// ******
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

// STATE[70%|PARTIAL]: LTCG-folded empty out-of-line call (source line 45) unreproducible
// void survarium::weapon_core_hide_state_base::on_animation_end_impl(bool&)
void weapon_core_hide_state_base::on_animation_end_impl( bool& animation_player_tick_result )
{
	// claude@MATCH: mirror of weapon_core_show_state_base::on_animation_end_impl - the
	// target opens with the same `mov byte[ebp-1],0; lea eax,[ebp-1]; call <empty>`
	// statement (the stripped/folded empty out-of-line call, see initialize). The two
	// stores below match byte-for-byte. NOTE: hide CLEARS m_is_shown (the weapon is now
	// hidden) -> `mov byte ptr [edx], 0` at [this+140h], where show set it to 1.
	m_is_shown = false;
	animation_player_tick_result = true;

	// FUNCTION BODY
	// <0x6fd229>|0x009|+0x00c:'41'	<- LTCG-folded empty call, see claude@MATCH
	// <0x6fd235>|0x015|+0x00c:'42'
	// <0x6fd241>|0x021|+0x006:'43'
	// ******
}

} // namespace survarium
