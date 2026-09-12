// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/pistol_weapon_core_hide_state.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/game_core/weapon_state_creation_params.h>
#include <vostok/game_core/weapon_core_state_cook_template.h>

#include <vostok/animation/cubic_spline_skeleton_animation.h>
#include <vostok/animation/linear_interpolator.h>
#include <vostok/animation/mixing_addition_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme_parameters.h>

namespace survarium {

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

pistol_weapon_core_hide_state::pistol_weapon_core_hide_state(
	weapon_core&							weapon,
	const float									animation_timescale,
	resources::managed_resource_ptr const*	animations,
	const u32										animations_count,
	bool&									is_shown
) : weapon_core_hide_state_base( weapon, is_shown ),
	m_time_scale( animation_timescale )
{
	ASSERT_CMP_U( animations_count, ==, 12 );

	u32 animation_index = 0;
	for ( u32 view_index = 0 ; view_index != 2 ; ++view_index ) {
		for ( u32 user_state_index = 0 ; user_state_index != 2 ; ++user_state_index ) {
			for ( u32 weapon_state_index = 0 ; weapon_state_index != 2 ; ++weapon_state_index ) {
				m_weapon_animations[view_index][user_state_index][weapon_state_index] = animations[animation_index++];
			}
		}
	}

	for ( u32 view_index = 0 ; view_index != 2 ; ++view_index ) {
		for ( u32 user_state_index = 0 ; user_state_index != 2 ; ++user_state_index ) {
			m_user_animations[view_index][user_state_index] = animations[animation_index++];
		}
	}

	ASSERT( UNKNOWN_EXPRESSION );
}

// claude@NOTE: systemic ceiling shared by every weapon_*_state weapon_and_hands_expression.
// Structure is a 3-statement MATCH; only the return diverges - the target inlines
// mixing::operator+ where our build emits a call. Inline-vs-call codegen; not source-steerable.
animation::mixing::expression pistol_weapon_core_hide_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool const							is_third_view,
	weapon_user_state_enum const		user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );

	animation::mixing::expression hands_expression = get_user_hands_expression( lexeme_pair.offset_lexeme, buffer, is_third_view, user_state_id, weight_driving_animation );

	return hands_expression + lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme;
}

// claude@NOTE: STRUCTURE MATCH; residual is the /Od register-coloring nibble at the out-of-line
// m_weapon.ammo_in_magazine() thiscall (target loads the weapon ptr into eax, our build into ecx).
weapon_lexeme_pair pistol_weapon_core_hide_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool const is_third_view, weapon_user_state_enum const user_state_id ) const
{
	pcstr weapon_animation_captions[2] =
	{
		"pistol-hide",
		"pistol-hide_empty"
	};

	u32 const animation_index = m_weapon.ammo_in_magazine( ) == 0;
	pcstr animation_identifier = weapon_animation_captions[animation_index];

	resources::managed_resource_ptr const& selected_animation =
		m_weapon_animations[is_third_view != false][user_state_id == type_crouch][animation_index];

	set_animation_to_wait( selected_animation );

	return get_weapon_lexeme_pair_impl(
		buffer,
		animation_identifier,
		selected_animation,
		&m_weapon,
		m_animation_playback_state,
		7,
		m_time_scale,
		animation::mixing::play_once_and_freeze_at_end,
		animation::linear_interpolator( 0.3f )
	);
}

// claude@NOTE: systemic inline-vs-call ceiling shared by every weapon_*_state
// get_user_hands_expression (matched weapon_core_show/hide_state siblings sit at the same %).
// 7-statement STRUCTURE MATCH (extra local: linear_interpolator interpolator); the target leaves
// the lexeme_parameters setters + ~animation_lexeme_parameters out-of-line and inlines the
// expression ctor (early return) to a bare simple_lock, where our build inlines the setters and
// calls the ctor. Not source-steerable.
animation::mixing::expression pistol_weapon_core_hide_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool const							is_third_view,
	weapon_user_state_enum const		user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	if ( user_state_id == type_sprint )
		return weapon_lexeme;

	u32 const user_state_index = user_state_id == type_crouch;

	pcstr const animation_captions[2] = { "stand_hide", "crouch_hide" };

	animation::linear_interpolator interpolator( 0.3f );

	animation::mixing::animation_lexeme override_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			animation_captions[user_state_index],
			m_user_animations[is_third_view != false][user_state_index],
			&weapon_lexeme,
			&weight_driving_animation
		)
		.animated_object( m_weapon.get_user( ) )
		.bones_mask( 2 )
		.playback_type( animation::mixing::play_once_and_freeze_at_end )
	);

	return override_lexeme;
}

pistol_weapon_core_hide_state* weapon_core_state_cook_template<survarium::pistol_weapon_core_hide_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	const u32									animations_count
)
{
	float weapon_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[0] )->length_in_frames( );
	float user_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[8] )->length_in_frames( );
	float time_scale = weapon_anim_length / user_anim_length;

	return new ( buffer.c_ptr( ) ) pistol_weapon_core_hide_state( params->weapon, time_scale, animations, animations_count, params->shown );
}

} // namespace survarium
