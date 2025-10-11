////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "pistol_weapon_core_idle_state.h"

namespace survarium {

// STATE[STUB]
// survarium::pistol_weapon_core_idle_state::pistol_weapon_core_idle_state(survarium::weapon_core&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
pistol_weapon_core_idle_state::pistol_weapon_core_idle_state( weapon_core& weapon, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const* animations, u32 animations_count )
{
	// LOCALS
	// u32 							animation_index
	// u32 							view_index<1>
	// u32 							user_state_index<2>
	// u32 							weapon_state_index<3>
	// ******

	// FUNCTION BODY
	// <0x7abadf>|0x000|0x000:'21'
	// <0x7abb02>|0x023|0x023:'22'
	// <0x7abb09>|0x02a|0x007|[1]:'23'
	// <0x7abb21>|0x042|0x018|[2]:'24'
	// <0x7abb39>|0x05a|0x018|[3]:'25'
	// <0x7abb51>|0x072|0x018:'26'
	// <0x7abb8b>|0x0ac|0x03a:'27'
	// <0x7abb8d>|0x0ae|0x002:'28'
	// <0x7abb8f>|0x0b0|0x002:'29'
	// <0x7abb91>|0x0b2|0x002:'30'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_idle_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression pistol_weapon_core_idle_state::weapon_and_hands_expression(
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	vostok::animation::mixing::animation_lexeme&	weight_driving_animation) const
{
	// LOCALS
	// weapon_lexeme_pair 			lexeme_pair
	// ******

	// FUNCTION BODY
	// <0x7abce1>|0x000|0x000:'35'
	// <0x7abd17>|0x036|0x036:'36'
	// <0x7abd36>|0x055|0x01f:'37'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::pistol_weapon_core_idle_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair pistol_weapon_core_idle_state::get_weapon_lexeme_pair( vostok::mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// pcstr[2] 					weapon_animation_captions
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// u32 							animation_index
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// 1
	// <0x7abc19>|0x000|0x000:'43'
	// <0x7abc20>|0x007|0x007:'44'
	// 1
	// <0x7abc27>|0x00e|0x007:'46'
	// <0x7abc42>|0x029|0x01b:'47'
	// <0x7abc4c>|0x033|0x00a:'48'
	// <0x7abc78>|0x05f|0x02c:'49'
	// ******
}

// STATE[STUB]
// survarium::pistol_weapon_core_idle_state* survarium::weapon_core_state_cook_template<survarium::pistol_weapon_core_idle_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
pistol_weapon_core_idle_state* weapon_core_state_cook_template<pistol_weapon_core_idle_state>::new_object(
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
	// <0x7abbb9>|0x000|0x000:'60'
	// ******
}

} // namespace survarium
