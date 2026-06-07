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
double_barreled_weapon_core_aimed_fire_state::double_barreled_weapon_core_aimed_fire_state(
	weapon_core&							weapon,
	float									animation_time_scale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
) : weapon_core_aimed_fire_state_base( weapon, animation_time_scale ),
	m_weapon_animation_index( u32( -1 ) )
{
	ASSERT_CMP_U( weapon.get_magazine_capacity( ), ==, 2 );
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

// STATE[99.76%|DONE]: every instruction/branch matches. Sole residual is the
// `m_weapon.ammo_in_magazine()` __thiscall `this` loaded into eax (target) vs ecx (base) -
// an argument-passing register choice (LTCG). Same shape as the double_barreled fire_state sibling.
void double_barreled_weapon_core_aimed_fire_state::initialize( )
{
	weapon_core_aimed_fire_state_base::initialize( );

	ASSERT( UNKNOWN_EXPRESSION );

	m_weapon_animation_index = m_weapon.ammo_in_magazine( ) != 2;
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
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	ASSERT( UNKNOWN_EXPRESSION );

	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );

	animation::mixing::expression hands_expression =
		get_user_hands_expression( lexeme_pair.offset_lexeme, buffer, is_third_view, user_state_id, weight_driving_animation );

	return lexeme_pair.main_lexeme + animation::mixing::expression( lexeme_pair.offset_lexeme ) + hands_expression;

	// FUNCTION BODY
	// <0x7ac240>|0x010|+0x012:'54'	ASSERT( UNKNOWN_EXPRESSION );
	// <0x7ac252>|0x022|+0x01f:'55'	lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );
	// <0x7ac271>|0x041|+0x02a:'56'	hands_expression = get_user_hands_expression( offset, ..., weight_driving_animation );
	// <0>
	// <0x7ac29b>|0x06b|+0x07a:'58'	return main + expression( offset ) + hands_expression;
	// ******
}

// STATE[100%|DONE]
weapon_lexeme_pair double_barreled_weapon_core_aimed_fire_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
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

// STATE[79.39%|PARTIAL]: structure now matches the target exactly - 12 statements both sides (sprint
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

	pcstr user_animation_captions[2] = { "aimed_stand_shot", "aimed_crouch_shot" };

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

	// FUNCTION BODY
	// <0x7abff1>|0x011|+0x006:'78'	if ( user_state_id == type_sprint )
	// <0x7abff7>|0x017|+0x010:'79'	return expression( weapon_lexeme );
	// <0>
	// <0x7ac007>|0x027|+0x00c:'81'	u32 user_animation_index = ( user_state_id == type_crouch );
	// <0x7ac013>|0x033|+0x020:'82'	selected_animation = m_user_animations[is_third_view!=false][user_animation_index];
	// <0x7ac033>|0x053|+0x059:'83'	if ( pinned_ptr_const<...>( selected_animation )->animation_type() != additive )
	// <0x7ac08c>|0x0ac|+0x010:'84'	return expression( weapon_lexeme );
	// <0>
	// <1>
	// <0x7ac09c>|0x0bc|+0x00e:'87'	pcstr user_animation_captions[2] = { "aimed_stand_shot", "aimed_crouch_shot" };
	// <0>
	// <0x7ac0aa>|0x0ca|+0x010:'89'	animation::linear_interpolator interpolator( s_aim_transition_time );
	// <0> .. <10>					animation_lexeme_parameters ctor + setter chain
	// <0x7ac0ba>|0x0da|+0x06e:'101'	override_lexeme_parameters( ... ).animated_object().playback_type(1).additivity_priority(1); animation_lexeme override_lexeme( params );
	// <0x7ac128>|0x148|+0x021:'102'	return expression( override_lexeme );
	// ******
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
	u32									animations_count
)
{
	return new ( buffer.c_ptr( ) ) double_barreled_weapon_core_aimed_fire_state(
		params->weapon,
		computed_shooting_animation_time_scale( *animations, params->rounds_per_second ),
		animations,
		animations_count
	);

	// FUNCTION BODY
	// <0x7abf79>|0x009|+0x05c:'114'	return new ( buffer.c_ptr( ) ) double_barreled_weapon_core_aimed_fire_state( ... );
	// ******
}

} // namespace survarium
