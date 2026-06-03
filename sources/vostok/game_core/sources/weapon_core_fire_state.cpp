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
// match. Sole residual is the per-call-site LTCG inline-vs-call of operator+<...> (mixing
// addition_lexeme): the target keeps the first `main + expression(offset)` operator+ OUT-OF-LINE,
// our /GL build inlines it (addition_lexeme ctor + cloned_in_buffer at this site), shifting the
// [ebp-XX] layout. Same unsteerable operator+ inline class as weapon_core_idle_state (85.65%);
// operator+ is a standalone COMDAT in both rich indexes, so the inline choice is whole-program,
// not source. weight_driving_animation is unreferenced in the target too (see .md).
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

	// LOCALS
	// animation::mixing::expression hands_expression
	// weapon_lexeme_pair 			lexeme_pair
	// ******

	// FUNCTION BODY (target rva 0x799df0; weight_driving_animation [ebp+18h] is unreferenced)
	// <0>
	// <0x7a9e00>|0x010|+0x01f:'46'	lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );
	// <0x7a9e1f>|0x02f|+0x026:'47'	hands_expression = get_user_hands_expression( lexeme_pair.offset_lexeme, ... );  (1st arg = OFFSET lexeme)
	// <0x7a9e45>|0x055|+0x07a:'48'	return main + expression( offset ) + hands_expression;
	//   target: expression(offset)@-118h; operator+<animation_lexeme>(main, expr) OUT-OF-LINE; operator+(.., hands)
	//   base:   expression(offset)@-118h; addition_lexeme<animation_lexeme,expression>(main, expr) INLINED; operator+<addition_lexeme,expression>(.., hands)
	// ******
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

// STATE[73.51%|PARTIAL]: every statement, branch and operand matches (sprint early-return, the
// animation_type()!=additive early-return, the captions array, the lexeme_parameters build and
// both expression returns). Residuals are the same unsteerable inline-vs-call LTCG class as
// get_weapon_lexeme_pair_impl: the target keeps the animation_lexeme_parameters setters
// (animated_object/playback_type/additivity_priority) OUT-OF-LINE while our /GL build inlines
// animated_object+playback_type (only additivity_priority stays a call), which shifts the layout
// and reschedules m_weapon.get_user(). The animation_type() check is also COMDAT-folded in the
// target onto a trivial `[+0x18]` getter symbol (delinked as inventory_holder::scheduler) - same
// bytes, different attributed name. Both whole-program decisions, not source-steerable. See .md.
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

	// FUNCTION BODY (target rva 0x799bc0)
	// <0x7a9bd1>|0x011|+0x006:'65'	if ( user_state_id == type_sprint )
	// <0x7a9bd7>|0x017|+0x010:'66'	return expression( weapon_lexeme );
	// <0>
	// <0x7a9be7>|0x027|+0x00f:'68'	u32 user_animation_index = ( user_state_id == type_crouch );
	// <0x7a9bf6>|0x036|+0x026:'69'	selected_animation = m_user_animations[is_third_view!=false][user_animation_index];
	// <0x7a9c1c>|0x05c|+0x05c:'70'	if ( pinned_ptr_const<cubic_spline_skeleton_animation>( selected_animation )->animation_type() != animation_type_additive )
	// <0x7a9c78>|0x0b8|+0x010:'71'	return expression( weapon_lexeme );
	// <0>
	// <1>
	// <0x7a9c88>|0x0c8|+0x00e:'74'	pcstr user_animation_captions[2] = { "stand_shoot", "crouch_shoot" };
	// <0> .. <9>							animation_lexeme_parameters ctor + setter chain sub-blocks (.85)
	// <0x7a9c96>|0x0d6|+0x06f:'85'	hands_lexeme_parameters( ... ).animated_object(get_user).playback_type(1).additivity_priority(1); animation_lexeme hands_lexeme( params );
	// <0x7a9d05>|0x145|+0x01c:'86'	return expression( hands_lexeme );
	// ******
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

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7a9b59>|0x009|+0x05c:'99'
	// ******
}

} // namespace survarium
