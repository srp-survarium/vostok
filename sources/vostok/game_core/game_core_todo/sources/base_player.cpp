////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "base_player.h"

namespace survarium {

// STATE[STUB]
// survarium::base_player::base_player(survarium::base_player_creation_params const&, survarium::scheduler&)
base_player::base_player( base_player_creation_params const& params, scheduler& the_scheduler )
{
}

// STATE[STUB]
// void survarium::base_player::~base_player()
void base_player::~base_player( )
{
	// FUNCTION BODY
	// <0x73ed79>|0x000|0x000:'36'
	// ******
}

// STATE[STUB]
// void survarium::base_player::tick_active_object()
void base_player::tick_active_object( )
{
	// LOCALS
	// weapon_core const* 			current_weapon<1>
	// u8 							broken_hands_count<2>
	// vostok::resources::resource_ptr<inventory_item,vostok::resources::unmanaged_intrusive_base> another_item<3>
	// profile_slot_enum 			another_weapon_slot_id<3>
	// weapon_core const* 			another_weapon<3>
	// weapon_core const* 			current_weapon<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x72de63 }, type_index: TypeIndex(0x6418) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x72de9a }, type_index: TypeIndex(0x641c) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x72dec0 }, type_index: TypeIndex(0x91ab) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x72dedb }, type_index: TypeIndex(0x6418) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x72df07 }, type_index: TypeIndex(0x919d) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x72df19 }, type_index: TypeIndex(0x6417) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x72df34 }, type_index: TypeIndex(0x6418) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x72df79 }, type_index: TypeIndex(0x6438) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x72df93 }, type_index: TypeIndex(0x916d) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x72e038 }, type_index: TypeIndex(0x6b7d) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x72e09e }, type_index: TypeIndex(0x6438) })
	// ******

	// FUNCTION BODY
	// <0x73ee4a>|0x000|0x000:'41'
	// 1
	// <0x73ee65>|0x01b|0x01b:'43'
	// <0x73ee81>|0x037|0x01c:'44'
	// 1
	// <0x73eea7>|0x05d|0x026:'46'
	// <0x73eec2>|0x078|0x01b:'47'
	// <0x73eedd>|0x093|0x01b:'48'
	// <0x73eeee>|0x0a4|0x011:'49'
	// <0x73ef1b>|0x0d1|0x02d:'50'
	// 1
	// 2
	// 3
	// <0x73ef36>|0x0ec|0x01b:'54'
	// <0x73ef60>|0x116|0x02a|[1]:'55'
	// <0x73ef88>|0x13e|0x028|[2]:'56'
	// <0x73efc2>|0x178|0x03a:'57'
	// <0x73efd9>|0x18f|0x017|[3]:'58'
	// <0x73eff9>|0x1af|0x020:'59'
	// <0x73f016>|0x1cc|0x01d:'60'
	// <0x73f04c>|0x202|0x036:'61'
	// <0x73f065>|0x21b|0x019:'62'
	// 1
	// <0x73f07d>|0x233|0x018:'64'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x73f085>|0x23b|0x008|[1]:'70'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::base_player::send_game_world_object(survarium::game_world_object const*, boost::function<vostok::network_core::udp_match_packet & __cdecl(void)> const&, boost::function<void __cdecl(vostok::network_core::udp_match_packet &)> const&) const
void base_player::send_game_world_object( game_world_object const* object, boost::function<vostok::network_core::udp_match_packet & __cdecl(void)> const& reciver_packet_allocator, boost::function<void __cdecl(vostok::network_core::udp_match_packet &)> const& reciver_enqueuer ) const
{
	// LOCALS
	// vostok::network_core::udp_match_packet& packet
	// inventory_item const* 		item
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x72dc30 }, type_index: TypeIndex(0x6b52) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x72dc80 }, type_index: TypeIndex(0x6b54) })
	// ******

	// FUNCTION BODY
	// <0x73ebef>|0x000|0x000:'80'
	// 1
	// <0x73ebfa>|0x00b|0x00b:'82'
	// <0x73ec06>|0x017|0x00c:'83'
	// 1
	// <0x73ec19>|0x02a|0x013:'85'
	// <0x73ec25>|0x036|0x00c:'86'
	// 1
	// <0x73ec35>|0x046|0x010:'88'
	// <0x73ec41>|0x052|0x00c:'89'
	// 1
	// <0x73ec71>|0x082|0x030:'91'
	// 1
	// <0x73ec82>|0x093|0x011:'93'
	// ******
}

// STATE[STUB]
// void survarium::base_player::deserialize_game_world_object(vostok::network_core::packet_reader&)
void base_player::deserialize_game_world_object( vostok::network_core::packet_reader& reader )
{
	// LOCALS
	// vostok::resources::resource_ptr<inventory_item,vostok::resources::unmanaged_intrusive_base> item
	// profile_slot_enum 			slot
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x72dd57 }, type_index: TypeIndex(0x6b80) })
	// ******

	// FUNCTION BODY
	// 1
	// <0x73ed09>|0x000|0x000:'115'
	// 1
	// <0x73ed17>|0x00e|0x00e:'117'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x73ed23>|0x01a|0x00c:'123'
	// <0x73ed3d>|0x034|0x01a:'124'
	// ******
}

// STATE[STUB]
// void survarium::base_player::subscribe_on_player_death(survarium::player_death_subscriber*)
void base_player::subscribe_on_player_death( player_death_subscriber* subscriber )
{
	// FUNCTION BODY
	// <0x73ebb9>|0x000|0x000:'129'
	// <0x73ebc5>|0x00c|0x00c:'130'
	// ******
}

// STATE[STUB]
// void survarium::base_player::unsubscribe_from_player_death(survarium::player_death_subscriber*)
void base_player::unsubscribe_from_player_death( player_death_subscriber* subscriber )
{
	// FUNCTION BODY
	// <0x73eb89>|0x000|0x000:'135'
	// <0x73eb95>|0x00c|0x00c:'136'
	// ******
}

// STATE[STUB]
// void survarium::call_player_death_subscriber_callback(survarium::player_death_subscriber const* const)
void call_player_death_subscriber_callback( player_death_subscriber const* subscriber )
{
	// FUNCTION BODY
	// <0x73eca9>|0x000|0x000:'141'
	// ******
}

// STATE[STUB]
// void survarium::base_player::on_player_death()
void base_player::on_player_death( )
{
	// FUNCTION BODY
	// <0x73ecc9>|0x000|0x000:'146'
	// ******
}

} // namespace survarium
