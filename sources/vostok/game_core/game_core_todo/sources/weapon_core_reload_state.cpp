////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon_core_reload_state.h"

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_reload_state::weapon_core_reload_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
weapon_core_reload_state::weapon_core_reload_state(
	weapon_core&						weapon,
	float								animation_time_scale,
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
	// <0x7aa80e><2>
	// <0x7aa873><3>
	// ******

	// FUNCTION BODY
	// <0x7aa7ce>|0x000|0x000:'27'
	// <0x7aa7f1>|0x023|0x023:'28'
	// <0x7aa7f8>|0x02a|0x007|[1]:'29'
	// <0x7aa810>|0x042|0x018:'30'
	// <0x7aa828>|0x05a|0x018:'31'
	// <0x7aa859>|0x08b|0x031:'32'
	// <0x7aa85b>|0x08d|0x002:'33'
	// <0x7aa85d>|0x08f|0x002|[2]:'34'
	// <0x7aa875>|0x0a7|0x018:'35'
	// <0x7aa88d>|0x0bf|0x018:'36'
	// <0x7aa8be>|0x0f0|0x031:'37'
	// <0x7aa8c0>|0x0f2|0x002:'38'
	// <0x7aa8c2>|0x0f4|0x002:'39'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_reload_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression weapon_core_reload_state::weapon_and_hands_expression(
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
	// <0x7aaaf0>|0x000|0x000:'44'
	// <0x7aab0f>|0x01f|0x01f:'45'
	// <0x7aab39>|0x049|0x02a:'46'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::weapon_core_reload_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair weapon_core_reload_state::get_weapon_lexeme_pair( vostok::mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7aaa49>|0x000|0x000:'51'
	// <0x7aaa50>|0x007|0x007:'52'
	// <0x7aaa73>|0x02a|0x023:'53'
	// 1
	// 2
	// 3
	// <0x7aaa7f>|0x036|0x00c:'57'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_reload_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression weapon_core_reload_state::get_user_hands_expression(
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
	// <0x7aa961>|0x000|0x000:'62'
	// <0x7aa967>|0x006|0x006:'63'
	// 1
	// <0x7aa977>|0x016|0x010:'65'
	// 1
	// <0x7aa989>|0x028|0x012:'67'
	// 1
	// <0x7aa995>|0x034|0x00c:'69'
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
	// <0x7aa9a3>|0x042|0x00e:'82'
	// <0x7aaa1c>|0x0bb|0x079:'83'
	// ******
}

// STATE[STUB]
// survarium::weapon_core_reload_state* survarium::weapon_core_state_cook_template<survarium::weapon_core_reload_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
weapon_core_reload_state* weapon_core_state_cook_template<weapon_core_reload_state>::new_object(
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
	// <0x7aa8e9>|0x000|0x000:'95'
	// ******
}

} // namespace survarium
