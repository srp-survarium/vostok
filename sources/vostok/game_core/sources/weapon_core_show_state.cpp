////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_show_state.h>

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
weapon_core_show_state::weapon_core_show_state(
	weapon_core&							weapon,
	float									animation_timescale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count,
	bool&									is_shown
) : weapon_core_show_state_base( weapon, is_shown )
{
	m_time_scale = animation_timescale;

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

// STATE[83.52%|PARTIAL]: structure (get_weapon_lexeme_pair, get_user_hands_expression, then the
// chained `+`) matches the target. Same residual class as pistol_weapon_core_show_state::
// weapon_and_hands_expression (#160, 83.52%): the target uses dedicated expression-returning
// `operator+(expression&, animation_lexeme&)` / `operator+(expression&, expression&)` overloads that
// do NOT exist in the shared mixing_addition_lexeme_inline.h, so the base emits the generic
// operator+<T1,T2> + expression(addition_lexeme) instead. Recovering needs adding those overloads to
// the shared animation header (out of this file's scope; a fix is under investigation on PR #192).
animation::mixing::expression weapon_core_show_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );

	animation::mixing::expression hands_expression = get_user_hands_expression( lexeme_pair.offset_lexeme, buffer, is_third_view, user_state_id, weight_driving_animation );

	return hands_expression + lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme;

	// FUNCTION BODY
	// <0x7af1f0>|0x010|+0x01f:'43'
	// <0x7af20f>|0x02f|+0x02a:'44'
	// <0x7af239>|0x059|+0x07a:'45'
	// ******
}

// STATE[100%|DONE]
weapon_lexeme_pair weapon_core_show_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	pcstr animation_identifier = "weapon-show";

	resources::managed_resource_ptr const& selected_animation =
		m_weapon_animations[is_third_view != false][user_state_id == type_crouch];

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
		animation::linear_interpolator( s_aim_transition_time )
	);
}

// STATE[72.12%|PARTIAL]: control structure matches the target exactly (the type_sprint early-return
// of weapon_lexeme; the user_state_index; the animation_lexeme_parameters builder chain; the final
// expression(override_lexeme)). Residual is purely whole-program inline decisions in shared
// animation/weapon_core headers: the target keeps weapon_core::get_user() and the
// animation_lexeme_parameters setters (animated_object/bones_mask/playback_type) OUT-OF-LINE while the
// base inlines them. These are `inline`-declared shared-header one-liners; forcing them out-of-line is
// out of this file's scope. Identical residual to pistol_weapon_core_show_state::get_user_hands_expression.
animation::mixing::expression weapon_core_show_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	if ( user_state_id == type_sprint )
		return weapon_lexeme;

	u32 user_state_index = user_state_id == type_crouch;

	pcstr animation_captions[2] = { "stand_show", "crouch_show" };

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

	// FUNCTION BODY
	// <0x7af071>|0x011|+0x006:'62'
	// <0x7af077>|0x017|+0x010:'63'
	// <0>
	// <1>
	// <0x7af087>|0x027|+0x00c:'66'
	// <0>
	// <0x7af093>|0x033|+0x00e:'68'
	// <0>
	// <1> <2> <3> <4> <5> <6> <7> <8> <9> <10> <11>
	// <0x7af0a1>|0x041|+0x079:'81'
	// <0x7af11a>|0x0ba|+0x01c:'82'
	// ******
}

// STATE[99.98%|PARTIAL]: real index bug, NOT a reloc artifact. Target line 90 reads
// `mov ecx,[ebp+14h]; add ecx, 10h` (=animations[0] + 0x10) for user_anim_length; sizeof
// managed_resource_ptr is 4, so 0x10 == animations[4] (the first USER animation: the ctor
// loads weapon anims into indices 0-3, user anims into 4-7). Our base emits `add ecx, 4`
// (animations[1]). NEXT STEP: change `animations[1]` -> `animations[4]` on the user_anim_length
// line, then rebuild. (Sibling pistol new_object uses animations[8] because its weapon block is
// 2x2x2=8 wide; same rule, first user anim.) The two `call ?end@buffer_string` reloc-target
// diffs are benign ICF-fold-representative churn - pistol new_object carries them and still
// scores 100.0, so they do NOT block this; the add-immediate is the sole real residual.
// claude@MATCH: animations[1] is the bug - target offset 0x10 -> animations[4].
weapon_core_show_state* weapon_core_state_cook_template<weapon_core_show_state>::new_object(
	mutable_buffer							buffer,
	weapon_state_creation_params const*		params,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
)
{
	float weapon_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[0] )->length_in_frames( );
	float user_anim_length = animation::cubic_spline_skeleton_animation_pinned( animations[1] )->length_in_frames( );	// claude@MATCH: should be animations[4] (target add ecx,10h); see STATE
	float time_scale = weapon_anim_length / user_anim_length;

	return new ( buffer.c_ptr( ) ) weapon_core_show_state( params->weapon, time_scale, animations, animations_count, params->shown );

	// FUNCTION BODY
	// <0x7aef99>|0x009|+0x02b:'89'	weapon_anim_length = ...animations[0]...
	// <0x7aefc4>|0x034|+0x030:'90'	user_anim_length   = ...animations[4]... (target add ecx,10h; base emits animations[1] = add ecx,4)
	// <0x7aeff4>|0x064|+0x00f:'91'	time_scale = weapon_anim_length / user_anim_length
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x7af003>|0x073|+0x053:'98'	return new ( buffer.c_ptr() ) weapon_core_show_state( ... )
	// ******
}

} // namespace survarium
