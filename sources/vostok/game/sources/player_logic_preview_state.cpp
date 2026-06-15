////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_logic_preview_state.h"

namespace survarium {

// STATE[STUB]
// base/const-member init-list is buildability scaffolding (type_preview is the
// obvious id for this state); a matcher confirms it
 player_logic_preview_state::player_logic_preview_state(
	resources::managed_resource_ptr*	animations,
	u32									animations_count,
	weapon_user_animations_selector&	owner
) :
	player_logic_base_state	( owner, type_preview ),
	m_animations			( animations ),
	m_animations_count		( animations_count )
{
	// FUNCTION BODY[0x777950]: 0
	// <0x777950>|0x000|+0x025:'26'	{
	// <0x777975>|0x025|      :'27'	}
	// ******
}

// STATE[STUB]
std::pair< animation::mixing::expression, animation::mixing::animation_lexeme > player_logic_preview_state::selected_animations(
	mutable_buffer&							buffer,
	weapon_animation_parameters const&		weapon_parameters,
	const bool								is_third_view
) const
{
	// LOCALS
	// animation::mixing::animation_lexeme movement_lexeme
	// animation::linear_interpolator 	l_interpolator
	// ******

	// FUNCTION BODY[0x777980]: 16
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <0x777990>|0x010|+0x12e:'48'
	// <0>
	// <0x777abe>|0x13e|+0x03e:'50'
	// ******
}

} // namespace survarium
