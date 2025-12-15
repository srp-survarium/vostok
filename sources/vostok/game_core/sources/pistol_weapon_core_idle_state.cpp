////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/pistol_weapon_core_idle_state.h>

namespace survarium {

// STATE[STUB]
// survarium::pistol_weapon_core_idle_state::pistol_weapon_core_idle_state(survarium::weapon_core&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
pistol_weapon_core_idle_state::pistol_weapon_core_idle_state( weapon_core& weapon, resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const* animations, u32 animations_count )
{
	// LOCALS
	// u32 							animation_index
	// u32 							view_index<1>
	// u32 							user_state_index<2>
	// u32 							weapon_state_index<3>
	// ******

	// FUNCTION BODY
	// <0x7abadf>|0x03f|+0x023:'21'
	// <0x7abb02>|0x062|+0x007:'22'
	// <0x7abb09>|0x069|+0x018|[1]:'23'
	// <0x7abb21>|0x081|+0x018|[2]:'24'
	// <0x7abb39>|0x099|+0x018|[3]:'25'
	// <0x7abb51>|0x0b1|+0x03a:'26'
	// <0x7abb8b>|0x0eb|+0x002:'27'
	// <0x7abb8d>|0x0ed|+0x002:'28'
	// <0x7abb8f>|0x0ef|+0x002:'29'
	// <0x7abb91>|0x0f1|+0x00c:'30'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_idle_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression pistol_weapon_core_idle_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	// LOCALS
	// weapon_lexeme_pair 			lexeme_pair
	// ******

	// FUNCTION BODY
	// <0x7abce1>|0x011|+0x036:'35'
	// <0x7abd17>|0x047|+0x01f:'36'
	// <0x7abd36>|0x066|+0x059:'37'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::pistol_weapon_core_idle_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair pistol_weapon_core_idle_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// pcstr[2] 					weapon_animation_captions
	// resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const& selected_animation
	// u32 							animation_index
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0>
	// <0x7abc19>|0x009|+0x007:'43'
	// <0x7abc20>|0x010|+0x007:'44'
	// <0>
	// <0x7abc27>|0x017|+0x01b:'46'
	// <0x7abc42>|0x032|+0x00a:'47'
	// <0x7abc4c>|0x03c|+0x02c:'48'
	// <0x7abc78>|0x068|+0x04f:'49'
	// ******
}

// STATE[STUB]
// survarium::pistol_weapon_core_idle_state* survarium::weapon_core_state_cook_template<survarium::pistol_weapon_core_idle_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
pistol_weapon_core_idle_state* weapon_core_state_cook_template<survarium::pistol_weapon_core_idle_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const*	animations,
	u32									animations_count
)
{
	return NULL;

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7abbb9>|0x009|+0x045:'60'
	// ******
}

} // namespace survarium
