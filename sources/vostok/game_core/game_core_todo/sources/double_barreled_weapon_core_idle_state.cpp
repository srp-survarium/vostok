////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "double_barreled_weapon_core_idle_state.h"

namespace survarium {

// STATE[STUB]
// survarium::double_barreled_weapon_core_idle_state::double_barreled_weapon_core_idle_state(survarium::weapon_core&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
double_barreled_weapon_core_idle_state::double_barreled_weapon_core_idle_state( weapon_core& weapon, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const* animations, u32 animations_count )
{
	// LOCALS
	// u32 							animation_index
	// u32 							view_index<1>
	// u32 							user_state_index<2>
	// u32 							weapon_state_index<3>
	// ******

	// FUNCTION BODY
	// <0x7acc6f>|0x000|0x000:'21'
	// <0x7acc7b>|0x00c|0x00c:'22'
	// 1
	// <0x7acc9e>|0x02f|0x023:'24'
	// <0x7acca5>|0x036|0x007|[1]:'25'
	// <0x7accbd>|0x04e|0x018|[2]:'26'
	// <0x7accd5>|0x066|0x018|[3]:'27'
	// <0x7acced>|0x07e|0x018:'28'
	// <0x7acd29>|0x0ba|0x03c:'29'
	// <0x7acd2b>|0x0bc|0x002:'30'
	// <0x7acd2d>|0x0be|0x002:'31'
	// <0x7acd32>|0x0c3|0x005:'32'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::double_barreled_weapon_core_idle_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression double_barreled_weapon_core_idle_state::weapon_and_hands_expression(
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	vostok::animation::mixing::animation_lexeme&	weight_driving_animation) const
{
	// LOCALS
	// weapon_lexeme_pair 			lexeme_pair
	// ******

	// FUNCTION BODY
	// <0x7ace91>|0x000|0x000:'37'
	// <0x7acec7>|0x036|0x036:'38'
	// <0x7acee6>|0x055|0x01f:'39'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::double_barreled_weapon_core_idle_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair double_barreled_weapon_core_idle_state::get_weapon_lexeme_pair( vostok::mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// pcstr[3] 					weapon_animation_captions
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// u32 							animation_index
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7acdb9>|0x000|0x000:'44'
	// 1
	// <0x7acdc5>|0x00c|0x00c:'46'
	// <0x7acdcc>|0x013|0x007:'47'
	// <0x7acdd3>|0x01a|0x007:'48'
	// 1
	// <0x7acdda>|0x021|0x007:'50'
	// <0x7acdee>|0x035|0x014:'51'
	// <0x7acdf8>|0x03f|0x00a:'52'
	// <0x7ace26>|0x06d|0x02e:'53'
	// ******
}

// STATE[STUB]
// survarium::double_barreled_weapon_core_idle_state* survarium::weapon_core_state_cook_template<survarium::double_barreled_weapon_core_idle_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
double_barreled_weapon_core_idle_state* weapon_core_state_cook_template<double_barreled_weapon_core_idle_state>::new_object(
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
	// <0x7acd59>|0x000|0x000:'64'
	// ******
}

} // namespace survarium
