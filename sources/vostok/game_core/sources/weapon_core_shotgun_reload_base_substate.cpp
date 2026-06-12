////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_shotgun_reload_base_substate.h>

#include <vostok/game_core/weapon_core.h>

#include <vostok/animation/mixing_addition_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme_parameters.h>
#include <vostok/animation/linear_interpolator.h>

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

// STATE[78.64%|PARTIAL]: STRUCTURE MATCH (16/16). Residual SIZE rows are LTCG inline-vs-call:
// ASSERT (target has extra conditional on animations_count), for-loop construct differences,
// and operator= for resource_ptr assignments.
weapon_core_shotgun_reload_base_substate::weapon_core_shotgun_reload_base_substate(
	weapon_core&							weapon,
	const float								animation_time_scale,
	resources::managed_resource_ptr const*	animations,
	const u32								animations_count,
	const animation::mixing::playback_enum	playback_type,
	const u32								time_synchronization_group,
	pcstr									animation_id,
	pcstr									hands_stand_animation_id,
	pcstr									hands_crouch_animation_id,
	pcstr									hands_jump_animation_id
) :
	m_weapon( weapon ),
	m_animation_to_wait_for( ),
	m_animation_playback_state( 0 ),
	m_animation_timescale( animation_time_scale ),
	m_playback_type( playback_type ),
	m_time_synchronization_group( time_synchronization_group ),
	m_animation_id( animation_id )
{
	m_hand_animation_captions[0] = hands_stand_animation_id;
	m_hand_animation_captions[1] = hands_crouch_animation_id;
	m_hand_animation_captions[2] = hands_jump_animation_id;

	ASSERT( UNKNOWN_EXPRESSION );

	u32 animation_index = 0;

	for ( u32 view_index = 0; view_index < 2; ++view_index )
	{
		for ( u32 user_state_index = 0; user_state_index < 2; ++user_state_index, ++animation_index )
		{
			m_weapon_animations[view_index][user_state_index] = animations[animation_index];
		}
	}

	for ( u32 view_index = 0; view_index < 2; ++view_index )
	{
		for ( u32 user_state_index = 0; user_state_index < 2; ++user_state_index, ++animation_index )
		{
			m_user_animations[view_index][user_state_index] = animations[animation_index];
		}
	}

	ASSERT( UNKNOWN_EXPRESSION );

	// STRUCTURE DIFF: target 16 / base 16 stmts
	// SIZE -0x17 | 57 | ASSERT( UNKNOWN_EXPRESSION );
	// SIZE +0x9  | 63 | for ( u32 user_state_index = 0; user_state_index < 2; ++user_state_index, ++animation_index )
	// SIZE -0xf  | 65 | m_weapon_animations[view_index][user_state_index] = animations[animation_index];
	// SIZE +0x9  | 71 | for ( u32 user_state_index = 0; user_state_index < 2; ++user_state_index, ++animation_index )
	// SIZE -0xf  | 73 | m_user_animations[view_index][user_state_index] = animations[animation_index];
	// VERDICT: STRUCTURE MATCH (shape ok) - SIZE rows are LTCG inline-vs-call differences for ASSERT, for-loop constructs, and resource_ptr operator=. Non-steerable.
}

// STATE[100%|DONE]: objdiff-unscored (newly emitted, absent from the delink unit's function list) but
// base 0x449160 is byte-identical to target 0x798790 (0xb0 bytes). See md.
weapon_lexeme_pair weapon_core_shotgun_reload_base_substate::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	m_animation_to_wait_for = m_weapon_animations[is_third_view != false][user_state_id == type_crouch];

	return get_weapon_lexeme_pair_impl(
		buffer,
		m_animation_id,
		m_animation_to_wait_for,
		&m_weapon,
		*m_animation_playback_state,
		m_time_synchronization_group,
		m_animation_timescale,
		m_playback_type,
		animation::linear_interpolator( s_aim_transition_time )
	);
}

// STATE[68.18%|PARTIAL]: control structure matches the target statement-for-statement (6 stmts,
// identical boundaries) - the get_weapon_lexeme_pair call, user_state_index, the override_lexeme
// animation_lexeme_parameters builder chain, and the chained `override + main + offset`. Residual
// is the documented whole-program LTCG inline-vs-call of the trivial animation_lexeme_parameters
// setters (animated_object/bones_mask/playback_type) + operator+ in shared animation headers:
// the target keeps them OUT-OF-LINE, our /GL inlines them (L83 0xa9 vs target 0x86). Same
// unsteerable class as pistol_weapon_core_show_state::weapon_and_hands_expression (72%) and
// get_weapon_lexeme_pair_impl (None); out of this file's scope. See md.
animation::mixing::expression weapon_core_shotgun_reload_base_substate::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	const bool							is_third_view,
	const weapon_user_state_enum		user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );

	const u32 user_state_index = user_state_id == type_crouch;

	animation::mixing::animation_lexeme override_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			m_hand_animation_captions[user_state_index],
			m_user_animations[is_third_view != false][user_state_index],
			&lexeme_pair.offset_lexeme,
			&weight_driving_animation
		)
		.animated_object( m_weapon.get_user( ) )
		.bones_mask( 2 )
		.playback_type( m_playback_type )
	);

	return override_lexeme + lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme;

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// SIZE +0x23 | 128 | );
	// SIZE -0x8  | 130 | return override_lexeme + lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme;
	// VERDICT: STRUCTURE MATCH (shape ok) - both SIZE rows are the per-call-site LTCG inline-vs-call wall: target keeps get_user/animated_object/bones_mask/playback_type setters and operator+ out-of-line, base inlines them. Non-steerable.
}

} // namespace survarium
