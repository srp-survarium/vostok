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

// STATE[99.97%|DONE]: structure byte-identical to target; residual is reloc/symbol-attribution noise.
double_barreled_weapon_core_reload_state::double_barreled_weapon_core_reload_state(
	weapon_core&							weapon,
	float									animation_timescale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
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

// STATE[83.52%|PARTIAL]: operator+ template-selection / inline-vs-call LTCG on the
// `hands + main + offset` chain (target keeps operator+<animation_lexeme> out-of-line;
// base inlines operator+<expression,animation_lexeme> + operator+<addition_lexeme,animation_lexeme>).
// Same wall as the fire_state sibling (83.18%). Not source-steerable.
animation::mixing::expression double_barreled_weapon_core_reload_state::weapon_and_hands_expression(
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
	// <0x7a93b0>|0x010|+0x01f:'46'	lexeme_pair = get_weapon_lexeme_pair(...)
	// <0>
	// <0x7a93cf>|0x02f|+0x02a:'48'	hands_expression = get_user_hands_expression(...)
	// <0>
	// <0x7a93f9>|0x059|+0x07a:'50'	return hands + main + offset
	// ******
}

// STATE[99.93%|DONE]: structure byte-identical to target; residual is reloc/symbol-attribution noise.
weapon_lexeme_pair double_barreled_weapon_core_reload_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
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

// STATE[73.66%|PARTIAL]: animation_lexeme_parameters setter-chain inline-vs-call LTCG -
// target keeps the parameter setters + weapon_core::get_user out-of-line; base inlines them.
// Same wall as the fire_state sibling get_user_hands_expression (73.17%). Not source-steerable.
animation::mixing::expression double_barreled_weapon_core_reload_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
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
		.weight_interpolator( interpolator )
		.bones_mask( 2 )
		.playback_type( animation::mixing::play_once_and_freeze_at_end )
	);

	return override_lexeme;

	// FUNCTION BODY
	// <0x7a9182>|0x012|+0x012:'73'	ASSERT
	// <0x7a9194>|0x024|+0x012:'74'	ASSERT
	// <0>
	// <1>
	// <0x7a91a6>|0x036|+0x00e:'77'	captions row 0
	// <0x7a91b4>|0x044|+0x00e:'78'	captions row 1
	// <0>
	// <1>
	// <0x7a91c2>|0x052|+0x00c:'81'	user_state_index = (user_state_id == type_crouch)
	// <0x7a91ce>|0x05e|+0x020:'82'	weapon_state_index = (ammo_in_magazine() != 1)
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
	// <12>	interpolator / get_user / params setter-chain (target keeps setters/get_user/~params OUT-OF-LINE; base inlines)
	// <0x7a91ee>|0x07e|+0x0a3:'96'	override_lexeme(...)
	// <0x7a9291>|0x121|+0x01c:'97'	return override_lexeme;
	// ******
}

// STATE[86.5%|PARTIAL]: base folds the STUB computed_reload_animation_time_scale (constant)
// into `fldz` at the call site; target keeps it out-of-line (LTCG returns float in xmm0,
// `movss [esp], xmm0`, extra spill slots). Call-boundary arg/inline-vs-call; not source-steerable
// until computed_reload_animation_time_scale is matched. Same wall as fire_state sibling new_object.
double_barreled_weapon_core_reload_state* weapon_core_state_cook_template<survarium::double_barreled_weapon_core_reload_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	u32									animations_count
)
{
	return new ( buffer.c_ptr( ) ) double_barreled_weapon_core_reload_state(
		params->weapon,
		computed_reload_animation_time_scale( animations[0], params->reload_time ),
		animations,
		animations_count
	);

	// FUNCTION BODY
	// <0x7a9109>|0x009|+0x05c:'109'	return new ( buffer.c_ptr( ) ) double_barreled_weapon_core_reload_state( params->weapon, computed_reload_animation_time_scale( animations[0], params->reload_time ), animations, animations_count );
	// ******
}

} // namespace survarium
