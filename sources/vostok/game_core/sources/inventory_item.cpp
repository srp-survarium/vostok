////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/inventory_item.h>

#include <vostok/game_core/inventory_item_props.h>

namespace survarium {

// STATE[78.8%|PARTIAL]: member-init shape matches; % gap is LTCG base-ctor inline. See inventory_item_ctor.md.
// STRUCTURE DIFF[target 0x590870 | base 0x449c20]: target 0 / base 0 stmts
// .. same ..
// ; aligned 0, size-diffs 0, quantity-diffs 0
// Both sides are 0-statement (member-init only); field-init order + offsets match
// (+108 type, +10C inventory=0, +110 slot=0x13, +114 amount=0, +116 dict_id=0).
// The 78.8% gap is NOT a structure divergence: target CALLs
// interactive_object::interactive_object for the base subobject, our base build
// INLINEs it (push 1 + unmanaged_resource::unmanaged_resource + ??_7interactive_object
// vtable store) - an LTCG inline-vs-call artifact of the base ctor, not this ctor's shape.
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
