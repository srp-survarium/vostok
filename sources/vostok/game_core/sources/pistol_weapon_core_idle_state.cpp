// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/pistol_weapon_core_idle_state.h>

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

pistol_weapon_core_idle_state::pistol_weapon_core_idle_state( weapon_core& weapon, resources::managed_resource_ptr const* animations, const u32 animations_count ) :
	weapon_core_idle_state_base( weapon )
{
	ASSERT_CMP_U( animations_count, ==, 8 );

	u32 animation_index = 0;
	for ( u32 view_index = 0 ; view_index != 2 ; ++view_index )
	{
		for ( u32 user_state_index = 0 ; user_state_index != 2 ; ++user_state_index )
		{
			for ( u32 weapon_state_index = 0 ; weapon_state_index != 2 ; ++weapon_state_index )
			{
				m_weapon_animations[view_index][user_state_index][weapon_state_index] = animations[animation_index++];
			}
		}
	}

	ASSERT( UNKNOWN_EXPRESSION );
}

// claude@NOTE: systemic ceiling shared by every weapon_*_state weapon_and_hands_expression
// override. Structure is a 3-statement MATCH; only the return statement diverges - the target
// inlines mixing::operator+ where our build emits `call operator+<...>`. Inline-vs-call codegen;
// not source-steerable. Left at the faithful `+` form.
animation::mixing::expression pistol_weapon_core_idle_state::weapon_and_hands_expression(
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

// claude@NOTE: STRUCTURE MATCH and byte-size identical; residual is a single /Od register-coloring
// nibble at the out-of-line m_weapon.ammo_in_magazine() thiscall (target loads the weapon ptr into
// eax, our build into ecx). Same quirk caps pistol_weapon_core_reload_state::get_weapon_lexeme_pair;
// not source-steerable (the plain idle/aimed getters lack the call and are 100%).
weapon_lexeme_pair pistol_weapon_core_idle_state::get_weapon_lexeme_pair( mutable_buffer& buffer, const bool is_third_view, const weapon_user_state_enum user_state_id ) const
{
	pcstr weapon_animation_captions[2] =
	{
		"pistol-idle",
		"pistol-idle_empty"
	};

	u32 const animation_index = m_weapon.ammo_in_magazine( ) == 0;
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

pistol_weapon_core_idle_state* weapon_core_state_cook_template<survarium::pistol_weapon_core_idle_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	const u32									animations_count
)
{
	return new ( buffer.c_ptr( ) ) pistol_weapon_core_idle_state( params->weapon, animations, animations_count );
}

} // namespace survarium
