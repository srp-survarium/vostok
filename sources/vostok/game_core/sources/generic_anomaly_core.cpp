// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/generic_anomaly_core.h>

#include <vostok/game_core/anomaly_state.h>
#include <vostok/game_core/artefact_container_core.h>
#include <vostok/game_core/damage_zone_core.h>
#include <vostok/game_core/zone_group.h>

#include <vostok/game_core/scheduler.h>

namespace survarium {

generic_anomaly_core::generic_anomaly_core( ):
	m_artefact_grab_time_ms		( 0 ),
	m_current_state				( NULL ),
	m_was_zone_trigger_event	( false ),
	m_was_shoot_trigger_event	( false ),
	m_physics_world				( NULL ),
	m_scheduler					( NULL )
{
}

generic_anomaly_core::~generic_anomaly_core( )
{
}

void generic_anomaly_core::activate( physics::world* world, survarium::scheduler& scheduler )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	m_physics_world = world;
	m_scheduler = &scheduler;
	m_scheduler->register_on_frame( &m_scheduler_identifier, boost::bind( &generic_anomaly_core::tick, this, _1, _2 ), true );

	for ( u32 a = 0 ; a < m_artefact_containers.size( ) ; ++a )
	{
		m_artefact_containers[a]->activate( this, world, scheduler );
	}

	m_was_zone_trigger_event = false;
	m_was_shoot_trigger_event = false;
	spawn_artefacts( );
}

void generic_anomaly_core::deactivate( )
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_scheduler->unregister( &m_scheduler_identifier );
	if ( m_current_state )
	{
		m_current_state->finalize( );
		m_current_state = NULL; // sushi@NOTE: Wouldn't you call free? No, since states are stored in a different place, this is just a pointer.
	}
	for ( u32 a = 0 ; a < m_artefact_containers.size( ) ; ++a )
		m_artefact_containers[a]->deactivate( );

	m_scheduler = NULL;
}

void generic_anomaly_core::inc_energy( float amount )
{
	ASSERT( UNKNOWN_EXPRESSION );
	if ( energy_enabled )
	{
		m_energy_current += amount;
		ASSERT( UNKNOWN_EXPRESSION );
	}
}

void generic_anomaly_core::dec_energy( float amount )
{
	ASSERT( UNKNOWN_EXPRESSION );
	if ( energy_enabled )
	{
		m_energy_current -= math::min( m_energy_current, amount );
		ASSERT( UNKNOWN_EXPRESSION );
	}
}

void generic_anomaly_core::tick( u32 const time_delta_ms, u32 const current_time_ms )
{
	m_current_time = current_time_ms;
	dec_energy( energy_decrease_speed * ( time_delta_ms / 1000.0f ) );

	anomaly_state* state = select_state( );
	ASSERT( UNKNOWN_EXPRESSION ); // sushi@NOTE: Something related to state?

	if ( state != m_current_state )
	{
		if ( m_current_state )
			m_current_state->finalize( );
		m_current_state = state;
		m_current_state->initialize( );
	}
	m_current_state->execute( time_delta_ms, current_time_ms );
	if ( m_artefact_grab_time_ms && m_artefact_grab_time_ms + 1000 * artefacts_respawn_time_sec < current_time_ms )
		spawn_artefacts( );
}

void generic_anomaly_core::spawn_artefacts( )
{
	ASSERT( UNKNOWN_EXPRESSION );
	u32 cont_total = m_artefact_containers.size( );

	buffer_vector< artefact_container_core* > empty_artefact_containers ( ALLOCA( sizeof( artefact_container_core* ) * cont_total ), cont_total );

	for ( u32 a = 0; a < cont_total ; ++a )
	{
		if ( !m_artefact_containers[a]->has_artefact( ) )
			empty_artefact_containers.push_back( m_artefact_containers[a] );
	}

	u32 artefacts_current	= cont_total - empty_artefact_containers.size( );
	u32 respawn_cnt			= artefacts_max_count - artefacts_current;

	if ( respawn_cnt )
	{
		std::random_shuffle( empty_artefact_containers.begin( ), empty_artefact_containers.end( ) ); // sushi@NOTE: How is this supposed to be reproducible?
		for ( u32 i = 0 ; i < respawn_cnt ; ++i )
			empty_artefact_containers[i]->spawn_artefact( ); // sushi@NOTE: This means everything will be spawned?
	}

	m_artefact_grab_time_ms = 0;
}

anomaly_state* generic_anomaly_core::select_state( )
{
	anomaly_state* state = m_states[0];

	for ( u32 i = 0; i < m_states.size( ) ; ++i )
	{
		anomaly_state* s = m_states[i];
		if ( !s->enabled )
			continue;

		if ( s->energy_threshold > m_energy_current )
			break;
		if ( s->zone_activity_trigger && !m_was_zone_trigger_event )
			continue;
		if ( s->shoot_trigger && !m_was_shoot_trigger_event )
			continue;

		state = s;
	}

	return state;
}

void anomaly_state::initialize( )
{
	for ( u32 g = 0 ; g < groups.size( ) ; ++g )
		groups[g]->initialize( );

	if ( active_time_sec )
		m_finish_time_ms = owner->current_time( ) + 1000 * active_time_sec;
	else
		m_finish_time_ms = 0;
}

void anomaly_state::execute( u32 const time_delta_ms, u32 const current_time_ms )
{
	for ( u32 g = 0 ; g < groups.size( ) ; ++g )
		groups[g]->execute( time_delta_ms, current_time_ms );

	if ( m_finish_time_ms && current_time_ms > m_finish_time_ms )
		owner->set_current_energy( (float)energy_on_exit );
}

void anomaly_state::finalize( )
{
	for ( u32 g = 0 ; g < groups.size( ) ; ++g )
		groups[g]->finalize( );
}

void zone_group::initialize( )
{
	charged_count = 0;
	recharge( );
}

void zone_group::execute( u32 const time_delta_ms, u32 const current_time_ms )
{
	if ( next_recharge_time && current_time_ms >= next_recharge_time )
		recharge( );

	for ( u32 z = 0 ; z < zones.size( ) ; ++z )
	{
		if ( zones[z].active )
			zones[z].zone->tick( time_delta_ms, current_time_ms );
	}
}

void zone_group::finalize( )
{
	for ( u32 z = 0 ; z < zones.size( ) ; ++z )
	{
		if ( zones[z].active )
		{
			zones[z].zone->deactivate( );
			zones[z].active = false;
		}
	}
}

void zone_group::recharge( )
{
	for ( u32 z = 0 ; z < zones.size( ) ; ++z )
	{
		if ( !zones[z].active )
		{
			zones[z].zone->activate( this, core( )->physics_world( ), core( )->scheduler( ) );
			zones[z].active = true;
		}
	}
	next_recharge_time = 0;
}

void zone_group::on_zone_act( damage_zone_core* zone, hit_receiver* receiver )
{
	core( )->on_zone_act( zone, receiver );

	for ( u32 z = 0 ; z < zones.size( ) ; ++z )
	{
		if ( zones[z].zone == zone )
		{
			ASSERT( UNKNOWN_EXPRESSION_T( zones[z].active ) );
			zones[z].zone->deactivate( );
			zones[z].active = false;
			next_recharge_time = core( )->current_time( ) + 1000 * recharge_time_sec;
			break;
		}
	}
}

} // namespace survarium
