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

// STATE[99.96%|DONE]: every instruction matches. Sole residual is the
// `weapon.get_magazine_capacity()` __thiscall `this` loaded into eax (target) vs ecx (base) -
// an argument-passing register choice (LTCG), same class as the double_barreled fire_state sibling.
// Nested for loops are braced like the fire/reload siblings: each closing brace carries its
// loop backjump as its own 0x2 statement (16/16 stmts, 0x1c2 both sides).
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

	// STRUCTURE DIFF: target 16 stmts / base 16 stmts (0x1c2 both) - no diverging rows
	// VERDICT: STRUCTURE MATCH - residual is the get_magazine_capacity `this` in eax (target) vs ecx (base), LTCG arg-register choice.
}

// STATE[99.76%|DONE]: every instruction/branch matches. Sole residual is the
// `m_weapon.ammo_in_magazine()` __thiscall `this` loaded into eax (target) vs ecx (base) -
// an argument-passing register choice (LTCG). Same shape as the double_barreled fire_state sibling.
void double_barreled_weapon_core_aimed_fire_state::initialize( )
{
	weapon_core_aimed_fire_state_base::initialize( );

	ASSERT( UNKNOWN_EXPRESSION );

	m_weapon_animation_index = m_weapon.ammo_in_magazine( ) != 2;

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts (0x43 both) - no diverging rows
	// VERDICT: STRUCTURE MATCH - residual is the ammo_in_magazine `this` in eax (target) vs ecx (base), LTCG arg-register choice.
}

// STATE[83.90%|PARTIAL]: every statement (the leading ASSERT, get_weapon_lexeme_pair,
// get_user_hands_expression with weight_driving_animation, return), branch and lexeme operand
// match. Sole residual is the operator+<...> template-selection / inline-vs-call LTCG on the
// `main + expression(offset) + hands` addition tree: the target keeps operator+<animation_lexeme>
// OUT-OF-LINE while our /GL build inlines addition_lexeme, shifting the [ebp-XX] layout. IDENTICAL
// wall to double_barreled_weapon_core_fire_state::weapon_and_hands_expression (#203, 83.18%) -
// whole-program decision, not source-steerable in this unit. See .md.
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

	return lexeme_pair.main_lexeme + animation::mixing::expression( lexeme_pair.offset_lexeme ) + hands_expression;

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// SIZE +0x29 | 114 | return lexeme_pair.main_lexeme + animation::mixing::expression( lexeme_pair.offset_lexeme ) + hands_expression;
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the operator+ inline-vs-call LTCG on the addition tree, non-steerable.
}

// STATE[100%|DONE]
weapon_lexeme_pair double_barreled_weapon_core_aimed_fire_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool const is_third_view, weapon_user_state_enum const user_state_id ) const
{
	pcstr weapon_animation_captions[2] = { "aimed_shot_first_barrel", "aimed_shot_second_barrel" };

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

// STATE[79.39%|PARTIAL]: structure now matches the target exactly - 10 statements both sides (sprint
// early-return, the animation_type()!=additive early-return, the captions array, the interpolator local,
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

	// STRUCTURE DIFF: target 10 stmts / base 10 stmts
	// SIZE +0x3  | 154 | return animation::mixing::expression( weapon_lexeme );
	// SIZE -0x13 | 161 | if ( resources::pinned_ptr_const< animation::cubic_spline_skeleton_animation >( selected_animation )->animation_type( ) != animation::animation_type_additive )
	// SIZE +0x3  | 162 | return animation::mixing::expression( weapon_lexeme );
	// SIZE +0x4  | 182 | );
	// VERDICT: STRUCTURE MATCH (shape ok) - residuals are per-site LTCG inline-vs-call: target inlines animation_type() (reads [ptr+18h] via temps) where base keeps the call; expression(lexeme) ctor arg-register shape; setter-chain inline depth.
}

// STATE[86.50%|PARTIAL]: control flow + placement-new + ctor call all match. Sole residual is the
// computed_shooting_animation_time_scale callee: it is a STUB returning a constant, so our /Od build
// FOLDS it to `fldz` at the call site (no call), while the target keeps it OUT-OF-LINE and the LTCG
// whole-program optimizer returns the float in xmm0 (movss [esp], xmm0). Argument passing /
// inline-vs-call at the call boundary - not source-steerable until that callee is matched. SAME wall
// as all the sibling new_objects (#203 86.5%, #182 92.08%, #199 86.5%).
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

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// SIZE -0x11 | 223 | );
	// VERDICT: STRUCTURE MATCH (1/1) - base folds the STUB computed_shooting_animation_time_scale to fldz at the call site while target keeps the call (xmm0 return); closes itself once that callee is matched.
}

} // namespace survarium
