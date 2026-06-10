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

	// STRUCTURE DIFF: target 5 stmts / base 5 stmts
	// SIZE +0x10 | 78 | 		if ( m_slots[i].item )
	// SIZE +0xd  | 79 | 			m_slots[i].item->holder_assigned( );
	// VERDICT: STRUCTURE MATCH (shape ok) - target CALLs resource_ptr's safe-bool/operator->
	// out-of-line; base INLINES them (the MFP-temp safe-bool compare + the operator-> assert
	// eater visible inline). Per-site LTCG inline-vs-call, non-steerable.
}

// STATE[70.28%|PARTIAL]: intrusive_ptr operator T*/operator* inline-vs-call (LTCG), non-steerable
void inventory::unset_holder( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_holder ) );

	for ( u32 i = 0 ; i < max_slots_count ; ++i )
		if ( m_slots[i].item )
			m_slots[i].item->holder_removed( );

	m_holder = NULL;

	// STRUCTURE DIFF: target 5 stmts / base 5 stmts
	// SIZE +0x10 | 95 | 		if ( m_slots[i].item )
	// SIZE +0xd  | 96 | 			m_slots[i].item->holder_removed( );
	// VERDICT: STRUCTURE MATCH (shape ok) - same two rows as set_holder: target CALLs the
	// safe-bool/operator-> out-of-line, base inlines them. LTCG, non-steerable.
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

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// SIZE -0x1c | 117 | m_slots[slot].item = item.c_ptr();
	// SIZE +0xf  | 118 | item->set_inventory( this, slot );
	// VERDICT: STRUCTURE MATCH (shape ok) - target INLINES operator=(T*) (ctor+swap+dtor
	// visible) but CALLs operator-> out-of-line; base does the exact reverse. Per-site LTCG
	// inline-vs-call, non-steerable.
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

	// STRUCTURE DIFF: target 13 stmts / base 13 stmts
	// SIZE +0x49 | 134 | inventory_item_ptr iitem = m_slots[slot_id].item;
	// SIZE +0x4  | 135 | if ( iitem != NULL )
	// SIZE +0x6  | 137 | 		inventory_item::action_behaviour_type const& behaviour_type = iitem->get_action_behaviuor( );
	// SIZE +0x4  | 138 | 		switch ( behaviour_type )
	// SIZE +0x7  | 144 | 				iitem->action( key_down );
	// VERDICT: STRUCTURE MATCH (shape ok) - target keeps the resource_ptr copy-ctor +
	// operator T*()/operator-> out-of-line; base inlines them. Per-site LTCG, non-steerable.
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

	// STRUCTURE DIFF: target 19 stmts / base 19 stmts
	// SIZE +0x1a | 182 | 		weapon_core_ptr weapon = static_cast_resource_ptr< weapon_core_ptr >( item_in_slot( current ) );
	// SIZE -0x3  | 185 | 			weapon->set_amount( 100 );
	// SIZE +0x2  | 193 | 		inventory_item_ptr& iitem = item_in_slot( current );
	// SIZE +0x12 | 194 | 		if ( iitem )
	// SIZE +0xc  | 195 | 			iitem->set_amount( 100 );
	// SIZE +0x2  | 201 | 		inventory_item_ptr& iitem = item_in_slot( current );
	// SIZE +0x12 | 202 | 		if ( iitem )
	// SIZE +0xc  | 203 | 			iitem->set_amount( 100 );
	// VERDICT: STRUCTURE MATCH (shape ok) - target INLINES item_in_slot() and CALLs the
	// resource_ptr safe-bool/operator-> out-of-line; base does the reverse. Per-site LTCG.
	// Direct m_slots[current].item access REBUILT WORSE (0x97 vs 0x4c) - keep item_in_slot().
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

	// STRUCTURE DIFF: target 31 stmts / base 31 stmts
	// SIZE +0x27 | 233 | 			weapon_core_ptr weapon = static_cast_resource_ptr< weapon_core_ptr >( item_in_slot( current ) );
	// SIZE +0x18 | 247 | 			const u32 amount = math::min( ... );  (AMMO loop; aligner mispairs as ONLY rows)
	// SIZE +0x11 | 248 | 			iitem->set_amount( amount );
	// SIZE +0x16 | 261 | 			if ( iitem )
	// SIZE +0x18 | 265 | 				const u32 amount = math::min( ... );
	// SIZE +0x11 | 266 | 				iitem->set_amount( amount );
	// SIZE +0x10 | 269 | 				iitem->set_amount( slot->item.condition_or_stack );
	// (+ small +-0x1..0x4 rows on if/for/decl lines)
	// VERDICT: STRUCTURE MATCH (shape ok) - 31/31 hand-aligned (the differ's AMMO-loop
	// TRGT_ONLY/BASE_ONLY pairs are equal-size mispairs: t0xe<->b246, t0x13<->b247, t0x10<->b248,
	// t0xf<->b249); locals incl. both `const u32 amount` match the PDB exactly. Residuals are
	// item_in_slot()/resource_ptr operator/math::min(u32,u32) inline-vs-call, LTCG, non-steerable.
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

	// STRUCTURE DIFF: target 30 stmts / base 30 stmts
	// SIZE +0x27 | 302 | 		weapon_core_ptr weapon = static_cast_resource_ptr< weapon_core_ptr >( item_in_slot( current ) );
	// SIZE +0x49 | 316 | 		inventory_item_ptr ammo = item_in_slot( current );
	// SIZE +0xd  | 318 | 		slot->item.amount_in_inventory += ammo->amount( );
	// SIZE +0x12 | 329 | 			if ( iitem )
	// SIZE +0xf  | 332 | 					slot->item.amount_in_inventory += iitem->amount( );
	// SIZE +0xf  | 334 | 					slot->item.condition_or_stack = iitem->amount( );
	// (+ small +-0x1..0x4 rows on for/decl lines)
	// VERDICT: STRUCTURE MATCH (shape ok) - target INLINES item_in_slot() and keeps the
	// resource_ptr copy-ctor/safe-bool/operator-> out-of-line; base does the reverse.
	// Per-site LTCG inline-vs-call, non-steerable.
}

// STATE[31.17%|PARTIAL]: resource_ptr safe-bool/operator-> inline-vs-call (LTCG), non-steerable
static void call_item_remove( inventory_slot& slot )
{
	if ( slot.item )
		slot.item->remove( );

	// STRUCTURE DIFF: target 2 stmts / base 2 stmts
	// SIZE +0x12 | 357 | if ( slot.item )
	// SIZE +0xf  | 358 | 	slot.item->remove( );
	// VERDICT: STRUCTURE MATCH (shape ok) - same safe-bool/operator-> rows as set_holder;
	// target CALLs them out-of-line, base inlines. LTCG, non-steerable. (Was STATE[BLOCKED]
	// with a stale carcass - the base symbol exists and pairs at 31.17%.)
}

// STATE[100%|DONE]
void inventory::remove( )
{
	for ( inventory_slot* slot = m_slots; slot != m_slots + max_slots_count; ++slot )
		call_item_remove( *slot );
	m_active_slot = max_slots_count;
}

// STATE[49.49%|PARTIAL]: serializes a slot's item unless its profile slot is in the ignored set.
// Emitted via the bind in inventory::serialize below (was DCE'd / no base symbol).
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

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// SIZE -0x9  | 374 | profile_slot_enum const* const	ignored_slots_end	= ignored_slots_for_serialization + 7;
	// SIZE +0x21 | 376 | if ( slot.item && std::find( ... ) == ignored_slots_end )
	// SIZE +0xf  | 377 | 	slot.item->serialize( packet, client_offset );
	// VERDICT: STRUCTURE MATCH (shape ok) - identical row set to call_item_deserialize:
	// std::find/profile_slot_id/operator-> inline-vs-call, LTCG, non-steerable.
}

// STATE[INLINED]: the target has NO standalone inventory::serialize symbol (LTCG inlined it
// into its caller) but call_item_serialize @0x6f02f0 exists and is only reachable through this
// bind - the function provably existed in the original TU; defined here to emit the helper.
void inventory::serialize( network_core::udp_match_packet& packet, u32 client_offset ) const
{
	std::for_each( m_slots, m_slots + max_slots_count, boost::bind( call_item_serialize, _1, boost::ref( packet ), client_offset ) );
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
	// SIZE -0x9  | 395 | profile_slot_enum const* const	ignored_slots_end	= ignored_slots_for_serialization + 7;
	// SIZE +0x21 | 397 | if ( slot.item && std::find( ... ) == ignored_slots_end )
	// SIZE +0xf  | 398 | 	slot.item->deserialize( reader );
	// VERDICT: STRUCTURE MATCH (shape ok) - the combined && guard (one statement) matches the
	// target; residuals are std::find/profile_slot_id/operator-> inline-vs-call, LTCG, non-steerable.
}

// STATE[100%|DONE]: was DCE'd; anchored from temp_include_all use_game_core_serialization,
// pairs at 100% (0x84 bytes both sides).
void inventory::deserialize( network_core::packet_reader& reader )
{
	std::for_each( m_slots, m_slots + max_slots_count, boost::bind( call_item_deserialize, _1, boost::ref( reader ) ) );
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
