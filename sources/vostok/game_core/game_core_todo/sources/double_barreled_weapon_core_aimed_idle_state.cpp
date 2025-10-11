////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "double_barreled_weapon_core_aimed_idle_state.h"

namespace survarium {

// STATE[STUB]
// survarium::double_barreled_weapon_core_aimed_idle_state::double_barreled_weapon_core_aimed_idle_state(survarium::weapon_core&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
double_barreled_weapon_core_aimed_idle_state::double_barreled_weapon_core_aimed_idle_state( weapon_core& weapon, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const* animations, u32 animations_count )
{
	// LOCALS
	// u32 							animation_index
	// u32 							view_index<1>
	// u32 							user_state_index<2>
	// u32 							weapon_state_index<3>
	// ******

	// FUNCTION BODY
	// <0x7ac94f>|0x000|0x000:'21'
	// <0x7ac95b>|0x00c|0x00c:'22'
	// 1
	// <0x7ac97e>|0x02f|0x023:'24'
	// <0x7ac985>|0x036|0x007|[1]:'25'
	// <0x7ac99d>|0x04e|0x018|[2]:'26'
	// <0x7ac9b5>|0x066|0x018|[3]:'27'
	// <0x7ac9cd>|0x07e|0x018:'28'
	// <0x7aca09>|0x0ba|0x03c:'29'
	// <0x7aca0b>|0x0bc|0x002:'30'
	// <0x7aca0d>|0x0be|0x002:'31'
	// <0x7aca12>|0x0c3|0x005:'32'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::double_barreled_weapon_core_aimed_idle_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression double_barreled_weapon_core_aimed_idle_state::weapon_and_hands_expression(
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	vostok::animation::mixing::animation_lexeme&	weight_driving_animation) const
{
	// LOCALS
	// weapon_lexeme_pair 			lexeme_pair
	// ******

	// FUNCTION BODY
	// <0x7acb71>|0x000|0x000:'37'
	// <0x7acba7>|0x036|0x036:'38'
	// <0x7acbc6>|0x055|0x01f:'39'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::double_barreled_weapon_core_aimed_idle_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair double_barreled_weapon_core_aimed_idle_state::get_weapon_lexeme_pair( vostok::mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// pcstr[3] 					weapon_animation_captions
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// u32 							animation_index
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7aca99>|0x000|0x000:'44'
	// 1
	// <0x7acaa5>|0x00c|0x00c:'46'
	// <0x7acaac>|0x013|0x007:'47'
	// <0x7acab3>|0x01a|0x007:'48'
	// 1
	// <0x7acaba>|0x021|0x007:'50'
	// <0x7acace>|0x035|0x014:'51'
	// <0x7acad8>|0x03f|0x00a:'52'
	// <0x7acb06>|0x06d|0x02e:'53'
	// ******
}

// STATE[STUB]
// survarium::double_barreled_weapon_core_aimed_idle_state* survarium::weapon_core_state_cook_template<survarium::double_barreled_weapon_core_aimed_idle_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
double_barreled_weapon_core_aimed_idle_state* weapon_core_state_cook_template<double_barreled_weapon_core_aimed_idle_state>::new_object(
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
	// <0x7aca39>|0x000|0x000:'65'
	// ******
}

} // namespace survarium
