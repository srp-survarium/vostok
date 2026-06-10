////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/base_player.h>

#include <vostok/game_core/base_player_creation_params.h>
#include <vostok/game_core/player_profile.h>
#include <vostok/game_core/scheduler.h>
#include <vostok/game_core/weapon_core.h>
#include <vostok/network_core/packet_reader.h>

namespace survarium {

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

base_player::~base_player( )
{
	ASSERT( UNKNOWN_EXPRESSION );
}

void base_player::tick_active_object( )
{
	( *m_current_active_object ).tick( );

	if ( m_current_active_object != m_target_active_object )
		if ( ( *m_current_active_object ).is_ready_to_be_deactivated( ) )
		{
			on_before_active_object_changed( m_current_active_object, m_target_active_object );
			( *m_current_active_object ).deactivate( );
			m_current_active_object	= m_target_active_object;
			( *m_current_active_object ).activate( *this, get_engine( ) );
			( *m_current_active_object ).tick( );
		}

	if ( m_current_active_object == m_target_active_object )
		if ( weapon_core const* const current_weapon = ( *m_current_active_object ).cast_weapon_core( ) )
			if ( u8 const broken_hands_count = ( *damage_model( ) ).broken_hands_count( ) )
				if ( !current_weapon->could_be_used( *this ) ) {
					profile_slot_enum another_weapon_slot_id = inventory( ).get_active_slot( ) != weapon1_slot ? weapon1_slot : weapon2_slot;
					inventory_item_ptr another_item = inventory( ).item_in_slot( another_weapon_slot_id );
					weapon_core const* const another_weapon = another_item ? ( *another_item ).cast_weapon_core( ) : NULL;
					if ( another_weapon && another_weapon->could_be_used( *this ) )
						inventory( ).action( another_weapon_slot_id, true );

				}

	// claude@MATCH: the trailing braced block is real - the target emits the cast (+0x54
	// vcall) and a dead store into a second block-scoped `current_weapon` local at the
	// function tail (lines 69-71 of the original; the value is never read - whatever
	// consumed it was compiled out of MASTER_GOLD).
	{
		weapon_core const* const current_weapon = ( *m_current_active_object ).cast_weapon_core( );
	}
}

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

void base_player::deserialize_game_world_object( network_core::packet_reader& reader )
{
	profile_slot_enum	slot	= (profile_slot_enum)reader.r< bool >( );

	ASSERT( UNKNOWN_EXPRESSION );

	inventory_item_ptr	item	= inventory( ).item_in_slot( slot );
	item->deserialize_game_world_object( reader );
}

void base_player::subscribe_on_player_death( player_death_subscriber* subscriber )
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_player_death_subscribers.push_back( subscriber );
}

void base_player::unsubscribe_from_player_death( player_death_subscriber* subscriber )
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_player_death_subscribers.erase( subscriber );
}

static void call_player_death_subscriber_callback( player_death_subscriber const* const subscriber )
{
	subscriber->subscription_callback( );
}

void base_player::on_player_death( )
{
	m_player_death_subscribers.for_each( call_player_death_subscriber_callback );
}

} // namespace survarium
