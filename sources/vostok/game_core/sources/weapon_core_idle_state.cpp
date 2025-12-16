////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_idle_state.h>

#include <vostok/game_core/weapon_core_state_cook_template.h>

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_idle_state::weapon_core_idle_state(survarium::weapon_core&, vostok::resources::managed_resource_ptr const*, const unsigned int)
weapon_core_idle_state::weapon_core_idle_state( weapon_core& weapon, resources::managed_resource_ptr const* animations, u32 animations_count ) :
	weapon_core_idle_state_base( weapon )
{
	// LOCALS
	// u32 							animation_index
	// u32 							view<1>
	// u32 							user_state<2>
	// ******

	// FUNCTION BODY
	// <0x7ad22f>|0x03f|+0x023:'21'
	// <0x7ad252>|0x062|+0x007:'22'
	// <0x7ad259>|0x069|+0x018|[1]:'23'
	// <0x7ad271>|0x081|+0x018|[2]:'24'
	// <0x7ad289>|0x099|+0x031:'25'
	// <0x7ad2ba>|0x0ca|+0x002:'26'
	// <0x7ad2bc>|0x0cc|+0x002:'27'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_idle_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression weapon_core_idle_state::weapon_and_hands_expression(
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
	// <0x7ad3d1>|0x011|+0x036:'32'
	// <0x7ad407>|0x047|+0x01f:'33'
	// <0x7ad426>|0x066|+0x059:'34'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::weapon_core_idle_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair weapon_core_idle_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// resources::managed_resource_ptr const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7ad339>|0x009|+0x007:'39'
	// <0x7ad340>|0x010|+0x023:'40'
	// <0x7ad363>|0x033|+0x04f:'41'
	// ******
}

// STATE[STUB]
weapon_core_idle_state* weapon_core_state_cook_template<weapon_core_idle_state>::new_object(
	mutable_buffer							buffer,
	weapon_state_creation_params const*		params,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
)
{
	return NULL;

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7ad2d9>|0x009|+0x045:'53'
	// ******
}

} // namespace survarium
