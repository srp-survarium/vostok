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

// STATE[91.03%|DONE]
inventory::inventory( ) :
	m_active_slot	( max_slots_count ),
	m_holder		( NULL ),
	m_victory_item	( NULL )
{
}


// STATE[71.08%|PARTIAL]
void inventory::set_holder( inventory_holder* holder )
{
	ASSERT( UNKNOWN_EXPRESSION_T( !m_holder ) );
	m_holder = holder;

	for ( u32 i = 0 ; i < max_slots_count ; ++i ) {
		if ( m_slots[i].item )
			m_slots[i].item->holder_assigned( );
	}

	// FUNCTION BODY[0x700420]: 6
	// <0x700429>|0x009|+0x00c:'28'
	// <0x700435>|0x015|+0x00c:'29'
	// <0>
	// <0x700441>|0x021|+0x018|[1]:'31'
	// <0x700459>|0x039|+0x016:'32'
	// <0x70046f>|0x04f|+0x022:'33'
	// ******
}

// STATE[70.28%|INCORRECT]
void inventory::unset_holder( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_holder ) );

	for ( u32 i = 0 ; i < max_slots_count ; ++i ) {
		if ( m_slots[i].item )
			m_slots[i].item->holder_removed( );
	}

	m_holder = NULL;

	// FUNCTION BODY[0x7003a0]: 7
	// <0x7003a9>|0x009|+0x00c:'38'
	// <0>
	// <0x7003b5>|0x015|+0x018|[1]:'40'
	// <0x7003cd>|0x02d|+0x016:'41'
	// <0x7003e3>|0x043|+0x022:'42'
	// <0>
	// <0x700405>|0x065|+0x00f:'44'
	// ******
}

// STATE[66.80%|PARTIAL]
void inventory::set_item(
	const profile_slot_enum			slot,
	inventory_item_ptr const&		item
)
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	m_slots[slot].item = item.c_ptr();	// sushi@MATCH: target does additional copying
	item->set_inventory( this, slot );	// sushi@MATCH: -> inlined in target

	// FUNCTION BODY[0x700c80]: 4
	// <0x700c89>|0x009|+0x00c:'49'
	// <0x700c95>|0x015|+0x00c:'50'
	// <0x700ca1>|0x021|+0x040:'51'
	// <0x700ce1>|0x061|+0x020:'52'
	// ******
}

// STATE[57.04%|PARTIAL]: inventory_item_ptr constructor inlined
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

	// FUNCTION BODY[0x700b80]: 26
	// <0x700b89>|0x009|+0x015:'57'
	// <0x700b9e>|0x01e|+0x010:'58'
	// <0>
	// <0x700bae>|0x02e|+0x017|[1]:'60'
	// <0x700bc5>|0x045|+0x016:'61'		switch ( behaviour_type )
	// <0>								{
	// <1>									case inventory_item::disabled:
	// <2>
	// <0x700bdb>|0x05b|+0x002:'65'			break;
	// <0>
	// <1>									case inventory_item::use_silent:
	// <0x700bdd>|0x05d|+0x01d:'68'				iitem->action( key_down ); !!
	// <0x700bfa>|0x07a|+0x002:'69'			break;
	// <0>
	// <1>									case inventory_item::inventory_active_item:
	// <0x700bfc>|0x07c|+0x00e:'72'				if ( m_active_slot == slot_id )
	// <0x700c0a>|0x08a|+0x011:'73'					return true;
	// <0>
	// <0x700c1b>|0x09b|+0x024:'75'				if ( m_holder->set_new_active_item( iitem ) )
	// <0x700c3f>|0x0bf|+0x00c:'76'					m_active_slot = slot_id;
	// <0>									break;
	// <1>
	// <2>									default: NODEFAULT( );
	// <0x700c4b>|0x0cb|+0x011:'80'			return true;
	// <0>								}
	// <0x700c5c>|0x0dc|+0x00f:'82'		return false;
	// ******
}



// STATE[36.41%|PARTIAL]
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

	// FUNCTION BODY[0x700a10]: 30
	// <0x700a19>|0x009|+0x01c|[1]:'87'
	// <0>
	// <0x700a35>|0x025|+0x00d|[2]:'89'		profile_slot_enum current = weapon_slots[i];
	// <0x700a42>|0x032|+0x032:'90'			weapon_core_ptr weapon = s
	// <0x700a74>|0x064|+0x01c:'91'			if ( weapon )
	// <0>
	// <0x700a90>|0x080|+0x019:'93'				weapon->set_amount( 100 );
	// <0x700aa9>|0x099|+0x01c:'94'				weapon->load_ammo_on_next_activate( );
	// <0>
	// <0x700ac5>|0x0b5|+0x00d:'96'			}
	// <0>
	// <1>
	// <0x700ad2>|0x0c2|+0x018|[2]:'99'		for ( u32 i = 0 ; i < AMMO_COUNT ; ++i )
	// <0>
	// <0x700aea>|0x0da|+0x00d|[3]:'101'		profile_slot_enum current = ammunition_slots[i];
	// <0>
	// <0x700af7>|0x0e7|+0x00e:'103'			inventory_item_ptr& iitem = item_in_slot( current );
	// <0x700b05>|0x0f5|+0x00c:'104'			if ( iitem )
	// <0x700b11>|0x101|+0x012:'105'				iitem->set_amount( 100 );
	// <0x700b23>|0x113|+0x002:'106'		}
	// <0>
	// <1>
	// <0x700b25>|0x115|+0x018|[3]:'109'	for ( u32 i = 0 ; i < ITEM_COUNT ; ++i )
	// <0>
	// <0x700b3d>|0x12d|+0x00d|[4]:'111'		profile_slot_enum current = ammunition_slots[i];
	// <0x700b4a>|0x13a|+0x00e:'112'			inventory_item_ptr& iitem = item_in_slot( current );
	// <0>
	// <0x700b58>|0x148|+0x00c:'114'
	// <0x700b64>|0x154|+0x012:'115'
	// <0x700b76>|0x166|+0x002:'116'
	// ******
}

// STATE[59.17%|PARTIAL]
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

	// FUNCTION BODY[0x700800]: 55
	// <0>
	// <1>
	// <0x70080a>|0x00a|+0x01c|[1]:'123'	for ( u32 i = 0 ; i < WEAPON_COUNT ; ++i )
	// <0>
	// <0x700826>|0x026|+0x00d|[2]:'125'		profile_slot_enum current = weapon_slots[i];
	// <0x700833>|0x033|+0x013:'126'			profile_slot* slot = &profile.slots[current];
	// <0>
	// <0x700846>|0x046|+0x009:'128'			if ( slot->item.id )
	// <0>										{
	// <0x70084f>|0x04f|+0x025|[3]:'130'			weapon_core_ptr weapon = static_cast_reso
	// <0x700874>|0x074|+0x01a:'131'				weapon->set_amount( slot->item.condition_or_stack );
	// <0x70088e>|0x08e|+0x01c:'132'				weapon->load_ammo_on_next_activate( );
	// <0x7008aa>|0x0aa|+0x008:'133'			}
	// <0x7008b2>|0x0b2|+0x005:'134'		}
	// <0>
	// <1>
	// <0x7008b7>|0x0b7|+0x018|[2]:'137'	for ( u32 i = 0 ; i < AMMO_COUNT ; ++i )
	// <0>									{
	// <0x7008cf>|0x0cf|+0x00d|[3]:'139'
	// <0x7008dc>|0x0dc|+0x013:'140'			slot = &profile.slots[current];
	// <0>
	// <0x7008ef>|0x0ef|+0x009:'142'			if ( slot->item.id )
	// <0>
	// <0x7008f8>|0x0f8|+0x00e|[4]:'144'			inventory_item_ptr& iitem = item_in_slot( current );
	// <0>
	// <1>
	// <0x700906>|0x106|+0x013:'147'				const u32 amount =
	// <0x700919>|0x119|+0x010:'148'				iitem->set_amount( amount );
	// <0x700929>|0x129|+0x00f:'149'
	// <0>
	// <0x700938>|0x138|+0x002:'151'		}
	// <0>
	// <1>
	// <0x70093a>|0x13a|+0x01c|[3]:'154'	for ( u32 i = 0 ; i < ITEM_COUNT ; ++i )
	// <0>
	// <0x700956>|0x156|+0x00d|[4]:'156'		profile_slot_enum current = ammunition_slots[i];
	// <0x700963>|0x163|+0x013:'157'			slot = &profile.slots[current];
	// <0>
	// <0x700976>|0x176|+0x009:'159'			if ( slot->item.id )
	// <0>										{
	// <0x70097f>|0x17f|+0x00e|[5]:'161'			inventory_item_ptr& iitem = item_in_slot( current );
	// <0x70098d>|0x18d|+0x00c:'162'				if ( iitem )
	// <0>											{
	// <0x700999>|0x199|+0x01a:'164'					if ( dict.item_by_id( current ) )
	// <0>												{
	// <1>
	// <0x7009b3>|0x1b3|+0x013|[6]:'167'					const u32 amount = math::min(
	// <0x7009c6>|0x1c6|+0x010:'168'						iitem->set_amount( amount );
	// <0x7009d6>|0x1d6|+0x00f:'169'						slot->item.amount_in_inventory -= amount;
	// <0x7009e5>|0x1e5|+0x002:'170'					} else
	// <0x7009e7>|0x1e7|+0x013:'171'
	// <0>
	// <0x7009fa>|0x1fa|+0x005:'173'				}
	// <0>										}
	// <1>									}
	// ******
}

// STATE[65.84%|PARTIAL]
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

	// FUNCTION BODY[0x7005f0]: 47
	// <0>
	// <1>
	// <0x7005fa>|0x00a|+0x01c|[1]:'182'	for ( auto i = 0 ; i < 2 ; ++i )
	// <0>
	// <0x700616>|0x026|+0x00d|[2]:'184'		profile_slot_enum current = weapon_slots[i];
	// <0x700623>|0x033|+0x013:'185'			profile_slot* slot
	// <0>
	// <0x700636>|0x046|+0x009:'187'
	// <0x70063f>|0x04f|+0x002:'188'
	// <0>
	// <0x700641>|0x051|+0x025:'190'
	// <0x700666>|0x076|+0x00c:'191'			ASSERT( UNKNOWN_EXPRESSION );
	// <0x700672>|0x082|+0x014:'192'			weapon->unload_ammo( );
	// <0x700686>|0x096|+0x022:'193'			slot->item.condition_or_stack = weapon->amount( );
	// <0x7006a8>|0x0b8|+0x00d:'194'
	// <0>
	// <0x7006b5>|0x0c5|+0x018|[2]:'196'	for ( u32 i = 0 ; i < 4 ; ++i )
	// <0>
	// <0x7006cd>|0x0dd|+0x00d|[3]:'198'
	// <0x7006da>|0x0ea|+0x013:'199'
	// <0>
	// <0x7006ed>|0x0fd|+0x009:'201'
	// <0x7006f6>|0x106|+0x002:'202'
	// <0>
	// <0x7006f8>|0x108|+0x015:'204'			inventory_item_ptr ammo =  item_in_slot( current );
	// <0x70070d>|0x11d|+0x00c:'205'
	// <0x700719>|0x129|+0x01c:'206'
	// <0x700735>|0x145|+0x00d:'207'
	// <0>
	// <1>
	// <0x700742>|0x152|+0x01c|[3]:'210'	for ( u32 i = 0 ; i < 13 ; ++i )
	// <0>
	// <0x70075e>|0x16e|+0x00d|[4]:'212'
	// <0x70076b>|0x17b|+0x013:'213'
	// <0>
	// <0x70077e>|0x18e|+0x009:'215'			if ( slot->item.id )
	// <0>
	// <0x700787>|0x197|+0x00e|[5]:'217'			inventory_item_ptr& iitem = item_in_slot( current );
	// <0x700795>|0x1a5|+0x00c:'218'				if ( iitem )
	// <0>
	// <0x7007a1>|0x1b1|+0x01a:'220'					if ( dict.item_by_id( slot->item.dict_id ).is_stack )
	// <0x7007bb>|0x1cb|+0x01c:'221'
	// <0x7007d7>|0x1e7|+0x002:'222'					else
	// <0x7007d9>|0x1e9|+0x015:'223'
	// <0>
	// <0x7007ee>|0x1fe|+0x005:'225'
	// <0>
	// ******
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

// STATE[PARTIAL]: serializes a slot's item unless its profile slot is in the ignored set.
static void call_item_serialize(
	inventory_slot const&				slot,
	network_core::udp_match_packet&		packet,
	const u32							client_offset
)
{
	profile_slot_enum const* const	ignored_slots_start	= ignored_slots_for_serialization;
	profile_slot_enum const* const	ignored_slots_end	= ignored_slots_for_serialization + 7;

	if ( slot.item )
	{
		if ( std::find( ignored_slots_start, ignored_slots_end, slot.item->profile_slot_id( ) ) == ignored_slots_end )
			slot.item->serialize( packet, client_offset );
	}
}

// STATE[PARTIAL]: deserializes a slot's item unless its profile slot is in the ignored set.
static void call_item_deserialize( inventory_slot& slot, network_core::packet_reader& reader )
{
	profile_slot_enum const* const	ignored_slots_start	= ignored_slots_for_serialization;
	profile_slot_enum const* const	ignored_slots_end	= ignored_slots_for_serialization + 7;

	if ( slot.item )
	{
		if ( std::find( ignored_slots_start, ignored_slots_end, slot.item->profile_slot_id( ) ) == ignored_slots_end )
			slot.item->deserialize( reader );
	}
}

// STATE[PARTIAL]: for_each over the slot array, deserializing each non-ignored item.
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
