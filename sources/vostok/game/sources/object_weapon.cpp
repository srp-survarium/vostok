////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "object_weapon.h"

namespace survarium {

// STATE[STUB]
 object_weapon::object_weapon(
	ai::weapon_types_enum		type,
	pcstr						name,
	u32							id,
	u32							ammo_count
)
{
	// FUNCTION BODY[0x5be3c0]: 0
	// <0x5be3c0>|0x000|+0x054:'19'	{
	// <0x5be414>|0x054|      :'20'	}
	// ******
}

// STATE[STUB]
 object_weapon::~object_weapon( )
{
	// FUNCTION BODY[0x5be370]: 0
	// <0x5be370>|0x000|+0x00d:'23'	{
	// <0x5be37d>|0x00d|      :'24'	}
	// ******
}

// STATE[STUB]
float3 object_weapon::get_random_surface_point( const u32 current_time ) const
{
	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ad360 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// ******

	return vostok::math::float3(1., 1., 1.);

	// FUNCTION BODY[0x5be360]
	// <0x5be360>|0x000|      :'27'	{
	// ******
}

// STATE[STUB]
float4x4 object_weapon::local_to_cell( float3 const& requester ) const
{
	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ad350 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// ******

	return vostok::math::float4x4();

	// FUNCTION BODY[0x5be350]
	// <0x5be350>|0x000|      :'33'	{
	// ******
}

} // namespace survarium
