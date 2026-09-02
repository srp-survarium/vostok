// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/double_barreled_weapon_core_aimed_idle_state.h>

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

double_barreled_weapon_core_aimed_idle_state::double_barreled_weapon_core_aimed_idle_state( weapon_core& weapon, resources::managed_resource_ptr const* animations, const u32 animations_count ) :
	weapon_core_aimed_state_base( weapon )
{
	ASSERT( UNKNOWN_EXPRESSION_T( animations ) );
	ASSERT_CMP_U( animations_count, ==, 12 );

	u32 animation_index = 0;
	for ( u32 view_index = 0 ; view_index != 2 ; ++view_index ) {
		for ( u32 user_state_index = 0 ; user_state_index != 2 ; ++user_state_index ) {
			for ( u32 weapon_state_index = 0 ; weapon_state_index != 3 ; ++weapon_state_index ) {
				m_weapon_animations[view_index][user_state_index][weapon_state_index] = animations[animation_index++];
			}
		}
	}

	ASSERT( UNKNOWN_EXPRESSION_T( animation_index == animations_count ) );
}

// claude@NOTE: systemic /Od ceiling - the expression() return materializes
// addition_lexeme via cloned_in_buffer (target) vs operator+ (base); identical
// residual to the matched weapon_core_aimed_state::weapon_and_hands_expression.
// Not source-steerable; structure matches (3/3 stmts).
animation::mixing::expression double_barreled_weapon_core_aimed_idle_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	const bool							is_third_view,
	const weapon_user_state_enum		user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	VOSTOK_UNREFERENCED_PARAMETERS( weight_driving_animation );

	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );

	return animation::mixing::expression( lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme );
}

weapon_lexeme_pair double_barreled_weapon_core_aimed_idle_state::get_weapon_lexeme_pair( mutable_buffer& buffer, const bool is_third_view, const weapon_user_state_enum user_state_id ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_weapon.ammo_in_magazine( ) < 3 ) );

	pcstr weapon_animation_captions[3] =
	{
		"aimed_both_barrels_empty",
		"aimed_one_barrel_loaded",
		"aimed_two_barrels_loaded"
	};

	u32 const animation_index = m_weapon.ammo_in_magazine( );
	pcstr animation_identifier = weapon_animation_captions[animation_index];

	resources::managed_resource_ptr const& selected_animation =
		m_weapon_animations[is_third_view != false][user_state_id == type_crouch][animation_index];

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

double_barreled_weapon_core_aimed_idle_state* weapon_core_state_cook_template<survarium::double_barreled_weapon_core_aimed_idle_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	const u32									animations_count
)
{
	return new ( buffer.c_ptr( ) ) double_barreled_weapon_core_aimed_idle_state( params->weapon, animations, animations_count );
}

} // namespace survarium
