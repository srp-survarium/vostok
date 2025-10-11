////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon_core_aimed_state.h"

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_aimed_state::weapon_core_aimed_state(survarium::weapon_core&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
weapon_core_aimed_state::weapon_core_aimed_state( weapon_core& weapon, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const* animations, u32 animations_count )
{
	// LOCALS
	// u32 							animation_index
	// u32 							view<1>
	// u32 							user_state<2>
	// ******

	// FUNCTION BODY
	// <0x7acf8f>|0x000|0x000:'20'
	// <0x7acfb2>|0x023|0x023:'21'
	// <0x7acfb9>|0x02a|0x007|[1]:'22'
	// <0x7acfd1>|0x042|0x018|[2]:'23'
	// <0x7acfe9>|0x05a|0x018:'24'
	// <0x7ad01a>|0x08b|0x031:'25'
	// <0x7ad01c>|0x08d|0x002:'26'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_aimed_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression weapon_core_aimed_state::weapon_and_hands_expression(
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	vostok::animation::mixing::animation_lexeme&	weight_driving_animation) const
{
	// LOCALS
	// weapon_lexeme_pair 			lexeme_pair
	// ******

	// FUNCTION BODY
	// <0x7ad131>|0x000|0x000:'31'
	// <0x7ad167>|0x036|0x036:'32'
	// <0x7ad186>|0x055|0x01f:'33'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::weapon_core_aimed_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair weapon_core_aimed_state::get_weapon_lexeme_pair( vostok::mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7ad099>|0x000|0x000:'38'
	// <0x7ad0a0>|0x007|0x007:'39'
	// <0x7ad0c3>|0x02a|0x023:'40'
	// ******
}

// STATE[STUB]
// survarium::weapon_core_aimed_state* survarium::weapon_core_state_cook_template<survarium::weapon_core_aimed_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
weapon_core_aimed_state* weapon_core_state_cook_template<weapon_core_aimed_state>::new_object(
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
	// <0x7ad039>|0x000|0x000:'51'
	// ******
}

} // namespace survarium
