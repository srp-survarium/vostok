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
// STRUCTURE DIFF:
// target: 0x590870            base: 0x449c20
// ; survarium::inventory_item::inventory_item(survarium::inventory_item::action_behaviour_type) ; target 0 stmts / base 0 stmts
// ; aligned 0, size-diffs 0, quantity-diffs 0
// VERDICT: STRUCTURE MATCH - both 0-stmt member-init, init order/offsets match; 78.8% gap is LTCG base-ctor inline-vs-call, not this ctor's shape  trail: inventory_item-ctor.md
inventory_item::inventory_item( inventory_item::action_behaviour_type type ) :
	m_action_behaviuor	( type ),
	m_inventory			( NULL ),
	m_slot_id			( max_slots_count ),
	m_amount			( 0 ),
	m_dict_id			( 0 )
{
}

// STATE[100%|DONE]
bool inventory_item::get_item_props( inventory_item_props& props )
{
	props.m_dict_id = m_dict_id;
	props.m_amount = m_amount;
	return false;

	// FUNCTION BODY
	// <0x5a07e7>|0x007|+0x010:'22'
	// <0x5a07f7>|0x017|+0x011:'23'
	// <0x5a0808>|0x028|+0x002:'24'
	// ******
}

// STATE[PARTIAL]: single append( m_amount ) ([+0x114] u16); client_offset unused (LTCG-dropped arg). Matches rva 0x590840.
void inventory_item::serialize( network_core::udp_match_packet& packet, u32 client_offset ) const
{
	VOSTOK_UNREFERENCED_PARAMETER( client_offset );
	packet.append( m_amount );

	// STRUCTURE DIFF[target 0x590840 | base 0x450230]: target 1 / base 1 stmts
	//   1: 0x009 <0x13> | 0x009 <0x1c> | packet.append( m_amount );   SIZE
	// ; aligned 0, size-diffs 1, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is packet<T>::append LTCG inline (target) vs call (base), non-steerable.
}

// STATE[PARTIAL]: single r< u16 > into m_amount ([+0x114]). Matches rva 0x590810.
void inventory_item::deserialize( network_core::packet_reader& reader )
{
	m_amount = reader.r< u16 >( );

	// STRUCTURE DIFF[target 0x590810 | base 0x4501f0]: target 1 / base 1 stmts
	//   1: 0x009 <0x12> | 0x009 <0x2a> | m_amount = reader.r< u16 >( );   SIZE
	// ; aligned 0, size-diffs 1, quantity-diffs 0, blank-gaps 0
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is packet_reader::r<u16> LTCG inline (target) vs call (base), non-steerable.
}

} // namespace survarium
