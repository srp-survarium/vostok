////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/hit_initiator.h>
#include <vostok/game_core/hit_info.h>

namespace survarium {

// STATE[STUB]
// survarium::hit_info::hit_info()
hit_info::hit_info( )
{
	// FUNCTION BODY
	// <0x73ea50>|0x000|+0x01c:'20'	{
	// <0x73ea6c>|0x01c|      :'21'	}
	// ******
}

// STATE[STUB]
// survarium::hit_info::hit_info(const unsigned char, const unsigned char, char const* const, char const* const, const float, const float, survarium::bullet* const)
hit_info::hit_info(
	u8					hit_initiator,
	u8					being_hit,
	pcstr				body_part_name,
	pcstr				damage_type,
	float				amount,
	float				armor_piercing,
	survarium::bullet*	bullet
)
{
	// FUNCTION BODY
	// <0x73ea80>|0x000|+0x059:'39'	{
	// <0x73ead9>|0x059|      :'40'	}
	// ******
}

// STATE[STUB]
// void survarium::hit_info::deserialize(vostok::network_core::packet_reader&)
void hit_info::deserialize( network_core::packet_reader& packet )
{
	// LOCALS
	// char[16] 					damage_type_info
	// char[16] 					c_body_part_name
	// ******

	// FUNCTION BODY
	// <0x73eaf0>|0x000|+0x00a:'43'	{
	// <0x73eafa>|0x00a|+0x00e:'44'
	// <0x73eb08>|0x018|+0x00e:'45'
	// <0>
	// <1>
	// <0x73eb16>|0x026|+0x00c:'48'
	// <0x73eb22>|0x032|+0x00b:'49'
	// <0>
	// <1>
	// <0x73eb2d>|0x03d|+0x00c:'52'
	// <0x73eb39>|0x049|+0x00e:'53'
	// <0>
	// <0x73eb47>|0x057|+0x010:'55'
	// <0x73eb57>|0x067|+0x010:'56'
	// <0>
	// <0x73eb67>|0x077|+0x00a:'58'
	// <0x73eb71>|0x081|      :'59'	}
	// ******
}

} // namespace survarium
