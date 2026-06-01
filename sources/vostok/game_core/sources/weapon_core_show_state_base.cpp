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

// STATE[75.13%|PARTIAL]: LTCG-folded empty out-of-line call (source line 28) unreproducible
// void survarium::weapon_core_show_state_base::initialize()
void weapon_core_show_state_base::initialize( )
{
	// claude@MATCH: between the base initialize() and instant_toggle_start() the target
	// emits `mov byte[ebp-1],0; lea eax,[ebp-1]; call <empty>` (source line 28) - a 1-byte
	// local set to false whose address is passed to an out-of-line function that is EMPTY
	// (single `ret`, delinker-misnamed `fixed_size_allocator::finalize_impl` via COMDAT
	// folding). The arg is passed in eax with no push/cleanup, and no nameable symbol for
	// it exists in EITHER rich index - it is the documented stripped/folded-call artifact
	// (assembly_patterns.md). set_is_firing_ptr / set_is_firing are inlined everywhere (no
	// standalone symbol), so this call is not one of them. Not reproducible from this
	// function's source; everything else (base init call + instant_toggle_start) matches.
	weapon_core_animation_end_aware_state::initialize( );
	m_weapon.instant_toggle_start( );

	// FUNCTION BODY
	// <0x6fd3f9>|0x009|+0x008:'27'
	// <0x6fd401>|0x011|+0x00c:'28'	<- LTCG-folded empty call, see claude@MATCH
	// <0>
	// <0x6fd40d>|0x01d|+0x00e:'30'
	// ******
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

// STATE[69.93%|PARTIAL]: LTCG-folded empty out-of-line call (source line 42) unreproducible
// void survarium::weapon_core_show_state_base::on_animation_end_impl(bool&)
void weapon_core_show_state_base::on_animation_end_impl( bool& animation_player_tick_result )
{
	// claude@MATCH: the target opens with the SAME `mov byte[ebp-1],0; lea eax,[ebp-1];
	// call <empty>` statement as initialize line 28 (source line 42) - the stripped/folded
	// empty out-of-line call (see initialize). The two stores below match byte-for-byte.
	m_is_shown = true;
	animation_player_tick_result = true;

	// FUNCTION BODY
	// <0x6fd329>|0x009|+0x00c:'42'	<- LTCG-folded empty call, see claude@MATCH
	// <0x6fd335>|0x015|+0x00c:'43'
	// <0x6fd341>|0x021|+0x006:'44'
	// ******
}

} // namespace survarium
