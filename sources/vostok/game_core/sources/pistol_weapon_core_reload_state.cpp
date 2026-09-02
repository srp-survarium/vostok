// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/pistol_weapon_core_reload_state.h>

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

pistol_weapon_core_reload_state::pistol_weapon_core_reload_state(
	weapon_core&							weapon,
	const float									animation_timescale,
	resources::managed_resource_ptr const*	animations,
	const u32										animations_count
) : weapon_core_reload_state_base( weapon, animation_timescale )
{
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

animation::mixing::expression pistol_weapon_core_reload_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool const							is_third_view,
	weapon_user_state_enum const		user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );

	if ( user_state_id == type_sprint )
		return lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme;

	ASSERT( UNKNOWN_EXPRESSION );

	animation::mixing::expression hands_expression = get_user_hands_expression( lexeme_pair.offset_lexeme, buffer, is_third_view, user_state_id, weight_driving_animation );

	return hands_expression + lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme;
}

weapon_lexeme_pair pistol_weapon_core_reload_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool const is_third_view, weapon_user_state_enum const user_state_id ) const
{
	pcstr weapon_animation_captions[2] =
	{
		"pistol-reload",
		"pistol-reload_empty"
	};

	u32 const weapon_state_index = m_weapon.ammo_in_magazine( ) == 0;

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

animation::mixing::expression pistol_weapon_core_reload_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool const							is_third_view,
	weapon_user_state_enum const		user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	// PDB records char const*[2][2] (no pointer const); each inner row attributes its two
	// element stores to its own line (two 0xe statements).
	pcstr user_animation_captions[2][2] = {
		{ "stand_reload_pistol", "stand_reload_empty_pistol" },
		{ "crouch_reload_pistol", "crouch_reload_empty_pistol" }
	};

	u32 const user_state_index = user_state_id == type_crouch;

	u32 const weapon_state_index = m_weapon.ammo_in_magazine( ) == 0;

	animation::linear_interpolator interpolator( s_aim_transition_time );

	animation::mixing::animation_lexeme override_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			user_animation_captions[user_state_index][weapon_state_index],
			m_user_animations[is_third_view != false][user_state_index][weapon_state_index],
			&weapon_lexeme,
			&weight_driving_animation
		)
		.animated_object( m_weapon.get_user( ) )
		.bones_mask( 2 )
		.playback_type( animation::mixing::play_once_and_freeze_at_end )
	);

	return override_lexeme;
}

pistol_weapon_core_reload_state* weapon_core_state_cook_template<survarium::pistol_weapon_core_reload_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	const u32									animations_count
)
{
	return new ( buffer.c_ptr( ) ) pistol_weapon_core_reload_state(
		params->weapon,
		computed_reload_animation_time_scale( animations[0], params->reload_time ),
		animations,
		animations_count
	);
}

} // namespace survarium
