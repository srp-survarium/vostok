////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "object_vegetation.h"

namespace survarium {

// STATE[STUB]
 object_vegetation::object_vegetation( base_game_scene& s ) :
	game_object_( s )
{
	// FUNCTION BODY[0x78dfb0]
	// <0x78dfb0>|0x000|      :'17'	{
	// ******
}

// STATE[STUB]
void object_vegetation::on_grass_loaded( resources::queries_result& data, boost::function< void( game_object_& ) >& cb )
{
	// FUNCTION BODY[0x78df10]: 2
	// <0x78df11>|0x001|+0x07f:'21'
	// <0x78df90>|0x080|+0x00a:'22'
	// ******
}

// STATE[STUB]
void object_vegetation::load(
	configs::binary_config_value const&		t,
	pcstr									project_resources_path,
	boost::function< void( game_object_& ) >&	cb
)
{
	// LOCALS
	// variant< 32 > 					ud
	// ******

	// FUNCTION BODY[0x78e060]: 13
	// <0x78e070>|0x010|+0x030:'27'
	// <0x78e0a0>|0x040|+0x003:'28'
	// <0x78e0a3>|0x043|+0x019:'29'
	// <0x78e0bc>|0x05c|+0x008:'30'
	// <0x78e0c4>|0x064|+0x005:'31'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x78e0c9>|0x069|+0x0c1:'39'
	// ******
}

// STATE[STUB]
void object_vegetation::insert( )
{
	// FUNCTION BODY[0x78e020]: 1
	// <0x78e023>|0x003|+0x03a:'45'
	// ******
}

// STATE[STUB]
void object_vegetation::remove( )
{
	// FUNCTION BODY[0x78dfe0]: 1
	// <0x78dfe3>|0x003|+0x03a:'50'
	// ******
}

} // namespace survarium
