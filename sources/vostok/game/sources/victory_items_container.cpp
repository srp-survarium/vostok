////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "victory_items_container.h"

namespace survarium {

// STATE[STUB]
victory_items_container::victory_items_container( base_game_scene& w ) :
	// ref member; the same-named param is the obvious source - a matcher
	// confirms when this TU is enabled
	m_game_scene( w )
{
	// FUNCTION BODY[0x73f310]: 0
	// <0x73f310>|0x000|+0x01b:'20'	{
	// <0x73f32b>|0x01b|      :'21'	}
	// ******
}

// STATE[STUB]
void victory_items_container::load( configs::binary_config_value const& cfg )
{
	// FUNCTION BODY[0x73f330]: 0
	// <0x73f330>|0x000|+0x000:'24'	{
	// <0x73f330>|0x000|      :'25'	}
	// ******
}

// STATE[STUB]
pcstr victory_items_container::use_info( usable_object_user_data* user )
{
	// CALL SITE INFO
	// <0x73f2b0> -> base_player* < unknown >()
	// <0x73f2bb> -> inventory_holder* < unknown >()
	// <0x73f2ca> -> game_team_id < unknown >() const
	// ******

	return NULL;

	// FUNCTION BODY[0x73f2a0]: 15
	// <0x73f2a0>|0x000|+0x003:'29'	{
	// <0x73f2a3>|0x003|+0x00f:'30'
	// <0x73f2b2>|0x012|+0x00d:'31'
	// <0>
	// <0x73f2bf>|0x01f|+0x002:'33'
	// <0x73f2c1>|0x021|+0x002:'34'
	// <0>
	// <0x73f2c3>|0x023|+0x009:'36'
	// <0x73f2cc>|0x02c|+0x009:'37'
	// <0>
	// <0x73f2d5>|0x035|+0x00d:'39'
	// <0x73f2e2>|0x042|+0x009:'40'
	// <0x73f2eb>|0x04b|+0x00e:'41'
	// <0x73f2f9>|0x059|+0x00c:'42'
	// <0>
	// <0x73f305>|0x065|-0x01d:'44'
	// <0x73f2e8>|0x048|+0x025:'45'
	// <0x73f30d>|0x06d|      :'45'	}
	// ******
}


} // namespace survarium
