////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon_core_shotgun_reload_base_substate.h"

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_shotgun_reload_base_substate::weapon_core_shotgun_reload_base_substate(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int, const vostok::animation::mixing::playback_enum, const unsigned int, char const*, char const*, char const*, char const*)
weapon_core_shotgun_reload_base_substate::weapon_core_shotgun_reload_base_substate(
	weapon_core&						weapon,
	float								animation_time_scale,
	vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*	animations,
	u32									animations_count,
	vostok::animation::mixing::playback_enum	playback_type,
	u32									time_synchronization_group,
	pcstr								animation_id,
	pcstr								hands_stand_animation_id,
	pcstr								hands_crouch_animation_id,
	pcstr								hands_jump_animation_id)
{
	// LOCALS
	// u32 							animation_index
	// u32 							view_index<1>
	// u32 							user_state_index<2>
	// u32 							view_index<2>
	// u32 							user_state_index<3>
	// ******

	// SKIPPED BLOCKS
	// <0x7a86c0><2>
	// <0x7a8725><3>
	// ******

	// FUNCTION BODY
	// <0x7a865c>|0x000|0x000:'36'
	// <0x7a8668>|0x00c|0x00c:'37'
	// <0x7a8674>|0x018|0x00c:'38'
	// 1
	// <0x7a8680>|0x024|0x00c:'40'
	// <0x7a86a3>|0x047|0x023:'41'
	// <0x7a86aa>|0x04e|0x007|[1]:'42'
	// <0x7a86c2>|0x066|0x018:'43'
	// <0x7a86da>|0x07e|0x018:'44'
	// <0x7a870b>|0x0af|0x031:'45'
	// <0x7a870d>|0x0b1|0x002:'46'
	// <0x7a870f>|0x0b3|0x002|[2]:'47'
	// <0x7a8727>|0x0cb|0x018:'48'
	// <0x7a873f>|0x0e3|0x018:'49'
	// <0x7a8770>|0x114|0x031:'50'
	// <0x7a8772>|0x116|0x002:'51'
	// <0x7a8774>|0x118|0x002:'52'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::weapon_core_shotgun_reload_base_substate::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair weapon_core_shotgun_reload_base_substate::get_weapon_lexeme_pair( vostok::mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// FUNCTION BODY
	// <0x7a8799>|0x000|0x000:'57'
	// 1
	// 2
	// <0x7a87c8>|0x02f|0x02f:'60'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_shotgun_reload_base_substate::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression weapon_core_shotgun_reload_base_substate::weapon_and_hands_expression(
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	vostok::animation::mixing::animation_lexeme&	weight_driving_animation) const
{
	// LOCALS
	// vostok::animation::mixing::animation_lexeme override_lexeme
	// u32 							user_state_index
	// weapon_lexeme_pair 			lexeme_pair
	// ******

	// FUNCTION BODY
	// <0x7a8851>|0x000|0x000:'69'
	// 1
	// <0x7a8870>|0x01f|0x01f:'71'
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
	// <0x7a887f>|0x02e|0x00f:'83'
	// <0x7a8905>|0x0b4|0x086:'84'
	// ******
}

} // namespace survarium
