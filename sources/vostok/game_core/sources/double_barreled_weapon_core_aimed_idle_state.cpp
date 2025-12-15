////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/double_barreled_weapon_core_aimed_idle_state.h>

namespace survarium {

// STATE[STUB]
// survarium::double_barreled_weapon_core_aimed_idle_state::double_barreled_weapon_core_aimed_idle_state(survarium::weapon_core&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
double_barreled_weapon_core_aimed_idle_state::double_barreled_weapon_core_aimed_idle_state( weapon_core& weapon, resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const* animations, u32 animations_count )
{
	// LOCALS
	// u32 							animation_index
	// u32 							view_index<1>
	// u32 							user_state_index<2>
	// u32 							weapon_state_index<3>
	// ******

	// FUNCTION BODY
	// <0x7ac94f>|0x03f|+0x00c:'21'
	// <0x7ac95b>|0x04b|+0x023:'22'
	// <0>
	// <0x7ac97e>|0x06e|+0x007:'24'
	// <0x7ac985>|0x075|+0x018|[1]:'25'
	// <0x7ac99d>|0x08d|+0x018|[2]:'26'
	// <0x7ac9b5>|0x0a5|+0x018|[3]:'27'
	// <0x7ac9cd>|0x0bd|+0x03c:'28'
	// <0x7aca09>|0x0f9|+0x002:'29'
	// <0x7aca0b>|0x0fb|+0x002:'30'
	// <0x7aca0d>|0x0fd|+0x005:'31'
	// <0x7aca12>|0x102|+0x00c:'32'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::double_barreled_weapon_core_aimed_idle_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression double_barreled_weapon_core_aimed_idle_state::weapon_and_hands_expression(
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
	// <0x7acb71>|0x011|+0x036:'37'
	// <0x7acba7>|0x047|+0x01f:'38'
	// <0x7acbc6>|0x066|+0x059:'39'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::double_barreled_weapon_core_aimed_idle_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair double_barreled_weapon_core_aimed_idle_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// pcstr[3] 					weapon_animation_captions
	// resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const& selected_animation
	// u32 							animation_index
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7aca99>|0x009|+0x00c:'44'
	// <0>
	// <0x7acaa5>|0x015|+0x007:'46'
	// <0x7acaac>|0x01c|+0x007:'47'
	// <0x7acab3>|0x023|+0x007:'48'
	// <0>
	// <0x7acaba>|0x02a|+0x014:'50'
	// <0x7acace>|0x03e|+0x00a:'51'
	// <0x7acad8>|0x048|+0x02e:'52'
	// <0x7acb06>|0x076|+0x04f:'53'
	// ******
}

// STATE[STUB]
// survarium::double_barreled_weapon_core_aimed_idle_state* survarium::weapon_core_state_cook_template<survarium::double_barreled_weapon_core_aimed_idle_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
double_barreled_weapon_core_aimed_idle_state* weapon_core_state_cook_template<survarium::double_barreled_weapon_core_aimed_idle_state>::new_object(
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
	// <0x7aca39>|0x009|+0x045:'65'
	// ******
}

} // namespace survarium
