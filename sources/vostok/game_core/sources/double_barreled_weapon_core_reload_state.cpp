////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/double_barreled_weapon_core_reload_state.h>

#include <vostok/game_core/weapon_core_state_cook_template.h>

namespace survarium {

// STATE[STUB]
// survarium::double_barreled_weapon_core_reload_state::double_barreled_weapon_core_reload_state(survarium::weapon_core&, const float, vostok::resources::managed_resource_ptr const*, const unsigned int)
double_barreled_weapon_core_reload_state::double_barreled_weapon_core_reload_state(
	weapon_core&							weapon,
	float									animation_timescale,
	resources::managed_resource_ptr const*	animations,
	u32										animations_count
) : weapon_core_reload_state_base( weapon, animation_timescale )
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
	// <0x7a8fe9><2>
	// <0x7a9001><3>
	// <0x7a9071><3>
	// <0x7a9089><4>
	// ******

	// FUNCTION BODY
	// <0x7a8f7e>|0x05e|+0x02b:'24'
	// <0x7a8fa9>|0x089|+0x023:'25'
	// <0x7a8fcc>|0x0ac|+0x007:'26'
	// <0x7a8fd3>|0x0b3|+0x018|[1]:'27'
	// <0x7a8feb>|0x0cb|+0x018:'28'
	// <0x7a9003>|0x0e3|+0x018:'29'
	// <0x7a901b>|0x0fb|+0x03a:'30'
	// <0x7a9055>|0x135|+0x002:'31'
	// <0x7a9057>|0x137|+0x002:'32'
	// <0x7a9059>|0x139|+0x002:'33'
	// <0x7a905b>|0x13b|+0x018|[2]:'34'
	// <0x7a9073>|0x153|+0x018:'35'
	// <0x7a908b>|0x16b|+0x018:'36'
	// <0x7a90a3>|0x183|+0x03a:'37'
	// <0x7a90dd>|0x1bd|+0x002:'38'
	// <0x7a90df>|0x1bf|+0x002:'39'
	// <0x7a90e1>|0x1c1|+0x002:'40'
	// <0x7a90e3>|0x1c3|+0x00c:'41'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::double_barreled_weapon_core_reload_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression double_barreled_weapon_core_reload_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	// LOCALS
	// animation::mixing::expression hands_expression
	// weapon_lexeme_pair 			lexeme_pair
	// ******

	// FUNCTION BODY
	// <0x7a93b0>|0x010|+0x01f:'46'
	// <0>
	// <0x7a93cf>|0x02f|+0x02a:'48'
	// <0>
	// <0x7a93f9>|0x059|+0x07a:'50'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::double_barreled_weapon_core_reload_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair double_barreled_weapon_core_reload_state::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// u32 							weapon_state_index
	// pcstr[2] 					weapon_animation_captions
	// resources::managed_resource_ptr const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7a92c9>|0x009|+0x00c:'55'
	// <0>
	// <1>
	// <0x7a92d5>|0x015|+0x007:'58'
	// <0x7a92dc>|0x01c|+0x007:'59'
	// <0>
	// <0x7a92e3>|0x023|+0x01d:'61'
	// <0x7a9300>|0x040|+0x00a:'62'
	// <0x7a930a>|0x04a|+0x02c:'63'
	// <0x7a9336>|0x076|+0x00c:'64'
	// <0>
	// <1>
	// <2>
	// <0x7a9342>|0x082|+0x056:'68'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::double_barreled_weapon_core_reload_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression double_barreled_weapon_core_reload_state::get_user_hands_expression(
	animation::mixing::animation_lexeme&	weapon_lexeme,
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	// LOCALS
	// animation::mixing::animation_lexeme override_lexeme
	// u32 							weapon_state_index
	// u32 							user_state_index
	// pcstr[2][2] 					user_animation_captions
	// ******

	// FUNCTION BODY
	// <0x7a9182>|0x012|+0x012:'73'
	// <0x7a9194>|0x024|+0x012:'74'
	// <0>
	// <1>
	// <0x7a91a6>|0x036|+0x00e:'77'
	// <0x7a91b4>|0x044|+0x00e:'78'
	// <0>
	// <1>
	// <0x7a91c2>|0x052|+0x00c:'81'
	// <0x7a91ce>|0x05e|+0x020:'82'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <0x7a91ee>|0x07e|+0x0a3:'96'
	// <0x7a9291>|0x121|+0x01c:'97'
	// ******
}

// STATE[STUB]
// survarium::double_barreled_weapon_core_reload_state* survarium::weapon_core_state_cook_template<survarium::double_barreled_weapon_core_reload_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::managed_resource_ptr const*, const unsigned int)
double_barreled_weapon_core_reload_state* weapon_core_state_cook_template<survarium::double_barreled_weapon_core_reload_state>::new_object(
	mutable_buffer						buffer,
	weapon_state_creation_params const*	params,
	resources::managed_resource_ptr const*	animations,
	u32									animations_count
)
{
	return NULL;

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7a9109>|0x009|+0x05c:'109'
	// ******
}

} // namespace survarium
