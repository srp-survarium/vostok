////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

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

// claude@NOTE: parked - structure recovered (random m_animations[i] pick via
// m_random, then an animation_lexeme built from animation_lexeme_parameters with a
// linear_interpolator( s_aim_transition_time ) builder chain, returned as
// make_pair( expression( movement_lexeme ), movement_lexeme )). Byte residual is the
// module-wide animation_lexeme_parameters builder-chain inline-vs-call wall (the
// VOSTOK_ANIMATION_API setters / create_animation_intervals go out-of-line in the
// target but inline here) - same ceiling as player_logic_stand_state::movement_lexeme
// and weapon_core_aimed_fire_state::get_user_hands_expression. NEXT: confirm the exact
// builder-chain setters + the m_animations[i] identifier once that wall is broken;
// the s_aim_transition_time + create_animation_intervals path is from the asm.
std::pair< animation::mixing::expression, animation::mixing::animation_lexeme > player_logic_preview_state::selected_animations(
	mutable_buffer&							buffer,
	weapon_animation_parameters const&		weapon_parameters,
	const bool								is_third_view
) const
{
	u32 const							animation_index	= m_random.random( m_animations_count );

	animation::linear_interpolator		l_interpolator( s_aim_transition_time );

	animation::mixing::animation_lexeme	movement_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			NULL,
			m_animations[ animation_index ],
			0,
			0
		)
		.weight_interpolator	( l_interpolator )
		.time_scale_interpolator( l_interpolator )
	);

	return std::make_pair< animation::mixing::expression, animation::mixing::animation_lexeme >(
		animation::mixing::expression( movement_lexeme ),
		movement_lexeme
	);
}

} // namespace survarium
