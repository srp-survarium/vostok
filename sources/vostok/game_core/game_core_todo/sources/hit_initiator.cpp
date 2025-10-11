////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "hit_initiator.h"

namespace survarium {

// STATE[STUB]
// survarium::hit_info::hit_info()
hit_info::hit_info( )
{
}

// STATE[STUB]
// survarium::hit_info::hit_info(const unsigned char, const unsigned char, char const* const, char const* const, const float, const float, survarium::bullet* const)
hit_info::hit_info(
	u8			hit_initiator,
	u8			being_hit,
	pcstr		body_part_name,
	pcstr		damage_type,
	float		amount,
	float		armor_piercing,
	bullet*		bullet)
{
}

// STATE[STUB]
// void survarium::hit_info::deserialize(vostok::network_core::packet_reader&)
void hit_info::deserialize( vostok::network_core::packet_reader& packet )
{
	// LOCALS
	// char[16] 					damage_type_info
	// char[16] 					c_body_part_name
	// ******

	// FUNCTION BODY
	// <0x73eafa>|0x000|0x000:'44'
	// <0x73eb08>|0x00e|0x00e:'45'
	// 1
	// 2
	// <0x73eb16>|0x01c|0x00e:'48'
	// <0x73eb22>|0x028|0x00c:'49'
	// 1
	// 2
	// <0x73eb2d>|0x033|0x00b:'52'
	// <0x73eb39>|0x03f|0x00c:'53'
	// 1
	// <0x73eb47>|0x04d|0x00e:'55'
	// <0x73eb57>|0x05d|0x010:'56'
	// 1
	// <0x73eb67>|0x06d|0x010:'58'
	// ******
}

} // namespace survarium
