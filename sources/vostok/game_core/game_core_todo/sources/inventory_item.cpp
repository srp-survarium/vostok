////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "inventory_item.h"

namespace survarium {

// STATE[STUB]
// survarium::inventory_item::inventory_item(survarium::inventory_item::action_behaviour_type)
inventory_item::inventory_item( inventory_item::action_behaviour_type type )
{
}

// STATE[STUB]
// bool survarium::inventory_item::get_item_props(survarium::inventory_item_props&)
bool inventory_item::get_item_props( inventory_item_props& props )
{
	return false;
	// FUNCTION BODY
	// <0x5a07e7>|0x000|0x000:'22'
	// <0x5a07f7>|0x010|0x010:'23'
	// <0x5a0808>|0x021|0x011:'24'
	// ******
}

// STATE[STUB]
// void survarium::inventory_item::serialize(vostok::network_core::udp_match_packet&, unsigned int) const
void inventory_item::serialize( vostok::network_core::udp_match_packet& packet, u32 client_offset ) const
{
	// FUNCTION BODY
	// 1
	// <0x5a0849>|0x000|0x000:'30'
	// ******
}

// STATE[STUB]
// void survarium::inventory_item::deserialize(vostok::network_core::packet_reader&)
void inventory_item::deserialize( vostok::network_core::packet_reader& reader )
{
	// FUNCTION BODY
	// <0x5a0819>|0x000|0x000:'35'
	// ******
}

} // namespace survarium
