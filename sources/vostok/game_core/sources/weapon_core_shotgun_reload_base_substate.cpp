// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include <vostok/game_core/weapon_core_shotgun_reload_base_substate.h>

#include <vostok/game_core/weapon_core.h>

#include <vostok/animation/mixing_addition_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme_parameters.h>
#include <vostok/animation/linear_interpolator.h>

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

weapon_core_shotgun_reload_base_substate::weapon_core_shotgun_reload_base_substate(
	weapon_core&							weapon,
	float const								animation_time_scale,
	resources::managed_resource_ptr const*	animations,
	u32 const								animations_count,
	animation::mixing::playback_enum const	playback_type,
	u32 const								time_synchronization_group,
	pcstr									animation_id,
	pcstr									hands_stand_animation_id,
	pcstr									hands_crouch_animation_id,
	pcstr									hands_jump_animation_id
) :
	m_weapon( weapon ),
	m_animation_playback_state( 0 ),
	m_animation_timescale( animation_time_scale ),
	m_playback_type( playback_type ),
	m_time_synchronization_group( time_synchronization_group ),
	m_animation_id( animation_id )
{
	m_hand_animation_captions[0] = hands_stand_animation_id;
	m_hand_animation_captions[1] = hands_crouch_animation_id;
	m_hand_animation_captions[2] = hands_jump_animation_id;

	ASSERT_CMP_U( animations_count, ==, total_animations_count );

	u32 animation_index = 0;
	for ( u32 view_index = 0 ; view_index != views_count ; ++view_index ) {
		for ( u32 user_state_index = 0 ; user_state_index != user_states_count ; ++user_state_index ) {
			m_weapon_animations[view_index][user_state_index] = animations[animation_index++];
		}
	}

	for ( u32 view_index = 0 ; view_index != views_count ; ++view_index ) {
		for ( u32 user_state_index = 0 ; user_state_index != user_states_count ; ++user_state_index ) {
			m_user_animations[view_index][user_state_index] = animations[animation_index++];
		}
	}

	ASSERT( UNKNOWN_EXPRESSION );
}

weapon_lexeme_pair weapon_core_shotgun_reload_base_substate::get_weapon_lexeme_pair( mutable_buffer& buffer, bool const is_third_view, weapon_user_state_enum const user_state_id ) const
{
	m_animation_to_wait_for = m_weapon_animations[is_third_view != false][user_state_id == type_crouch];

	return get_weapon_lexeme_pair_impl(
		buffer,
		m_animation_id,
		m_animation_to_wait_for,
		&m_weapon,
		*m_animation_playback_state,
		m_time_synchronization_group,
		m_animation_timescale,
		m_playback_type,
		animation::linear_interpolator( 0.3f )
	);
}

animation::mixing::expression weapon_core_shotgun_reload_base_substate::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	const bool							is_third_view,
	const weapon_user_state_enum		user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );

	const u32 user_state_index = user_state_id == type_crouch;

	animation::mixing::animation_lexeme override_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			m_hand_animation_captions[user_state_index],
			m_user_animations[is_third_view != false][user_state_index],
			&lexeme_pair.offset_lexeme,
			&weight_driving_animation
		)
		.animated_object( m_weapon.get_user( ) )
		.bones_mask( 2 )
		.playback_type( m_playback_type )
	);

	return override_lexeme + lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme;
}

} // namespace survarium
