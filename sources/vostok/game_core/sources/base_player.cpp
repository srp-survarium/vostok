// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/base_player.h>

#include <vostok/game_core/base_player_creation_params.h>
#include <vostok/game_core/player_profile.h>
#include <vostok/game_core/scheduler.h>
#include <vostok/game_core/weapon_core.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>
#include <vostok/animation/animation_playback_state.h>

namespace survarium {

base_player::base_player( base_player_creation_params const& params, survarium::scheduler& the_scheduler ) :
	inventory_holder			( the_scheduler, params.inventory ),
	hit_initiator				( params.initial_info.id, params.initial_info.profile->is_local ),
	m_recoil_params				( params.recoil_params ),
	m_dispersion_params			( params.dispersion_params ),
	m_breath_holding_params		( params.breath_holding_params ),
	m_damage_model				( params.damage_model ),
	m_movement_speed_factor		( 1.0f ),
	m_force_animation_selection	( false ),
	m_is_alive					( false ),
	m_is_replaying_history		( false ),
	m_has_been_inserted			( false )
{
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

void base_player::send_game_world_object( game_world_object const* object, reciver_packet_allocator_type const& reciver_packet_allocator, reciver_enqueuer_type const& reciver_enqueuer ) const
{
	network_core::udp_match_packet&	packet	= reciver_packet_allocator( );

	ASSERT( UNKNOWN_EXPRESSION );
	packet.append		( id );

	ASSERT( UNKNOWN_EXPRESSION );
	inventory_item const*	item	= object->owner( );

	ASSERT( UNKNOWN_EXPRESSION );
	packet.append		( item ? item->profile_slot_id( ) : invalid_slot );

	object->serialize	( packet );

	reciver_enqueuer	( packet );
}

// claude@NOTE: structure matches the target (3 stmts), but the base inlines what the
// target keeps out-of-line - reader.r<bool>, inventory(), inventory::item_in_slot
// (and its bounds ASSERT), the intrusive_ptr ctor/operator*/dtor - so the byte match
// collapses. Inline-vs-call, not source-steerable.
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

// claude@NOTE: reconstructed forwarding target of base_player::get_animation_playback_state.
// The real callee COMDAT-folded to a bare `ret` whole-program (name unrecoverable); this empty
// body re-folds to the same `ret`, so the caller's `call rel32` is byte-identical. result is
// taken BY VALUE - the target pushes its two members (interval_id, interval_time) individually.
// sushi@TODO: helper NAME is a guess (the real callee folded away); structure/bytes of the
// forwarding call are name-independent, but identify the true callee to retire this base-only symbol.
bool query_animation_playback_state( pcvoid const object, u32 const mask, animation::animation_playback_state result )
{
	VOSTOK_UNREFERENCED_PARAMETERS( object, mask, result );
	VOSTOK_UNREACHABLE_CODE( );
	return false;
}

} // namespace survarium
