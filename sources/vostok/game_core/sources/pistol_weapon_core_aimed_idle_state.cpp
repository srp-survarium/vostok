////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/pistol_weapon_core_aimed_idle_state.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/game_core/weapon_state_creation_params.h>
#include <vostok/game_core/weapon_core_state_cook_template.h>

#include <vostok/animation/mixing_addition_lexeme.h>
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

// STATE[100%|DONE]
pistol_weapon_core_aimed_idle_state::pistol_weapon_core_aimed_idle_state( weapon_core& weapon, resources::managed_resource_ptr const* animations, u32 animations_count ) :
	weapon_core_aimed_state_base( weapon )
{
	ASSERT_CMP_U( animations_count, ==, 8 );

	u32 animation_index = 0;
	for ( u32 view = 0 ; view != 2 ; ++view )
		for ( u32 user_state = 0 ; user_state != 2 ; ++user_state )
			for ( u32 weapon_state = 0 ; weapon_state != 2 ; ++weapon_state )
				m_weapon_animations[view][user_state][weapon_state] = animations[animation_index++];

	ASSERT( UNKNOWN_EXPRESSION );
}

// STATE[85.65%|PARTIAL]: structure matches (ASSERT_U, get_weapon_lexeme_pair, return expr).
// Wall: target inlines operator+<animation_lexeme,animation_lexeme> into the return, base keeps
// the out-of-line `call operator+` (standalone symbol in BOTH rich indexes -> whole-program LTCG
// inline of the shared mixing header). Non-steerable from this file. Same wall as the siblings.
animation::mixing::expression pistol_weapon_core_aimed_idle_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	ASSERT_U( weight_driving_animation );

	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );

	return animation::mixing::expression( lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme );

	// STRUCTURE DIFF[target 0x79b9d0 | base 0x44e1c0]: target 3 / base 5 stmts
	// 0x011 <0x36> | 0x011 <0x38> | ASSERT_U( weight_driving_animation );   SIZE
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// 0x066 <0x59> | 0x068 <0x38> | return animation::mixing::expression( lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme );   SIZE
	// ; aligned 1, size-diffs 2, quantity-diffs 2
	// VERDICT: STRUCTURE MATCH (shape ok) - SIZE diffs are operator+ inline-vs-call (whole-program LTCG), non-steerable. trail: weapon_and_hands_expression.md
}

// STATE[99.92%|DONE]: structure matches (captions split per-element to align the target's two
// L43/L44 stores). Sole byte residual: `m_weapon.ammo_in_magazine()` loads `this` into eax
// (target, LTCG callee takes it in eax) vs ecx (base) at offset 0x1a - the permitted call-boundary
// arg-passing class. trail: get_weapon_lexeme_pair.md
weapon_lexeme_pair pistol_weapon_core_aimed_idle_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	pcstr weapon_animation_captions[2] =
	{
		"pistol-aimed_idle",
		"pistol-empty_aimed_idle"
	};

	u32 animation_index = m_weapon.ammo_in_magazine( ) == 0;
	pcstr animation_identifier = weapon_animation_captions[animation_index];

	resources::managed_resource_ptr const& selected_animation =
		m_weapon_animations[is_third_view != false][user_state_id == type_crouch][animation_index];

	return get_weapon_lexeme_pair_impl(
		buffer,
		animation_identifier,
		selected_animation,
		&m_weapon,
		m_animation_playback_state,
		u32( -1 ),
		1.0f,
		animation::mixing::play_cyclically,
		animation::linear_interpolator( s_aim_transition_time )
	);

	// STRUCTURE DIFF[target 0x79b910 | base 0x44e100]: target 7 / base 10 stmts
	// .. same .. (captions now per-element; all 7 stmts aligned, size-diffs 0)
	// quantity-diffs are EMPTY-only-base collapsed blank-line gaps inside the brace-init
	// ; aligned 7, size-diffs 0, quantity-diffs 3
	// VERDICT: STRUCTURE MATCH - sole byte diff is ammo_in_magazine eax-vs-ecx arg-passing, non-steerable. trail: get_weapon_lexeme_pair.md
}

// STATE[100%|DONE]
pistol_weapon_core_aimed_idle_state* weapon_core_state_cook_template<survarium::pistol_weapon_core_aimed_idle_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	u32									animations_count
)
{
	return new ( buffer.c_ptr( ) ) pistol_weapon_core_aimed_idle_state( params->weapon, animations, animations_count );
}

} // namespace survarium
