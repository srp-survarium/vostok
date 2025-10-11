////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "pistol_weapon_core_hide_state.h"

namespace survarium {

// STATE[STUB]
// survarium::pistol_weapon_core_hide_state::pistol_weapon_core_hide_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int, bool&)
pistol_weapon_core_hide_state::pistol_weapon_core_hide_state(
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
	// <0x7adfbc><2>
	// <0x7adfd4><3>
	// <0x7ae044><3>
	// ******

	// FUNCTION BODY
	// <0x7adf7c>|0x000|0x000:'27'
	// <0x7adf9f>|0x023|0x023:'28'
	// <0x7adfa6>|0x02a|0x007|[1]:'29'
	// <0x7adfbe>|0x042|0x018:'30'
	// <0x7adfd6>|0x05a|0x018:'31'
	// <0x7adfee>|0x072|0x018:'32'
	// <0x7ae028>|0x0ac|0x03a:'33'
	// <0x7ae02a>|0x0ae|0x002:'34'
	// <0x7ae02c>|0x0b0|0x002:'35'
	// <0x7ae02e>|0x0b2|0x002|[2]:'36'
	// <0x7ae046>|0x0ca|0x018:'37'
	// <0x7ae05e>|0x0e2|0x018:'38'
	// <0x7ae08f>|0x113|0x031:'39'
	// <0x7ae091>|0x115|0x002:'40'
	// <0x7ae093>|0x117|0x002:'41'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_hide_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression pistol_weapon_core_hide_state::weapon_and_hands_expression(
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
	// <0x7ae360>|0x000|0x000:'46'
	// <0x7ae37f>|0x01f|0x01f:'47'
	// <0x7ae3a9>|0x049|0x02a:'48'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::pistol_weapon_core_hide_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair pistol_weapon_core_hide_state::get_weapon_lexeme_pair( vostok::mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// pcstr[2] 					weapon_animation_captions
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// u32 							animation_index
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// 1
	// <0x7ae289>|0x000|0x000:'54'
	// <0x7ae290>|0x007|0x007:'55'
	// 1
	// <0x7ae297>|0x00e|0x007:'57'
	// <0x7ae2b2>|0x029|0x01b:'58'
	// <0x7ae2bc>|0x033|0x00a:'59'
	// <0x7ae2e8>|0x05f|0x02c:'60'
	// 1
	// 2
	// <0x7ae2f4>|0x06b|0x00c:'63'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_hide_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression pistol_weapon_core_hide_state::get_user_hands_expression(
	vostok::animation::mixing::animation_lexeme&	weapon_lexeme,
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	vostok::animation::mixing::animation_lexeme&	weight_driving_animation) const
{
	// LOCALS
	// vostok::animation::mixing::animation_lexeme override_lexeme
	// pcstr[2] 					animation_captions
	// vostok::animation::linear_interpolator interpolator
	// u32 							user_state_index
	// ******

	// FUNCTION BODY
	// <0x7ae191>|0x000|0x000:'68'
	// <0x7ae197>|0x006|0x006:'69'
	// 1
	// <0x7ae1a7>|0x016|0x010:'71'
	// <0x7ae1b3>|0x022|0x00c:'72'
	// 1
	// <0x7ae1c1>|0x030|0x00e:'74'
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
	// <0x7ae1d1>|0x040|0x010:'86'
	// <0x7ae24a>|0x0b9|0x079:'87'
	// ******
}

// STATE[STUB]
// survarium::pistol_weapon_core_hide_state* survarium::weapon_core_state_cook_template<survarium::pistol_weapon_core_hide_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
pistol_weapon_core_hide_state* weapon_core_state_cook_template<pistol_weapon_core_hide_state>::new_object(
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
	// <0x7ae0b9>|0x000|0x000:'94'
	// <0x7ae0e4>|0x02b|0x02b:'95'
	// <0x7ae114>|0x05b|0x030:'96'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x7ae123>|0x06a|0x00f:'103'
	// ******
}

} // namespace survarium
