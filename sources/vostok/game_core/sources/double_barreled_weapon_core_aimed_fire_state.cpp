// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/double_barreled_weapon_core_aimed_fire_state.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/game_core/weapon_state_creation_params.h>
#include <vostok/game_core/weapon_core_state_cook_template.h>

#include <vostok/animation/mixing_addition_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme_parameters.h>
#include <vostok/animation/cubic_spline_skeleton_animation.h>
#include <vostok/animation/linear_interpolator.h>
#include <vostok/resources_pinned_ptr.h>

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

// computed_shooting_animation_time_scale is defined (STUB) in the timescale inline header;
// new_object below is the only current caller, so pull in the definition here to resolve it.
float computed_shooting_animation_time_scale( resources::managed_resource_ptr const& shooting_animation, float rounds_per_second );
}
#include <vostok/game_core/weapon_animations_timescale_inline.h>
namespace survarium {

double_barreled_weapon_core_aimed_fire_state::double_barreled_weapon_core_aimed_fire_state(
	weapon_core&							weapon,
	float const								animation_time_scale,
	resources::managed_resource_ptr const*	animations,
	u32 const								animations_count
) : weapon_core_aimed_fire_state_base( weapon, animation_time_scale ),
	m_weapon_animation_index( u32( -1 ) )
{
	ASSERT_CMP_U( weapon.get_magazine_capacity( ), ==, 2 );
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

void double_barreled_weapon_core_aimed_fire_state::initialize( )
{
	weapon_core_aimed_fire_state_base::initialize( );

	ASSERT( UNKNOWN_EXPRESSION );

	m_weapon_animation_index = m_weapon.ammo_in_magazine( ) != 2;
}

animation::mixing::expression double_barreled_weapon_core_aimed_fire_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool const							is_third_view,
	weapon_user_state_enum const		user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	ASSERT( UNKNOWN_EXPRESSION );

	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );

	animation::mixing::expression hands_expression =
		get_user_hands_expression( lexeme_pair.offset_lexeme, buffer, is_third_view, user_state_id, weight_driving_animation );

	return hands_expression + lexeme_pair.main_lexeme + animation::mixing::expression( lexeme_pair.offset_lexeme );
}

weapon_lexeme_pair double_barreled_weapon_core_aimed_fire_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool const is_third_view, weapon_user_state_enum const user_state_id ) const
{
	pcstr weapon_animation_captions[2] =
	{
		"aimed_shot_first_barrel",
		"aimed_shot_second_barrel"
	};

	pcstr animation_identifier = weapon_animation_captions[m_weapon_animation_index];

	resources::managed_resource_ptr const& selected_animation =
		m_weapon_animations[is_third_view != false][user_state_id == type_crouch][m_weapon_animation_index];

	set_animation_to_wait( selected_animation );

	return get_weapon_lexeme_pair_impl(
		buffer,
		animation_identifier,
		selected_animation,
		&m_weapon,
		m_animation_playback_state,
		1,
		m_animation_timescale,
		animation::mixing::play_once_and_freeze_at_end,
		animation::linear_interpolator( s_aim_transition_time )
	);
}

// and - per claude@MATCH - the animation_lexeme_parameters ctor+setter chain folded into ONE statement
// (target L101=0x6e vs base 0x72) feeding the animation_lexeme override_lexeme construction, like the
// matching show/hide siblings). Residual is the unsteerable inline-vs-call LTCG class: the target keeps
// the lexeme_parameters setters OUT-OF-LINE (animated_object/playback_type are calls, additivity_priority
// inline) while our /GL build INLINES animated_object+playback_type; animation_type() is also COMDAT-folded
// onto a trivial getter. Both whole-program decisions, not source-steerable.
animation::mixing::expression double_barreled_weapon_core_aimed_fire_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool const							is_third_view,
	weapon_user_state_enum const		user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	if ( user_state_id == type_sprint )
		return animation::mixing::expression( weapon_lexeme );

	u32 const user_animation_index = ( user_state_id == type_crouch );

	resources::managed_resource_ptr const& selected_animation =
		m_user_animations[is_third_view != false][user_animation_index];

	if ( resources::pinned_ptr_const< animation::cubic_spline_skeleton_animation >( selected_animation )->animation_type( ) != animation::animation_type_additive )
		return animation::mixing::expression( weapon_lexeme );

	pcstr const user_animation_captions[2] = { "aimed_stand_shot", "aimed_crouch_shot" };

	animation::linear_interpolator interpolator( s_aim_transition_time );

	// claude@MATCH: chain ctor + all setters into ONE statement (target L101 carcass) feeding the
	// animation_lexeme construction - matches the show/hide siblings; a named params local + separate
	// setter statement split this into 2 statements (14 vs target's 12).
	animation::mixing::animation_lexeme override_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			user_animation_captions[user_animation_index],
			selected_animation,
			&weapon_lexeme,
			&weight_driving_animation
		)
		.animated_object( m_weapon.get_user( ) )
		.playback_type( animation::mixing::play_once_and_freeze_at_end )
		.additivity_priority( 1 )
	);

	return animation::mixing::expression( override_lexeme );
}

double_barreled_weapon_core_aimed_fire_state* weapon_core_state_cook_template<survarium::double_barreled_weapon_core_aimed_fire_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	u32 const							animations_count
)
{
	return new ( buffer.c_ptr( ) ) double_barreled_weapon_core_aimed_fire_state(
		params->weapon,
		computed_shooting_animation_time_scale( *animations, params->rounds_per_second ),
		animations,
		animations_count
	);
}

} // namespace survarium
