////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_aimed_state.h>

#include <vostok/game_core/weapon_core_state_cook_template.h>

namespace survarium {

// STATE[STUB]
weapon_core_aimed_state::weapon_core_aimed_state( weapon_core& weapon, resources::managed_resource_ptr const* animations, u32 animations_count ) :
	weapon_core_aimed_state_base( weapon )
{
	// LOCALS
	// u32 							animation_index
	// u32 							view<1>
	// u32 							user_state<2>
	// ******

	// FUNCTION BODY
	// <0x7acf8f>|0x03f|+0x023:'20'
	// <0x7acfb2>|0x062|+0x007:'21'
	// <0x7acfb9>|0x069|+0x018|[1]:'22'
	// <0x7acfd1>|0x081|+0x018|[2]:'23'
	// <0x7acfe9>|0x099|+0x031:'24'
	// <0x7ad01a>|0x0ca|+0x002:'25'
	// <0x7ad01c>|0x0cc|+0x002:'26'
	// ******
}

// STATE[STUB]
animation::mixing::expression weapon_core_aimed_state::weapon_and_hands_expression(
	mutable_buffer&							buffer,
	bool									is_third_view,
	weapon_user_state_enum					user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	// LOCALS
	// weapon_lexeme_pair 			lexeme_pair
	// ******

	// FUNCTION BODY
	// <0x7ad131>|0x011|+0x036:'31'
	// <0x7ad167>|0x047|+0x01f:'32'
	// <0x7ad186>|0x066|+0x059:'33'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::weapon_core_aimed_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair weapon_core_aimed_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7ad099>|0x009|+0x007:'38'
	// <0x7ad0a0>|0x010|+0x023:'39'
	// <0x7ad0c3>|0x033|+0x04f:'40'
	// ******
}

// STATE[STUB]
weapon_core_aimed_state* weapon_core_state_cook_template< weapon_core_aimed_state >::new_object(
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
	// <0x7ad039>|0x009|+0x045:'51'
	// ******
}

} // namespace survarium
