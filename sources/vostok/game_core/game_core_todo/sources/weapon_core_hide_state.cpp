////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon_core_hide_state.h"

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_hide_state::weapon_core_hide_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int, bool&)
weapon_core_hide_state::weapon_core_hide_state(
	weapon_core&						weapon,
	float								animation_timescale,
	vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*	animations,
	u32									animations_count,
	bool&								is_shown)
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
	// <0x7ae9ac>|0x000|0x000:'32'
	// <0x7ae9b9>|0x00d|0x00d:'33'
	// <0x7ae9dc>|0x030|0x023:'34'
	// <0x7ae9e3>|0x037|0x007|[1]:'35'
	// <0x7ae9fb>|0x04f|0x018:'36'
	// <0x7aea13>|0x067|0x018:'37'
	// <0x7aea44>|0x098|0x031:'38'
	// <0x7aea46>|0x09a|0x002:'39'
	// 1
	// <0x7aea48>|0x09c|0x002|[2]:'41'
	// <0x7aea60>|0x0b4|0x018:'42'
	// <0x7aea78>|0x0cc|0x018:'43'
	// <0x7aeaa9>|0x0fd|0x031:'44'
	// <0x7aeaab>|0x0ff|0x002:'45'
	// <0x7aeaad>|0x101|0x002:'46'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_hide_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression weapon_core_hide_state::weapon_and_hands_expression(
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
	// <0x7aed40>|0x000|0x000:'51'
	// <0x7aed5f>|0x01f|0x01f:'52'
	// <0x7aed89>|0x049|0x02a:'53'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::weapon_core_hide_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair weapon_core_hide_state::get_weapon_lexeme_pair( vostok::mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7aec99>|0x000|0x000:'58'
	// <0x7aeca0>|0x007|0x007:'59'
	// <0x7aecc3>|0x02a|0x023:'60'
	// 1
	// 2
	// 3
	// <0x7aeccf>|0x036|0x00c:'64'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_hide_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression weapon_core_hide_state::get_user_hands_expression(
	vostok::animation::mixing::animation_lexeme&	weapon_lexeme,
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	vostok::animation::mixing::animation_lexeme&	weight_driving_animation) const
{
	// LOCALS
	// vostok::animation::mixing::animation_lexeme override_lexeme
	// pcstr[2] 					animation_captions
	// u32 							user_state_index
	// ******

	// FUNCTION BODY
	// <0x7aebb1>|0x000|0x000:'69'
	// <0x7aebb7>|0x006|0x006:'70'
	// 1
	// 2
	// <0x7aebc7>|0x016|0x010:'73'
	// 1
	// <0x7aebd9>|0x028|0x012:'75'
	// 1
	// <0x7aebe5>|0x034|0x00c:'77'
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
	// <0x7aebf3>|0x042|0x00e:'90'
	// <0x7aec6c>|0x0bb|0x079:'91'
	// ******
}

// STATE[STUB]
// survarium::weapon_core_hide_state* survarium::weapon_core_state_cook_template<survarium::weapon_core_hide_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
weapon_core_hide_state* weapon_core_state_cook_template<weapon_core_hide_state>::new_object(
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
	// <0x7aead9>|0x000|0x000:'98'
	// <0x7aeb04>|0x02b|0x02b:'99'
	// <0x7aeb34>|0x05b|0x030:'100'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// <0x7aeb43>|0x06a|0x00f:'108'
	// ******
}

} // namespace survarium
