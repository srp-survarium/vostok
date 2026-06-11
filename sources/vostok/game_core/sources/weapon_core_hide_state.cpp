////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_hide_state.h>

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

// STATE[100%|DONE]
weapon_core_hide_state::weapon_core_hide_state(
	weapon_core&							weapon,
	float									animation_timescale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count,
	bool&									is_shown
) : weapon_core_hide_state_base( weapon, is_shown )
{
	m_time_scale = animation_timescale;

	m_body_part_mask_for_user = animation::body_part_whole_body_but_hands;

	ASSERT_CMP_U( animations_count, ==, 8 );

	u32 animation_index = 0;
	for ( u32 view_index = 0 ; view_index != 2 ; ++view_index )
		for ( u32 user_state_index = 0 ; user_state_index != 2 ; ++user_state_index )
			m_weapon_animations[view_index][user_state_index] = animations[animation_index++];

	for ( u32 view_index = 0 ; view_index != 2 ; ++view_index )
		for ( u32 user_state_index = 0 ; user_state_index != 2 ; ++user_state_index )
			m_user_animations[view_index][user_state_index] = animations[animation_index++];

	ASSERT( UNKNOWN_EXPRESSION );
}

// STATE[83.52%|PARTIAL]: control structure (get_weapon_lexeme_pair, get_user_hands_expression, then
// the chained `+`) matches the target exactly. Residual is the shared-header operator+ wall: the target
// uses dedicated `expression operator+(expression&, animation_lexeme&)` / `operator+(expression&,
// expression&)` overloads (returning expression) that are NOT present in shared
// mixing_addition_lexeme_inline.h, so the base falls back to the generic addition_lexeme& operator+.
// Identical residual/score to pistol_weapon_core_show_state::weapon_and_hands_expression (83.52%) and
// weapon_core_idle_state::weapon_and_hands_expression. Fixing it (adding the overloads) is out of this
// file's scope and would shift other matched operator+ functions (#192).
animation::mixing::expression weapon_core_hide_state::weapon_and_hands_expression(
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
	// SIZE -0x27 | 79 | return hands_expression + lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme;
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the chained operator+ (target inlines
	// expression-returning overloads absent from the shared mixing header), non-steerable.
}

// STATE[100%|DONE]
weapon_lexeme_pair weapon_core_hide_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	pcstr animation_identifier = "weapon-hide";

	resources::managed_resource_ptr const& selected_animation =
		m_weapon_animations[is_third_view != false][user_state_id == type_crouch];

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
}

// STATE[73.52%|PARTIAL]: control structure matches the target exactly - the type_sprint early-return of
// expression(weapon_lexeme), the trailing ASSERT, the user_animation_index, the animation_lexeme_parameters
// builder chain (animated_object/bones_mask/playback_type) and the final expression(hands_only_lexeme).
// Residual is the shared-header whole-program inline wall: the target keeps weapon_core::get_user() and the
// animation_lexeme_parameters setters OUT-OF-LINE while the base inlines them (same residual class as
// pistol_weapon_core_show_state::get_user_hands_expression). Forcing them out-of-line is out of this file's scope.
animation::mixing::expression weapon_core_hide_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool const							is_third_view,
	weapon_user_state_enum const		user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	if ( user_state_id == type_sprint )
		return weapon_lexeme;

	ASSERT( UNKNOWN_EXPRESSION );

	u32 const user_state_index = user_state_id == type_crouch;

	pcstr const animation_captions[2] = { "stand_hide", "crouch_hide" };

	animation::mixing::animation_lexeme hands_only_lexeme(
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

	return hands_only_lexeme;

	// STRUCTURE DIFF: target 7 stmts / base 7 stmts
	// SIZE +0x3 | 126 | return weapon_lexeme;
	// SIZE +0x6 | 132 | pcstr const animation_captions[2] = { "stand_hide", "crouch_hide" };
	// SIZE +0x16 | 145 | );
	// VERDICT: STRUCTURE MATCH (shape ok) - +0x3 is the promoted expression(animation_lexeme)
	// ctor convention; +0x6 on the captions is disp8-vs-disp32 slot noise cascading from the
	// bigger base frame; +0x16 is the lexeme_parameters setters/dtor inlined in base vs
	// out-of-line in target, all non-steerable LTCG.
}

// STATE[100%|DONE]
weapon_core_hide_state* weapon_core_state_cook_template<survarium::weapon_core_hide_state>::new_object(
	mutable_buffer							buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	u32									animations_count
)
{
	float weapon_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[0] )->length_in_frames( );
	float user_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[4] )->length_in_frames( );
	float time_scale = weapon_anim_length / user_anim_length;

	return new ( buffer.c_ptr( ) ) weapon_core_hide_state( params->weapon, time_scale, animations, animations_count, params->shown );
}

} // namespace survarium
