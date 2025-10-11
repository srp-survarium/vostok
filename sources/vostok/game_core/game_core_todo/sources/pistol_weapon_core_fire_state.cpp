////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "pistol_weapon_core_fire_state.h"

namespace survarium {

// STATE[STUB]
// survarium::pistol_weapon_core_fire_state::pistol_weapon_core_fire_state(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
pistol_weapon_core_fire_state::pistol_weapon_core_fire_state(
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
	// <0x7ab21b><2>
	// <0x7ab233><3>
	// <0x7ab2a3><3>
	// ******

	// FUNCTION BODY
	// <0x7ab1db>|0x000|0x000:'26'
	// <0x7ab1fe>|0x023|0x023:'27'
	// <0x7ab205>|0x02a|0x007|[1]:'28'
	// <0x7ab21d>|0x042|0x018:'29'
	// <0x7ab235>|0x05a|0x018:'30'
	// <0x7ab24d>|0x072|0x018:'31'
	// <0x7ab287>|0x0ac|0x03a:'32'
	// <0x7ab289>|0x0ae|0x002:'33'
	// <0x7ab28b>|0x0b0|0x002:'34'
	// <0x7ab28d>|0x0b2|0x002|[2]:'35'
	// <0x7ab2a5>|0x0ca|0x018:'36'
	// <0x7ab2bd>|0x0e2|0x018:'37'
	// <0x7ab2ee>|0x113|0x031:'38'
	// <0x7ab2f0>|0x115|0x002:'39'
	// <0x7ab2f2>|0x117|0x002:'40'
	// ******
}

// STATE[STUB]
// void survarium::pistol_weapon_core_fire_state::initialize()
void pistol_weapon_core_fire_state::initialize( )
{
	// LOCALS
	// bool 						last_shot
	// ******

	// FUNCTION BODY
	// <0x7ab719>|0x000|0x000:'45'
	// <0x7ab721>|0x008|0x008:'46'
	// <0x7ab77e>|0x065|0x05d:'47'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_fire_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression pistol_weapon_core_fire_state::weapon_and_hands_expression(
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
	// <0x7ab5c1>|0x000|0x000:'52'
	// 1
	// <0x7ab5e0>|0x01f|0x01f:'54'
	// <0x7ab5e6>|0x025|0x006:'55'
	// 1
	// <0x7ab644>|0x083|0x05e:'57'
	// 1
	// <0x7ab656>|0x095|0x012:'59'
	// <0x7ab680>|0x0bf|0x02a:'60'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::pistol_weapon_core_fire_state::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair pistol_weapon_core_fire_state::get_weapon_lexeme_pair( vostok::mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// LOCALS
	// pcstr[2] 					weapon_animation_captions
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& selected_animation
	// pcstr 						animation_identifier
	// ******

	// FUNCTION BODY
	// 1
	// <0x7ab4e9>|0x000|0x000:'66'
	// <0x7ab4f0>|0x007|0x007:'67'
	// 1
	// <0x7ab4f7>|0x00e|0x007:'69'
	// <0x7ab507>|0x01e|0x010:'70'
	// <0x7ab539>|0x050|0x032:'71'
	// 1
	// 2
	// 3
	// <0x7ab545>|0x05c|0x00c:'75'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::pistol_weapon_core_fire_state::get_user_hands_expression(vostok::animation::mixing::animation_lexeme&, vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression pistol_weapon_core_fire_state::get_user_hands_expression(
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
	// <0x7ab391>|0x000|0x000:'80'
	// <0x7ab397>|0x006|0x006:'81'
	// 1
	// <0x7ab3a7>|0x016|0x010:'83'
	// <0x7ab3b3>|0x022|0x00c:'84'
	// <0x7ab3d3>|0x042|0x020:'85'
	// <0x7ab42c>|0x09b|0x059:'86'
	// 1
	// 2
	// <0x7ab43c>|0x0ab|0x010:'89'
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
	// <0x7ab44a>|0x0b9|0x00e:'101'
	// <0x7ab4b8>|0x127|0x06e:'102'
	// ******
}

// STATE[STUB]
// survarium::pistol_weapon_core_fire_state* survarium::weapon_core_state_cook_template<survarium::pistol_weapon_core_fire_state>::new_object(vostok::mutable_buffer, survarium::weapon_state_creation_params const*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int)
pistol_weapon_core_fire_state* weapon_core_state_cook_template<pistol_weapon_core_fire_state>::new_object(
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
	// <0x7ab319>|0x000|0x000:'114'
	// ******
}

} // namespace survarium
