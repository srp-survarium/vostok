////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon_core_show_state.h"

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_show_state::weapon_core_show_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int, bool&)
weapon_core_show_state::weapon_core_show_state(
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
	// <0x7aeebc><2>
	// <0x7aef21><3>
	// ******

	// FUNCTION BODY
	// <0x7aee7c>|0x000|0x000:'26'
	// <0x7aee9f>|0x023|0x023:'27'
	// <0x7aeea6>|0x02a|0x007|[1]:'28'
	// <0x7aeebe>|0x042|0x018:'29'
	// <0x7aeed6>|0x05a|0x018:'30'
	// <0x7aef07>|0x08b|0x031:'31'
	// <0x7aef09>|0x08d|0x002:'32'
	// <0x7aef0b>|0x08f|0x002|[2]:'33'
	// <0x7aef23>|0x0a7|0x018:'34'
	// <0x7aef3b>|0x0bf|0x018:'35'
	// <0x7aef6c>|0x0f0|0x031:'36'
	// <0x7aef6e>|0x0f2|0x002:'37'
	// <0x7aef70>|0x0f4|0x002:'38'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_show_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression weapon_core_show_state::weapon_and_hands_expression(
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
	// <0x7af1f0>|0x000|0x000:'43'
	// <0x7af20f>|0x01f|0x01f:'44'
	// <0x7af239>|0x049|0x02a:'45'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::weapon_core_show_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair weapon_core_show_state::get_weapon_lexeme_pair( vostok::mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7af149>|0x000|0x000:'51'
	// <0x7af150>|0x007|0x007:'52'
	// <0x7af173>|0x02a|0x023:'53'
	// 1
	// 2
	// 3
	// <0x7af17f>|0x036|0x00c:'57'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_show_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression weapon_core_show_state::get_user_hands_expression(
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
	// <0x7af071>|0x000|0x000:'62'
	// <0x7af077>|0x006|0x006:'63'
	// 1
	// 2
	// <0x7af087>|0x016|0x010:'66'
	// 1
	// <0x7af093>|0x022|0x00c:'68'
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
	// <0x7af0a1>|0x030|0x00e:'81'
	// <0x7af11a>|0x0a9|0x079:'82'
	// ******
}

// STATE[STUB]
// survarium::weapon_core_show_state* survarium::weapon_core_state_cook_template<survarium::weapon_core_show_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
weapon_core_show_state* weapon_core_state_cook_template<weapon_core_show_state>::new_object(
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
	// <0x7aef99>|0x000|0x000:'89'
	// <0x7aefc4>|0x02b|0x02b:'90'
	// <0x7aeff4>|0x05b|0x030:'91'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x7af003>|0x06a|0x00f:'98'
	// ******
}

} // namespace survarium
