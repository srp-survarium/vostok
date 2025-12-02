////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/usable_object.h>

namespace survarium {

// STATE[STUB]
// survarium::usable_object::usable_object()
usable_object::usable_object( )
{
	// FUNCTION BODY
	// <0x5a0960>|0x000|+0x085:'17'	{
	// <0x5a09e5>|0x085|      :'18'	}
	// ******
}

// STATE[STUB]
// void survarium::usable_object::~usable_object()
usable_object::~usable_object( )
{
	// FUNCTION BODY
	// <0x5a0b00>|0x000|+0x01c:'21'	{
	// <0x5a0b1c>|0x01c|+0x03d:'22'
	// <0x5a0b59>|0x059|      :'23'	}
	// ******
}

// STATE[STUB]
// void survarium::usable_object::load(vostok::configs::binary_config_value const&)
void usable_object::load( configs::binary_config_value const& cfg )
{
	// LOCALS
	// configs::binary_config_value collision_table
	// ******

	// FUNCTION BODY
	// <0x5a0b80>|0x000|+0x00a:'26'	{
	// <0x5a0b8a>|0x00a|+0x030:'27'
	// <0x5a0bba>|0x03a|+0x00e:'28'
	// <0x5a0bc8>|0x048|+0x00c:'29'
	// <0>
	// <0x5a0bd4>|0x054|+0x022:'31'
	// <0x5a0bf6>|0x076|      :'32'	}
	// ******
}

// STATE[STUB]
// void survarium::usable_object::resolve_links(survarium::base_project*, vostok::configs::binary_config_value)
void usable_object::resolve_links( base_project* p, configs::binary_config_value cfg )
{
	// LOCALS
	// configs::binary_config_value collision_table
	// u32 							i<1>
	// pcstr 						geom_name<2>
	// ******

	// CALL SITE INFO
	// <0x5a0ac4> -> base_game_object* <unknown>(pcstr)
	// ******

	// FUNCTION BODY
	// <0x5a0a40>|0x000|+0x009:'35'	{
	// <0x5a0a49>|0x009|+0x030:'36'
	// <0x5a0a79>|0x039|+0x01d|[1]:'37'
	// <0>
	// <0x5a0a96>|0x056|+0x01f|[2]:'39'
	// <0x5a0ab5>|0x075|+0x03d:'40'
	// <0>
	// <0x5a0af2>|0x0b2|      :'42'	}
	// ******
}

// STATE[STUB]
// void survarium::usable_object::insert(vostok::physics::world*)
void usable_object::insert( physics::world* world )
{
	// LOCALS
	// u32 							i<1>
	// ******

	// FUNCTION BODY
	// <0x5a09f0>|0x000|+0x009:'45'	{
	// <0x5a09f9>|0x009|+0x01d|[1]:'46'
	// <0x5a0a16>|0x026|+0x01b:'47'
	// <0x5a0a31>|0x041|      :'48'	}
	// ******
}

// STATE[STUB]
// void survarium::usable_object::remove()
void usable_object::remove( )
{
	// LOCALS
	// u32 							i<1>
	// ******

	// FUNCTION BODY
	// <0x5a0910>|0x000|+0x009:'51'	{
	// <0x5a0919>|0x009|+0x01d|[1]:'52'
	// <0x5a0936>|0x026|+0x017:'53'
	// <0x5a094d>|0x03d|      :'54'	}
	// ******
}

// STATE[STUB]
// vostok::math::float4x4 survarium::usable_object::get_transform()
float4x4 usable_object::get_transform( )
{
	// CALL SITE INFO
	// <0x5a0900> -> float4x4 <unknown>()
	// ******

	return vostok::math::float4x4();
	// FUNCTION BODY
	// <0x5a08e0>|0x000|+0x007:'57'	{
	// <0x5a08e7>|0x007|+0x01e:'58'
	// <0x5a0905>|0x025|      :'59'	}
	// ******
}

} // namespace survarium
