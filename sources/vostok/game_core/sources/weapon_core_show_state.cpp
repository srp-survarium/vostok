////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_show_state.h>

namespace survarium {

// STATE[STUB]
weapon_core_show_state::weapon_core_show_state(
	weapon_core&							weapon,
	float									animation_timescale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count,
	bool&									is_shown
) : weapon_core_show_state_base		( weapon, is_shown )
{
	// LOCALS
	// u32 							animation_index
	// u32 							view<1>
	// u32 							user_state<2>
	// u32 							view<2>
	// u32 							user_state<3>
	// ******

	// SKIPPED BLOCKS
	// <0x7aeebc><2>
	// <0x7aef21><3>
	// ******

	// FUNCTION BODY[0x7aee10]: 13
	// <0x7aee7c>|0x06c|+0x023:'26'
	// <0x7aee9f>|0x08f|+0x007:'27'
	// <0x7aeea6>|0x096|+0x018|[1]:'28'
	// <0x7aeebe>|0x0ae|+0x018:'29'
	// <0x7aeed6>|0x0c6|+0x031:'30'
	// <0x7aef07>|0x0f7|+0x002:'31'
	// <0x7aef09>|0x0f9|+0x002:'32'
	// <0x7aef0b>|0x0fb|+0x018|[2]:'33'
	// <0x7aef23>|0x113|+0x018:'34'
	// <0x7aef3b>|0x12b|+0x031:'35'
	// <0x7aef6c>|0x15c|+0x002:'36'
	// <0x7aef6e>|0x15e|+0x002:'37'
	// <0x7aef70>|0x160|+0x00c:'38'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_show_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression weapon_core_show_state::weapon_and_hands_expression(
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

	// FUNCTION BODY[0x7af1e0]: 3
	// <0x7af1f0>|0x010|+0x01f:'43'
	// <0x7af20f>|0x02f|+0x02a:'44'
	// <0x7af239>|0x059|+0x07a:'45'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::weapon_core_show_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair weapon_core_show_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// resources::managed_resource_ptr const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY[0x7af140]: 7
	// <0x7af149>|0x009|+0x007:'51'
	// <0x7af150>|0x010|+0x023:'52'
	// <0x7af173>|0x033|+0x00c:'53'
	// <0>
	// <1>
	// <2>
	// <0x7af17f>|0x03f|+0x056:'57'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_show_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression weapon_core_show_state::get_user_hands_expression(
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
	// u32 							user_state_index
	// ******

	// FUNCTION BODY[0x7af060]: 21
	// <0x7af071>|0x011|+0x006:'62'
	// <0x7af077>|0x017|+0x010:'63'
	// <0>
	// <1>
	// <0x7af087>|0x027|+0x00c:'66'
	// <0>
	// <0x7af093>|0x033|+0x00e:'68'
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
	// <0x7af0a1>|0x041|+0x079:'81'
	// <0x7af11a>|0x0ba|+0x01c:'82'
	// ******
}

/*
// STATE[STUB]
// survarium::weapon_core_show_state* survarium::weapon_core_state_cook_template<survarium::weapon_core_show_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::managed_resource_ptr const*, const unsigned int)
weapon_core_show_state* weapon_core_state_cook_template<survarium::weapon_core_show_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	u32									animations_count
)
{
	// LOCALS
	// float 						time_scale
	// float 						weapon_anim_length
	// float 						user_anim_length
	// ******

	return NULL;

	// FUNCTION BODY[0x7aef90]: 10
	// <0x7aef99>|0x009|+0x02b:'89'
	// <0x7aefc4>|0x034|+0x030:'90'
	// <0x7aeff4>|0x064|+0x00f:'91'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x7af003>|0x073|+0x053:'98'
	// ******
}
*/
} // namespace survarium
