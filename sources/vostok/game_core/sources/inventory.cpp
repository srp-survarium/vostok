////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/inventory.h>

#include <vostok/game_core/base_player.h>
#include <vostok/game_core/inventory_holder.h>
#include <vostok/game_core/victory_item_core.h>

namespace survarium {

// STATE[STUB]
inventory::inventory( ) :
	m_active_slot	( max_slots_count ),
	m_holder		( NULL ),
	m_victory_item	( NULL )
{
}


// STATE[STUB]
void inventory::set_holder( inventory_holder* holder )
{
	ASSERT( UNKNOWN_EXPRESSION_T( !m_holder ) );
	m_holder = holder;

	for ( u32 i = 0 ; i < max_slots_count ; ++i )
	{
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

// STATE[STUB]
void inventory::unset_holder( )
{
	// LOCALS
	// u32 								i<1>
	// ******

	// CALL SITE INFO
	// <0x700403> -> void < unknown >()
	// ******

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

// STATE[STUB]
void inventory::set_item(
	const profile_slot_enum			slot,
	inventory_item_ptr const&		item
)
{
	// CALL SITE INFO
	// <0x700cff> -> void < unknown >( inventory*, profile_slot_enum )
	// ******

	// FUNCTION BODY[0x700c80]: 4
	// <0x700c89>|0x009|+0x00c:'49'
	// <0x700c95>|0x015|+0x00c:'50'
	// <0x700ca1>|0x021|+0x040:'51'
	// <0x700ce1>|0x061|+0x020:'52'
	// ******
}

// STATE[STUB]
bool inventory::action(
	const profile_slot_enum		slot_id,
	bool						key_down
)
{
	// LOCALS
	// inventory_item_ptr 				iitem
	// inventory_item::action_behaviour_type const& behaviour_type<1>
	// ******

	// CALL SITE INFO
	// <0x700bf8> -> void < unknown >( bool )
	// <0x700c36> -> bool < unknown >( inventory_item_ptr const& )
	// ******

	return false;

	// FUNCTION BODY[0x700b80]: 26
	// <0x700b89>|0x009|+0x015:'57'
	// <0x700b9e>|0x01e|+0x010:'58'
	// <0>
	// <0x700bae>|0x02e|+0x017|[1]:'60'
	// <0x700bc5>|0x045|+0x016:'61'
	// <0>
	// <1>
	// <2>
	// <0x700bdb>|0x05b|+0x002:'65'
	// <0>
	// <1>
	// <0x700bdd>|0x05d|+0x01d:'68'
	// <0x700bfa>|0x07a|+0x002:'69'
	// <0>
	// <1>
	// <0x700bfc>|0x07c|+0x00e:'72'
	// <0x700c0a>|0x08a|+0x011:'73'
	// <0>
	// <0x700c1b>|0x09b|+0x024:'75'
	// <0x700c3f>|0x0bf|+0x00c:'76'
	// <0>
	// <1>
	// <2>
	// <0x700c4b>|0x0cb|+0x011:'80'
	// <0>
	// <0x700c5c>|0x0dc|+0x00f:'82'
	// ******
}

// STATE[STUB]
void inventory::setup_demo_profile( )
{
	// LOCALS
	// u32 								i<1>
	// weapon_core_ptr 					weapon<2>
	// profile_slot_enum 				current<2>
	// u32 								i<2>
	// inventory_item_ptr& 				iitem<3>
	// profile_slot_enum 				current<3>
	// u32 								i<3>
	// inventory_item_ptr& 				iitem<4>
	// profile_slot_enum 				current<4>
	// ******

	// FUNCTION BODY[0x700a10]: 30
	// <0x700a19>|0x009|+0x01c|[1]:'87'
	// <0>
	// <0x700a35>|0x025|+0x00d|[2]:'89'
	// <0x700a42>|0x032|+0x032:'90'
	// <0x700a74>|0x064|+0x01c:'91'
	// <0>
	// <0x700a90>|0x080|+0x019:'93'
	// <0x700aa9>|0x099|+0x01c:'94'
	// <0>
	// <0x700ac5>|0x0b5|+0x00d:'96'
	// <0>
	// <1>
	// <0x700ad2>|0x0c2|+0x018|[2]:'99'
	// <0>
	// <0x700aea>|0x0da|+0x00d|[3]:'101'
	// <0>
	// <0x700af7>|0x0e7|+0x00e:'103'
	// <0x700b05>|0x0f5|+0x00c:'104'
	// <0x700b11>|0x101|+0x012:'105'
	// <0x700b23>|0x113|+0x002:'106'
	// <0>
	// <1>
	// <0x700b25>|0x115|+0x018|[3]:'109'
	// <0>
	// <0x700b3d>|0x12d|+0x00d|[4]:'111'
	// <0x700b4a>|0x13a|+0x00e:'112'
	// <0>
	// <0x700b58>|0x148|+0x00c:'114'
	// <0x700b64>|0x154|+0x012:'115'
	// <0x700b76>|0x166|+0x002:'116'
	// ******
}

// STATE[STUB]
void inventory::setup_from_profile( player_profile& profile, items_dictionary const& dict )
{
	// LOCALS
	// profile_slot* 					slot
	// u32 								i<1>
	// profile_slot_enum 				current<2>
	// weapon_core_ptr 					weapon<3>
	// u32 								i<2>
	// profile_slot_enum 				current<3>
	// const u32 						amount<4>
	// inventory_item_ptr& 				iitem<4>
	// u32 								i<3>
	// profile_slot_enum 				current<4>
	// inventory_item_ptr& 				iitem<5>
	// const u32 						amount<6>
	// ******

	// FUNCTION BODY[0x700800]: 55
	// <0>
	// <1>
	// <0x70080a>|0x00a|+0x01c|[1]:'123'
	// <0>
	// <0x700826>|0x026|+0x00d|[2]:'125'
	// <0x700833>|0x033|+0x013:'126'
	// <0>
	// <0x700846>|0x046|+0x009:'128'
	// <0>
	// <0x70084f>|0x04f|+0x025|[3]:'130'
	// <0x700874>|0x074|+0x01a:'131'
	// <0x70088e>|0x08e|+0x01c:'132'
	// <0x7008aa>|0x0aa|+0x008:'133'
	// <0x7008b2>|0x0b2|+0x005:'134'
	// <0>
	// <1>
	// <0x7008b7>|0x0b7|+0x018|[2]:'137'
	// <0>
	// <0x7008cf>|0x0cf|+0x00d|[3]:'139'
	// <0x7008dc>|0x0dc|+0x013:'140'
	// <0>
	// <0x7008ef>|0x0ef|+0x009:'142'
	// <0>
	// <0x7008f8>|0x0f8|+0x00e|[4]:'144'
	// <0>
	// <1>
	// <0x700906>|0x106|+0x013:'147'
	// <0x700919>|0x119|+0x010:'148'
	// <0x700929>|0x129|+0x00f:'149'
	// <0>
	// <0x700938>|0x138|+0x002:'151'
	// <0>
	// <1>
	// <0x70093a>|0x13a|+0x01c|[3]:'154'
	// <0>
	// <0x700956>|0x156|+0x00d|[4]:'156'
	// <0x700963>|0x163|+0x013:'157'
	// <0>
	// <0x700976>|0x176|+0x009:'159'
	// <0>
	// <0x70097f>|0x17f|+0x00e|[5]:'161'
	// <0x70098d>|0x18d|+0x00c:'162'
	// <0>
	// <0x700999>|0x199|+0x01a:'164'
	// <0>
	// <1>
	// <0x7009b3>|0x1b3|+0x013|[6]:'167'
	// <0x7009c6>|0x1c6|+0x010:'168'
	// <0x7009d6>|0x1d6|+0x00f:'169'
	// <0x7009e5>|0x1e5|+0x002:'170'
	// <0x7009e7>|0x1e7|+0x013:'171'
	// <0>
	// <0x7009fa>|0x1fa|+0x005:'173'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void inventory::unload_to_profile( player_profile& profile, items_dictionary const& dict )
{
	// LOCALS
	// profile_slot* 					slot
	// u32 								i<1>
	// weapon_core_ptr 					weapon<2>
	// profile_slot_enum 				current<2>
	// u32 								i<2>
	// inventory_item_ptr 				ammo<3>
	// profile_slot_enum 				current<3>
	// u32 								i<3>
	// profile_slot_enum 				current<4>
	// inventory_item_ptr& 				iitem<5>
	// ******

	// FUNCTION BODY[0x7005f0]: 47
	// <0>
	// <1>
	// <0x7005fa>|0x00a|+0x01c|[1]:'182'
	// <0>
	// <0x700616>|0x026|+0x00d|[2]:'184'
	// <0x700623>|0x033|+0x013:'185'
	// <0>
	// <0x700636>|0x046|+0x009:'187'
	// <0x70063f>|0x04f|+0x002:'188'
	// <0>
	// <0x700641>|0x051|+0x025:'190'
	// <0x700666>|0x076|+0x00c:'191'
	// <0x700672>|0x082|+0x014:'192'
	// <0x700686>|0x096|+0x022:'193'
	// <0x7006a8>|0x0b8|+0x00d:'194'
	// <0>
	// <0x7006b5>|0x0c5|+0x018|[2]:'196'
	// <0>
	// <0x7006cd>|0x0dd|+0x00d|[3]:'198'
	// <0x7006da>|0x0ea|+0x013:'199'
	// <0>
	// <0x7006ed>|0x0fd|+0x009:'201'
	// <0x7006f6>|0x106|+0x002:'202'
	// <0>
	// <0x7006f8>|0x108|+0x015:'204'
	// <0x70070d>|0x11d|+0x00c:'205'
	// <0x700719>|0x129|+0x01c:'206'
	// <0x700735>|0x145|+0x00d:'207'
	// <0>
	// <1>
	// <0x700742>|0x152|+0x01c|[3]:'210'
	// <0>
	// <0x70075e>|0x16e|+0x00d|[4]:'212'
	// <0x70076b>|0x17b|+0x013:'213'
	// <0>
	// <0x70077e>|0x18e|+0x009:'215'
	// <0>
	// <0x700787>|0x197|+0x00e|[5]:'217'
	// <0x700795>|0x1a5|+0x00c:'218'
	// <0>
	// <0x7007a1>|0x1b1|+0x01a:'220'
	// <0x7007bb>|0x1cb|+0x01c:'221'
	// <0x7007d7>|0x1e7|+0x002:'222'
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

// STATE[STUB]
void inventory::remove( )
{
	for ( inventory_slot* slot = m_slots; slot != m_slots + max_slots_count; ++slot )
		call_item_remove( *slot );
	m_active_slot = max_slots_count;

	// FUNCTION BODY
	// <0x700539>|0x009|+0x032:'237'
	// <0x70056b>|0x03b|+0x00d:'238'
	// ******
}

// STATE[BLOCKED]
static void call_item_serialize(
	inventory_slot const&				slot,
	network_core::udp_match_packet&		packet,
	const u32							client_offset
)
{
	// LOCALS
	// profile_slot_enum const* const 	ignored_slots_start
	// profile_slot_enum const* const 	ignored_slots_end
	// ******

	// CALL SITE INFO
	// <0x700360> -> void < unknown >( network_core::udp_match_packet&, u32 ) const
	// ******

	// FUNCTION BODY[0x7002f0]: 5
	// <0x7002f6>|0x006|+0x007:'253'
	// <0x7002fd>|0x00d|+0x010:'254'
	// <0>
	// <0x70030d>|0x01d|+0x035:'256'
	// <0x700342>|0x052|+0x020:'257'
	// ******
}

// STATE[BLOCKED]
static void call_item_deserialize( inventory_slot& slot, network_core::packet_reader& reader )
{
	// LOCALS
	// profile_slot_enum const* const 	ignored_slots_start
	// profile_slot_enum const* const 	ignored_slots_end
	// ******

	// CALL SITE INFO
	// <0x7002dc> -> void < unknown >( network_core::packet_reader& )
	// ******

	// FUNCTION BODY[0x700270]: 5
	// <0x700276>|0x006|+0x007:'262'
	// <0x70027d>|0x00d|+0x010:'263'
	// <0>
	// <0x70028d>|0x01d|+0x035:'265'
	// <0x7002c2>|0x052|+0x01c:'266'
	// ******
}

// STATE[BLOCKED]
void inventory::deserialize( network_core::packet_reader& reader )
{
	// FUNCTION BODY[0x7004a0]: 1
	// <0x7004a9>|0x009|+0x075:'276'
	// ******
}

// STATE[UNCHECKED]
void inventory::set_victory_item( victory_item_core* item )
{
	// CALL SITE INFO
	// <0x700246> -> base_player* < unknown >()
	// ******

	if ( !item )
	{
		m_victory_item->set_carrier_id( u8(-1) );
		m_victory_item = NULL;
	}
	else
	{
		m_victory_item = item;
		m_victory_item->set_carrier_id( m_holder->cast_to_base_player( )->id );
	}

	// FUNCTION BODY[0x7001f0]: 10
	// <0x7001f9>|0x009|+0x006:'281'
	// <0>
	// <0x7001ff>|0x00f|+0x016:'283'
	// <0x700215>|0x025|+0x00c:'284'
	// <0>
	// <0x700221>|0x031|+0x002:'286'
	// <0>
	// <0x700223>|0x033|+0x00c:'288'
	// <0x70022f>|0x03f|+0x037:'289'
	// <0>
	// ******
}

} // namespace survarium
