////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_reload_state.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/game_core/weapon_state_creation_params.h>
#include <vostok/game_core/weapon_core_state_cook_template.h>
#include <vostok/game_core/weapon_animations_timescale_inline.h>

#include <vostok/animation/linear_interpolator.h>
#include <vostok/animation/mixing_addition_lexeme.h>
#include <vostok/animation/mixing_animation_lexeme_parameters.h>

namespace survarium {

static float s_reload_transition_time = 0.3f;

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
weapon_core_reload_state::weapon_core_reload_state(
	weapon_core&							weapon,
	float									animation_time_scale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
) : weapon_core_reload_state_base( weapon, animation_time_scale )
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

// STATE[83.52%|PARTIAL]: first two statements match byte-for-byte. The return
// `hands + main + offset` cannot match: the target selects operator+ overloads that DO NOT
// EXIST in the on-disk mixing headers. Target uses `operator+<animation_lexeme>(expression&,
// animation_lexeme&) -> expression` (a `template<T> operator+(expression&, T&)` form) for the
// first `+`, then `operator+(expression&, expression&) -> expression` (non-template) for the
// second - both return `expression` by value and call the real `expression::is_empty()`.
// vostok/animation/mixing_addition_lexeme_inline.h only provides `template<T1,T2> operator+
// -> addition_lexeme&`, and mixing_expression.h's `is_empty()` is a `return false` STUB. So no
// reshaping of the return expression can select the target's overloads - this is a cross-unit
// header gap (the whole mixing operator+ family + is_empty body), not source-steerable here.
// Re-match attempts (all rebuilt, report.json): baseline 83.52; `+ expression(offset)` -> 77.48;
// `expression(hands+main) + offset` -> 56.65. See the .md / assembly_patterns.md.
animation::mixing::expression weapon_core_reload_state::weapon_and_hands_expression(
	mutable_buffer&							buffer,
	bool									is_third_view,
	weapon_user_state_enum					user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );

	animation::mixing::expression hands_expression = get_user_hands_expression( lexeme_pair.offset_lexeme, buffer, is_third_view, user_state_id, weight_driving_animation );

	return hands_expression + lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme;

	// FUNCTION BODY
	// <0x7aaaf0>|0x010|+0x01f:'44'
	// <0x7aab0f>|0x02f|+0x02a:'45'
	// <0x7aab39>|0x059|+0x07a:'46'
	// ******
}

// STATE[100%|DONE]
weapon_lexeme_pair weapon_core_reload_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	pcstr animation_identifier = "weapon-reload";

	resources::managed_resource_ptr const& selected_animation =
		m_weapon_animations[is_third_view != false][user_state_id == type_crouch];

	set_animation_to_wait( selected_animation );

	return get_weapon_lexeme_pair_impl(
		buffer,
		animation_identifier,
		selected_animation,
		&m_weapon,
		m_animation_playback_state,
		2,
		m_animation_timescale,
		animation::mixing::play_once_and_freeze_at_end,
		animation::linear_interpolator( s_reload_transition_time )
	);
}

// STATE[73.6%|PARTIAL]: all 7 statements (srcline 62,63,65,67,69,82,83) match the target
// carcass; residual is target keeping animation_lexeme_parameters setters / weapon_core::get_user /
// ~animation_lexeme_parameters OUT-OF-LINE while base inlines them (in-class header inlines under
// /Ob2 /GL whole-program inlining) - not de-inlinable from this TU. See the .md.
animation::mixing::expression weapon_core_reload_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	if ( user_state_id == type_sprint )
		return weapon_lexeme;

	ASSERT( UNKNOWN_EXPRESSION );

	u32 const user_state_index = user_state_id == type_crouch;

	pcstr const animation_captions[2] = { "stand_reload", "crouch_reload" };

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
	// <0x7aa961>|0x011|+0x006:'62'
	// <0x7aa967>|0x017|+0x010:'63'
	// <0>
	// <0x7aa977>|0x027|+0x012:'65'
	// <0>
	// <0x7aa989>|0x039|+0x00c:'67'
	// <0>
	// <0x7aa995>|0x045|+0x00e:'69'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <0x7aa9a3>|0x053|+0x079:'82'	override_lexeme(...) - target keeps setters/get_user/~params OUT-OF-LINE; base inlines them (/Ob2 /GL whole-program decision)
	// <0x7aaa1c>|0x0cc|+0x01c:'83'	return override_lexeme;
	// ******
}

// STATE[86.5%|PARTIAL]: placement-new + ctor call + arg order all match; sole residual is the
// timescale arg - target emits `call computed_reload_animation_time_scale`, base inlines that
// helper to a constant `fldz` because it is still an inline STUB (return 0.0f) in
// weapon_animations_timescale_inline.h. Bounded by that helper's own unit; should reach 100%
// once it is matched non-stub. See the .md.
weapon_core_reload_state* weapon_core_state_cook_template<weapon_core_reload_state>::new_object(
	mutable_buffer							buffer,
	weapon_state_creation_params const*		params,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
)
{
	return new ( buffer.c_ptr( ) ) weapon_core_reload_state(
		params->weapon,
		computed_reload_animation_time_scale( animations[0], params->reload_time ),
		animations,
		animations_count
	);

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7aa8e9>|0x009|+0x05c:'95'	target: call computed_reload_animation_time_scale; base inlines the 0.0f STUB
	// ******
}

} // namespace survarium
