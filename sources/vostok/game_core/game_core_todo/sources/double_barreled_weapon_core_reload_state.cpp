////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "double_barreled_weapon_core_reload_state.h"

namespace survarium {

// STATE[STUB]
// survarium::double_barreled_weapon_core_reload_state::double_barreled_weapon_core_reload_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
double_barreled_weapon_core_reload_state::double_barreled_weapon_core_reload_state(
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
	// <0x7a8fe9><2>
	// <0x7a9001><3>
	// <0x7a9071><3>
	// <0x7a9089><4>
	// ******

	// FUNCTION BODY
	// <0x7a8f7e>|0x000|0x000:'24'
	// <0x7a8fa9>|0x02b|0x02b:'25'
	// <0x7a8fcc>|0x04e|0x023:'26'
	// <0x7a8fd3>|0x055|0x007|[1]:'27'
	// <0x7a8feb>|0x06d|0x018:'28'
	// <0x7a9003>|0x085|0x018:'29'
	// <0x7a901b>|0x09d|0x018:'30'
	// <0x7a9055>|0x0d7|0x03a:'31'
	// <0x7a9057>|0x0d9|0x002:'32'
	// <0x7a9059>|0x0db|0x002:'33'
	// <0x7a905b>|0x0dd|0x002|[2]:'34'
	// <0x7a9073>|0x0f5|0x018:'35'
	// <0x7a908b>|0x10d|0x018:'36'
	// <0x7a90a3>|0x125|0x018:'37'
	// <0x7a90dd>|0x15f|0x03a:'38'
	// <0x7a90df>|0x161|0x002:'39'
	// <0x7a90e1>|0x163|0x002:'40'
	// <0x7a90e3>|0x165|0x002:'41'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::double_barreled_weapon_core_reload_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression double_barreled_weapon_core_reload_state::weapon_and_hands_expression(
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
	// <0x7a93b0>|0x000|0x000:'46'
	// 1
	// <0x7a93cf>|0x01f|0x01f:'48'
	// 1
	// <0x7a93f9>|0x049|0x02a:'50'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::double_barreled_weapon_core_reload_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair double_barreled_weapon_core_reload_state::get_weapon_lexeme_pair( vostok::mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// u32 							weapon_state_index
	// pcstr[2] 					weapon_animation_captions
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// <0x7a92c9>|0x000|0x000:'55'
	// 1
	// 2
	// <0x7a92d5>|0x00c|0x00c:'58'
	// <0x7a92dc>|0x013|0x007:'59'
	// 1
	// <0x7a92e3>|0x01a|0x007:'61'
	// <0x7a9300>|0x037|0x01d:'62'
	// <0x7a930a>|0x041|0x00a:'63'
	// <0x7a9336>|0x06d|0x02c:'64'
	// 1
	// 2
	// 3
	// <0x7a9342>|0x079|0x00c:'68'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::double_barreled_weapon_core_reload_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression double_barreled_weapon_core_reload_state::get_user_hands_expression(
	vostok::animation::mixing::animation_lexeme&	weapon_lexeme,
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	vostok::animation::mixing::animation_lexeme&	weight_driving_animation) const
{
	// LOCALS
	// vostok::animation::mixing::animation_lexeme override_lexeme
	// u32 							weapon_state_index
	// u32 							user_state_index
	// pcstr[2][2] 					user_animation_captions
	// ******

	// FUNCTION BODY
	// <0x7a9182>|0x000|0x000:'73'
	// <0x7a9194>|0x012|0x012:'74'
	// 1
	// 2
	// <0x7a91a6>|0x024|0x012:'77'
	// <0x7a91b4>|0x032|0x00e:'78'
	// 1
	// 2
	// <0x7a91c2>|0x040|0x00e:'81'
	// <0x7a91ce>|0x04c|0x00c:'82'
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
	// 12
	// 13
	// <0x7a91ee>|0x06c|0x020:'96'
	// <0x7a9291>|0x10f|0x0a3:'97'
	// ******
}

// STATE[STUB]
// survarium::double_barreled_weapon_core_reload_state* survarium::weapon_core_state_cook_template<survarium::double_barreled_weapon_core_reload_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
double_barreled_weapon_core_reload_state* weapon_core_state_cook_template<double_barreled_weapon_core_reload_state>::new_object(
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
	// <0x7a9109>|0x000|0x000:'109'
	// ******
}

} // namespace survarium
