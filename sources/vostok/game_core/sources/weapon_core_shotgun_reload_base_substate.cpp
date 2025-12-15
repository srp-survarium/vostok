////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_shotgun_reload_base_substate.h>

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_shotgun_reload_base_substate::weapon_core_shotgun_reload_base_substate(survarium::weapon_core&, const float, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const*, const unsigned int, const vostok::animation::mixing::playback_enum, const unsigned int, char const*, char const*, char const*, char const*)
weapon_core_shotgun_reload_base_substate::weapon_core_shotgun_reload_base_substate(
	weapon_core&						weapon,
	float								animation_time_scale,
	resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const*	animations,
	u32									animations_count,
	animation::mixing::playback_enum	playback_type,
	u32									time_synchronization_group,
	pcstr								animation_id,
	pcstr								hands_stand_animation_id,
	pcstr								hands_crouch_animation_id,
	pcstr								hands_jump_animation_id
)
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
	// <0x7a865c>|0x0bc|+0x00c:'36'
	// <0x7a8668>|0x0c8|+0x00c:'37'
	// <0x7a8674>|0x0d4|+0x00c:'38'
	// <0>
	// <0x7a8680>|0x0e0|+0x023:'40'
	// <0x7a86a3>|0x103|+0x007:'41'
	// <0x7a86aa>|0x10a|+0x018|[1]:'42'
	// <0x7a86c2>|0x122|+0x018:'43'
	// <0x7a86da>|0x13a|+0x031:'44'
	// <0x7a870b>|0x16b|+0x002:'45'
	// <0x7a870d>|0x16d|+0x002:'46'
	// <0x7a870f>|0x16f|+0x018|[2]:'47'
	// <0x7a8727>|0x187|+0x018:'48'
	// <0x7a873f>|0x19f|+0x031:'49'
	// <0x7a8770>|0x1d0|+0x002:'50'
	// <0x7a8772>|0x1d2|+0x002:'51'
	// <0x7a8774>|0x1d4|+0x00c:'52'
	// ******
}

// STATE[STUB]
// survarium::weapon_lexeme_pair survarium::weapon_core_shotgun_reload_base_substate::get_weapon_lexeme_pair(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum) const
weapon_lexeme_pair weapon_core_shotgun_reload_base_substate::get_weapon_lexeme_pair( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const
{
	// FUNCTION BODY
	// <0x7a8799>|0x009|+0x02f:'57'
	// <0>
	// <1>
	// <0x7a87c8>|0x038|+0x072:'60'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_shotgun_reload_base_substate::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression weapon_core_shotgun_reload_base_substate::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	// LOCALS
	// animation::mixing::animation_lexeme override_lexeme
	// u32 							user_state_index
	// weapon_lexeme_pair 			lexeme_pair
	// ******

	// FUNCTION BODY
	// <0x7a8851>|0x011|+0x01f:'69'
	// <0>
	// <0x7a8870>|0x030|+0x00f:'71'
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
	// <0x7a887f>|0x03f|+0x086:'83'
	// <0x7a8905>|0x0c5|+0x095:'84'
	// ******
}

} // namespace survarium
