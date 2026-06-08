////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/pistol_weapon_core_idle_state.h>

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
pistol_weapon_core_idle_state::pistol_weapon_core_idle_state( weapon_core& weapon, resources::managed_resource_ptr const* animations, u32 animations_count ) :
	weapon_core_idle_state_base( weapon )
{
	ASSERT_CMP_U( animations_count, ==, 8 );

	u32 animation_index = 0;
	for ( u32 view = 0 ; view != 2 ; ++view )
		for ( u32 user_state = 0 ; user_state != 2 ; ++user_state )
			for ( u32 weapon_state = 0 ; weapon_state != 2 ; ++weapon_state )
				m_weapon_animations[view][user_state][weapon_state] = animations[animation_index++];

	ASSERT( UNKNOWN_EXPRESSION );
}

// STATE[85.65%|PARTIAL]: residual is the per-call-site inline-vs-call of
// operator+<animation_lexeme,animation_lexeme> (side verified vs --view diff: the TARGET keeps
// the out-of-line `call operator+`, OUR BASE inlines it here; operator+ is standalone in BOTH
// rich indexes -> source-steerable (force the out-of-line call), NOT non-steerable LTCG, not yet
// resolved), plus the line-35 ASSERT eater shape (target's
// expression_eater gets only the lexeme; ASSERT_U adds the assert_untyped `push 0`).
// Identical shape/diff to weapon_core_idle_state::weapon_and_hands_expression (#151).
animation::mixing::expression pistol_weapon_core_idle_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	ASSERT_U( weight_driving_animation );

	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( buffer, is_third_view, user_state_id );

	return animation::mixing::expression( lexeme_pair.main_lexeme + lexeme_pair.offset_lexeme );

	// FUNCTION BODY
	// <0x7abce1>|0x011|+0x036:'35'	ASSERT_U( weight_driving_animation );
	// <0x7abd17>|0x047|+0x01f:'36'	weapon_lexeme_pair lexeme_pair = get_weapon_lexeme_pair( ... );
	// <0x7abd36>|0x066|+0x059:'37'	return expression( main + offset );  (target inlines operator+)
	// ******
}

// STATE[99.92%|DONE]: sole residual is the `m_weapon.ammo_in_magazine()` call boundary - the
// callee is LTCG-optimized to take `this` in eax (target `weapon_core::ammo_in_magazine` @0x9b270
// is `mov ax,[eax+47Ah]; ret`), so the caller loads m_weapon into eax (target) vs ecx (base): a
// link-time custom calling convention, the permitted arg-passing class. Plus the
// `s_aim_transition_time` reloc and the `playback_enum`/`playing_type_enum` typedef-alias in the
// get_weapon_lexeme_pair_impl callee mangling; those two are byte-identical to the reference
// weapon_core_idle_state::get_weapon_lexeme_pair (#151), which scores 100% lacking only this nit.
weapon_lexeme_pair pistol_weapon_core_idle_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	pcstr weapon_animation_captions[2] = { "pistol-idle", "pistol-idle_empty" };

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

	// FUNCTION BODY
	// <0x79bc10>|0x000|+0x009:'41'	pcstr weapon_animation_captions[2] = { "pistol-idle", "pistol-idle_empty" };
	// <0x79bc19>|0x009|+0x007:'43'	weapon_animation_captions[0] = "pistol-idle";
	// <0x79bc20>|0x010|+0x007:'44'	weapon_animation_captions[1] = "pistol-idle_empty";
	// <0x79bc27>|0x017|+0x01b:'46'	u32 animation_index = m_weapon.ammo_in_magazine( ) == 0;
	// <0x79bc42>|0x032|+0x00a:'47'	pcstr animation_identifier = weapon_animation_captions[animation_index];
	// <0x79bc4c>|0x03c|+0x02c:'48'	selected_animation = m_weapon_animations[...];
	// <0x79bc78>|0x068|+0x04f:'49'	return get_weapon_lexeme_pair_impl( ... );
	// ******
}

// STATE[100%|DONE]
pistol_weapon_core_idle_state* weapon_core_state_cook_template<survarium::pistol_weapon_core_idle_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	u32									animations_count
)
{
	return new ( buffer.c_ptr( ) ) pistol_weapon_core_idle_state( params->weapon, animations, animations_count );
}

} // namespace survarium
