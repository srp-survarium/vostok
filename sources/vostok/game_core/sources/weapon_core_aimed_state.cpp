////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_aimed_state.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/game_core/weapon_state_creation_params.h>
#include <vostok/game_core/weapon_core_state_cook_template.h>

#include <vostok/animation/mixing_addition_lexeme.h>
#include <vostok/animation/linear_interpolator.h>

namespace survarium {

static float s_aim_transition_time = 0.3f;

weapon_lexeme_pair get_weapon_lexeme_pair_impl(
	mutable_buffer&								buffer,
	pcstr										identifier,
	resources::managed_resource_ptr const&		animation,
	pcvoid										animated_object,
	animation::animation_playback_state const&	playback_state,
	u32											time_synchronization_group,
	float										time_scale,
	animation::mixing::playback_enum			playback_type,
	animation::base_interpolator const&			interpolator_for_offset_lexeme
);

// STATE[100%|DONE]
weapon_core_aimed_state::weapon_core_aimed_state( weapon_core& weapon, resources::managed_resource_ptr const* animations, u32 animations_count ) :
	weapon_core_aimed_state_base( weapon )
{
	ASSERT_CMP_U( animations_count, ==, 4 );

	u32 animation_index = 0;
	for ( u32 view = 0 ; view != 2 ; ++view )
		for ( u32 user_state = 0 ; user_state != 2 ; ++user_state )
			m_weapon_animations[view][user_state] = animations[animation_index++];
}

// STATE[85.65%|PARTIAL]: residual is the per-call-site LTCG inline-vs-call of
// operator+<animation_lexeme,animation_lexeme> (target inlines it here, base keeps the
// out-of-line call; operator+ is standalone in BOTH rich indexes -> whole-program inline
// decision, not a source bug), plus the line-31 ASSERT eater shape (target's
// expression_eater gets only the lexeme; ASSERT_U adds the assert_untyped `push 0`).
// Identical shape/diff to weapon_core_idle_state::weapon_and_hands_expression - see the .md.
animation::mixing::expression weapon_core_aimed_state::weapon_and_hands_expression(
	mutable_buffer&							buffer,
	bool									is_third_view,
	weapon_user_state_enum					user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	ASSERT_U( weight_driving_animation );

	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );

	return animation::mixing::expression( lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme );

	// FUNCTION BODY
	// <0x7ad131>|0x011|+0x036:'31'	ASSERT_U( weight_driving_animation );
	// <0x7ad167>|0x047|+0x01f:'32'	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( ... );
	// <0x7ad186>|0x066|+0x059:'33'	return expression( main + offset );  (target inlines operator+)
	// ******
}

// STATE[100%|DONE]
weapon_lexeme_pair weapon_core_aimed_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	pcstr animation_identifier = "weapon-aimed_idle";

	resources::managed_resource_ptr const& selected_animation =
		m_weapon_animations[is_third_view != false][user_state_id == type_crouch];

	return get_weapon_lexeme_pair_impl(
		buffer,
		animation_identifier,
		selected_animation,
		&m_weapon,
		m_animation_playback_state,
		u32( -1 ),
		1.0f,
		animation::mixing::play_cyclically,
		animation::linear_interpolator( s_aim_transition_time )
	);
}

// STATE[100%|DONE]
weapon_core_aimed_state* weapon_core_state_cook_template< weapon_core_aimed_state >::new_object(
	mutable_buffer							buffer,
	weapon_state_creation_params const*		params,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
)
{
	return new ( buffer.c_ptr( ) ) weapon_core_aimed_state( params->weapon, animations, animations_count );
}

} // namespace survarium
