////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "hit_type_parameters.h"

namespace survarium {

// STATE[STUB]
// survarium::hit_type_parameters::hit_type_parameters(char const*, const float, const float, float, const unsigned int)
hit_type_parameters::hit_type_parameters(
	pcstr		type,
	float		absorption,
	float		armor,
	float		reduce,
	u32			bdb_count)
{
}

// STATE[STUB]
// void survarium::hit_type_parameters::apply_damage(const float, const unsigned int)
void hit_type_parameters::apply_damage( float delta, u32 time_in_ms )
{
	// LOCALS
	// std::pair<body_part_parameters *,float>* it_begin
	// std::pair<body_part_parameters *,float>* it_end
	// std::pair<body_part_parameters *,float>* it<1>
	// ******

	// FUNCTION BODY
	// <0x596bc9>|0x000|0x000:'26'
	// <0x596bde>|0x015|0x015:'27'
	// 1
	// <0x596bed>|0x024|0x00f|[1]:'29'
	// <0x596c06>|0x03d|0x019:'30'
	// <0x596c17>|0x04e|0x011:'31'
	// ******
}

// STATE[STUB]
// void survarium::hit_type_parameters::set_parameters(float, float, float)
void hit_type_parameters::set_parameters( float armor, float reduce, float absorbtion )
{
	// FUNCTION BODY
	// <0x596b17>|0x000|0x000:'46'
	// <0x596b24>|0x00d|0x00d:'47'
	// <0x596b31>|0x01a|0x00d:'48'
	// ******
}

} // namespace survarium
