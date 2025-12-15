////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/pistol_weapon_core_hide_state.h>

namespace survarium {

// STATE[STUB]
// survarium::pistol_weapon_core_hide_state::pistol_weapon_core_hide_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int, bool&)
pistol_weapon_core_hide_state::pistol_weapon_core_hide_state(
	weapon_core&						weapon,
	float								animation_timescale,
	resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const*	animations,
	u32									animations_count,
	bool&								is_shown
)
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
	// <0x7adf7c>|0x06c|+0x023:'27'
	// <0x7adf9f>|0x08f|+0x007:'28'
	// <0x7adfa6>|0x096|+0x018|[1]:'29'
	// <0x7adfbe>|0x0ae|+0x018:'30'
	// <0x7adfd6>|0x0c6|+0x018:'31'
	// <0x7adfee>|0x0de|+0x03a:'32'
	// <0x7ae028>|0x118|+0x002:'33'
	// <0x7ae02a>|0x11a|+0x002:'34'
	// <0x7ae02c>|0x11c|+0x002:'35'
	// <0x7ae02e>|0x11e|+0x018|[2]:'36'
	// <0x7ae046>|0x136|+0x018:'37'
	// <0x7ae05e>|0x14e|+0x031:'38'
	// <0x7ae08f>|0x17f|+0x002:'39'
	// <0x7ae091>|0x181|+0x002:'40'
	// <0x7ae093>|0x183|+0x00c:'41'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_hide_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression pistol_weapon_core_hide_state::weapon_and_hands_expression(
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
	// <0x7ae360>|0x010|+0x01f:'46'
	// <0x7ae37f>|0x02f|+0x02a:'47'
	// <0x7ae3a9>|0x059|+0x07a:'48'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::pistol_weapon_core_hide_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair pistol_weapon_core_hide_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// pcstr[2] 					weapon_animation_captions
	// resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const& selected_animation
	// u32 							animation_index
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0>
	// <0x7ae289>|0x009|+0x007:'54'
	// <0x7ae290>|0x010|+0x007:'55'
	// <0>
	// <0x7ae297>|0x017|+0x01b:'57'
	// <0x7ae2b2>|0x032|+0x00a:'58'
	// <0x7ae2bc>|0x03c|+0x02c:'59'
	// <0x7ae2e8>|0x068|+0x00c:'60'
	// <0>
	// <1>
	// <0x7ae2f4>|0x074|+0x056:'63'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_hide_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression pistol_weapon_core_hide_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	// LOCALS
	// animation::mixing::animation_lexeme override_lexeme
	// pcstr[2] 					animation_captions
	// animation::linear_interpolator interpolator
	// u32 							user_state_index
	// ******

	// FUNCTION BODY
	// <0x7ae191>|0x011|+0x006:'68'
	// <0x7ae197>|0x017|+0x010:'69'
	// <0>
	// <0x7ae1a7>|0x027|+0x00c:'71'
	// <0x7ae1b3>|0x033|+0x00e:'72'
	// <0>
	// <0x7ae1c1>|0x041|+0x010:'74'
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
	// <0x7ae1d1>|0x051|+0x079:'86'
	// <0x7ae24a>|0x0ca|+0x021:'87'
	// ******
}

// STATE[STUB]
// survarium::pistol_weapon_core_hide_state* survarium::weapon_core_state_cook_template<survarium::pistol_weapon_core_hide_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
pistol_weapon_core_hide_state* weapon_core_state_cook_template<survarium::pistol_weapon_core_hide_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const*	animations,
	u32									animations_count
)
{
	// LOCALS
	// float 						time_scale
	// float 						weapon_anim_length
	// float 						user_anim_length
	// ******

	return NULL;

	// FUNCTION BODY
	// <0x7ae0b9>|0x009|+0x02b:'94'
	// <0x7ae0e4>|0x034|+0x030:'95'
	// <0x7ae114>|0x064|+0x00f:'96'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x7ae123>|0x073|+0x053:'103'
	// ******
}

} // namespace survarium
