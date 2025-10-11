////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "pistol_weapon_core_show_state.h"

namespace survarium {

// STATE[STUB]
// survarium::pistol_weapon_core_show_state::pistol_weapon_core_show_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int, bool&)
pistol_weapon_core_show_state::pistol_weapon_core_show_state(
	weapon_core&						weapon,
	float								animation_timescale,
	vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*	animations,
	u32									animations_count,
	bool&								is_shown)
{
	// LOCALS
	// u32 							animation_index
	// u32 							view_index<1>
	// u32 							user_state_index<2>
	// u32 							weapon_state_index<3>
	// u32 							view_index<2>
	// u32 							user_state_index<3>
	// ******

	// SKIPPED BLOCKS
	// <0x7ae4dc><2>
	// <0x7ae4f4><3>
	// <0x7ae564><3>
	// ******

	// FUNCTION BODY
	// 1
	// <0x7ae49c>|0x000|0x000:'28'
	// <0x7ae4bf>|0x023|0x023:'29'
	// <0x7ae4c6>|0x02a|0x007|[1]:'30'
	// <0x7ae4de>|0x042|0x018:'31'
	// <0x7ae4f6>|0x05a|0x018:'32'
	// <0x7ae50e>|0x072|0x018:'33'
	// <0x7ae548>|0x0ac|0x03a:'34'
	// <0x7ae54a>|0x0ae|0x002:'35'
	// <0x7ae54c>|0x0b0|0x002:'36'
	// <0x7ae54e>|0x0b2|0x002|[2]:'37'
	// <0x7ae566>|0x0ca|0x018:'38'
	// <0x7ae57e>|0x0e2|0x018:'39'
	// <0x7ae5af>|0x113|0x031:'40'
	// <0x7ae5b1>|0x115|0x002:'41'
	// <0x7ae5b3>|0x117|0x002:'42'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_show_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression pistol_weapon_core_show_state::weapon_and_hands_expression(
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
	// <0x7ae870>|0x000|0x000:'47'
	// <0x7ae88f>|0x01f|0x01f:'48'
	// <0x7ae8b9>|0x049|0x02a:'49'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::pistol_weapon_core_show_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair pistol_weapon_core_show_state::get_weapon_lexeme_pair( vostok::mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// pcstr[2] 					weapon_animation_captions
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// u32 							animation_index
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// 1
	// <0x7ae799>|0x000|0x000:'55'
	// <0x7ae7a0>|0x007|0x007:'56'
	// 1
	// <0x7ae7a7>|0x00e|0x007:'58'
	// <0x7ae7c2>|0x029|0x01b:'59'
	// <0x7ae7cc>|0x033|0x00a:'60'
	// <0x7ae7f8>|0x05f|0x02c:'61'
	// 1
	// 2
	// <0x7ae804>|0x06b|0x00c:'64'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_show_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression pistol_weapon_core_show_state::get_user_hands_expression(
	vostok::animation::mixing::animation_lexeme&	weapon_lexeme,
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	vostok::animation::mixing::animation_lexeme&	weight_driving_animation) const
{
	// LOCALS
	// pcstr[2] 					animation_captions
	// vostok::animation::mixing::animation_lexeme hands_only_lexeme
	// u32 							user_animation_index
	// ******

	// FUNCTION BODY
	// <0x7ae6b1>|0x000|0x000:'69'
	// <0x7ae6b7>|0x006|0x006:'70'
	// 1
	// <0x7ae6c7>|0x016|0x010:'72'
	// <0x7ae6d3>|0x022|0x00c:'73'
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
	// 11
	// 12
	// <0x7ae6e7>|0x036|0x014:'86'
	// <0x7ae760>|0x0af|0x079:'87'
	// ******
}

// STATE[STUB]
// survarium::pistol_weapon_core_show_state* survarium::weapon_core_state_cook_template<survarium::pistol_weapon_core_show_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
pistol_weapon_core_show_state* weapon_core_state_cook_template<pistol_weapon_core_show_state>::new_object(
	vostok::mutable_buffer				buffer,
	weapon_state_creation_params const*	params,
	vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*	animations,
	u32									animations_count)
{
	// LOCALS
	// float 						time_scale
	// float 						weapon_anim_length
	// float 						user_anim_length
	// ******

	return NULL;
	// FUNCTION BODY
	// <0x7ae5d9>|0x000|0x000:'94'
	// <0x7ae604>|0x02b|0x02b:'95'
	// <0x7ae634>|0x05b|0x030:'96'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x7ae643>|0x06a|0x00f:'103'
	// ******
}

} // namespace survarium
