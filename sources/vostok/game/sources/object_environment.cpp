////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "object_environment.h"

namespace survarium {

// STATE[STUB]
 object_environment::object_environment( base_game_scene& w ) :
	game_object_( w )
{
	// FUNCTION BODY[0x5c3a00]: 0
	// <0x5c3a00>|0x000|+0x023:'17'	{
	// <0x5c3a23>|0x023|      :'18'	}
	// ******
}

// STATE[STUB]
 object_environment::~object_environment( )
{
	// FUNCTION BODY[0x5c39c0]: 0
	// <0x5c39c0>|0x000|+0x009:'21'	{
	// <0x5c39c9>|0x009|      :'22'	}
	// ******
}

// STATE[STUB]
void object_environment::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	// FUNCTION BODY[0x5c3a30]: 3
	// <0x5c3a3e>|0x00e|+0x00f:'26'
	// <0>
	// <0x5c3a4d>|0x01d|+0x098:'28'
	// ******
}

// STATE[STUB]
void object_environment::material_ready( resources::queries_result& data, boost::function< void( game_object_& ) >& cb )
{
	// FUNCTION BODY[0x5c3920]: 2
	// <0x5c3921>|0x001|+0x07f:'33'
	// <0x5c39a0>|0x080|+0x00a:'34'
	// ******
}

// STATE[STUB]
void object_environment::remove( )
{
	// FUNCTION BODY[0x5c3910]: 0
	// <0x5c3910>|0x000|+0x000:'38'	{
	// <0x5c3910>|0x000|      :'39'	}
	// ******
}

// STATE[STUB]
void object_environment::insert( )
{
	// FUNCTION BODY[0x5c3af0]: 1
	// <0x5c3af0>|0x000|+0x026:'43'
	// ******
}

} // namespace survarium
