////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_hide_state.h>

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_hide_state::weapon_core_hide_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int, bool&)
weapon_core_hide_state::weapon_core_hide_state(
	weapon_core&						weapon,
	float								animation_timescale,
	resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const*	animations,
	u32									animations_count,
	bool&								is_shown
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
	// <0x7ae9f9><2>
	// <0x7aea5e><3>
	// ******

	// FUNCTION BODY
	// <0x7ae9ac>|0x06c|+0x00d:'32'
	// <0x7ae9b9>|0x079|+0x023:'33'
	// <0x7ae9dc>|0x09c|+0x007:'34'
	// <0x7ae9e3>|0x0a3|+0x018|[1]:'35'
	// <0x7ae9fb>|0x0bb|+0x018:'36'
	// <0x7aea13>|0x0d3|+0x031:'37'
	// <0x7aea44>|0x104|+0x002:'38'
	// <0x7aea46>|0x106|+0x002:'39'
	// <0>
	// <0x7aea48>|0x108|+0x018|[2]:'41'
	// <0x7aea60>|0x120|+0x018:'42'
	// <0x7aea78>|0x138|+0x031:'43'
	// <0x7aeaa9>|0x169|+0x002:'44'
	// <0x7aeaab>|0x16b|+0x002:'45'
	// <0x7aeaad>|0x16d|+0x00c:'46'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_hide_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression weapon_core_hide_state::weapon_and_hands_expression(
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
	// <0x7aed40>|0x010|+0x01f:'51'
	// <0x7aed5f>|0x02f|+0x02a:'52'
	// <0x7aed89>|0x059|+0x07a:'53'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::weapon_core_hide_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair weapon_core_hide_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7aec99>|0x009|+0x007:'58'
	// <0x7aeca0>|0x010|+0x023:'59'
	// <0x7aecc3>|0x033|+0x00c:'60'
	// <0>
	// <1>
	// <2>
	// <0x7aeccf>|0x03f|+0x056:'64'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_hide_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression weapon_core_hide_state::get_user_hands_expression(
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
	// <0x7aebb1>|0x011|+0x006:'69'
	// <0x7aebb7>|0x017|+0x010:'70'
	// <0>
	// <1>
	// <0x7aebc7>|0x027|+0x012:'73'
	// <0>
	// <0x7aebd9>|0x039|+0x00c:'75'
	// <0>
	// <0x7aebe5>|0x045|+0x00e:'77'
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
	// <0x7aebf3>|0x053|+0x079:'90'
	// <0x7aec6c>|0x0cc|+0x01c:'91'
	// ******
}

// STATE[STUB]
// survarium::weapon_core_hide_state* survarium::weapon_core_state_cook_template<survarium::weapon_core_hide_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
weapon_core_hide_state* weapon_core_state_cook_template<survarium::weapon_core_hide_state>::new_object(
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
	// <0x7aead9>|0x009|+0x02b:'98'
	// <0x7aeb04>|0x034|+0x030:'99'
	// <0x7aeb34>|0x064|+0x00f:'100'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x7aeb43>|0x073|+0x053:'108'
	// ******
}

} // namespace survarium
