////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "inventory.h"

namespace survarium {

// STATE[STUB]
// survarium::inventory::inventory()
inventory::inventory( )
{
}

// STATE[STUB]
// void survarium::inventory::set_holder(survarium::inventory_holder*)
void inventory::set_holder( inventory_holder* holder )
{
	// LOCALS
	// u32 							i<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ef48f }, type_index: TypeIndex(0x6b73) })
	// ******

	// FUNCTION BODY
	// <0x700429>|0x000|0x000:'28'
	// <0x700435>|0x00c|0x00c:'29'
	// 1
	// <0x700441>|0x018|0x00c|[1]:'31'
	// <0x700459>|0x030|0x018:'32'
	// <0x70046f>|0x046|0x016:'33'
	// ******
}

// STATE[STUB]
// void survarium::inventory::unset_holder()
void inventory::unset_holder( )
{
	// LOCALS
	// u32 							i<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ef403 }, type_index: TypeIndex(0x6b73) })
	// ******

	// FUNCTION BODY
	// <0x7003a9>|0x000|0x000:'38'
	// 1
	// <0x7003b5>|0x00c|0x00c|[1]:'40'
	// <0x7003cd>|0x024|0x018:'41'
	// <0x7003e3>|0x03a|0x016:'42'
	// 1
	// <0x700405>|0x05c|0x022:'44'
	// ******
}

// STATE[STUB]
// void survarium::inventory::set_item(const survarium::profile_slot_enum, vostok::resources::resource_ptr<survarium::inventory_item,vostok::resources::unmanaged_intrusive_base> const&)
void inventory::set_item( profile_slot_enum slot, vostok::resources::resource_ptr<inventory_item,vostok::resources::unmanaged_intrusive_base> const& item )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6efcff }, type_index: TypeIndex(0x6b6a) })
	// ******

	// FUNCTION BODY
	// <0x700c89>|0x000|0x000:'49'
	// <0x700c95>|0x00c|0x00c:'50'
	// <0x700ca1>|0x018|0x00c:'51'
	// <0x700ce1>|0x058|0x040:'52'
	// ******
}

// STATE[STUB]
// bool survarium::inventory::action(const survarium::profile_slot_enum, bool)
bool inventory::action( profile_slot_enum slot_id, bool key_down )
{
	// LOCALS
	// vostok::resources::resource_ptr<inventory_item,vostok::resources::unmanaged_intrusive_base> iitem
	// inventory_item::action_behaviour_type const& behaviour_type<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6efbf8 }, type_index: TypeIndex(0x6b72) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6efc36 }, type_index: TypeIndex(0xa825) })
	// ******

	return false;
	// FUNCTION BODY
	// <0x700b89>|0x000|0x000:'57'
	// <0x700b9e>|0x015|0x015:'58'
	// 1
	// <0x700bae>|0x025|0x010|[1]:'60'
	// <0x700bc5>|0x03c|0x017:'61'
	// 1
	// 2
	// 3
	// <0x700bdb>|0x052|0x016:'65'
	// 1
	// 2
	// <0x700bdd>|0x054|0x002:'68'
	// <0x700bfa>|0x071|0x01d:'69'
	// 1
	// 2
	// <0x700bfc>|0x073|0x002:'72'
	// <0x700c0a>|0x081|0x00e:'73'
	// 1
	// <0x700c1b>|0x092|0x011:'75'
	// <0x700c3f>|0x0b6|0x024:'76'
	// 1
	// 2
	// 3
	// <0x700c4b>|0x0c2|0x00c:'80'
	// 1
	// <0x700c5c>|0x0d3|0x011:'82'
	// ******
}

// STATE[STUB]
// void survarium::inventory::setup_demo_profile()
void inventory::setup_demo_profile( )
{
	// LOCALS
	// u32 							i<1>
	// vostok::resources::resource_ptr<weapon_core,vostok::resources::unmanaged_intrusive_base> weapon<2>
	// profile_slot_enum 			current<2>
	// u32 							i<2>
	// vostok::resources::resource_ptr<inventory_item,vostok::resources::unmanaged_intrusive_base>& iitem<3>
	// profile_slot_enum 			current<3>
	// u32 							i<3>
	// vostok::resources::resource_ptr<inventory_item,vostok::resources::unmanaged_intrusive_base>& iitem<4>
	// profile_slot_enum 			current<4>
	// ******

	// FUNCTION BODY
	// <0x700a19>|0x000|0x000|[1]:'87'
	// 1
	// <0x700a35>|0x01c|0x01c|[2]:'89'
	// <0x700a42>|0x029|0x00d:'90'
	// <0x700a74>|0x05b|0x032:'91'
	// 1
	// <0x700a90>|0x077|0x01c:'93'
	// <0x700aa9>|0x090|0x019:'94'
	// 1
	// <0x700ac5>|0x0ac|0x01c:'96'
	// 1
	// 2
	// <0x700ad2>|0x0b9|0x00d|[2]:'99'
	// 1
	// <0x700aea>|0x0d1|0x018|[3]:'101'
	// 1
	// <0x700af7>|0x0de|0x00d:'103'
	// <0x700b05>|0x0ec|0x00e:'104'
	// <0x700b11>|0x0f8|0x00c:'105'
	// <0x700b23>|0x10a|0x012:'106'
	// 1
	// 2
	// <0x700b25>|0x10c|0x002|[3]:'109'
	// 1
	// <0x700b3d>|0x124|0x018|[4]:'111'
	// <0x700b4a>|0x131|0x00d:'112'
	// 1
	// <0x700b58>|0x13f|0x00e:'114'
	// <0x700b64>|0x14b|0x00c:'115'
	// <0x700b76>|0x15d|0x012:'116'
	// ******
}

// STATE[STUB]
// void survarium::inventory::setup_from_profile(survarium::player_profile&, survarium::items_dictionary const&)
void inventory::setup_from_profile( player_profile& profile, items_dictionary const& dict )
{
	// LOCALS
	// profile_slot* 				slot
	// u32 							i<1>
	// profile_slot_enum 			current<2>
	// vostok::resources::resource_ptr<weapon_core,vostok::resources::unmanaged_intrusive_base> weapon<3>
	// u32 							i<2>
	// profile_slot_enum 			current<3>
	// u32 							amount<4>
	// vostok::resources::resource_ptr<inventory_item,vostok::resources::unmanaged_intrusive_base>& iitem<4>
	// u32 							i<3>
	// profile_slot_enum 			current<4>
	// vostok::resources::resource_ptr<inventory_item,vostok::resources::unmanaged_intrusive_base>& iitem<5>
	// u32 							amount<6>
	// ******

	// FUNCTION BODY
	// 1
	// 2
	// <0x70080a>|0x000|0x000|[1]:'123'
	// 1
	// <0x700826>|0x01c|0x01c|[2]:'125'
	// <0x700833>|0x029|0x00d:'126'
	// 1
	// <0x700846>|0x03c|0x013:'128'
	// 1
	// <0x70084f>|0x045|0x009|[3]:'130'
	// <0x700874>|0x06a|0x025:'131'
	// <0x70088e>|0x084|0x01a:'132'
	// <0x7008aa>|0x0a0|0x01c:'133'
	// <0x7008b2>|0x0a8|0x008:'134'
	// 1
	// 2
	// <0x7008b7>|0x0ad|0x005|[2]:'137'
	// 1
	// <0x7008cf>|0x0c5|0x018|[3]:'139'
	// <0x7008dc>|0x0d2|0x00d:'140'
	// 1
	// <0x7008ef>|0x0e5|0x013:'142'
	// 1
	// <0x7008f8>|0x0ee|0x009|[4]:'144'
	// 1
	// 2
	// <0x700906>|0x0fc|0x00e:'147'
	// <0x700919>|0x10f|0x013:'148'
	// <0x700929>|0x11f|0x010:'149'
	// 1
	// <0x700938>|0x12e|0x00f:'151'
	// 1
	// 2
	// <0x70093a>|0x130|0x002|[3]:'154'
	// 1
	// <0x700956>|0x14c|0x01c|[4]:'156'
	// <0x700963>|0x159|0x00d:'157'
	// 1
	// <0x700976>|0x16c|0x013:'159'
	// 1
	// <0x70097f>|0x175|0x009|[5]:'161'
	// <0x70098d>|0x183|0x00e:'162'
	// 1
	// <0x700999>|0x18f|0x00c:'164'
	// 1
	// 2
	// <0x7009b3>|0x1a9|0x01a|[6]:'167'
	// <0x7009c6>|0x1bc|0x013:'168'
	// <0x7009d6>|0x1cc|0x010:'169'
	// <0x7009e5>|0x1db|0x00f:'170'
	// <0x7009e7>|0x1dd|0x002:'171'
	// 1
	// <0x7009fa>|0x1f0|0x013:'173'
	// 1
	// 2
	// ******
}

// STATE[STUB]
// void survarium::inventory::unload_to_profile(survarium::player_profile&, survarium::items_dictionary const&)
void inventory::unload_to_profile( player_profile& profile, items_dictionary const& dict )
{
	// LOCALS
	// profile_slot* 				slot
	// u32 							i<1>
	// vostok::resources::resource_ptr<weapon_core,vostok::resources::unmanaged_intrusive_base> weapon<2>
	// profile_slot_enum 			current<2>
	// u32 							i<2>
	// vostok::resources::resource_ptr<inventory_item,vostok::resources::unmanaged_intrusive_base> ammo<3>
	// profile_slot_enum 			current<3>
	// u32 							i<3>
	// profile_slot_enum 			current<4>
	// vostok::resources::resource_ptr<inventory_item,vostok::resources::unmanaged_intrusive_base>& iitem<5>
	// ******

	// FUNCTION BODY
	// 1
	// 2
	// <0x7005fa>|0x000|0x000|[1]:'182'
	// 1
	// <0x700616>|0x01c|0x01c|[2]:'184'
	// <0x700623>|0x029|0x00d:'185'
	// 1
	// <0x700636>|0x03c|0x013:'187'
	// <0x70063f>|0x045|0x009:'188'
	// 1
	// <0x700641>|0x047|0x002:'190'
	// <0x700666>|0x06c|0x025:'191'
	// <0x700672>|0x078|0x00c:'192'
	// <0x700686>|0x08c|0x014:'193'
	// <0x7006a8>|0x0ae|0x022:'194'
	// 1
	// <0x7006b5>|0x0bb|0x00d|[2]:'196'
	// 1
	// <0x7006cd>|0x0d3|0x018|[3]:'198'
	// <0x7006da>|0x0e0|0x00d:'199'
	// 1
	// <0x7006ed>|0x0f3|0x013:'201'
	// <0x7006f6>|0x0fc|0x009:'202'
	// 1
	// <0x7006f8>|0x0fe|0x002:'204'
	// <0x70070d>|0x113|0x015:'205'
	// <0x700719>|0x11f|0x00c:'206'
	// <0x700735>|0x13b|0x01c:'207'
	// 1
	// 2
	// <0x700742>|0x148|0x00d|[3]:'210'
	// 1
	// <0x70075e>|0x164|0x01c|[4]:'212'
	// <0x70076b>|0x171|0x00d:'213'
	// 1
	// <0x70077e>|0x184|0x013:'215'
	// 1
	// <0x700787>|0x18d|0x009|[5]:'217'
	// <0x700795>|0x19b|0x00e:'218'
	// 1
	// <0x7007a1>|0x1a7|0x00c:'220'
	// <0x7007bb>|0x1c1|0x01a:'221'
	// <0x7007d7>|0x1dd|0x01c:'222'
	// <0x7007d9>|0x1df|0x002:'223'
	// 1
	// <0x7007ee>|0x1f4|0x015:'225'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::call_item_remove(survarium::inventory_slot&)
void call_item_remove( inventory_slot& slot )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ef398 }, type_index: TypeIndex(0x6b73) })
	// ******

	// FUNCTION BODY
	// <0x700376>|0x000|0x000:'231'
	// <0x700382>|0x00c|0x00c:'232'
	// ******
}

// STATE[STUB]
// void survarium::inventory::remove()
void inventory::remove( )
{
	// FUNCTION BODY
	// <0x700539>|0x000|0x000:'237'
	// <0x70056b>|0x032|0x032:'238'
	// ******
}

// STATE[STUB]
// void survarium::call_item_serialize(survarium::inventory_slot const&, vostok::network_core::udp_match_packet&, const unsigned int)
void call_item_serialize( inventory_slot const& slot, vostok::network_core::udp_match_packet& packet, u32 client_offset )
{
	// LOCALS
	// profile_slot_enum const* 	ignored_slots_start
	// profile_slot_enum const* 	ignored_slots_end
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ef360 }, type_index: TypeIndex(0x6b7f) })
	// ******

	// FUNCTION BODY
	// <0x7002f6>|0x000|0x000:'253'
	// <0x7002fd>|0x007|0x007:'254'
	// 1
	// <0x70030d>|0x017|0x010:'256'
	// <0x700342>|0x04c|0x035:'257'
	// ******
}

// STATE[STUB]
// void survarium::call_item_deserialize(survarium::inventory_slot&, vostok::network_core::packet_reader&)
void call_item_deserialize( inventory_slot& slot, vostok::network_core::packet_reader& reader )
{
	// LOCALS
	// profile_slot_enum const* 	ignored_slots_start
	// profile_slot_enum const* 	ignored_slots_end
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ef2dc }, type_index: TypeIndex(0x6b80) })
	// ******

	// FUNCTION BODY
	// <0x700276>|0x000|0x000:'262'
	// <0x70027d>|0x007|0x007:'263'
	// 1
	// <0x70028d>|0x017|0x010:'265'
	// <0x7002c2>|0x04c|0x035:'266'
	// ******
}

// STATE[STUB]
// void survarium::inventory::deserialize(vostok::network_core::packet_reader&)
void inventory::deserialize( vostok::network_core::packet_reader& reader )
{
	// FUNCTION BODY
	// <0x7004a9>|0x000|0x000:'276'
	// ******
}

// STATE[STUB]
// void survarium::inventory::set_victory_item(survarium::victory_item_core*)
void inventory::set_victory_item( victory_item_core* item )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ef246 }, type_index: TypeIndex(0xa829) })
	// ******

	// FUNCTION BODY
	// <0x7001f9>|0x000|0x000:'281'
	// 1
	// <0x7001ff>|0x006|0x006:'283'
	// <0x700215>|0x01c|0x016:'284'
	// 1
	// <0x700221>|0x028|0x00c:'286'
	// 1
	// <0x700223>|0x02a|0x002:'288'
	// <0x70022f>|0x036|0x00c:'289'
	// 1
	// ******
}

	/* TYPEDEFS

	typedef
		scheduler::record*
		iterator_type;

} // namespace survarium
