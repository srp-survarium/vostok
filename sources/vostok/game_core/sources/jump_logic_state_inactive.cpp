////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "jump_logic_state_inactive.h"

namespace survarium {

// STATE[100%|DONE]: empty frame, never touches the return ptr (target @0x57ce30)
std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> jump_logic_state_inactive::selected_animations(
	mutable_buffer&						buffer,
	bool								__formal1,
	animation_delegate const&			__formal2,
	weapon_animation_parameters const&	__formal3
)
{
	UNREACHABLE_CODE( );
}

} // namespace survarium
