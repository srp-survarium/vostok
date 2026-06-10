////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_aimed_fire_state.h>

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
// the timescale inline definition (as the fire_state sibling does) to resolve it.
float computed_shooting_animation_time_scale( resources::managed_resource_ptr const& shooting_animation, float rounds_per_second );
}
#include <vostok/game_core/weapon_animations_timescale_inline.h>
namespace survarium {

// STATE[100%|DONE]
weapon_core_aimed_fire_state::weapon_core_aimed_fire_state(
	weapon_core&							weapon,
	float									animation_timescale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
) : weapon_core_aimed_fire_state_base( weapon, animation_timescale )
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

// STATE[78.53%|PARTIAL]: every statement, branch, lexeme operand and both addition-tree shapes
// match (the type_sprint/type_jump early return building main+offset, and the full
// main + expression(offset) + hands tree). Residual is the operator+ inline-vs-out-of-line /GL
// decision flipping at BOTH operator+ sites: in the .1 (sprint/jump) branch the target INLINES
// operator+<animation_lexeme,animation_lexeme> (addition_lexeme ctor + cloned_in_buffer visible)
// while our build keeps it OUT-OF-LINE; in the .2 branch the target keeps operator+<animation_lexeme>
// OUT-OF-LINE while our build INLINES addition_lexeme. Same mixing operator+ template-selection /
// inline wall as weapon_core_fire_state::weapon_and_hands_expression (82.52%), blocked on the mixing
// unit; two diverging sites here vs one there, hence the lower %. See .md.
animation::mixing::expression weapon_core_aimed_fire_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );

	if ( user_state_id == type_sprint || user_state_id == type_jump )
		return lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme;

	animation::mixing::expression hands_expression =
		get_user_hands_expression( lexeme_pair.offset_lexeme, buffer, is_third_view, user_state_id );

	return lexeme_pair.main_lexeme + animation::mixing::expression( lexeme_pair.offset_lexeme ) + hands_expression;

	// LOCALS
	// animation::mixing::expression hands_expression
	// weapon_lexeme_pair 			lexeme_pair
	// ******

	// FUNCTION BODY (target rva 0x799890; weight_driving_animation [ebp+18h] is unreferenced)
	// <0>
	// <0x7a98a1>|0x011|+0x01f:'46'	lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );
	// <0x7a98c0>|0x030|+0x00c:'47'	if ( user_state_id == type_sprint || user_state_id == type_jump )
	// <0x7a98cc>|0x03c|+0x05e:'48'	return main + offset (.1: addition_lexeme<animation_lexeme,animation_lexeme>)
	// <0x7a992a>|0x09a|+0x026:'49'	hands = get_user_hands_expression( offset, buffer, is_third_view, user_state_id );
	// <0x7a9950>|0x0c0|+0x07a:'50'	return main + expression(offset) + hands (.2)
	// ******
	// Sole residual is the SAME operator+<animation_lexeme> OUT-OF-LINE vs addition_lexeme INLINE /GL
	// template-selection wall as weapon_core_fire_state::weapon_and_hands_expression.
}

// STATE[100%|DONE]
weapon_lexeme_pair weapon_core_aimed_fire_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	pcstr animation_identifier = "weapon-aimed_fire";

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

// STATE[73.51%|PARTIAL]: every statement, branch and operand matches (sprint early-return, the
// animation_type()!=additive early-return, the captions array, the lexeme_parameters build and
// both expression returns). Residuals are the same unsteerable inline-vs-call LTCG class as
// weapon_core_fire_state::get_user_hands_expression (73.51%): the target keeps the
// animation_lexeme_parameters setters OUT-OF-LINE while our /GL build inlines
// animated_object+playback_type (only additivity_priority stays a call), shifting the layout.
// animation_type() is also COMDAT-folded onto a trivial getter (delinked as inventory_holder::scheduler).
// Both whole-program decisions, not source-steerable. See .md.
animation::mixing::expression weapon_core_aimed_fire_state::get_user_hands_expression(
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

	pcstr user_animation_captions[2] = { "aimed_shoot", "crouch_aimed_shoot" };

	animation::mixing::animation_lexeme_parameters hands_lexeme_parameters(
		buffer,
		user_animation_captions[user_animation_index],
		selected_animation,
		&weapon_lexeme,
		NULL
	);
	hands_lexeme_parameters
		.animated_object						( m_weapon.get_user( ) )
		.playback_type							( animation::mixing::play_once_and_freeze_at_end )
		.additivity_priority					( 1 );

	animation::mixing::animation_lexeme hands_lexeme( hands_lexeme_parameters );

	return animation::mixing::expression( hands_lexeme );

	// LOCALS
	// u32 							user_animation_index
	// resources::managed_resource_ptr const& selected_animation
	// animation::mixing::animation_lexeme hands_lexeme
	// pcstr[2] 					user_animation_captions
	// ******

	// FUNCTION BODY (target rva 0x799660)
	// <0x7a9671>|0x011|+0x006:'67'	if ( user_state_id == type_sprint )
	// <0x7a9677>|0x017|+0x010:'68'	return expression( weapon_lexeme );
	// <0>
	// <0x7a9687>|0x027|+0x00f:'70'	u32 user_animation_index = ( user_state_id == type_crouch );
	// <0x7a9696>|0x036|+0x026:'71'	selected_animation = m_user_animations[is_third_view!=false][user_animation_index];
	// <0x7a96bc>|0x05c|+0x05c:'72'	if ( pinned_ptr_const<cubic_spline_skeleton_animation>( selected_animation )->animation_type() != animation_type_additive )
	// <0x7a9718>|0x0b8|+0x010:'73'	return expression( weapon_lexeme );
	// <0>
	// <1>
	// <0x7a9728>|0x0c8|+0x00e:'76'	pcstr user_animation_captions[2] = { "aimed_shoot", "crouch_aimed_shoot" };
	// <0> .. <9>							animation_lexeme_parameters ctor + setter chain sub-blocks (.87)
	// <0x7a9736>|0x0d6|+0x06f:'87'	hands_lexeme_parameters( ... ).animated_object(get_user).playback_type(1).additivity_priority(1); animation_lexeme hands_lexeme( params );
	// <0x7a97a5>|0x145|+0x01c:'88'	return expression( hands_lexeme );
	// ******
}

// STATE[86.5%|PARTIAL]: control flow + placement-new + ctor call all match. Sole residual is the
// LTCG-specialized calling convention of computed_shooting_animation_time_scale: the target's
// whole-program optimizer passes its `managed_resource_ptr const&` arg in a register and returns
// the float in xmm0 (movss), while our STUB callee keeps default cdecl (stack push + st0/fstp).
// Argument passing at the call boundary - identical wall to weapon_core_fire_state::new_object
// (86.5%); not source-steerable until that callee is matched. See .md.
weapon_core_aimed_fire_state* weapon_core_state_cook_template<survarium::weapon_core_aimed_fire_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	u32									animations_count
)
{
	return new ( buffer.c_ptr( ) ) weapon_core_aimed_fire_state(
		params->weapon,
		computed_shooting_animation_time_scale( *animations, params->rounds_per_second ),
		animations,
		animations_count
	);

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7a95f9>|0x009|+0x05c:'100'
	// ******
}

} // namespace survarium
