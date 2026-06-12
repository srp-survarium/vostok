////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\empty_hands.h"

namespace survarium {

// STATE[STUB]
 empty_hands::empty_hands( resources::managed_resource_ptr* const animations, const u32 animations_count )
{
	// FUNCTION BODY[0x5bca70]: 1
	// <0x5bca9c>|0x02c|+0x011:'28'
	// ******
}

// STATE[STUB]
void empty_hands::activate( base_player& user, engine& engine )
{
	// CALL SITE INFO
	// <0x5bca56> -> float4x4 const& < unknown >() const
	// ******

	// FUNCTION BODY[0x5bca40]: 3
	// <0>
	// <0x5bca40>|0x000|+0x008:'34'
	// <0x5bca48>|0x008|+0x021:'35'
	// ******
}

// STATE[STUB]
animation::mixing::expression empty_hands::selected_animations( mutable_buffer& buffer, const bool is_third_view ) const
{
	// LOCALS
	// animation::mixing::animation_lexeme lexeme
	// animation::linear_interpolator 	l_interpolator
	// ******

	// FUNCTION BODY[0x5bcac0]: 24
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
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <0x5bcad0>|0x010|+0x13e:'61'
	// <0>
	// <0x5bcc0e>|0x14e|+0x017:'63'
	// ******
}

// STATE[STUB]
void empty_hands::update_bones_matrices(
	animation::skeleton_ptr const&			user_skeleton,
	float4x4* const							user_matrices,
	const u32								user_matrices_count,
	const u32								current_time_in_ms,
	float4x4&								character_head_transform,
	float4x4&								character_transform,
	animation::animation_player const&		animation_player
)
{
	// LOCALS
	// float4x4 const& 					user_transform
	// ******

	// CALL SITE INFO
	// <0x5bcc83> -> float4x4 const& < unknown >() const
	// ******

	// FUNCTION BODY[0x5bcc30]: 12
	// <0>
	// <1>
	// <0x5bcc3a>|0x00a|+0x026:'78'
	// <0x5bcc60>|0x030|+0x014:'79'
	// <0>
	// <0x5bcc74>|0x044|+0x011:'81'
	// <0>
	// <0x5bcc85>|0x055|+0x05d:'83'
	// <0x5bcce2>|0x0b2|+0x01b:'84'
	// <0>
	// <0x5bccfd>|0x0cd|+0x00f:'86'
	// <0x5bcd0c>|0x0dc|+0x081:'87'
	// ******
}

} // namespace survarium
