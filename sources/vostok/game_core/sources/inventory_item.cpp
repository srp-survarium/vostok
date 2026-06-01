////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/inventory_item.h>

#include <vostok/game_core/inventory_item_props.h>

namespace survarium {

// STATE[78.8%|PARTIAL]: member-init body byte-exact; only divergence is the
// interactive_object base ctor - target emits `call interactive_object::interactive_object()`
// (an out-of-line base ctor), base inlines unmanaged_resource(1)+vtable instead.
// claude@NOTE: target ctor calls interactive_object::interactive_object() (rva 0x9ccb0),
// proving interactive_object has a user-declared out-of-line default ctor. base has no
// declared ctor so the trivial base init is inlined here (push 1 / mov [eax],0). Fixing
// this requires giving interactive_object an out-of-line ctor (separate function, wide
// blast radius across every interactive_object-derived class) - out of scope for this
// leaf. The remaining `~` rows are pure ecx/eax/edx register + push/pop esi LTCG noise.
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
