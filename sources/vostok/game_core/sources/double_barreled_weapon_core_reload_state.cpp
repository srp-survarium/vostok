// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/double_barreled_weapon_core_reload_state.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/game_core/weapon_state_creation_params.h>
#include <vostok/game_core/weapon_core_state_cook_template.h>
#include <vostok/game_core/weapon_animations_timescale_inline.h>

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

double_barreled_weapon_core_reload_state::double_barreled_weapon_core_reload_state(
	weapon_core&							weapon,
	const float									animation_timescale,
	resources::managed_resource_ptr const*	animations,
	const u32										animations_count
) : weapon_core_reload_state_base( weapon, animation_timescale )
{
	ASSERT_CMP_U( weapon.get_magazine_capacity( ), ==, 2 );
	ASSERT_CMP_U( animations_count, ==, 16 );

	u32 animation_index = 0;
	for ( u32 view = 0 ; view != 2 ; ++view ) {
		for ( u32 user_state = 0 ; user_state != 2 ; ++user_state ) {
			for ( u32 weapon_state = 0 ; weapon_state != 2 ; ++weapon_state ) {
				m_weapon_animations[view][user_state][weapon_state] = animations[animation_index++];
			}
		}
	}

	for ( u32 view = 0 ; view != 2 ; ++view ) {
		for ( u32 user_state = 0 ; user_state != 2 ; ++user_state ) {
			for ( u32 weapon_state = 0 ; weapon_state != 2 ; ++weapon_state ) {
				m_user_animations[view][user_state][weapon_state] = animations[animation_index++];
			}
		}
	}

	ASSERT( UNKNOWN_EXPRESSION );
}

animation::mixing::expression double_barreled_weapon_core_reload_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	const bool								is_third_view,
	const weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );

	animation::mixing::expression hands_expression = get_user_hands_expression( lexeme_pair.offset_lexeme, buffer, is_third_view, user_state_id, weight_driving_animation );

	return hands_expression + lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme;
}

weapon_lexeme_pair double_barreled_weapon_core_reload_state::get_weapon_lexeme_pair( mutable_buffer& buffer, const bool is_third_view, const weapon_user_state_enum user_state_id ) const
{
	ASSERT( UNKNOWN_EXPRESSION );

	pcstr weapon_animation_captions[2] = {
		"reload_first_barrel",
		"reload_both_barrels"
	};

	u32 weapon_state_index = m_weapon.ammo_in_magazine( ) != 1;

	pcstr animation_identifier = weapon_animation_captions[weapon_state_index];

	resources::managed_resource_ptr const& selected_animation =
		m_weapon_animations[is_third_view != false][user_state_id == type_crouch][weapon_state_index];

	set_animation_to_wait( selected_animation );

	return get_weapon_lexeme_pair_impl(
		buffer,
		animation_identifier,
		selected_animation,
		&m_weapon,
		m_animation_playback_state,
		2,
		m_animation_timescale,
		animation::mixing::play_once_and_freeze_at_end,
		animation::linear_interpolator( s_aim_transition_time )
	);
}

animation::mixing::expression double_barreled_weapon_core_reload_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	const bool								is_third_view,
	const weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	pcstr const user_animation_captions[2][2] = {
		{ "stand_reload_first_barrel", "stand_reload_both_barrels" },
		{ "crouch_reload_first_barrel", "crouch_reload_both_barrels" }
	};

	u32 const user_state_index = user_state_id == type_crouch;

	u32 const weapon_state_index = m_weapon.ammo_in_magazine( ) != 1;

	animation::mixing::animation_lexeme override_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			user_animation_captions[user_state_index][weapon_state_index],
			m_user_animations[is_third_view != false][user_state_index][weapon_state_index],
			&weapon_lexeme,
			&weight_driving_animation
		)
		.animated_object( m_weapon.get_user( ) )
		.weight_interpolator( animation::linear_interpolator( s_aim_transition_time ) )
		.bones_mask( 2 )
		.playback_type( animation::mixing::play_once_and_freeze_at_end )
	);

	return override_lexeme;
}

double_barreled_weapon_core_reload_state* weapon_core_state_cook_template<survarium::double_barreled_weapon_core_reload_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	const u32									animations_count
)
{
	return new ( buffer.c_ptr( ) ) double_barreled_weapon_core_reload_state(
		params->weapon,
		computed_reload_animation_time_scale( animations[0], params->reload_time ),
		animations,
		animations_count
	);
}

} // namespace survarium
