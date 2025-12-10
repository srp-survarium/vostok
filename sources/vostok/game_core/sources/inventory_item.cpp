////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/inventory_item.h>

namespace survarium {

// STATE[STUB]
inventory_item::inventory_item( inventory_item::action_behaviour_type type ) :
	m_action_behaviuor	( type ),
	m_inventory			( NULL ),
	m_slot_id			( max_slots_count ),
	m_amount			( 0 ),
	m_dict_id			( 0 )
{
}

// STATE[STUB]
bool inventory_item::get_item_props( inventory_item_props& props )
{
	props->m_dict_id = m_dict_id;
	props->m_amount = m_amount;
	return false;

	// FUNCTION BODY
	// <0x5a07e7>|0x007|+0x010:'22'
	// <0x5a07f7>|0x017|+0x011:'23'
	// <0x5a0808>|0x028|+0x002:'24'
	// ******
}

// STATE[BLOCKED]
void inventory_item::serialize( network_core::udp_match_packet& packet, u32 client_offset ) const
{
	// FUNCTION BODY
	// <0>
	// <0x5a0849>|0x009|+0x013:'30'
	// ******
}

// STATE[BLOCKED]
void inventory_item::deserialize( network_core::packet_reader& reader )
{
	// FUNCTION BODY
	// <0x5a0819>|0x009|+0x012:'35'
	// ******
}

} // namespace survarium
