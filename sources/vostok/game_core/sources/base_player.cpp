////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/base_player.h>

#include <vostok/game_core/base_player_creation_params.h>
#include <vostok/game_core/player_profile.h>
#include <vostok/game_core/scheduler.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

// STATE[INPROGRESS]: body is empty `{ }`; everything is the member-init list (decoded
// from the @0x72f0b0 asm: recoil_params<-params+0x10, dispersion_params<-params+0x20,
// breath_holding_params<-params+0x58, damage_model<-params+0x114, movement_speed_factor
// <-clear_value, then the 4 bools=0). NOT yet anchored (base_player is abstract) so it
// reads 0/unpaired in report.json. NEXT: (1) anchor via a concrete derived stub in
// temp_include_all.cpp that constructs a base_player_creation_params and escapes &obj;
// (2) like the dtor, full match is gated on the inventory_holder/collision_user/
// hit_initiator/hit_receiver base CTORS being out-of-line (target calls them out-of-line).
base_player::base_player( base_player_creation_params const& params, survarium::scheduler& the_scheduler ) :
	inventory_holder		( the_scheduler, params.inventory ),
	hit_initiator			( params.initial_info.id, params.initial_info.profile->is_local ),
	m_recoil_params			( params.recoil_params ),
	m_dispersion_params		( params.dispersion_params ),
	m_breath_holding_params	( params.breath_holding_params ),
	m_damage_model			( params.damage_model )
{
	// FUNCTION BODY
	// <0x73f0b0>|0x000|+0x1da:'31'	{
	// <0x73f28a>|0x1da|      :'32'	}
	// ******
}

// STATE[72.19%|PARTIAL]: body is just `ASSERT( UNKNOWN_EXPRESSION )` (line 36, the
// finalize_impl @0x09); rest is compiler-generated member/base destruction. Gated on
// the base-class destructors (hit_receiver/hit_initiator/collision_user/inventory_holder)
// being OUT-OF-LINE: target calls e.g. `survarium::hit_receiver::~hit_receiver` out-of-line
// and does NOT reset the 4 base vtables up front, while our build inlines those base dtors
// (loose_ptr_base::~loose_ptr_base etc.) and lays all 4 vtables down at the top. The
// remaining `dummy::nonnull` vs `finalize_impl` diffs are folded-empty-fn cosmetics (both
// 0x3f210). Reach 100% by matching those base-class dtors out-of-line first.
base_player::~base_player( )
{
	ASSERT( UNKNOWN_EXPRESSION );
	// FUNCTION BODY
	// <0x73ed70>|0x000|+0x009:'35'	{
	// <0x73ed79>|0x009|+0x00c:'36'	ASSERT( UNKNOWN_EXPRESSION )
	// <0x73ed85>|0x015|      :'37'	}
	// ******
}

// STATE[3.38%|INPROGRESS]: large (~616B) function dispatching through the active
// object's vtable (m_current_active_object @0x40, m_target_active_object @0x44) plus
// weapon_core::could_be_used and inventory::{get_active_slot,item_in_slot,action}. Now
// PAIRED (was unpaired) after setting the protected access specifier (target IAE) +
// anchoring. NEXT: decode the @0x72ee40 asm control flow - two nested if-blocks guarded
// by current/target active-object comparisons (operator!= / c_ptr ==), the vtable calls
// at [vtbl+0x1c/0x20/0x28/0x2c/0x54/0x70/0x7c] need mapping to the concrete active-object
// interface (offsets exceed interactive_object's own table, so the dynamic type has more
// virtuals), and the broken_hands_count/another_weapon_slot locals. Real engine logic,
// reconstruct statement-by-statement against the carcass below.
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

// STATE[INPROGRESS]: reads the source profile slot, fetches that inventory item and forwards
// deserialize_game_world_object to it. The trailing local-byte stub is a compiled-out assert. DCE'd, no base symbol.
void base_player::deserialize_game_world_object( network_core::packet_reader& reader )
{
	profile_slot_enum	slot	= (profile_slot_enum)reader.r< bool >( );

	inventory_item_ptr	item	= inventory( ).item_in_slot( slot );
	item->deserialize_game_world_object( reader );

	// VERDICT: STRUCTURE UNVERIFIED - DCE'd, no base symbol (target rva 0x72ed00); needs an opaque anchor in temp_include_all - a follow-up matcher's job, out of my scope.
}

// STATE[100%|DONE]
void base_player::subscribe_on_player_death( player_death_subscriber* subscriber )
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_player_death_subscribers.push_back( subscriber );
}

// STATE[100%|DONE]
void base_player::unsubscribe_from_player_death( player_death_subscriber* subscriber )
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_player_death_subscribers.erase( subscriber );
}

// STATE[100%|DONE]
static void call_player_death_subscriber_callback( player_death_subscriber const* const subscriber )
{
	subscriber->subscription_callback( );
}

// STATE[100%|DONE]
void base_player::on_player_death( )
{
	m_player_death_subscribers.for_each( call_player_death_subscriber_callback );
}

} // namespace survarium
