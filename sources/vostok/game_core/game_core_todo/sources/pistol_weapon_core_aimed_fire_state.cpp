////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "pistol_weapon_core_aimed_fire_state.h"

namespace survarium {

// STATE[STUB]
// survarium::pistol_weapon_core_aimed_fire_state::pistol_weapon_core_aimed_fire_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
pistol_weapon_core_aimed_fire_state::pistol_weapon_core_aimed_fire_state(
	weapon_core&						weapon,
	float								animation_time_scale,
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
	// ******

	// SKIPPED BLOCKS
	// <0x7aac6b><2>
	// <0x7aac83><3>
	// <0x7aacf3><3>
	// ******

	// FUNCTION BODY
	// <0x7aac2b>|0x000|0x000:'26'
	// <0x7aac4e>|0x023|0x023:'27'
	// <0x7aac55>|0x02a|0x007|[1]:'28'
	// <0x7aac6d>|0x042|0x018:'29'
	// <0x7aac85>|0x05a|0x018:'30'
	// <0x7aac9d>|0x072|0x018:'31'
	// <0x7aacd7>|0x0ac|0x03a:'32'
	// <0x7aacd9>|0x0ae|0x002:'33'
	// <0x7aacdb>|0x0b0|0x002:'34'
	// <0x7aacdd>|0x0b2|0x002|[2]:'35'
	// <0x7aacf5>|0x0ca|0x018:'36'
	// <0x7aad0d>|0x0e2|0x018:'37'
	// <0x7aad3e>|0x113|0x031:'38'
	// <0x7aad40>|0x115|0x002:'39'
	// <0x7aad42>|0x117|0x002:'40'
	// ******
}

// STATE[STUB]
// void survarium::pistol_weapon_core_aimed_fire_state::initialize()
void pistol_weapon_core_aimed_fire_state::initialize( )
{
	// LOCALS
	// bool 						last_shot
	// ******

	// FUNCTION BODY
	// <0x7ab0e9>|0x000|0x000:'45'
	// <0x7ab0f1>|0x008|0x008:'46'
	// <0x7ab14e>|0x065|0x05d:'47'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_aimed_fire_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression pistol_weapon_core_aimed_fire_state::weapon_and_hands_expression(
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
	// <0x7ab010>|0x000|0x000:'52'
	// <0x7ab02f>|0x01f|0x01f:'53'
	// <0x7ab059>|0x049|0x02a:'54'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::pistol_weapon_core_aimed_fire_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair pistol_weapon_core_aimed_fire_state::get_weapon_lexeme_pair( vostok::mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// pcstr[2] 					weapon_animation_captions
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// 1
	// <0x7aaf39>|0x000|0x000:'60'
	// <0x7aaf40>|0x007|0x007:'61'
	// 1
	// <0x7aaf47>|0x00e|0x007:'63'
	// <0x7aaf57>|0x01e|0x010:'64'
	// <0x7aaf89>|0x050|0x032:'65'
	// 1
	// 2
	// 3
	// <0x7aaf95>|0x05c|0x00c:'69'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_aimed_fire_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression pistol_weapon_core_aimed_fire_state::get_user_hands_expression(
	vostok::animation::mixing::animation_lexeme&	weapon_lexeme,
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	vostok::animation::mixing::animation_lexeme&	weight_driving_animation) const
{
	// LOCALS
	// vostok::animation::mixing::animation_lexeme override_lexeme
	// u32 							user_animation_index
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// pcstr[2] 					user_animation_captions
	// ******

	// FUNCTION BODY
	// <0x7aade1>|0x000|0x000:'75'
	// <0x7aade7>|0x006|0x006:'76'
	// 1
	// <0x7aadf7>|0x016|0x010:'78'
	// <0x7aae03>|0x022|0x00c:'79'
	// <0x7aae23>|0x042|0x020:'80'
	// <0x7aae7c>|0x09b|0x059:'81'
	// 1
	// 2
	// <0x7aae8c>|0x0ab|0x010:'84'
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
	// <0x7aae9a>|0x0b9|0x00e:'96'
	// <0x7aaf08>|0x127|0x06e:'97'
	// ******
}

// STATE[STUB]
// survarium::pistol_weapon_core_aimed_fire_state* survarium::weapon_core_state_cook_template<survarium::pistol_weapon_core_aimed_fire_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
pistol_weapon_core_aimed_fire_state* weapon_core_state_cook_template<pistol_weapon_core_aimed_fire_state>::new_object(
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
	// <0x7aad69>|0x000|0x000:'109'
	// ******
}

} // namespace survarium
