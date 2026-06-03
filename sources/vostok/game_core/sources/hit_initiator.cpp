////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/hit_initiator.h>
#include <vostok/game_core/hit_info.h>

namespace survarium {

// STATE[UNCHECKED]
 hit_info::hit_info( )
{
	// FUNCTION BODY[0x73ea50]: 0
	// ******
}

// STATE[UNCHECKED]
 hit_info::hit_info(
	const u8					hit_initiator,
	const u8					being_hit,
	pcstr const					body_part_name,
	pcstr const					damage_type,
	const float					amount,
	const float					armor_piercing,
	survarium::bullet* const	bullet
) :
	body_part_name	( body_part_name ),
	damage_type		( damage_type ),
	bullet			( bullet ),
	amount			( amount ),
	armor_piercing	( armor_piercing ),
	hit_initiator	( hit_initiator ),
	being_hit		( being_hit )
{
	// FUNCTION BODY[0x73ea80]: 0
	// ******
}

// STATE[BLOCKED]
void hit_info::deserialize( network_core::packet_reader& packet )
{
	// LOCALS
	// char[16] 						damage_type_info
	// char[16] 						c_body_part_name
	// ******

	// FUNCTION BODY[0x73eaf0]: 15
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
	// ******
}

} // namespace survarium
