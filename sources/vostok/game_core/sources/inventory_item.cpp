////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/inventory_item.h>

namespace survarium {

// STATE[STUB]
inventory_item::inventory_item( inventory_item::action_behaviour_type type )
{
	// FUNCTION BODY
	// <0x5a0870>|0x000|      :'18'	{
	// ******
}

// STATE[STUB]
bool inventory_item::get_item_props( inventory_item_props& props )
{
	return false;
	// FUNCTION BODY
	// <0x5a07e0>|0x000|+0x007:'21'	{
	// <0x5a07e7>|0x007|+0x010:'22'
	// <0x5a07f7>|0x017|+0x011:'23'
	// <0x5a0808>|0x028|+0x002:'24'
	// <0x5a080a>|0x02a|      :'25'	}
	// ******
}

// STATE[STUB]
void inventory_item::serialize( network_core::udp_match_packet& packet, u32 client_offset ) const
{
	// FUNCTION BODY
	// <0x5a0840>|0x000|+0x009:'28'	{
	// <0>
	// <0x5a0849>|0x009|+0x013:'30'
	// <0x5a085c>|0x01c|      :'31'	}
	// ******
}

// STATE[STUB]
void inventory_item::deserialize( network_core::packet_reader& reader )
{
	// FUNCTION BODY
	// <0x5a0810>|0x000|+0x009:'34'	{
	// <0x5a0819>|0x009|+0x012:'35'
	// <0x5a082b>|0x01b|      :'36'	}
	// ******
}

} // namespace survarium
