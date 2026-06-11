////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_fire_state.h>

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

// new_object is the only caller of computed_shooting_animation_time_scale; pull in
// the timescale inline definition (as the pistol sibling does) to resolve it.
float computed_shooting_animation_time_scale( resources::managed_resource_ptr const& shooting_animation, float rounds_per_second );
}
#include <vostok/game_core/weapon_animations_timescale_inline.h>
namespace survarium {

// STATE[100%|DONE]
weapon_core_fire_state::weapon_core_fire_state(
	weapon_core&							weapon,
	float									animation_timescale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
) : weapon_core_fire_state_base( weapon, animation_timescale )
{
	ASSERT_CMP_U( animations_count, ==, 8 );

	u32 animation_index = 0;
	for ( u32 view = 0 ; view != 2 ; ++view )
		for ( u32 user_state = 0 ; user_state != 2 ; ++user_state )
			m_weapon_animations[view][user_state] = animations[animation_index++];

	for ( u32 view = 0 ; view != 2 ; ++view )
		for ( u32 user_state = 0 ; user_state != 2 ; ++user_state )
			m_user_animations[view][user_state] = animations[animation_index++];

	ASSERT( UNKNOWN_EXPRESSION );
}

// STATE[82.52%|PARTIAL]: every statement, branch, lexeme operand and the addition-tree shape
// match. Sole residual is the operator+<...> divergence on the first `main + expression(offset)`:
// the target selects operator+<animation_lexeme> and keeps it OUT-OF-LINE, while our /GL build
// builds addition_lexeme<animation_lexeme,expression> INLINE (addition_lexeme ctor + cloned_in_buffer
// at this site), shifting the [ebp-XX] layout. This is the SAME operator+ template-selection /
// inline residual under active investigation on PR #192 (target operator+<animation_lexeme> vs base
// operator+<expression,animation_lexeme>) and on weapon_core_idle_state (85.65%) - if #192 lands a
// source fix it replicates here; do not re-derive it in this unit. weight_driving_animation is
// unreferenced in the target too (see .md).
animation::mixing::expression weapon_core_fire_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );

	animation::mixing::expression hands_expression =
		get_user_hands_expression( lexeme_pair.offset_lexeme, buffer, is_third_view, user_state_id );

	return lexeme_pair.main_lexeme + animation::mixing::expression( lexeme_pair.offset_lexeme ) + hands_expression;

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts
	// SIZE +0x29 | 84 | return lexeme_pair.main_lexeme + animation::mixing::expression( lexeme_pair.offset_lexeme ) + hands_expression;
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the mixing operator+ template-selection /
	// inline wall (target operator+<animation_lexeme> out-of-line vs base addition_lexeme inlined),
	// blocked on the mixing unit (PR #192), non-steerable here.
}

// STATE[100%|DONE]
weapon_lexeme_pair weapon_core_fire_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	pcstr animation_identifier = "weapon-fire";

	resources::managed_resource_ptr const& selected_animation =
		m_weapon_animations[is_third_view != false][user_state_id == type_crouch];

	set_animation_to_wait( selected_animation );

	return get_weapon_lexeme_pair_impl(
		buffer,
		animation_identifier,
		selected_animation,
		&m_weapon,
		m_animation_playback_state,
		1,
		m_animation_timescale,
		m_weapon.get_bullets_in_queue( ) <= 1 ? animation::mixing::play_once_and_freeze_at_end : animation::mixing::play_cyclically,
		animation::linear_interpolator( s_aim_transition_time )
	);
}

// STATE[77.00%|PARTIAL]: lexeme_parameters setters / animation_type() inline-vs-call LTCG,
// non-steerable; chained-temporary params merge landed (9/9, 73.51 -> 77.00).
animation::mixing::expression weapon_core_fire_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id
) const
{
	if ( user_state_id == type_sprint )
		return animation::mixing::expression( weapon_lexeme );

	u32 user_animation_index = ( user_state_id == type_crouch );

	resources::managed_resource_ptr const& selected_animation =
		m_user_animations[is_third_view != false][user_animation_index];

	if ( resources::pinned_ptr_const< animation::cubic_spline_skeleton_animation >( selected_animation )->animation_type( ) != animation::animation_type_additive )
		return animation::mixing::expression( weapon_lexeme );

	pcstr user_animation_captions[2] = { "stand_shoot", "crouch_shoot" };

	animation::mixing::animation_lexeme hands_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			user_animation_captions[user_animation_index],
			selected_animation,
			&weapon_lexeme,
			NULL
		)
		.animated_object		( m_weapon.get_user( ) )
		.playback_type			( animation::mixing::play_once_and_freeze_at_end )
		.additivity_priority	( 1 )
	);

	return animation::mixing::expression( hands_lexeme );

	// STRUCTURE DIFF: target 9 stmts / base 9 stmts (was 9/11 before the chained-temporary merge)
	// SIZE +0x3  | 133 | return animation::mixing::expression( weapon_lexeme );
	// SIZE -0x13 | 140 | if ( pinned_ptr_const<...>( selected_animation )->animation_type( ) != ... )
	// SIZE +0x3  | 141 | return animation::mixing::expression( weapon_lexeme );
	// SIZE +0x4  | 156 | ); (the chained params temporary declaration of hands_lexeme)
	// VERDICT: STRUCTURE MATCH (9/9) - residuals are per-site LTCG: target inlines animation_type()
	// where base keeps the COMDAT call; setter-chain/expression-ctor call boundaries differ by
	// promoted-convention bytes. Non-steerable from this TU.
}

// STATE[86.5%|PARTIAL]: control flow + placement-new + ctor call all match. Sole residual is the
// LTCG-specialized calling convention of computed_shooting_animation_time_scale: the target's
// whole-program optimizer passes its `managed_resource_ptr const&` arg in a register and returns
// the float in xmm0 (movss), while our STUB callee keeps default cdecl (stack push + st0/fstp).
// Argument passing at the call boundary - identical wall to pistol_weapon_core_fire_state::new_object
// (92.08%); not source-steerable until that callee is matched. See .md.
weapon_core_fire_state* weapon_core_state_cook_template<survarium::weapon_core_fire_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	u32									animations_count
)
{
	return new ( buffer.c_ptr( ) ) weapon_core_fire_state(
		params->weapon,
		computed_shooting_animation_time_scale( *animations, params->rounds_per_second ),
		animations,
		animations_count
	);

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// SIZE -0x11 | 211 | );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is computed_shooting_animation_time_scale's
	// LTCG-promoted convention (register arg, xmm0 return) vs our cdecl stub, blocked on that callee.
}

} // namespace survarium
