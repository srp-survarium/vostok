////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "object_light.h"

namespace survarium {

static u32 light_ids = 1000000;

// STATE[STUB]
 object_light::object_light( base_game_scene& w ) :
	game_object_static( w )
{
	m_light_id = ++light_ids;

	// FUNCTION BODY[0x5c3870]: 1
	// <0x5c3894>|0x024|+0x011:'20'
	// ******
}

// STATE[STUB]
 object_light::~object_light( )
{
	// FUNCTION BODY[0x5c3830]: 0
	// <0x5c3830>|0x000|+0x009:'24'	{
	// <0x5c3839>|0x009|      :'25'	}
	// ******
}

// STATE[STUB]
void object_light::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	// FUNCTION BODY[0x5c37f0]: 4
	// <0x5c37fa>|0x00a|+0x014:'29'
	// <0x5c380e>|0x01e|+0x00d:'30'
	// <0>
	// <0x5c381b>|0x02b|+0x00c:'32'
	// ******
}

// STATE[STUB]
void object_light::insert( )
{
	// FUNCTION BODY[0x5c38e0]: 1
	// <0x5c38e0>|0x000|+0x02d:'42'
	// ******
}

// STATE[STUB]
void object_light::remove( )
{
	// FUNCTION BODY[0x5c38b0]: 1
	// <0x5c38b0>|0x000|+0x026:'47'
	// ******
}

} // namespace survarium
