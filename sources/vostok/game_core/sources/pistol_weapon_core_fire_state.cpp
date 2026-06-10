////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/pistol_weapon_core_fire_state.h>

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

// STATE[100%|DONE]
pistol_weapon_core_fire_state::pistol_weapon_core_fire_state(
	weapon_core&							weapon,
	float									animation_time_scale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
) : weapon_core_fire_state_base( weapon, animation_time_scale ),
	m_weapon_animation_index( u32( -1 ) )
{
	ASSERT_CMP_U( animations_count, ==, 12 );

	u32 animation_index = 0;
	for ( u32 view = 0 ; view != 2 ; ++view )
		for ( u32 user_state = 0 ; user_state != 2 ; ++user_state )
			for ( u32 weapon_state = 0 ; weapon_state != 2 ; ++weapon_state )
				m_weapon_animations[view][user_state][weapon_state] = animations[animation_index++];

	for ( u32 view = 0 ; view != 2 ; ++view )
		for ( u32 user_state = 0 ; user_state != 2 ; ++user_state )
			m_user_animations[view][user_state] = animations[animation_index++];

	ASSERT( UNKNOWN_EXPRESSION );
}

// STATE[92.62%|PARTIAL]: every instruction/branch matches; residual is the __thiscall `this`
// for m_weapon.ammo_in_magazine() loaded into ecx (base) vs eax (target) - an LTCG
// argument-passing register choice, same class as the aimed sibling (99.76).
void pistol_weapon_core_fire_state::initialize( )
{
	weapon_core_fire_state_base::initialize( );

	bool last_shot = m_weapon.get_bullets_in_queue( )
		? ( m_weapon.ammo_in_magazine( ) == 1 )
		: ( m_weapon.ammo_in_magazine( ) == 0 );

	m_weapon_animation_index = last_shot ? 1u : 0u;

	// STRUCTURE DIFF: target 3 / base 3 stmts, 0x85 bytes both (after the ternary final-store fix)
	// VERDICT: STRUCTURE MATCH - residual is the ammo_in_magazine `this` in eax (target) vs ecx (base), LTCG arg-register choice, non-steerable.
}

// STATE[79.09%|PARTIAL]: operator+ template-selection / inline-vs-call LTCG on the addition
// tree (the weapon_core_fire_state #193 wall), non-steerable from this TU.
animation::mixing::expression pistol_weapon_core_fire_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool const							is_third_view,
	weapon_user_state_enum const		user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );

	if ( user_state_id == type_sprint )
		return animation::mixing::expression( lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme );

	ASSERT( UNKNOWN_EXPRESSION );

	animation::mixing::expression hands_expression =
		get_user_hands_expression( lexeme_pair.offset_lexeme, buffer, is_third_view, user_state_id, weight_driving_animation );

	return lexeme_pair.main_lexeme + animation::mixing::expression( lexeme_pair.offset_lexeme ) + hands_expression;

	// STRUCTURE DIFF: target 6 / base 6 stmts
	// SIZE -0x21 | 94 | return animation::mixing::expression( lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme );
	// SIZE -0x11 | 101 | return lexeme_pair.main_lexeme + animation::mixing::expression( lexeme_pair.offset_lexeme ) + hands_expression;
	// VERDICT: STRUCTURE MATCH (6/6) - both rows are the mixing operator+ template-selection / inline-vs-call wall (target keeps operator+<animation_lexeme> out-of-line, base inlines addition_lexeme), whole-program LTCG, non-steerable.
}

// STATE[100%|DONE]
weapon_lexeme_pair pistol_weapon_core_fire_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	pcstr weapon_animation_captions[2] = { "pistol-shot", "pistol-shot_last" };

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
		m_playback_type,
		animation::linear_interpolator( s_aim_transition_time )
	);
}

// STATE[77.13%|PARTIAL]: lexeme_parameters setters / animation_type() inline-vs-call LTCG,
// non-steerable (identical residual to weapon_core_fire_state::get_user_hands_expression).
animation::mixing::expression pistol_weapon_core_fire_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool const							is_third_view,
	weapon_user_state_enum const		user_state_id,
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

	pcstr user_animation_captions[2] = { "stand_shot_pistol", "crouch_shot_pistol" };

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

	// STRUCTURE DIFF: target 9 / base 9 stmts
	// SIZE +0x3  | 147 | return animation::mixing::expression( weapon_lexeme );
	// SIZE -0x13 | 153 | if ( pinned_ptr_const<...>( selected_animation )->animation_type( ) != ... )
	// SIZE +0x3  | 154 | return animation::mixing::expression( weapon_lexeme );
	// SIZE +0x4  | 169 | ); (the chained params temporary declaration of override_lexeme)
	// VERDICT: STRUCTURE MATCH (9/9) - per-site LTCG: target inlines animation_type() where base keeps the COMDAT call; setter-chain/expression-ctor boundaries differ by promoted-convention bytes, non-steerable.
}

// STATE[92.08%|PARTIAL]: control flow + placement-new + ctor call all match. Sole residual is
// the LTCG-specialized calling convention of computed_shooting_animation_time_scale: the target's
// whole-program optimizer passes its `managed_resource_ptr const&` arg in a register and returns
// the float in xmm0 (movss), while our STUB callee keeps default cdecl (stack push + st0/fstp).
// Argument passing at the call boundary - not source-steerable until that callee is matched.
pistol_weapon_core_fire_state* weapon_core_state_cook_template<survarium::pistol_weapon_core_fire_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	u32									animations_count
)
{
	return new ( buffer.c_ptr( ) ) pistol_weapon_core_fire_state(
		params->weapon,
		computed_shooting_animation_time_scale( *animations, params->rounds_per_second ),
		animations,
		animations_count
	);

	// STRUCTURE DIFF: target 1 / base 1 stmts
	// SIZE -0x11 | 191 | ); (the placement-new ctor-call statement)
	// VERDICT: STRUCTURE MATCH - residual is the LTCG-promoted convention of computed_shooting_animation_time_scale (target: ref in reg + xmm0 return; base: cdecl push + st0/fstp), blocked on that callee, non-steerable here.
}

} // namespace survarium
