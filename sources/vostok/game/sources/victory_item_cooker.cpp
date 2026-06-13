////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "victory_item_cook.h"

namespace survarium {

// STATE[STUB]
victory_item_cook::victory_item_cook( game_world& game_world ) :
	// ref member; the same-named param is the obvious source - a matcher
	// confirms when this TU is enabled
	m_game_world( game_world )
{
	// FUNCTION BODY[0x768fe0]: 0
	// <0x768fe0>|0x000|+0x01d:'17'	{
	// <0x768ffd>|0x01d|      :'18'	}
	// ******
}

// STATE[STUB]
victory_item_core* victory_item_cook::create_resource( )
{
	return NULL;

	// FUNCTION BODY[0x769220]: 1
	// <0x769220>|0x000|+0x003:'21'	{
	// <0x769223>|0x003|+0x020:'22'
	// <0x769243>|0x023|-0x001:'22'
	// <0x769242>|0x022|+0x004:'23'
	// <0x769246>|0x026|      :'23'	}
	// ******
}

// STATE[STUB]
void victory_item_cook::on_config_loaded( resources::queries_result& data )
{
	// LOCALS
	// configs::binary_config_ptr 		cfg
	// resources::query_result_for_cook* const parent
	// resources::request[1] 			requests
	// ******

	// CALL SITE INFO
	// <0x7692b1> -> victory_item_core* < unknown >()
	// <0x7692c7> -> void < unknown >( configs::binary_config_value const& )
	// ******

	// FUNCTION BODY[0x769250]: 22
	// <0x769259>|0x009|+0x00b:'27'
	// <0>
	// <0x769264>|0x014|+0x046:'29'
	// <0>
	// <0x7692aa>|0x05a|+0x009:'31'
	// <0>
	// <0x7692b3>|0x063|+0x016:'33'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7692c9>|0x079|+0x008:'38'
	// <0x7692d1>|0x081|+0x016:'39'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x7692e7>|0x097|+0x080:'48'
	// ******
}

// STATE[STUB]
void victory_item_cook::on_subresources_loaded( resources::queries_result& data, victory_item* object_to_cook )
{
	// LOCALS
	// resources::query_result_for_cook* const parent
	// ******

	// FUNCTION BODY[0x769010]: 14
	// <0x769010>|0x000|+0x00b:'52'	{
	// <0x76901b>|0x00b|+0x003:'53'
	// <0>
	// <0x76901e>|0x00e|+0x03d:'55'
	// <0>
	// <0x76905b>|0x04b|+0x0ab:'57'
	// <0x769106>|0x0f6|+0x018:'58'
	// <0>
	// <1>
	// <2>
	// <0x76911e>|0x10e|-0x0f7:'62'
	// <0>
	// <1>
	// <0x769027>|0x017|+0x1a2:'65'
	// <0x7691c9>|0x1b9|-0x0b9:'65'
	// <0x769110>|0x100|+0x0f3:'66'
	// <0x769203>|0x1f3|-0x0ee:'66'
	// <0x769115>|0x105|+0x0f9:'67'
	// <0x76920e>|0x1fe|      :'67'	}
	// ******
}


} // namespace survarium
