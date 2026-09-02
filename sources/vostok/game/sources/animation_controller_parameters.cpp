// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "animation_controller_parameters.h"

namespace survarium {

bool operator==(
	simple_animation_controller_parameters const&	first,
	simple_animation_controller_parameters const&	second
)
{
	return first.emitter == second.emitter;
}

bool operator!=(
	simple_animation_controller_parameters const&	first,
	simple_animation_controller_parameters const&	second
)
{
	return first.emitter != second.emitter;
}

bool operator==(
	movement_animation_controller_parameters const&	first,
	movement_animation_controller_parameters const&	second
)
{
	return	( first.position == second.position )			&&
			( first.eyes_direction == second.eyes_direction )	&&
			( first.velocity == second.velocity )			&&
			( first.animation == second.animation );
}

bool operator!=(
	movement_animation_controller_parameters const&	first,
	movement_animation_controller_parameters const&	second
)
{
	return !( first == second );
}

void simple_animation_controller_parameters::reset( )
{
	emitter = NULL;
}

void movement_animation_controller_parameters::reset( )
{
	animation = NULL;
}

} // namespace survarium
