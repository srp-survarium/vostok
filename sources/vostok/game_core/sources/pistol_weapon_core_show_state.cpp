////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/pistol_weapon_core_show_state.h>

#include <vostok/game_core/weapon_core_state_cook_template.h>

namespace survarium {

// STATE[STUB]
pistol_weapon_core_show_state::pistol_weapon_core_show_state(
	weapon_core&							weapon,
	float									animation_timescale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count,
	bool&									is_shown
) : weapon_core_show_state_base( weapon, is_shown )
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
	// <0>
	// <0x7ae49c>|0x06c|+0x023:'28'
	// <0x7ae4bf>|0x08f|+0x007:'29'
	// <0x7ae4c6>|0x096|+0x018|[1]:'30'
	// <0x7ae4de>|0x0ae|+0x018:'31'
	// <0x7ae4f6>|0x0c6|+0x018:'32'
	// <0x7ae50e>|0x0de|+0x03a:'33'
	// <0x7ae548>|0x118|+0x002:'34'
	// <0x7ae54a>|0x11a|+0x002:'35'
	// <0x7ae54c>|0x11c|+0x002:'36'
	// <0x7ae54e>|0x11e|+0x018|[2]:'37'
	// <0x7ae566>|0x136|+0x018:'38'
	// <0x7ae57e>|0x14e|+0x031:'39'
	// <0x7ae5af>|0x17f|+0x002:'40'
	// <0x7ae5b1>|0x181|+0x002:'41'
	// <0x7ae5b3>|0x183|+0x00c:'42'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_show_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression pistol_weapon_core_show_state::weapon_and_hands_expression(
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
	// <0x7ae870>|0x010|+0x01f:'47'
	// <0x7ae88f>|0x02f|+0x02a:'48'
	// <0x7ae8b9>|0x059|+0x07a:'49'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::pistol_weapon_core_show_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair pistol_weapon_core_show_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// pcstr[2] 					weapon_animation_captions
	// resources::managed_resource_ptr const& selected_animation
	// u32 							animation_index
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0>
	// <0x7ae799>|0x009|+0x007:'55'
	// <0x7ae7a0>|0x010|+0x007:'56'
	// <0>
	// <0x7ae7a7>|0x017|+0x01b:'58'
	// <0x7ae7c2>|0x032|+0x00a:'59'
	// <0x7ae7cc>|0x03c|+0x02c:'60'
	// <0x7ae7f8>|0x068|+0x00c:'61'
	// <0>
	// <1>
	// <0x7ae804>|0x074|+0x04e:'64'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_show_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression pistol_weapon_core_show_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	// LOCALS
	// pcstr[2] 					animation_captions
	// animation::mixing::animation_lexeme hands_only_lexeme
	// u32 							user_animation_index
	// ******

	// FUNCTION BODY
	// <0x7ae6b1>|0x011|+0x006:'69'
	// <0x7ae6b7>|0x017|+0x010:'70'
	// <0>
	// <0x7ae6c7>|0x027|+0x00c:'72'
	// <0x7ae6d3>|0x033|+0x014:'73'
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
	// <0x7ae6e7>|0x047|+0x079:'86'
	// <0x7ae760>|0x0c0|+0x01c:'87'
	// ******
}

// STATE[STUB]
pistol_weapon_core_show_state* weapon_core_state_cook_template<pistol_weapon_core_show_state>::new_object(
	mutable_buffer							buffer,
	weapon_state_creation_params const*		params,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
)
{
	// LOCALS
	// float 						time_scale
	// float 						weapon_anim_length
	// float 						user_anim_length
	// ******

	return NULL;

	// FUNCTION BODY
	// <0x7ae5d9>|0x009|+0x02b:'94'
	// <0x7ae604>|0x034|+0x030:'95'
	// <0x7ae634>|0x064|+0x00f:'96'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x7ae643>|0x073|+0x053:'103'
	// ******
}

} // namespace survarium
