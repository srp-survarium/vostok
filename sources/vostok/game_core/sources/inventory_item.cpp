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
// target: 0x590870            base: 0x449760
// ; survarium::inventory_item::inventory_item(survarium::inventory_item::action_behaviour_type) ; target 0 stmts / base 0 stmts
// ; aligned 0, size-diffs 0, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - 0-stmt member-init, init order/offsets match; residual is LTCG base-ctor inline-vs-call, non-steerable. trail: inventory_item-ctor.md
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
}

// STATE[69.33%|PARTIAL]: single append( m_amount ) ([+0x114] u16); client_offset unused (LTCG-dropped arg). Matches rva 0x590840.
void inventory_item::serialize( network_core::udp_match_packet& packet, u32 client_offset ) const
{
	VOSTOK_UNREFERENCED_PARAMETER( client_offset );
	packet.append( m_amount );

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// b.diff   |t.addr  |b.addr  |t.sz|b.sz|b.ln|b.code
	// ---------+--------+--------+----+----+----+------
	// SIZE +0x9|0x590849|0x4630c9|0x13|0x1c|0   |packet.append( m_amount );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is packet<T>::append LTCG inline (target) vs call (base), non-steerable.
}

// STATE[46.91%|PARTIAL]: single r< u16 > into m_amount ([+0x114]). Matches rva 0x590810.
void inventory_item::deserialize( network_core::packet_reader& reader )
{
	m_amount = reader.r< u16 >( );

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// b.diff   |t.addr  |b.addr  |t.sz|b.sz|b.ln|b.code
	// ---------+--------+--------+----+----+----+------
	// SIZE +0xc|0x590819|0x463099|0x12|0x1e|0   |m_amount = reader.r< u16 >( );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the target's LTCG-folded r<u16> call vs the base's inlined wrapper + inner r() call, non-steerable.
}

} // namespace survarium
