////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "pistol_weapon_core_reload_state.h"

namespace survarium {

// STATE[STUB]
// survarium::pistol_weapon_core_reload_state::pistol_weapon_core_reload_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
pistol_weapon_core_reload_state::pistol_weapon_core_reload_state(
	weapon_core&						weapon,
	float								animation_timescale,
	vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*	animations,
	u32									animations_count)
{
	// LOCALS
	// u32 							animation_index
	// u32 							view_index<1>
	// u32 							user_state_index<2>
	// u32 							weapon_state_index<3>
	// u32 							view_index<2>
	// u32 							user_state_index<3>
	// u32 							weapon_state_index<4>
	// ******

	// SKIPPED BLOCKS
	// <0x7a8a4e><2>
	// <0x7a8a66><3>
	// <0x7a8ad6><3>
	// <0x7a8aee><4>
	// ******

	// FUNCTION BODY
	// <0x7a8a0e>|0x000|0x000:'24'
	// <0x7a8a31>|0x023|0x023:'25'
	// <0x7a8a38>|0x02a|0x007|[1]:'26'
	// <0x7a8a50>|0x042|0x018:'27'
	// <0x7a8a68>|0x05a|0x018:'28'
	// <0x7a8a80>|0x072|0x018:'29'
	// <0x7a8aba>|0x0ac|0x03a:'30'
	// <0x7a8abc>|0x0ae|0x002:'31'
	// <0x7a8abe>|0x0b0|0x002:'32'
	// <0x7a8ac0>|0x0b2|0x002|[2]:'33'
	// <0x7a8ad8>|0x0ca|0x018:'34'
	// <0x7a8af0>|0x0e2|0x018:'35'
	// <0x7a8b08>|0x0fa|0x018:'36'
	// <0x7a8b42>|0x134|0x03a:'37'
	// <0x7a8b44>|0x136|0x002:'38'
	// <0x7a8b46>|0x138|0x002:'39'
	// <0x7a8b48>|0x13a|0x002:'40'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_reload_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression pistol_weapon_core_reload_state::weapon_and_hands_expression(
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
	// <0x7a8dd1>|0x000|0x000:'45'
	// 1
	// <0x7a8df0>|0x01f|0x01f:'47'
	// <0x7a8df6>|0x025|0x006:'48'
	// 1
	// <0x7a8e54>|0x083|0x05e:'50'
	// 1
	// <0x7a8e66>|0x095|0x012:'52'
	// 1
	// <0x7a8e90>|0x0bf|0x02a:'54'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::pistol_weapon_core_reload_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair pistol_weapon_core_reload_state::get_weapon_lexeme_pair( vostok::mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// u32 							weapon_state_index
	// pcstr[2] 					weapon_animation_captions
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// 1
	// <0x7a8cf9>|0x000|0x000:'60'
	// <0x7a8d00>|0x007|0x007:'61'
	// 1
	// 2
	// <0x7a8d07>|0x00e|0x007:'64'
	// <0x7a8d22>|0x029|0x01b:'65'
	// <0x7a8d2c>|0x033|0x00a:'66'
	// <0x7a8d58>|0x05f|0x02c:'67'
	// 1
	// 2
	// 3
	// <0x7a8d64>|0x06b|0x00c:'71'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_reload_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression pistol_weapon_core_reload_state::get_user_hands_expression(
	vostok::animation::mixing::animation_lexeme&	weapon_lexeme,
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	vostok::animation::mixing::animation_lexeme&	weight_driving_animation) const
{
	// LOCALS
	// vostok::animation::mixing::animation_lexeme override_lexeme
	// u32 							weapon_state_index
	// vostok::animation::linear_interpolator interpolator
	// u32 							user_state_index
	// pcstr[2][2] 					user_animation_captions
	// ******

	// FUNCTION BODY
	// 1
	// <0x7a8be1>|0x000|0x000:'77'
	// <0x7a8bef>|0x00e|0x00e:'78'
	// 1
	// 2
	// <0x7a8bfd>|0x01c|0x00e:'81'
	// <0x7a8c09>|0x028|0x00c:'82'
	// 1
	// <0x7a8c27>|0x046|0x01e:'84'
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
	// <0x7a8c37>|0x056|0x010:'96'
	// <0x7a8cb9>|0x0d8|0x082:'97'
	// ******
}

// STATE[STUB]
// survarium::pistol_weapon_core_reload_state* survarium::weapon_core_state_cook_template<survarium::pistol_weapon_core_reload_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
pistol_weapon_core_reload_state* weapon_core_state_cook_template<pistol_weapon_core_reload_state>::new_object(
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
	// <0x7a8b69>|0x000|0x000:'109'
	// ******
}

} // namespace survarium
