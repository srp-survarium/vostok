////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/inventory_item.h>

#include <vostok/game_core/inventory_item_props.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

// STATE[78.8%|PARTIAL]: member-init shape matches; % gap is LTCG base-ctor inline. See inventory_item_ctor.md.
inventory_item::inventory_item( inventory_item::action_behaviour_type type ) :
	m_action_behaviuor	( type ),
	m_inventory			( NULL ),
	m_slot_id			( max_slots_count ),
	m_amount			( 0 ),
	m_dict_id			( 0 )
{
	// STRUCTURE DIFF: target 0 stmts / base 0 stmts
	// VERDICT: STRUCTURE MATCH (shape ok) - 0-stmt member-init, init order/offsets match; residual is LTCG base-ctor inline-vs-call, non-steerable. trail: inventory_item-ctor.md
}

// STATE[100%|DONE]
bool inventory_item::get_item_props( inventory_item_props& props )
{
	props.m_dict_id = m_dict_id;
	props.m_amount = m_amount;
	return false;
}

// STATE[69.33%|PARTIAL]: single append( m_amount ) ([+0x114] u16); client_offset unused (LTCG-dropped arg). Matches rva 0x590840.
void inventory_item::serialize( network_core::udp_match_packet& packet, u32 client_offset ) const
{
	VOSTOK_UNREFERENCED_PARAMETER( client_offset );
	packet.append( m_amount );

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// SIZE +0x9 | 41 | packet.append( m_amount );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE: target CALLS the append(u16) overload out-of-line (push value), base INLINES it down to the (&tmp, sizeof) append(pcvoid,u32) call; cross-module LTCG, non-steerable.
}

// STATE[46.91%|PARTIAL]: single r< u16 > into m_amount ([+0x114]). Matches rva 0x590810.
void inventory_item::deserialize( network_core::packet_reader& reader )
{
	m_amount = reader.r< u16 >( );

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// SIZE +0x18 | 52 | m_amount = reader.r< u16 >( );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE: target CALLS packet_reader::r<u16> out-of-line (LTCG this-in-EDX), base INLINES the read+advance; cross-module LTCG, non-steerable. Report None = paired at 0.0 (proto3 default omission), the whole 1-stmt body is this wall.
}

} // namespace survarium
