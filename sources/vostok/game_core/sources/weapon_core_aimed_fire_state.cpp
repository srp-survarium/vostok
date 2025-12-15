////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_aimed_fire_state.h>

#include <vostok/game_core/weapon_core_state_cook_template.h>

namespace survarium {

// STATE[STUB]
weapon_core_aimed_fire_state::weapon_core_aimed_fire_state(
	weapon_core&							weapon,
	float									animation_timescale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
) : weapon_core_aimed_fire_state_base( weapon, animation_timescale )
{
	// LOCALS
	// u32 							animation_index
	// u32 							view<1>
	// u32 							user_state<2>
	// u32 							view<2>
	// u32 							user_state<3>
	// ******

	// SKIPPED BLOCKS
	// <0x7a951e><2>
	// <0x7a9583><3>
	// ******

	// FUNCTION BODY
	// <0x7a94de>|0x05e|+0x023:'28'
	// <0x7a9501>|0x081|+0x007:'29'
	// <0x7a9508>|0x088|+0x018|[1]:'30'
	// <0x7a9520>|0x0a0|+0x018:'31'
	// <0x7a9538>|0x0b8|+0x031:'32'
	// <0x7a9569>|0x0e9|+0x002:'33'
	// <0x7a956b>|0x0eb|+0x002:'34'
	// <0x7a956d>|0x0ed|+0x018|[2]:'35'
	// <0x7a9585>|0x105|+0x018:'36'
	// <0x7a959d>|0x11d|+0x031:'37'
	// <0x7a95ce>|0x14e|+0x002:'38'
	// <0x7a95d0>|0x150|+0x002:'39'
	// <0x7a95d2>|0x152|+0x00c:'40'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_aimed_fire_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression weapon_core_aimed_fire_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	// LOCALS
	// animation::mixing::expression hands_expression
	// weapon_lexeme_pair 			lexeme_pair
	// ******

	// FUNCTION BODY
	// <0>
	// <0x7a98a1>|0x011|+0x01f:'46'
	// <0x7a98c0>|0x030|+0x00c:'47'
	// <0x7a98cc>|0x03c|+0x05e:'48'
	// <0x7a992a>|0x09a|+0x026:'49'
	// <0x7a9950>|0x0c0|+0x07a:'50'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::weapon_core_aimed_fire_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair weapon_core_aimed_fire_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7a97d9>|0x009|+0x007:'56'
	// <0x7a97e0>|0x010|+0x023:'57'
	// <0x7a9803>|0x033|+0x00c:'58'
	// <0>
	// <1>
	// <2>
	// <0x7a980f>|0x03f|+0x074:'62'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_aimed_fire_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
animation::mixing::expression weapon_core_aimed_fire_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id
) const
{
	// LOCALS
	// u32 							user_animation_index
	// resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const& selected_animation
	// animation::mixing::animation_lexeme hands_lexeme
	// pcstr[2] 					user_animation_captions
	// ******

	// FUNCTION BODY
	// <0x7a9671>|0x011|+0x006:'67'
	// <0x7a9677>|0x017|+0x010:'68'
	// <0>
	// <0x7a9687>|0x027|+0x00f:'70'
	// <0x7a9696>|0x036|+0x026:'71'
	// <0x7a96bc>|0x05c|+0x05c:'72'
	// <0x7a9718>|0x0b8|+0x010:'73'
	// <0>
	// <1>
	// <0x7a9728>|0x0c8|+0x00e:'76'
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
	// <0x7a9736>|0x0d6|+0x06f:'87'
	// <0x7a97a5>|0x145|+0x01c:'88'
	// ******
}

// STATE[STUB]
// survarium::weapon_core_aimed_fire_state* survarium::weapon_core_state_cook_template<survarium::weapon_core_aimed_fire_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
weapon_core_aimed_fire_state* weapon_core_state_cook_template< weapon_core_aimed_fire_state >::new_object(
	mutable_buffer							buffer,
	weapon_state_creation_params const*		params,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
)
{
	return NULL;

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
