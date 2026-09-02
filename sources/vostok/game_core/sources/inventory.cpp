// SPDX-License-Identifier: GPL-3.0-or-later

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

inventory::inventory( ) :
	m_active_slot	( max_slots_count ),
	m_holder		( NULL ),
	m_victory_item	( NULL )
{
}


void inventory::set_holder( inventory_holder* holder )
{
	ASSERT( UNKNOWN_EXPRESSION_T( !m_holder ) );
	m_holder = holder;

	for ( u32 i = 0 ; i < max_slots_count ; ++i )
		if ( m_slots[i].item )
			m_slots[i].item->holder_assigned( );
}

void inventory::unset_holder( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_holder ) );

	for ( u32 i = 0 ; i < max_slots_count ; ++i )
		if ( m_slots[i].item )
			m_slots[i].item->holder_removed( );

	m_holder = NULL;
}

void inventory::set_item(
	const profile_slot_enum			slot,
	inventory_item_ptr const&		item
)
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	m_slots[slot].item = item.c_ptr();	// sushi@MATCH: target does additional copying
	item->set_inventory( this, slot );	// sushi@MATCH: -> inlined in target
}

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
}

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
		profile_slot_enum current = item_slots[i];
		inventory_item_ptr& iitem = item_in_slot( current );
		if ( iitem )
			iitem->set_amount( 100 );
	}
}

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
		profile_slot_enum current = item_slots[i];
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
}

#line 178
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
}

static void call_item_remove( inventory_slot& slot )
{
	if ( slot.item )
		slot.item->remove( );
}

void inventory::remove( )
{
	std::for_each( m_slots, m_slots + max_slots_count, call_item_remove );
	m_active_slot = max_slots_count;
}

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
}

void inventory::serialize( network_core::udp_match_packet& packet, u32 client_offset ) const
{
	std::for_each( m_slots, m_slots + max_slots_count, boost::bind( call_item_serialize, _1, boost::ref( packet ), client_offset ) );
}

static void call_item_deserialize( inventory_slot& slot, network_core::packet_reader& reader )
{
	profile_slot_enum const* const	ignored_slots_start	= ignored_slots_for_serialization;
	profile_slot_enum const* const	ignored_slots_end	= ignored_slots_for_serialization + 7;

	if ( slot.item && std::find( ignored_slots_start, ignored_slots_end, slot.item->profile_slot_id( ) ) == ignored_slots_end )
		slot.item->deserialize( reader );
}

void inventory::deserialize( network_core::packet_reader& reader )
{
	std::for_each( m_slots, m_slots + max_slots_count, boost::bind( call_item_deserialize, _1, boost::ref( reader ) ) );
}

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
