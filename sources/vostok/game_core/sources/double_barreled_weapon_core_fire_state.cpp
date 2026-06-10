////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/double_barreled_weapon_core_fire_state.h>

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
// an argument-passing register choice (LTCG), same class as the reference idle getters.
// Nested for loops are braced like the reload sibling: each closing brace carries its
// loop backjump as its own 0x2 statement (16/16 stmts, 0x1c2 both sides).
double_barreled_weapon_core_fire_state::double_barreled_weapon_core_fire_state(
	weapon_core&							weapon,
	float									animation_time_scale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
) : weapon_core_fire_state_base( weapon, animation_time_scale ),
	m_weapon_animation_index( u32( -1 ) )
{
	ASSERT_CMP_U( weapon.get_magazine_capacity( ), ==, 2 );
	ASSERT_CMP_U( animations_count, ==, 12 );

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
			m_user_animations[view][user_state] = animations[animation_index++];
		}
	}

	ASSERT( UNKNOWN_EXPRESSION );
}

// STATE[99.76%|DONE]: every instruction/branch matches. Sole residual is the
// `m_weapon.ammo_in_magazine()` __thiscall `this` loaded into eax (target) vs ecx (base) -
// an argument-passing register choice (LTCG). m_weapon_animation_index is the bool
// (ammo_in_magazine() != 2) stored directly; matches the target's setne/movzx store.
void double_barreled_weapon_core_fire_state::initialize( )
{
	weapon_core_fire_state_base::initialize( );

	ASSERT( UNKNOWN_EXPRESSION );

	m_weapon_animation_index = m_weapon.ammo_in_magazine( ) != 2;

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts (0x43 both) - no diverging rows
	// VERDICT: STRUCTURE MATCH - residual is the ammo_in_magazine `this` in eax (target) vs ecx (base), LTCG arg-register choice.
}

// STATE[83.18%|PARTIAL]: every statement, branch, lexeme operand and the addition-tree shape
// match. Sole residual is the operator+<...> template-selection / inline-vs-call LTCG on the
// `main + expression(offset)` sub-expression: the target selects operator+<animation_lexeme>
// and keeps it OUT-OF-LINE, while our /GL build builds operator+<addition_lexeme,expression>
// INLINE, shifting the [ebp-XX] layout. IDENTICAL shape/diff to the reference
// weapon_core_fire_state::weapon_and_hands_expression (#193, 82.52%) - whole-program decision,
// not source-steerable in this unit. See .md.
animation::mixing::expression double_barreled_weapon_core_fire_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );

	animation::mixing::expression hands_expression =
		get_user_hands_expression( lexeme_pair.offset_lexeme, buffer, is_third_view, user_state_id, weight_driving_animation );

	return lexeme_pair.main_lexeme + animation::mixing::expression( lexeme_pair.offset_lexeme ) + hands_expression;

	// STRUCTURE DIFF: target 3 stmts / base 3 stmts
	// SIZE +0x29 | 100 | return lexeme_pair.main_lexeme + animation::mixing::expression( lexeme_pair.offset_lexeme ) + hands_expression;
	// VERDICT: STRUCTURE MATCH (3/3) - operator+ template-selection / inline-vs-call LTCG on the addition chain (target keeps operator+<animation_lexeme> out-of-line, base inlines); whole-program, non-steerable from this TU.
}

// STATE[100%|DONE]
weapon_lexeme_pair double_barreled_weapon_core_fire_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	pcstr weapon_animation_captions[2] = { "shot_first_barrel", "shot_second_barrel" };

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

// STATE[77.13%|PARTIAL]: animation_lexeme_parameters setters / animation_type() inline-vs-call LTCG.
animation::mixing::expression double_barreled_weapon_core_fire_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	if ( user_state_id == type_sprint )
		return animation::mixing::expression( weapon_lexeme );

	u32 user_animation_index = ( user_state_id == type_crouch );

	resources::managed_resource_ptr const& selected_animation =
		m_user_animations[is_third_view != false][user_animation_index];

	if ( resources::pinned_ptr_const< animation::cubic_spline_skeleton_animation >( selected_animation )->animation_type( ) != animation::animation_type_additive )
		return animation::mixing::expression( weapon_lexeme );

	pcstr user_animation_captions[2] = { "stand_shot_double_barrel", "crouch_shot_double_barrel" };

	animation::mixing::animation_lexeme override_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			user_animation_captions[user_animation_index],
			selected_animation,
			&weapon_lexeme,
			&weight_driving_animation
		)
		.animated_object		( m_weapon.get_user( ) )
		.playback_type			( animation::mixing::play_once_and_freeze_at_end )
		.additivity_priority	( 1 )
	);

	return animation::mixing::expression( override_lexeme );

	// STRUCTURE DIFF: target 9 stmts / base 9 stmts (was 9/11 before the chained-temporary merge, 73.17 -> 77.13)
	// SIZE +0x3  | 151 | return animation::mixing::expression( weapon_lexeme );
	// SIZE -0x13 | 158 | if ( pinned_ptr_const<...>( selected_animation )->animation_type( ) != ... )
	// SIZE +0x3  | 159 | return animation::mixing::expression( weapon_lexeme );
	// SIZE +0x4  | 174 | animation::mixing::animation_lexeme override_lexeme( ... chained params temporary ... );
	// VERDICT: STRUCTURE MATCH (9/9) - residuals are per-site LTCG: target inlines animation_type() (member read + temps) where base keeps the COMDAT call; setter-chain/expression-ctor call boundaries differ by promoted-convention bytes. Non-steerable from this TU.
}

// STATE[86.5%|PARTIAL]: control flow + placement-new + ctor call all match. Sole residual is the
// computed_shooting_animation_time_scale callee: it is a STUB returning a constant, so our /Od build
// FOLDS it to `fldz` at the call site (no call), while the target keeps it OUT-OF-LINE and the LTCG
// whole-program optimizer returns the float in xmm0 (movss [esp], xmm0) and re-uses spill slots
// (frame 48h vs 0Ch). Argument passing / inline-vs-call at the call boundary - not source-steerable
// until that callee is matched. SAME wall as the pistol sibling new_object (#174, 92.08%).
double_barreled_weapon_core_fire_state* weapon_core_state_cook_template<survarium::double_barreled_weapon_core_fire_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	u32									animations_count
)
{
	return new ( buffer.c_ptr( ) ) double_barreled_weapon_core_fire_state(
		params->weapon,
		computed_shooting_animation_time_scale( *animations, params->rounds_per_second ),
		animations,
		animations_count
	);

	// STRUCTURE DIFF: target 1 stmt / base 1 stmt
	// SIZE -0x11 | 214 | ); (the whole placement-new return statement)
	// VERDICT: STRUCTURE MATCH (1/1) - base folds the STUB computed_shooting_animation_time_scale to fldz at the call site while target keeps the call (xmm0 return, movss [esp]); closes itself once that callee is matched.
}

} // namespace survarium
