////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/base_player.h>

#include <vostok/game_core/base_player_creation_params.h>
#include <vostok/game_core/player_profile.h>
#include <vostok/game_core/scheduler.h>

namespace survarium {

// STATE[STUB]
base_player::base_player( base_player_creation_params const& params, survarium::scheduler& the_scheduler ) :
	inventory_holder	( the_scheduler, params.inventory ),
	hit_initiator		( params.initial_info.id, params.initial_info.profile->is_local ),
	m_recoil_params		( params.recoil_params )
{
	// FUNCTION BODY
	// <0x73f0b0>|0x000|+0x1da:'31'	{
	// <0x73f28a>|0x1da|      :'32'	}
	// ******
}

// STATE[STUB]
base_player::~base_player( )
{
	// FUNCTION BODY
	// <0x73ed70>|0x000|+0x009:'35'	{
	// <0x73ed79>|0x009|+0x00c:'36'
	// <0x73ed85>|0x015|      :'37'	}
	// ******
}

// STATE[STUB]
// void survarium::base_player::tick_active_object()
void base_player::tick_active_object( )
{
	// LOCALS
	// weapon_core const* 			current_weapon<1>
	// u8 							broken_hands_count<2>
	// resources::resource_ptr<inventory_item,resources::unmanaged_intrusive_base> another_item<3>
	// profile_slot_enum 			another_weapon_slot_id<3>
	// weapon_core const* 			another_weapon<3>
	// weapon_core const* 			current_weapon<1>
	// ******

	// CALL SITE INFO
	// <0x73ee63> -> void <unknown>()
	// <0x73ee9a> -> bool <unknown>() const
	// <0x73eec0> -> void <unknown>(resources::resource_ptr<interactive_object,resources::unmanaged_intrusive_base> const&, resources::resource_ptr<interactive_object,resources::unmanaged_intrusive_base> const&) const
	// <0x73eedb> -> void <unknown>()
	// <0x73ef07> -> engine& <unknown>()
	// <0x73ef19> -> void <unknown>(base_player&, engine&)
	// <0x73ef34> -> void <unknown>()
	// <0x73ef79> -> weapon_core* <unknown>()
	// <0x73ef93> -> resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base> const& <unknown>() const
	// <0x73f038> -> weapon_core* <unknown>()
	// <0x73f09e> -> weapon_core* <unknown>()
	// ******

	// FUNCTION BODY
	// <0x73ee40>|0x000|+0x00a:'40'	{
	// <0x73ee4a>|0x00a|+0x01b:'41'
	// <0>
	// <0x73ee65>|0x025|+0x01c:'43'
	// <0x73ee81>|0x041|+0x026:'44'
	// <0>
	// <0x73eea7>|0x067|+0x01b:'46'
	// <0x73eec2>|0x082|+0x01b:'47'
	// <0x73eedd>|0x09d|+0x011:'48'
	// <0x73eeee>|0x0ae|+0x02d:'49'
	// <0x73ef1b>|0x0db|+0x01b:'50'
	// <0>
	// <1>
	// <2>
	// <0x73ef36>|0x0f6|+0x02a:'54'
	// <0x73ef60>|0x120|+0x028|[1]:'55'
	// <0x73ef88>|0x148|+0x03a|[2]:'56'
	// <0x73efc2>|0x182|+0x017:'57'
	// <0x73efd9>|0x199|+0x020|[3]:'58'
	// <0x73eff9>|0x1b9|+0x01d:'59'
	// <0x73f016>|0x1d6|+0x036:'60'
	// <0x73f04c>|0x20c|+0x019:'61'
	// <0x73f065>|0x225|+0x018:'62'
	// <0>
	// <0x73f07d>|0x23d|+0x008:'64'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x73f085>|0x245|+0x01e|[1]:'70'
	// <0>
	// <0x73f0a3>|0x263|      :'72'	}
	// ******
}

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::base_player::send_game_world_object(survarium::game_world_object const*, boost::function<vostok::network_core::udp_match_packet & __cdecl(void)> const&, boost::function<void __cdecl(vostok::network_core::udp_match_packet &)> const&) const
void base_player::send_game_world_object( game_world_object const* object, boost::function<network_core::udp_match_packet &()> const& reciver_packet_allocator, boost::function<void(network_core::udp_match_packet &)> const& reciver_enqueuer ) const
{
	// LOCALS
	// network_core::udp_match_packet& packet
	// inventory_item const* 		item
	// ******

	// CALL SITE INFO
	// <0x73ec30> -> inventory_item const* <unknown>() const
	// <0x73ec80> -> void <unknown>(network_core::udp_match_packet&) const
	// ******

	// FUNCTION BODY
	// <0x73ebe0>|0x000|+0x00f:'79'	{
	// <0x73ebef>|0x00f|+0x00b:'80'
	// <0>
	// <0x73ebfa>|0x01a|+0x00c:'82'
	// <0x73ec06>|0x026|+0x013:'83'
	// <0>
	// <0x73ec19>|0x039|+0x00c:'85'
	// <0x73ec25>|0x045|+0x010:'86'
	// <0>
	// <0x73ec35>|0x055|+0x00c:'88'
	// <0x73ec41>|0x061|+0x030:'89'
	// <0>
	// <0x73ec71>|0x091|+0x011:'91'
	// <0>
	// <0x73ec82>|0x0a2|+0x00c:'93'
	// <0x73ec8e>|0x0ae|      :'94'	}
	// ******
}

// STATE[BLOCKED]: udp_match_packet/packet_reader cluster is never-compiled (see game_core/README.md) - body is matchable from asm but cannot compile/diff until that header cluster is built.
// void survarium::base_player::deserialize_game_world_object(vostok::network_core::packet_reader&)
void base_player::deserialize_game_world_object( network_core::packet_reader& reader )
{
	// LOCALS
	// resources::resource_ptr<inventory_item,resources::unmanaged_intrusive_base> item
	// profile_slot_enum 			slot
	// ******

	// CALL SITE INFO
	// <0x73ed57> -> void <unknown>(network_core::packet_reader&)
	// ******

	// FUNCTION BODY
	// <0x73ed00>|0x000|+0x009:'113'	{
	// <0>
	// <0x73ed09>|0x009|+0x00e:'115'
	// <0>
	// <0x73ed17>|0x017|+0x00c:'117'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x73ed23>|0x023|+0x01a:'123'
	// <0x73ed3d>|0x03d|+0x01c:'124'
	// <0x73ed59>|0x059|      :'125'	}
	// ******
}

// STATE[STUB]
// void survarium::base_player::subscribe_on_player_death(survarium::player_death_subscriber*)
void base_player::subscribe_on_player_death( player_death_subscriber* subscriber )
{
	// FUNCTION BODY
	// <0x73ebb0>|0x000|+0x009:'128'	{
	// <0x73ebb9>|0x009|+0x00c:'129'
	// <0x73ebc5>|0x015|+0x014:'130'
	// <0x73ebd9>|0x029|      :'131'	}
	// ******
}

// STATE[STUB]
// void survarium::base_player::unsubscribe_from_player_death(survarium::player_death_subscriber*)
void base_player::unsubscribe_from_player_death( player_death_subscriber* subscriber )
{
	// FUNCTION BODY
	// <0x73eb80>|0x000|+0x009:'134'	{
	// <0x73eb89>|0x009|+0x00c:'135'
	// <0x73eb95>|0x015|+0x012:'136'
	// <0x73eba7>|0x027|      :'137'	}
	// ******
}

// STATE[STUB]
// void survarium::call_player_death_subscriber_callback(survarium::player_death_subscriber const* const)
void call_player_death_subscriber_callback( player_death_subscriber const* subscriber )
{
	// FUNCTION BODY
	// <0x73eca0>|0x000|+0x009:'140'	{
	// <0x73eca9>|0x009|+0x008:'141'
	// <0x73ecb1>|0x011|      :'142'	}
	// ******
}

// STATE[STUB]
// void survarium::base_player::on_player_death()
void base_player::on_player_death( )
{
	// FUNCTION BODY
	// <0x73ecc0>|0x000|+0x009:'145'	{
	// <0x73ecc9>|0x009|+0x029:'146'
	// <0x73ecf2>|0x032|      :'147'	}
	// ******
}

} // namespace survarium
