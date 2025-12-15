////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_fire_state.h>

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_fire_state::weapon_core_fire_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
weapon_core_fire_state::weapon_core_fire_state(
	weapon_core&						weapon,
	float								animation_timescale,
	resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const*	animations,
	u32									animations_count
)
{
	// LOCALS
	// u32 							animation_index
	// u32 							view<1>
	// u32 							user_state<2>
	// u32 							view<2>
	// u32 							user_state<3>
	// ******

	// SKIPPED BLOCKS
	// <0x7a9a7e><2>
	// <0x7a9ae3><3>
	// ******

	// FUNCTION BODY
	// <0x7a9a3e>|0x05e|+0x023:'28'
	// <0x7a9a61>|0x081|+0x007:'29'
	// <0x7a9a68>|0x088|+0x018|[1]:'30'
	// <0x7a9a80>|0x0a0|+0x018:'31'
	// <0x7a9a98>|0x0b8|+0x031:'32'
	// <0x7a9ac9>|0x0e9|+0x002:'33'
	// <0x7a9acb>|0x0eb|+0x002:'34'
	// <0x7a9acd>|0x0ed|+0x018|[2]:'35'
	// <0x7a9ae5>|0x105|+0x018:'36'
	// <0x7a9afd>|0x11d|+0x031:'37'
	// <0x7a9b2e>|0x14e|+0x002:'38'
	// <0x7a9b30>|0x150|+0x002:'39'
	// <0x7a9b32>|0x152|+0x00c:'40'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_fire_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression weapon_core_fire_state::weapon_and_hands_expression(
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
	// <0x7a9e00>|0x010|+0x01f:'46'
	// <0x7a9e1f>|0x02f|+0x026:'47'
	// <0x7a9e45>|0x055|+0x07a:'48'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::weapon_core_fire_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair weapon_core_fire_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7a9d39>|0x009|+0x007:'54'
	// <0x7a9d40>|0x010|+0x023:'55'
	// <0x7a9d63>|0x033|+0x00c:'56'
	// <0>
	// <1>
	// <2>
	// <0x7a9d6f>|0x03f|+0x074:'60'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_fire_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
animation::mixing::expression weapon_core_fire_state::get_user_hands_expression(
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
	// <0x7a9bd1>|0x011|+0x006:'65'
	// <0x7a9bd7>|0x017|+0x010:'66'
	// <0>
	// <0x7a9be7>|0x027|+0x00f:'68'
	// <0x7a9bf6>|0x036|+0x026:'69'
	// <0x7a9c1c>|0x05c|+0x05c:'70'
	// <0x7a9c78>|0x0b8|+0x010:'71'
	// <0>
	// <1>
	// <0x7a9c88>|0x0c8|+0x00e:'74'
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
	// <0x7a9c96>|0x0d6|+0x06f:'85'
	// <0x7a9d05>|0x145|+0x01c:'86'
	// ******
}

// STATE[STUB]
// survarium::weapon_core_fire_state* survarium::weapon_core_state_cook_template<survarium::weapon_core_fire_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
weapon_core_fire_state* weapon_core_state_cook_template<survarium::weapon_core_fire_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const*	animations,
	u32									animations_count
)
{
	return NULL;

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
