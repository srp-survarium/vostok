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

// STATE[83.52%|PARTIAL]: structure matches (get_weapon_lexeme_pair, get_user_hands_expression,
// then the chained `+`). Wall: the target uses dedicated expression-returning operator+ overloads
// (`expression operator+( expression&, animation_lexeme& )` etc.) that are not declared in the
// shared mixing_addition_lexeme_inline.h - only the generic `addition_lexeme& operator+(T1&,T2&)`
// exists, so base emits the generic chain + expression(addition_lexeme) where target inlines the
// expression overloads. Adding those overloads touches the shared animation header (out of this
// file's scope, would shift other operator+ matches). Same wall as the idle siblings.
animation::mixing::expression pistol_weapon_core_show_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool const							is_third_view,
	weapon_user_state_enum const		user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );

	animation::mixing::expression hands_expression = get_user_hands_expression( lexeme_pair.offset_lexeme, buffer, is_third_view, user_state_id, weight_driving_animation );

	return hands_expression + lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme;

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts
	// SIZE -0x27 | 75 | return hands_expression + lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme;
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the chained operator+ (target inlines expression-returning overloads absent from the shared mixing header), non-steerable.
}

// STATE[99.92%|DONE]: structure matches (captions split per-element to align the target's two
// L55/L56 stores). Sole byte residual: `m_weapon.ammo_in_magazine()` loads `this` into eax
// (target, LTCG callee takes it in eax) vs ecx (base) - the permitted call-boundary arg-passing
// class. trail: get_weapon_lexeme_pair.md
weapon_lexeme_pair pistol_weapon_core_show_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool const is_third_view, weapon_user_state_enum const user_state_id ) const
{
	pcstr weapon_animation_captions[2] =
	{
		"pistol-show",
		"pistol-show_empty"
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
		6,
		m_time_scale,
		animation::mixing::play_once_and_freeze_at_end,
		animation::instant_interpolator( )
	);

	// STRUCTURE DIFF: target 7 stmts / base 7 stmts - no diverging rows (0xc8 both)
	// VERDICT: STRUCTURE MATCH - sole byte diff is ammo_in_magazine eax-vs-ecx arg-passing, non-steerable.
}

// STATE[72.12%|PARTIAL]: control structure matches the target exactly (the type_sprint early-return
// of expression(weapon_lexeme); the user_animation_index; the animation_lexeme_parameters builder
// chain; expression(hands_only_lexeme)). Wall: whole-program inline decisions in shared
// animation/weapon_core headers - target keeps weapon_core::get_user() and the
// animation_lexeme_parameters setters (animated_object/bones_mask/playback_type) OUT-OF-LINE (proven:
// `bones_mask` is a standalone symbol in the target index, absent from base), while base inlines all
// of them (frame 0x110 -> 0x128, direct member stores). These are `inline` one-liners in shared
// headers; forcing them out-of-line is out of this file's scope. Non-steerable LTCG inline class.
animation::mixing::expression pistol_weapon_core_show_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool const							is_third_view,
	weapon_user_state_enum const		user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	if ( user_state_id == type_sprint )
		return weapon_lexeme;

	u32 const user_animation_index = user_state_id == type_crouch;

	pcstr const animation_captions[2] = { "stand_show", "crouch_show" };

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

	// STRUCTURE DIFF: target 6 stmts / base 6 stmts
	// SIZE +0x3  | 143 | return weapon_lexeme;
	// SIZE +0x16 | 160 | );  (the chained animation_lexeme_parameters builder declaration)
	// VERDICT: STRUCTURE MATCH (shape ok) - SIZE diffs are weapon_core::get_user / animation_lexeme_parameters setters kept out-of-line in target, inlined in base (whole-program LTCG inline of shared headers), non-steerable.
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
