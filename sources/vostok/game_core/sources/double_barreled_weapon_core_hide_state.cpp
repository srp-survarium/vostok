////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/double_barreled_weapon_core_hide_state.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/game_core/weapon_state_creation_params.h>
#include <vostok/game_core/weapon_core_state_cook_template.h>

#include <vostok/animation/cubic_spline_skeleton_animation.h>
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

// STATE[99.96%|DONE]: sole residual is `weapon` for the get_magazine_capacity( ) call staged into
// eax (target) vs ecx (base) - a call-boundary argument-passing register choice. Byte-identical
// otherwise. Same residual the matched double_barreled_weapon_core_show_state ctor accepts (99.96%).
// m_time_scale is initialized in the member-init list (target has no body statement for it);
// nested for loops are braced like the fire/reload siblings: each closing brace carries its
// loop backjump as its own statement (16/16 stmts, 0x1c8 both sides).
double_barreled_weapon_core_hide_state::double_barreled_weapon_core_hide_state(
	weapon_core&							weapon,
	float const								animation_timescale,
	resources::managed_resource_ptr const*	animations,
	u32 const								animations_count,
	bool&									is_shown
) : weapon_core_hide_state_base( weapon, is_shown ),
	m_time_scale( animation_timescale )
{
	ASSERT_CMP_U( weapon.get_magazine_capacity( ), ==, 2 );
	ASSERT_CMP_U( animations_count, ==, 16 );

	u32 animation_index = 0;
	for ( u32 view_index = 0 ; view_index != 2 ; ++view_index ) {
		for ( u32 user_state_index = 0 ; user_state_index != 2 ; ++user_state_index ) {
			for ( u32 weapon_state_index = 0 ; weapon_state_index != 3 ; ++weapon_state_index ) {
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

	// STRUCTURE DIFF: target 16 stmts / base 16 stmts (0x1c8 both) - no diverging rows
	// VERDICT: STRUCTURE MATCH - residual is the get_magazine_capacity `this` in eax (target) vs ecx (base), LTCG arg-register choice.
}

// STATE[83.52%|PARTIAL]: control structure (get_weapon_lexeme_pair, get_user_hands_expression, then
// the chained `+`) matches the target exactly. Residual is the shared-header operator+ wall: the target
// uses dedicated `expression operator+(expression&, animation_lexeme&)` / `operator+(expression&,
// expression&)` overloads (returning expression) absent from shared mixing_addition_lexeme_inline.h, so
// the base falls back to the generic addition_lexeme& operator+. Identical residual/score to
// double_barreled_weapon_core_show_state::weapon_and_hands_expression and the pistol hide sibling (83.52%).
// Out of this file's scope (adding the overloads would shift other matched operator+ functions).
animation::mixing::expression double_barreled_weapon_core_hide_state::weapon_and_hands_expression(
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
	// SIZE -0x27 | 80 | return hands_expression + lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme;
	// VERDICT: STRUCTURE MATCH (3/3) - operator+ overload-selection / inline-vs-call LTCG on the addition chain (target's expression-returning operator+ overloads absent from shared mixing headers); non-steerable from this TU.
}

// STATE[99.93%|DONE]: sole residual is the inlined `m_weapon.ammo_in_magazine( )` `this` read into
// eax (target, `mov eax, [ecx+128h]`) vs ecx (base) - a call-boundary/inline `this` register choice.
// Byte-identical otherwise (including the leading compiled-out ASSERT). Same residual the matched
// double_barreled_weapon_core_show_state::get_weapon_lexeme_pair accepts as DONE (99.93%).
weapon_lexeme_pair double_barreled_weapon_core_hide_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool const is_third_view, weapon_user_state_enum const user_state_id ) const
{
	ASSERT( UNKNOWN_EXPRESSION );

	pcstr weapon_animation_captions[3] =
	{
		"hide_both_barrels_empty",
		"hide_one_barrel_loaded",
		"hide_two_barrels_loaded"
	};

	u32 const weapon_animation_index = m_weapon.ammo_in_magazine( );
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
		7,
		m_time_scale,
		animation::mixing::play_once_and_freeze_at_end,
		animation::linear_interpolator( s_aim_transition_time )
	);

	// STRUCTURE DIFF: target 9 stmts / base 9 stmts (0xde both) - no diverging rows
	// VERDICT: STRUCTURE MATCH - residual is the inlined ammo_in_magazine `this` in eax (target) vs ecx (base), LTCG arg-register choice.
}

// STATE[72.12%|PARTIAL]: structure 7/7 - the captions are a multi-line brace-init (PDB records
// char const* const[2], so the old per-element assignments were ill-formed against the recorded
// type; the brace-init attributes each element store to its own line, same two 0x7 statements).
// Residual is the shared-header whole-program inline wall: the target keeps weapon_core::get_user()
// and the three animation_lexeme_parameters setters OUT-OF-LINE (real `call`s) while the base
// inlines them to direct member stores (setter statement 0x8f vs target 0x79), plus the early-return
// expression ctor staging. Identical residual class to the show sibling. Note: hide has NO
// linear_interpolator local (verified from asm).
animation::mixing::expression double_barreled_weapon_core_hide_state::get_user_hands_expression(
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

	// PDB records the array as char const* const[2] (so it cannot be element-assigned after the
	// declaration); the multi-line brace-init attributes each element store to its own line,
	// matching the target's two 0x7-byte statements.
	pcstr const animation_captions[2] =
	{
		"stand_hide",
		"crouch_hide"
	};

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

	// STRUCTURE DIFF: target 7 stmts / base 7 stmts
	// SIZE +0x3  | 148 | return weapon_lexeme;
	// SIZE +0x16 | 172 | );
	// VERDICT: STRUCTURE MATCH (7/7) - residuals are the early-return expression ctor staging and the animation_lexeme_parameters setters kept out-of-line in target vs inlined in base; whole-program LTCG, non-steerable from this TU.
}

// STATE[100%|DONE]
double_barreled_weapon_core_hide_state* weapon_core_state_cook_template<survarium::double_barreled_weapon_core_hide_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	u32									animations_count
)
{
	float weapon_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[0] )->length_in_frames( );
	float user_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[12] )->length_in_frames( );
	float time_scale = weapon_anim_length / user_anim_length;

	return new ( buffer.c_ptr( ) ) double_barreled_weapon_core_hide_state( params->weapon, time_scale, animations, animations_count, params->shown );
}

} // namespace survarium
