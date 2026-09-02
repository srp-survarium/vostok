// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "jump_logic_state_inactive.h"

namespace survarium {

std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> jump_logic_state_inactive::selected_animations(
	mutable_buffer&						buffer,
	const bool								__formal1,
	fastdelegate::FastDelegate<float(float,float,u32,u32,u32,float)> const& __formal2,
	weapon_animation_parameters const&	__formal3
)
{
	UNREACHABLE_CODE( );
}

} // namespace survarium
