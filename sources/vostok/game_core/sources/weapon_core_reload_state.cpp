////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_reload_state.h>

#include <vostok/game_core/weapon_core_state_cook_template.h>

namespace survarium {

// STATE[STUB]
weapon_core_reload_state::weapon_core_reload_state(
	weapon_core&							weapon,
	float									animation_time_scale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
) : weapon_core_reload_state_base( weapon, animation_time_scale )
{
	// LOCALS
	// u32 							animation_index
	// u32 							view<1>
	// u32 							user_state<2>
	// u32 							view<2>
	// u32 							user_state<3>
	// ******

	// SKIPPED BLOCKS
	// <0x7aa80e><2>
	// <0x7aa873><3>
	// ******

	// FUNCTION BODY
	// <0x7aa7ce>|0x05e|+0x023:'27'
	// <0x7aa7f1>|0x081|+0x007:'28'
	// <0x7aa7f8>|0x088|+0x018|[1]:'29'
	// <0x7aa810>|0x0a0|+0x018:'30'
	// <0x7aa828>|0x0b8|+0x031:'31'
	// <0x7aa859>|0x0e9|+0x002:'32'
	// <0x7aa85b>|0x0eb|+0x002:'33'
	// <0x7aa85d>|0x0ed|+0x018|[2]:'34'
	// <0x7aa875>|0x105|+0x018:'35'
	// <0x7aa88d>|0x11d|+0x031:'36'
	// <0x7aa8be>|0x14e|+0x002:'37'
	// <0x7aa8c0>|0x150|+0x002:'38'
	// <0x7aa8c2>|0x152|+0x00c:'39'
	// ******
}

// STATE[STUB]
animation::mixing::expression weapon_core_reload_state::weapon_and_hands_expression(
	mutable_buffer&							buffer,
	bool									is_third_view,
	weapon_user_state_enum					user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	// LOCALS
	// animation::mixing::expression hands_expression
	// weapon_lexeme_pair 			lexeme_pair
	// ******

	// FUNCTION BODY
	// <0x7aaaf0>|0x010|+0x01f:'44'
	// <0x7aab0f>|0x02f|+0x02a:'45'
	// <0x7aab39>|0x059|+0x07a:'46'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::weapon_core_reload_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair weapon_core_reload_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// resources::managed_resource_ptr const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7aaa49>|0x009|+0x007:'51'
	// <0x7aaa50>|0x010|+0x023:'52'
	// <0x7aaa73>|0x033|+0x00c:'53'
	// <0>
	// <1>
	// <2>
	// <0x7aaa7f>|0x03f|+0x056:'57'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_reload_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression weapon_core_reload_state::get_user_hands_expression(
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

	// FUNCTION BODY
	// <0x7aa961>|0x011|+0x006:'62'
	// <0x7aa967>|0x017|+0x010:'63'
	// <0>
	// <0x7aa977>|0x027|+0x012:'65'
	// <0>
	// <0x7aa989>|0x039|+0x00c:'67'
	// <0>
	// <0x7aa995>|0x045|+0x00e:'69'
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
	// <0x7aa9a3>|0x053|+0x079:'82'
	// <0x7aaa1c>|0x0cc|+0x01c:'83'
	// ******
}

// STATE[STUB]
weapon_core_reload_state* weapon_core_state_cook_template<weapon_core_reload_state>::new_object(
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
	// <0x7aa8e9>|0x009|+0x05c:'95'
	// ******
}

} // namespace survarium
