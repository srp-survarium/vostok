////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/inventory.h>

#include <vostok/game_core/base_player.h>
#include <vostok/game_core/inventory_holder.h>
#include <vostok/game_core/inventory_item.h>
#include <vostok/game_core/items_dictionary.h>
#include <vostok/game_core/victory_item_core.h>
#include <vostok/game_core/profile_slot.h>
#include <vostok/game_core/player_profile.h>
#include <vostok/game_core/weapon_core.h>
#include <vostok/game_core/profile_slots.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <boost/bind.hpp>

namespace survarium {

// Equipment slots whose items carry no per-frame network state and are skipped by the
// item serialize/deserialize sweeps.
static profile_slot_enum ignored_slots_for_serialization[ 7 ] = {
	helmet_slot,
	mask_slot,
	torso_slot,
	back_slot,
	pants_slot,
	gloves_slot,
	boots_slot,
};

// sushi@TODO: Maybe move somewhere else.
profile_slot_enum weapon_slots[WEAPON_COUNT] = {
	weapon1_slot,
	weapon2_slot,
};
profile_slot_enum ammunition_slots[AMMO_COUNT] = {
	ammo1_weapon1_slot,
	ammo1_weapon2_slot,
	ammo2_weapon1_slot,
	ammo2_weapon2_slot,
};
profile_slot_enum item_slots[ITEM_COUNT] = {
	helmet_slot,
	mask_slot,
	torso_slot,
	back_slot,
	pants_slot,
	gloves_slot,
	boots_slot,
	quick_slot1,
	quick_slot2,
	quick_slot3,
	quick_slot4,
	quick_slot5,
	quick_slot6,
};

// STATE[100%|DONE]
inventory::inventory( ) :
	m_active_slot	( max_slots_count ),
	m_holder		( NULL ),
	m_victory_item	( NULL )
{
}


// STATE[71.08%|PARTIAL]: intrusive_ptr operator T*/operator* inline-vs-call (LTCG), non-steerable
void inventory::set_holder( inventory_holder* holder )
{
	ASSERT( UNKNOWN_EXPRESSION_T( !m_holder ) );
	m_holder = holder;

	for ( u32 i = 0 ; i < max_slots_count ; ++i )
		if ( m_slots[i].item )
			m_slots[i].item->holder_assigned( );

	// STRUCTURE DIFF[target 0x6f0420 | base]: target 6 / base 6 stmts
	// .. same ..
	// 0x039 <0x16> | 0x039 <0x26> | if ( m_slots[i].item )   SIZE
	// 0x04f <0x22> | 0x05f <0x2f> | m_slots[i].item->holder_assigned( );   SIZE
	// ; aligned 4, size-diffs 2, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - SIZE diffs are intrusive_ptr operator T*()/operator*() inline-vs-call (LTCG); target calls out-of-line, base inlines. trail: set_holder.md
}

// STATE[70.28%|PARTIAL]: intrusive_ptr operator T*/operator* inline-vs-call (LTCG), non-steerable
void inventory::unset_holder( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_holder ) );

	for ( u32 i = 0 ; i < max_slots_count ; ++i )
		if ( m_slots[i].item )
			m_slots[i].item->holder_removed( );

	m_holder = NULL;

	// STRUCTURE DIFF[target 0x6f03a0 | base]: target 7 / base 7 stmts
	// .. same ..
	// 0x02d <0x16> | 0x02d <0x26> | if ( m_slots[i].item )   SIZE
	// 0x043 <0x22> | 0x053 <0x2f> | m_slots[i].item->holder_removed( );   SIZE
	// .. same ..
	// ; aligned 5, size-diffs 2, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - SIZE diffs are intrusive_ptr operator T*()/operator*() inline-vs-call (LTCG); for-loop braces dropped to match target (was the INCORRECT tag). trail: unset_holder.md
}

// STATE[69.33%|PARTIAL]: intrusive_ptr operator= and operator* inline-vs-call (LTCG), non-steerable
void inventory::set_item(
	const profile_slot_enum			slot,
	inventory_item_ptr const&		item
)
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	m_slots[slot].item = item.c_ptr();	// sushi@MATCH: target does additional copying
	item->set_inventory( this, slot );	// sushi@MATCH: -> inlined in target

	// STRUCTURE DIFF[target 0x6f0c80 | base]: target 4 / base 4 stmts
	// .. same ..
	// 0x021 <0x40> | 0x021 <0x24> | m_slots[slot].item = item.c_ptr();	SIZE
	// 0x061 <0x20> | 0x045 <0x2f> | item->set_inventory( this, slot );	SIZE
	// ; aligned 2, size-diffs 2, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - target INLINES intrusive_ptr::operator=(T*) (ctor+swap+dtor visible) and operator* on item; base emits them as out-of-line calls. Per-call-site LTCG inline-vs-call, non-steerable. trail: set_item.md
}

// STATE[58.21%|PARTIAL]: inventory_item_ptr copy-ctor + intrusive_ptr operators inline-vs-call (LTCG), non-steerable
bool inventory::action(
	const profile_slot_enum		slot_id,
	bool						key_down
)
{
	inventory_item_ptr iitem = m_slots[slot_id].item;
	if ( iitem != NULL )
	{
		inventory_item::action_behaviour_type const& behaviour_type = iitem->get_action_behaviuor( );
		switch ( behaviour_type )
		{
			case inventory_item::disabled:
			break;

			case inventory_item::use_silent:
				iitem->action( key_down );	// sushi@MATCH: Size difference
			break;

			case inventory_item::inventory_active_item:
				if ( m_active_slot == slot_id )
					return true;

				if ( m_holder->set_new_active_item( iitem ) )
					m_active_slot = slot_id;
			break;

			default: NODEFAULT( );
		}
		return true;
	}
	return false;

	// STRUCTURE DIFF[target 0x6f0b80 | base]: target 20 / base 20 stmts
	// 0x009 <0x15> | 0x009 <0x5e> | inventory_item_ptr iitem = m_slots[slot_id].item;   SIZE
	// 0x01e <0x10> | 0x067 <0x14> | if ( iitem != NULL )   SIZE
	// .. same ..
	// 0x02e <0x17> | 0x07b <0x1d> | inventory_item::action_behaviour_type const& behaviour_type = iitem->get_action_behaviuor( );   SIZE
	// 0x045 <0x16> | 0x098 <0x1a> | switch ( behaviour_type )   SIZE
	// .. same ..
	// 0x05d <0x1d> | 0x0b4 <0x24> | iitem->action( key_down );   SIZE
	// .. same ..
	// ; aligned 15, size-diffs 5, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - target keeps inventory_item_ptr copy-ctor + operator T*()/operator* out-of-line; base inlines them. Per-call-site LTCG inline-vs-call, non-steerable. trail: action.md
}



// STATE[58.45%|PARTIAL]: item_in_slot + intrusive_ptr operators inline-vs-call (LTCG), non-steerable
void inventory::setup_demo_profile( )
{
	for ( u32 i = 0 ; i < WEAPON_COUNT ; ++i )
	{
		profile_slot_enum current = weapon_slots[i];
		weapon_core_ptr weapon = static_cast_resource_ptr< weapon_core_ptr >( item_in_slot( current ) ); // sushi@MATCH: Problems start here
		if ( weapon ) // sushi@MATCH: This check is correct (matches against c_ptr)
		{
			weapon->set_amount( 100 );
			weapon->load_ammo_on_next_activate( );
		}
	}

	for ( u32 i = 0 ; i < AMMO_COUNT ; ++i )
	{
		profile_slot_enum current = ammunition_slots[i];
		inventory_item_ptr& iitem = item_in_slot( current );
		if ( iitem ) // sushi@MATCH: This check isn't.
			iitem->set_amount( 100 );
	}

	for ( u32 i = 0 ; i < ITEM_COUNT ; ++i )
	{
		profile_slot_enum current = ammunition_slots[i];
		inventory_item_ptr& iitem = item_in_slot( current );
		if ( iitem )
			iitem->set_amount( 100 );
	}

	// STRUCTURE DIFF[target 0x6f0a10 | base]: target / base same stmt set
	// .. same ..
	// 0x032 <0x32> | 0x032 <0x4c> | weapon_core_ptr weapon = static_cast_resource_ptr< weapon_core_ptr >( item_in_slot( current ) );   SIZE
	// .. same ..
	// 0x080 <0x19> | 0x09a <0x16> | weapon->set_amount( 100 );   SIZE
	// .. same ..
	// 0x0e7 <0xe> | 0x0fe <0x10> | inventory_item_ptr& iitem = item_in_slot( current );   SIZE
	// 0x0f5 <0xc> | 0x10e <0x1e> | if ( iitem )   SIZE
	// 0x101 <0x12> | 0x12c <0x1e> | iitem->set_amount( 100 );   SIZE
	// 0x13a <0xe> | 0x171 <0x10> | inventory_item_ptr& iitem = item_in_slot( current );   SIZE
	// 0x148 <0xc> | 0x181 <0x1e> | if ( iitem )   SIZE
	// 0x154 <0x12> | 0x19f <0x1e> | iitem->set_amount( 100 );   SIZE
	// ; aligned 18, size-diffs 8, quantity-diffs 0 (ONLY-target rows are alignment artifacts of the SIZE diffs)
	// VERDICT: STRUCTURE MATCH (shape ok) - target INLINES item_in_slot() (direct &m_slots[current].item) and calls intrusive_ptr operator T*()/operator* out-of-line; base does the reverse. Per-call-site LTCG inline-vs-call. Direct m_slots[current].item member access REBUILT WORSE (0x97 vs 0x4c) - keep item_in_slot(). trail: setup_demo_profile.md
}

// STATE[47.35%|PARTIAL]: item_in_slot + intrusive_ptr operators inline-vs-call (LTCG), non-steerable
void inventory::setup_from_profile( player_profile& profile, items_dictionary const& dict )
{
	profile_slot* slot;
	for ( u32 i = 0 ; i < WEAPON_COUNT ; ++i )
	{
		profile_slot_enum current = weapon_slots[i];
		slot = &profile.slots[current];

		if ( slot->item.id )
		{
			weapon_core_ptr weapon = static_cast_resource_ptr< weapon_core_ptr >( item_in_slot( current ) ); // sushi@MATCH: Problems start here
			weapon->set_amount( slot->item.condition_or_stack );
			weapon->load_ammo_on_next_activate( );
		}
	}

	for ( u32 i = 0 ; i < AMMO_COUNT ; ++i )
	{
		profile_slot_enum current = ammunition_slots[i];
		slot = &profile.slots[current];

		if ( slot->item.id )
		{
			inventory_item_ptr& iitem = item_in_slot( current );
			const u32 amount = math::min( slot->item.condition_or_stack, slot->item.amount_in_inventory );
			iitem->set_amount( amount );
			slot->item.amount_in_inventory -= amount;
		}
	}

	for ( u32 i = 0 ; i < ITEM_COUNT ; ++i )
	{
		profile_slot_enum current = ammunition_slots[i];
		slot = &profile.slots[current];

		if ( slot->item.id )
		{
			inventory_item_ptr& iitem = item_in_slot( current );
			if ( iitem )
			{
				if ( dict.item_by_id( current ).is_stack )
				{
					const u32 amount = math::min( slot->item.condition_or_stack, slot->item.amount_in_inventory );
					iitem->set_amount( amount );
					slot->item.amount_in_inventory -= amount;
				} else
					iitem->set_amount( slot->item.condition_or_stack );
			}
		}
	}

	// STRUCTURE DIFF[target 0x6f0800 | base]: target / base same stmt set
	// .. same ..
	// 0x046 <0x9> | 0x045 <0xd> | if ( slot->item.id )   SIZE
	// 0x04f <0x25> | 0x052 <0x4c> | weapon_core_ptr weapon = static_cast_resource_ptr< weapon_core_ptr >( item_in_slot( current ) );   SIZE
	// 0x074 <0x1a> | 0x09e <0x1b> | weapon->set_amount( slot->item.condition_or_stack );   SIZE
	// .. same ..
	// 0x17f <0xe> | 0x1e0 <0x10> | inventory_item_ptr& iitem = item_in_slot( current );   SIZE
	// 0x18d <0xc> | 0x1f0 <0x22> | if ( iitem )   SIZE
	// 0x199 <0x1a> | 0x212 <0x17> | if ( dict.item_by_id( current ).is_stack )   SIZE
	// 0x1b3 <0x13> | 0x229 <0x2b> | const u32 amount = math::min( ... );   SIZE
	// 0x1c6 <0x10> | 0x254 <0x21> | iitem->set_amount( amount );   SIZE
	// 0x1e7 <0x13> | 0x286 <0x23> | iitem->set_amount( slot->item.condition_or_stack );   SIZE
	// ; aligned 32, size-diffs 12, quantity-diffs 5 (the AMMO-loop ONLY-target/ONLY-base rows are alignment artifacts of the item_in_slot SIZE diff, NOT a real missing statement - confirmed in --view diff: same stmt set, item_in_slot inlined in target)
	// VERDICT: STRUCTURE MATCH (shape ok) - target INLINES item_in_slot() and calls intrusive_ptr operators out-of-line; base does the reverse. Per-call-site LTCG inline-vs-call, non-steerable. trail: setup_from_profile.md
}

// STATE[55.56%|PARTIAL]: item_in_slot + intrusive_ptr operators inline-vs-call (LTCG), non-steerable
void inventory::unload_to_profile( player_profile& profile, items_dictionary const& dict )
{
	profile_slot* slot;
	for ( u32 i = 0 ; i < WEAPON_COUNT ; ++i )
	{
		profile_slot_enum current = weapon_slots[i];
		slot = &profile.slots[current];

		if ( slot->item.id == 0 )
			continue;

		weapon_core_ptr weapon = static_cast_resource_ptr< weapon_core_ptr >( item_in_slot( current ) );
		ASSERT( UNKNOWN_EXPRESSION );
		weapon->unload_ammo( );
		slot->item.condition_or_stack = weapon->amount( );
	}

	for ( u32 i = 0 ; i < AMMO_COUNT ; ++i )
	{
		profile_slot_enum current = ammunition_slots[i];
		slot = &profile.slots[current];

		if ( slot->item.id == 0 )
			continue;

		inventory_item_ptr ammo = item_in_slot( current );
		ASSERT( UNKNOWN_EXPRESSION );
		slot->item.amount_in_inventory += ammo->amount( );
	}

	for ( u32 i = 0 ; i < ITEM_COUNT ; ++i )
	{
		profile_slot_enum current = item_slots[i];
		slot = &profile.slots[current];

		if ( slot->item.id )
		{
			inventory_item_ptr& iitem = item_in_slot( current );
			if ( iitem )
			{
				if ( dict.item_by_id( slot->item.dict_id ).is_stack )
					slot->item.amount_in_inventory += iitem->amount( );
				else
					slot->item.condition_or_stack = iitem->amount( );
			}
		}
	}

	// STRUCTURE DIFF[target 0x6f05f0 | base]: target 43 / base 43 stmts
	// .. same ..
	// 0x051 <0x25> | 0x050 <0x4c> | weapon_core_ptr weapon = static_cast_resource_ptr< weapon_core_ptr >( item_in_slot( current ) );   SIZE
	// 0x0c5 <0x18> | 0x0eb <0x1c> | for ( u32 i = 0 ; i < AMMO_COUNT ; ++i )   SIZE
	// 0x108 <0x15> | 0x132 <0x5e> | inventory_item_ptr ammo = item_in_slot( current );   SIZE
	// 0x129 <0x1c> | 0x19c <0x29> | slot->item.amount_in_inventory += ammo->amount( );   SIZE
	// 0x197 <0xe> | 0x21b <0x10> | inventory_item_ptr& iitem = item_in_slot( current );   SIZE
	// 0x1a5 <0xc> | 0x22b <0x1e> | if ( iitem )   SIZE
	// 0x1b1 <0x1a> | 0x249 <0x1b> | if ( dict.item_by_id( slot->item.dict_id ).is_stack )   SIZE
	// 0x1cb <0x1c> | 0x264 <0x2b> | slot->item.amount_in_inventory += iitem->amount( );   SIZE
	// 0x1e9 <0x15> | 0x291 <0x24> | slot->item.condition_or_stack = iitem->amount( );   SIZE
	// ; aligned 33, size-diffs 10, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - target INLINES item_in_slot() and keeps intrusive_ptr copy-ctor/operator T*()/operator* out-of-line; base does the reverse. Per-call-site LTCG inline-vs-call, non-steerable. trail: unload_to_profile.md
}

// STATE[BLOCKED]
static void call_item_remove( inventory_slot& slot )
{
	if ( slot.item )
		slot.item->remove( );

	// FUNCTION BODY
	// <0x700376>|0x006|+0x00c:'231'
	// <0x700382>|0x012|+0x018:'232'
	// ******
}

// STATE[100%|DONE]
void inventory::remove( )
{
	for ( inventory_slot* slot = m_slots; slot != m_slots + max_slots_count; ++slot )
		call_item_remove( *slot );
	m_active_slot = max_slots_count;
}

// STATE[INPROGRESS]: serializes a slot's item unless its profile slot is in the ignored set. DCE'd, no base symbol.
static void call_item_serialize(
	inventory_slot const&				slot,
	network_core::udp_match_packet&		packet,
	const u32							client_offset
)
{
	profile_slot_enum const* const	ignored_slots_start	= ignored_slots_for_serialization;
	profile_slot_enum const* const	ignored_slots_end	= ignored_slots_for_serialization + 7;

	if ( slot.item && std::find( ignored_slots_start, ignored_slots_end, slot.item->profile_slot_id( ) ) == ignored_slots_end )
		slot.item->serialize( packet, client_offset );

	// VERDICT: STRUCTURE UNVERIFIED - DCE'd, no base symbol (target rva 0x6f02f0); needs an opaque anchor in temp_include_all - a follow-up matcher's job, out of my scope. (slot.item guard combined to a single && to match the target's single-statement condition, mirroring call_item_deserialize.)
}

// STATE[46.64%|PARTIAL]: deserializes a slot's item unless its profile slot is in the ignored set.
// Structure matched (combined && guard); residual is LTCG inline-vs-call SIZE.
static void call_item_deserialize( inventory_slot& slot, network_core::packet_reader& reader )
{
	profile_slot_enum const* const	ignored_slots_start	= ignored_slots_for_serialization;
	profile_slot_enum const* const	ignored_slots_end	= ignored_slots_for_serialization + 7;

	if ( slot.item && std::find( ignored_slots_start, ignored_slots_end, slot.item->profile_slot_id( ) ) == ignored_slots_end )
		slot.item->deserialize( reader );

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// b.diff    |t.addr  |b.addr  |t.sz|b.sz|b.ln|b.code
	// ----------+--------+--------+----+----+----+------
	// SIZE -0x9 |0x6f027d|0x47831d|0x10|0x7 |0   |profile_slot_enum const* const	ignored_slots_end	= ignored_slots_for_serialization + 7;
	// SIZE +0x21|0x6f028d|0x478324|0x35|0x56|+2  |if ( slot.item && std::find( ignored_slots_start, ignored_slots_end, slot.item->profile_slot_id( ) ) == ignored_slots_end )
	// SIZE +0xf |0x6f02c2|0x47837a|0x1c|0x2b|+3  |slot.item->deserialize( reader );
	// VERDICT: STRUCTURE MATCH (shape ok) - 4/4 after the earlier combined-&&-guard fix; residual
	//   rows are LTCG inline-vs-call SIZE (find/profile_slot_id inlined into the target, plus the
	//   target materializing ignored_slots_end from a folded constant), non-steerable.
}

// STATE[INPROGRESS]: for_each over the slot array, deserializing each non-ignored item. DCE'd, no base symbol.
void inventory::deserialize( network_core::packet_reader& reader )
{
	std::for_each( m_slots, m_slots + max_slots_count, boost::bind( call_item_deserialize, _1, boost::ref( reader ) ) );

	// VERDICT: STRUCTURE UNVERIFIED - DCE'd, no base symbol (target rva 0x6f04a0); needs an opaque anchor in temp_include_all - a follow-up matcher's job, out of my scope.
}

// STATE[100%|DONE]
void inventory::set_victory_item( victory_item_core* item )
{
	if ( !item )
	{
		m_victory_item->set_carrier_id( u8(-1) );
		m_victory_item = item;
	}
	else
	{
		m_victory_item = item;
		m_victory_item->set_carrier_id( m_holder->cast_to_base_player( )->id );
	}
}

} // namespace survarium
