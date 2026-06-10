////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/double_barreled_weapon_core_show_state.h>

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

// STATE[99.96%|DONE]: sole residual is `weapon` for the get_magazine_capacity( ) call staged into
// eax (target) vs ecx (base) - a call-boundary argument-passing register choice. Byte-identical
// otherwise; same class as pistol_weapon_core_show_state::get_weapon_lexeme_pair.
double_barreled_weapon_core_show_state::double_barreled_weapon_core_show_state(
	weapon_core&							weapon,
	float									animation_timescale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count,
	bool&									is_shown
) : weapon_core_show_state_base( weapon, is_shown )
{
	m_time_scale = animation_timescale;

	ASSERT_CMP_U( weapon.get_magazine_capacity( ), ==, 2 );
	ASSERT_CMP_U( animations_count, ==, 16 );

	u32 animation_index = 0;
	for ( u32 view_index = 0 ; view_index != 2 ; ++view_index )
		for ( u32 user_state_index = 0 ; user_state_index != 2 ; ++user_state_index )
			for ( u32 weapon_state_index = 0 ; weapon_state_index != 3 ; ++weapon_state_index )
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
// expression(addition_lexeme) instead of the target's expression-returning overloads. Recovering this
// needs adding those overloads to the shared animation header (out of this file's scope; would also
// shift other operator+ matched functions). Identical wall to pistol_weapon_core_show_state::weapon_and_hands_expression.
animation::mixing::expression double_barreled_weapon_core_show_state::weapon_and_hands_expression(
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
	// <0x7ade40>|0x010|+0x01f:'47'
	// <0x7ade5f>|0x02f|+0x02a:'48'
	// <0x7ade89>|0x059|+0x07a:'49'
	// ******
}

// STATE[99.93%|DONE]: sole residual is the inlined `m_weapon.ammo_in_magazine( )` result landing in
// eax (target) vs ecx (base) - a call-boundary/inline result register choice. Byte-identical otherwise;
// same residual the reference pistol_weapon_core_show_state::get_weapon_lexeme_pair accepts.
weapon_lexeme_pair double_barreled_weapon_core_show_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	ASSERT( UNKNOWN_EXPRESSION );

	pcstr weapon_animation_captions[3] = { "show_both_barrels_empty", "show_one_barrel_loaded", "show_two_barrels_loaded" };

	u32 weapon_animation_index = m_weapon.ammo_in_magazine( );
	pcstr animation_identifier = weapon_animation_captions[weapon_animation_index];

	resources::managed_resource_ptr const& selected_animation =
		m_weapon_animations[is_third_view != false][user_state_id == type_crouch][weapon_animation_index];

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

// STATE[63.63%|PARTIAL]: control structure matches the target exactly (the type_sprint early-return
// of expression(weapon_lexeme); the user_state_index; the animation_lexeme_parameters builder chain
// animated_object/bones_mask/playback_type; the expression(override_lexeme) return). Residual is purely
// whole-program inline decisions in shared animation headers: the target keeps weapon_core::get_user()
// and the three animation_lexeme_parameters setters OUT-OF-LINE (real `call`s; frame 0x110), while the
// base inlines all of them (frame 0x128, direct member stores at [eax+20h]/[edx+4Ch]/[eax+3Ch]). Also
// the target's early-return constructs the result expression via a ctor staged in eax (no source arg)
// vs the base's expression::expression<animation_lexeme>(edi,eax). Both are out-of-this-file inline /
// overload-resolution decisions in shared mixing headers; forcing them would shift other matched
// functions. Identical wall to pistol_weapon_core_show_state::get_user_hands_expression (#72%). LTCG inline class.
animation::mixing::expression double_barreled_weapon_core_show_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	if ( user_state_id == type_sprint )
		return weapon_lexeme;

	u32 user_state_index = user_state_id == type_crouch;

	pcstr animation_captions[2] = { "stand_show", "crouch_show" };

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

	// FUNCTION BODY
	// <0x7adc81>|0x011|+0x006:'73'
	// <0x7adc87>|0x017|+0x010:'74'
	// <0>
	// <1>
	// <0x7adc97>|0x027|+0x00e:'77'
	// <0x7adca5>|0x035|+0x00c:'78'
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
	// <0x7adcb1>|0x041|+0x079:'90'
	// <0x7add2a>|0x0ba|+0x01c:'91'
	// ******
}

// STATE[100%|DONE]
double_barreled_weapon_core_show_state* weapon_core_state_cook_template<survarium::double_barreled_weapon_core_show_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	u32									animations_count
)
{
	float weapon_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[0] )->length_in_frames( );
	float user_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[12] )->length_in_frames( );
	float time_scale = weapon_anim_length / user_anim_length;

	return new ( buffer.c_ptr( ) ) double_barreled_weapon_core_show_state( params->weapon, time_scale, animations, animations_count, params->shown );
}

} // namespace survarium
