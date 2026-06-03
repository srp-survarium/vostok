////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/pistol_weapon_core_aimed_fire_state.h>

#include <vostok/game_core/weapon_core.h>
#include <vostok/game_core/weapon_state_creation_params.h>
#include <vostok/game_core/weapon_core_state_cook_template.h>

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

// computed_shooting_animation_time_scale is defined (STUB) in the timescale inline header;
// new_object below is the only current caller, so pull in the definition here to resolve it.
float computed_shooting_animation_time_scale( resources::managed_resource_ptr const& shooting_animation, float rounds_per_second );
}
#include <vostok/game_core/weapon_animations_timescale_inline.h>
namespace survarium {

// STATE[100%|DONE]: byte-identical to target @0x79abc0; pairs in report.json at 100% (ctor set
// protected in the .h to match the @@IAE mangled access char).
pistol_weapon_core_aimed_fire_state::pistol_weapon_core_aimed_fire_state(
	weapon_core&							weapon,
	float									animation_time_scale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
) : weapon_core_aimed_fire_state_base( weapon, animation_time_scale ),
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

// STATE[99.76%|PARTIAL]: objdiff-scored 99.76% (target @0x79b0e0; pairs in report.json now that
// initialize is protected-virtual to match @@MAE). The `last_shot ? 1u : 0u` ternary reproduced
// the target's final-store boolize (neg;sbb;neg into m_weapon_animation_index) byte-for-byte.
// Sole residual is the LTCG this-register at the ammo_in_magazine() call boundary: target loads
// the m_weapon pointer into eax (`mov eax,[ecx+128h]` @0x30/0x50) before the call, base loads ecx -
// argument passing at the call boundary, not source-steerable until ammo_in_magazine is matched.
void pistol_weapon_core_aimed_fire_state::initialize( )
{
	weapon_core_aimed_fire_state_base::initialize( );

	bool last_shot = m_weapon.get_bullets_in_queue( )
		? ( m_weapon.ammo_in_magazine( ) == 1 )
		: ( m_weapon.ammo_in_magazine( ) == 0 );

	m_weapon_animation_index = last_shot ? 1u : 0u;

	// FUNCTION BODY (kept: PARTIAL - LTCG this-register at ammo_in_magazine() call boundary)
	// <0x7ab0e9>|0x009 weapon_core_aimed_fire_state_base::initialize();
	// <0x7ab0f1>|0x011 last_shot = get_bullets_in_queue() ? (ammo==1) : (ammo==0);
	//                  @0x30/0x50 target `mov eax,[ecx+128h]` vs base `mov ecx,...` before ammo_in_magazine
	// <0x7ab14e>|0x06e m_weapon_animation_index = last_shot ? 1u : 0u;  (boolize matches both sides)
	// ******
}

// STATE[INPROGRESS]: large addition_lexeme/operator+ machinery. Next: model on
// pistol_weapon_core_fire_state::weapon_and_hands_expression (sibling, also INPROGRESS) but with
// the user_state==type_sprint branch that adds get_user_hands_expression; see target asm @0x79b000.
// vostok::animation::mixing::expression survarium::pistol_weapon_core_aimed_fire_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression pistol_weapon_core_aimed_fire_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	return animation::mixing::expression( weight_driving_animation );

	// LOCALS
	// animation::mixing::expression hands_expression
	// weapon_lexeme_pair 			lexeme_pair
	// ******

	// FUNCTION BODY
	// <0x7ab010>|0x010|+0x01f:'52'
	// <0x7ab02f>|0x02f|+0x02a:'53'
	// <0x7ab059>|0x059|+0x07a:'54'
	// ******
}

// STATE[100%|DONE]: byte-identical to target @0x79af30; pairs in report.json at 100% (declared
// private const in the .h to match the @@ABE mangled access char). Captions "pistol-aimed_shot" /
// "pistol-aimed_last_shot".
weapon_lexeme_pair pistol_weapon_core_aimed_fire_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	pcstr weapon_animation_captions[2] = { "pistol-aimed_shot", "pistol-aimed_last_shot" };

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

// STATE[INPROGRESS]: large lexeme machinery (override animation + operator+). Next: model on
// the weapon_core_fire_state::get_user_hands_expression sibling shape; see target asm @0x79add0.
// vostok::animation::mixing::expression survarium::pistol_weapon_core_aimed_fire_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression pistol_weapon_core_aimed_fire_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	return animation::mixing::expression( weapon_lexeme );

	// LOCALS
	// animation::mixing::animation_lexeme override_lexeme
	// u32 							user_animation_index
	// resources::managed_resource_ptr const& selected_animation
	// pcstr[2] 					user_animation_captions
	// ******

	// FUNCTION BODY
	// <0x7aade1>|0x011|+0x006:'75'
	// <0x7aade7>|0x017|+0x010:'76'
	// <0>
	// <0x7aadf7>|0x027|+0x00c:'78'
	// <0x7aae03>|0x033|+0x020:'79'
	// <0x7aae23>|0x053|+0x059:'80'
	// <0x7aae7c>|0x0ac|+0x010:'81'
	// <0>
	// <1>
	// <0x7aae8c>|0x0bc|+0x00e:'84'
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
	// <0x7aae9a>|0x0ca|+0x06e:'96'
	// <0x7aaf08>|0x138|+0x01c:'97'
	// ******
}

// STATE[92.08%|PARTIAL]: objdiff-scored 92.08% (control flow + placement-new + ctor call all
// match). Sole residual is the LTCG calling convention of computed_shooting_animation_time_scale:
// the target passes its `managed_resource_ptr const&` arg in a register and returns the float in
// xmm0 (movss), while our STUB callee keeps default cdecl (stack push + st0/fstp). Argument
// passing at the call boundary - not source-steerable until that callee is matched. target @0x79ad60.
pistol_weapon_core_aimed_fire_state* weapon_core_state_cook_template<survarium::pistol_weapon_core_aimed_fire_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	u32									animations_count
)
{
	return new ( buffer.c_ptr( ) ) pistol_weapon_core_aimed_fire_state(
		params->weapon,
		computed_shooting_animation_time_scale( *animations, params->rounds_per_second ),
		animations,
		animations_count
	);

	// FUNCTION BODY (kept: PARTIAL - LTCG calling convention of computed_shooting_animation_time_scale)
	// <0x7aad69>|0x009 new(buffer.c_ptr()) pistol_weapon_core_aimed_fire_state( params->weapon,
	//                  computed_shooting_animation_time_scale(*animations, params->rounds_per_second),
	//                  animations, animations_count );
	// TARGET @0x3d: call computed... (ref in reg, ret xmm0 -> movss [esp])
	// BASE   @0x3d: push ref; call computed...; add esp,4; fstp [esp]  (cdecl, st0)
	// ******
}

} // namespace survarium
