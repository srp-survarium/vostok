// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/inventory_item.h>

#include <vostok/game_core/inventory_item_props.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

inventory_item::inventory_item( inventory_item::action_behaviour_type type ) :
	m_action_behaviuor	( type ),
	m_inventory			( NULL ),
	m_slot_id			( max_slots_count ),
	m_amount			( 0 ),
	m_dict_id			( 0 )
{
}

bool inventory_item::get_item_props( inventory_item_props& props )
{
	props.m_dict_id = m_dict_id;
	props.m_amount = m_amount;
	return false;
}

void inventory_item::serialize( network_core::udp_match_packet& packet, u32 client_offset ) const
{
	VOSTOK_UNREFERENCED_PARAMETER( client_offset );
	packet.append( m_amount );
}

void inventory_item::deserialize( network_core::packet_reader& reader )
{
	m_amount = reader.r< u16 >( );
}

} // namespace survarium
