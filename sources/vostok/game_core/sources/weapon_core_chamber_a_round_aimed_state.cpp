////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_chamber_a_round_aimed_state.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/game_core/weapon_state_creation_params.h>
#include <vostok/game_core/weapon_core_state_cook_template.h>

#include <vostok/animation/linear_interpolator.h>
#include <vostok/animation/mixing_addition_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme_parameters.h>

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
weapon_core_chamber_a_round_aimed_state::weapon_core_chamber_a_round_aimed_state(
	weapon_core&							weapon,
	float									animation_time_scale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
) : weapon_core_chamber_a_round_aimed_state_base( weapon, animation_time_scale )
{
	ASSERT_CMP_U( animations_count, ==, 8 );

	u32 animation_index = 0;
	for ( u32 view = 0 ; view != 2 ; ++view )
		for ( u32 user_state = 0 ; user_state != 2 ; ++user_state )
			m_weapon_animations[view][user_state] = animations[animation_index++];

	for ( u32 view = 0 ; view != 2 ; ++view )
		for ( u32 user_state = 0 ; user_state != 2 ; ++user_state )
			m_user_animations[view][user_state] = animations[animation_index++];

	ASSERT( UNKNOWN_EXPRESSION );
}

// STATE[83.18%|PARTIAL]: shared cross-cutting wall, identical to
// weapon_core_chamber_a_round_state::weapon_and_hands_expression. Target uses the
// expression-returning operator+ family (expression::expression<animation_lexeme>,
// operator+<animation_lexeme>(expression&,..), operator+(expression&,expression&))
// which is NOT declared in the shared mixing_addition_lexeme headers; our generic
// addition_lexeme operator+ resolves a different chain. Recovering needs that operator+
// family added to the shared API - a separate unit, out of this unit's scope. See .md.
animation::mixing::expression weapon_core_chamber_a_round_aimed_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );

	animation::mixing::expression hands_expression = get_user_hands_expression( lexeme_pair.offset_lexeme, buffer, is_third_view, user_state_id, weight_driving_animation );

	return hands_expression + lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme;

	// FUNCTION BODY
	// <0x7aa250>|0x010|+0x01f:'45'
	// <0x7aa26f>|0x02f|+0x02a:'46'
	// <0x7aa299>|0x059|+0x07a:'47'
	// ******
}

// STATE[100%|DONE]
weapon_lexeme_pair weapon_core_chamber_a_round_aimed_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	pcstr animation_identifier = "weapon-chamber_a_round_aimed";

	resources::managed_resource_ptr const& selected_animation =
		m_weapon_animations[is_third_view != false][user_state_id == type_crouch];

	set_animation_to_wait( selected_animation );

	return get_weapon_lexeme_pair_impl(
		buffer,
		animation_identifier,
		selected_animation,
		&m_weapon,
		m_animation_playback_state,
		8,
		m_animation_timescale,
		animation::mixing::play_once_and_freeze_at_end,
		animation::linear_interpolator( s_aim_transition_time )
	);
}

// STATE[73.86%|PARTIAL]: shared cross-cutting wall, identical to
// weapon_core_chamber_a_round_state::get_user_hands_expression. Structure matches the
// carcass (if-sprint early return, ASSERT, captions, lexeme_parameters fluent build,
// return override_lexeme). Target keeps animation_lexeme_parameters setters
// (animated_object/bones_mask/playback_type) and the parameters dtor as OUT-OF-LINE
// calls; our /Od build inlines those trivial in-class setters and the dtor down to its
// ASSERT. Whole-program inline decision - not steerable from this file. See .md.
animation::mixing::expression weapon_core_chamber_a_round_aimed_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	if ( user_state_id == type_sprint )
		return weapon_lexeme;

	ASSERT( UNKNOWN_EXPRESSION );

	u32 user_animation_index = user_state_id == type_crouch;

	pcstr animation_captions[3] = { "stand_chamber_a_round_aimed", "crouch_chamber_a_round_aimed", "jump_chamber_a_round_aimed" };

	animation::mixing::animation_lexeme override_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			animation_captions[user_animation_index],
			m_user_animations[is_third_view != false][user_animation_index],
			&weapon_lexeme,
			&weight_driving_animation
		)
		.animated_object( m_weapon.get_user( ) )
		.bones_mask( 2 )
		.playback_type( animation::mixing::play_once_and_freeze_at_end )
	);

	return override_lexeme;

	// FUNCTION BODY
	// <0x7aa0b1>|0x011|+0x006:'63'
	// <0x7aa0b7>|0x017|+0x010:'64'
	// <0>
	// <0x7aa0c7>|0x027|+0x012:'66'
	// <0>
	// <0x7aa0d9>|0x039|+0x00c:'68'
	// <0>
	// <0x7aa0e5>|0x045|+0x015:'70'
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
	// <0x7aa0fa>|0x05a|+0x079:'83'
	// <0x7aa173>|0x0d3|+0x01c:'84'
	// ******
}

// STATE[100%|DONE]
weapon_core_chamber_a_round_aimed_state* weapon_core_state_cook_template<survarium::weapon_core_chamber_a_round_aimed_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	u32									animations_count
)
{
	return new ( buffer.c_ptr( ) ) weapon_core_chamber_a_round_aimed_state(
		params->weapon,
		1.0f,
		animations,
		animations_count
	);
}

} // namespace survarium
