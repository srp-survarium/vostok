////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/pistol_weapon_core_show_state.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/game_core/weapon_state_creation_params.h>
#include <vostok/game_core/weapon_core_state_cook_template.h>

#include <vostok/animation/cubic_spline_skeleton_animation.h>
#include <vostok/animation/instant_interpolator.h>
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

// STATE[100%|DONE]
pistol_weapon_core_show_state::pistol_weapon_core_show_state(
	weapon_core&							weapon,
	float									animation_timescale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count,
	bool&									is_shown
) : weapon_core_show_state_base( weapon, is_shown )
{
	m_time_scale = animation_timescale;

	ASSERT_CMP_U( animations_count, ==, 12 );

	u32 animation_index = 0;
	for ( u32 view_index = 0 ; view_index != 2 ; ++view_index )
		for ( u32 user_state_index = 0 ; user_state_index != 2 ; ++user_state_index )
			for ( u32 weapon_state_index = 0 ; weapon_state_index != 2 ; ++weapon_state_index )
				m_weapon_animations[view_index][user_state_index][weapon_state_index] = animations[animation_index++];

	for ( u32 view_index = 0 ; view_index != 2 ; ++view_index )
		for ( u32 user_state_index = 0 ; user_state_index != 2 ; ++user_state_index )
			m_user_animations[view_index][user_state_index] = animations[animation_index++];

	ASSERT( UNKNOWN_EXPRESSION );
}

// STATE[83.52%|PARTIAL]: structure (get_weapon_lexeme_pair, get_user_hands_expression, then the
// chained `+`) matches the target. Residual: the target uses dedicated `expression operator+(
// expression&, animation_lexeme&)` / `operator+(expression&, expression&)` overloads (returning
// expression) that DO NOT EXIST in the current shared mixing_addition_lexeme_inline.h - only the
// generic `addition_lexeme& operator+(T1&,T2&)` is declared, so the base build emits
// `operator+<expression,animation_lexeme>` + `operator+<addition_lexeme,animation_lexeme>` +
// expression(addition_lexeme) instead of the target's expression-returning overloads. Recovering
// this needs adding those overloads to the shared animation header (out of this file's scope; would
// also shift other operator+ matched functions). Same class of residual as
// weapon_core_idle_state::weapon_and_hands_expression (#151, 85% PARTIAL).
animation::mixing::expression pistol_weapon_core_show_state::weapon_and_hands_expression(
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
	// <0x7ae870>|0x010|+0x01f:'47'
	// <0x7ae88f>|0x02f|+0x02a:'48'
	// <0x7ae8b9>|0x059|+0x07a:'49'
	// ******
}

// STATE[99.92%|DONE]: sole residual is the `m_weapon.ammo_in_magazine()` __thiscall `this` loaded
// into eax (target) vs ecx (base) - a call-boundary argument-passing register choice. Byte-identical
// otherwise; same residual the reference pistol_weapon_core_idle_state::get_weapon_lexeme_pair accepts.
weapon_lexeme_pair pistol_weapon_core_show_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	pcstr weapon_animation_captions[2] = { "pistol-show", "pistol-show_empty" };

	u32 animation_index = m_weapon.ammo_in_magazine( ) == 0;
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
		6,
		m_time_scale,
		animation::mixing::play_once_and_freeze_at_end,
		animation::instant_interpolator( )
	);
}

// STATE[72.12%|PARTIAL]: control structure matches the target exactly (the type_sprint early-return
// of expression(weapon_lexeme); the user_animation_index; the animation_lexeme_parameters builder
// chain; expression(hands_only_lexeme)). Residual is purely whole-program inline decisions in shared
// animation/weapon_core headers: the target keeps weapon_core::get_user() and the
// animation_lexeme_parameters setters (animated_object/bones_mask/playback_type) OUT-OF-LINE (proven:
// `bones_mask` is a standalone symbol in the target index but absent from the base index), while the
// base inlines all of them (frame 0x110 -> 0x128, direct member stores). These are `inline`-declared
// one-liners in shared headers; forcing them out-of-line is out of this file's scope. LTCG inline class.
animation::mixing::expression pistol_weapon_core_show_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum					user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	if ( user_state_id == type_sprint )
		return weapon_lexeme;

	u32 user_animation_index = user_state_id == type_crouch;

	pcstr animation_captions[2] = { "stand_show", "crouch_show" };

	animation::mixing::animation_lexeme hands_only_lexeme(
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

	return hands_only_lexeme;

	// FUNCTION BODY
	// <0x7ae6b1>|0x011|+0x006:'69'
	// <0x7ae6b7>|0x017|+0x010:'70'
	// <0>
	// <0x7ae6c7>|0x027|+0x00c:'72'
	// <0x7ae6d3>|0x033|+0x014:'73'
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
	// <0x7ae6e7>|0x047|+0x079:'86'
	// <0x7ae760>|0x0c0|+0x01c:'87'
	// ******
}

// STATE[100%|DONE]
pistol_weapon_core_show_state* weapon_core_state_cook_template<pistol_weapon_core_show_state>::new_object(
	mutable_buffer							buffer,
	weapon_state_creation_params const*		params,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
)
{
	float weapon_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[0] )->length_in_frames( );
	float user_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[8] )->length_in_frames( );
	float time_scale = weapon_anim_length / user_anim_length;

	return new ( buffer.c_ptr( ) ) pistol_weapon_core_show_state( params->weapon, time_scale, animations, animations_count, params->shown );
}

} // namespace survarium
