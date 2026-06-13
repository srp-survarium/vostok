////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "generic_anomaly.h"

namespace survarium {

// STATE[STUB]
generic_anomaly::generic_anomaly( base_game_scene& w ) :
	// ref member; the same-named param is the obvious source - a matcher
	// confirms when this TU is enabled
	m_game_scene( w )
{
	// FUNCTION BODY[0x5c3630]
	// <0x5c3630>|0x000|      :'23'	{
	// ******
}

// STATE[STUB]
void generic_anomaly::load( configs::binary_config_value const& config )
{
	// FUNCTION BODY[0x5c3650]: 0
	// <0x5c3650>|0x000|+0x000:'26'	{
	// <0x5c3650>|0x000|      :'27'	}
	// ******
}

// STATE[STUB]
// class-elaborated: the inherited scheduler() accessor hides the type name
void generic_anomaly::activate( physics::world* world, class scheduler& scheduler )
{
	// FUNCTION BODY[0x5c3670]: 0
	// <0x5c3670>|0x000|+0x000:'34'	{
	// <0x5c3670>|0x000|      :'35'	}
	// ******
}

// STATE[STUB]
void generic_anomaly::deactivate( )
{
	// FUNCTION BODY[0x5c3620]: 0
	// <0x5c3620>|0x000|+0x000:'45'	{
	// <0x5c3620>|0x000|      :'46'	}
	// ******
}

// STATE[STUB]
void generic_anomaly::tick( const u32 time_delta_ms, const u32 current_time_ms )
{
	// FUNCTION BODY[0x5c3660]: 0
	// <0x5c3660>|0x000|+0x000:'54'	{
	// <0x5c3660>|0x000|      :'55'	}
	// ******
}


} // namespace survarium
