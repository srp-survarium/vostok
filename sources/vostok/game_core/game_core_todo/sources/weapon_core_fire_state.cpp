////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon_core_fire_state.h"

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_fire_state::weapon_core_fire_state(survarium::weapon_core&, float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
weapon_core_fire_state::weapon_core_fire_state(
	weapon_core&						weapon,
	float								animation_timescale,
	vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*	animations,
	u32									animations_count)
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
	// <0x7a9a3e>|0x000|0x000:'28'
	// <0x7a9a61>|0x023|0x023:'29'
	// <0x7a9a68>|0x02a|0x007|[1]:'30'
	// <0x7a9a80>|0x042|0x018:'31'
	// <0x7a9a98>|0x05a|0x018:'32'
	// <0x7a9ac9>|0x08b|0x031:'33'
	// <0x7a9acb>|0x08d|0x002:'34'
	// <0x7a9acd>|0x08f|0x002|[2]:'35'
	// <0x7a9ae5>|0x0a7|0x018:'36'
	// <0x7a9afd>|0x0bf|0x018:'37'
	// <0x7a9b2e>|0x0f0|0x031:'38'
	// <0x7a9b30>|0x0f2|0x002:'39'
	// <0x7a9b32>|0x0f4|0x002:'40'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_fire_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression weapon_core_fire_state::weapon_and_hands_expression(
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	vostok::animation::mixing::animation_lexeme&	weight_driving_animation) const
{
	// LOCALS
	// vostok::animation::mixing::expression hands_expression
	// weapon_lexeme_pair 			lexeme_pair
	// ******

	// FUNCTION BODY
	// 1
	// <0x7a9e00>|0x000|0x000:'46'
	// <0x7a9e1f>|0x01f|0x01f:'47'
	// <0x7a9e45>|0x045|0x026:'48'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::weapon_core_fire_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair weapon_core_fire_state::get_weapon_lexeme_pair( vostok::mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7a9d39>|0x000|0x000:'54'
	// <0x7a9d40>|0x007|0x007:'55'
	// <0x7a9d63>|0x02a|0x023:'56'
	// 1
	// 2
	// 3
	// <0x7a9d6f>|0x036|0x00c:'60'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_fire_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
vostok::animation::mixing::expression weapon_core_fire_state::get_user_hands_expression(
	vostok::animation::mixing::animation_lexeme&	weapon_lexeme,
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id) const
{
	// LOCALS
	// u32 							user_animation_index
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// vostok::animation::mixing::animation_lexeme hands_lexeme
	// pcstr[2] 					user_animation_captions
	// ******

	// FUNCTION BODY
	// <0x7a9bd1>|0x000|0x000:'65'
	// <0x7a9bd7>|0x006|0x006:'66'
	// 1
	// <0x7a9be7>|0x016|0x010:'68'
	// <0x7a9bf6>|0x025|0x00f:'69'
	// <0x7a9c1c>|0x04b|0x026:'70'
	// <0x7a9c78>|0x0a7|0x05c:'71'
	// 1
	// 2
	// <0x7a9c88>|0x0b7|0x010:'74'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// <0x7a9c96>|0x0c5|0x00e:'85'
	// <0x7a9d05>|0x134|0x06f:'86'
	// ******
}

// STATE[STUB]
// survarium::weapon_core_fire_state* survarium::weapon_core_state_cook_template<survarium::weapon_core_fire_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
weapon_core_fire_state* weapon_core_state_cook_template<weapon_core_fire_state>::new_object(
	vostok::mutable_buffer				buffer,
	weapon_state_creation_params const*	params,
	vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*	animations,
	u32									animations_count)
{
	return NULL;
	// FUNCTION BODY
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x7a9b59>|0x000|0x000:'99'
	// ******
}

} // namespace survarium
