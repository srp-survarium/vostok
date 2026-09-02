// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "player_logic_preview_state.h"

#include <vostok/animation/mixing_animation_lexeme_parameters.h>
#include <vostok/animation/mixing_expression.h>
#include <vostok/animation/linear_interpolator.h>

namespace survarium {

static float s_aim_transition_time = 0.3f;

 player_logic_preview_state::player_logic_preview_state(
	resources::managed_resource_ptr*	animations,
	u32									animations_count,
	weapon_user_animations_selector&	owner
) :
	player_logic_base_state	( owner, type_preview ),
	m_animations			( animations ),
	m_animations_count		( animations_count ),
	m_random				( 0 )
{
}

// The target inlines the weight-group and bones-mask setters at this call site;
// their shared declarations currently force the base calls out of line.

std::pair< animation::mixing::expression, animation::mixing::animation_lexeme > player_logic_preview_state::selected_animations(
	mutable_buffer&							buffer,
	weapon_animation_parameters const&		weapon_parameters,
	const bool								is_third_view
) const
{
	VOSTOK_UNREFERENCED_PARAMETER( is_third_view );

	animation::linear_interpolator		l_interpolator( s_aim_transition_time );

	animation::mixing::animation_lexeme	movement_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			NULL,
			m_animations[ m_random.random( m_animations_count ) ],
			0,
			0
		)
		.weight_synchronization_group_id	( 0 )
		.weight_interpolator				( l_interpolator )
		.animated_object					( m_user )
		.bones_mask						( weapon_parameters.body_part_mask )
	);

	return std::make_pair< animation::mixing::expression, animation::mixing::animation_lexeme >(
		animation::mixing::expression( movement_lexeme ),
		movement_lexeme
	);
}

} // namespace survarium
