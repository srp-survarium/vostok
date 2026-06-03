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

// STATE[SKIPPED]: sushi@TODO: Most likely something to do with inheritance + NO_VTABLE + virtual methods
generic_anomaly_core::generic_anomaly_core( ):
	m_artefact_grab_time_ms		( 0 ),
	m_current_state				( NULL ),
	m_was_zone_trigger_event	( false ),
	m_was_shoot_trigger_event	( false ),
	m_physics_world				( NULL ),
	m_scheduler					( NULL )
{
}

// STATE[SKIPPED]: sushi@TODO: Has two simple instructions swapped
generic_anomaly_core::~generic_anomaly_core( )
{
}

// STATE[79.69%|PARTIAL]: boost::bind in target inlined for one more layer
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

	// FUNCTION BODY[0x59dc10]: 14
	// <0x59dc1a>|0x00a|+0x00c:'28'
	// <0x59dc26>|0x016|+0x00c:'29'
	// <0x59dc32>|0x022|+0x009:'30'
	// <0x59dc3b>|0x02b|+0x009:'31'
	// <0x59dc44>|0x034|+0x0ce:'32'
	// <0>
	// <0x59dd12>|0x102|+0x022|[1]:'34'	for ( u32 a = 0 ; a < m_artefact_containers.size( ) ; ++a )
	// <0>								{
	// <0x59dd34>|0x124|+0x050:'36'
	// <0x59dd84>|0x174|+0x002:'37'		}
	// <0>
	// <0x59dd86>|0x176|+0x007:'39'
	// <0x59dd8d>|0x17d|+0x007:'40'
	// <0x59dd94>|0x184|+0x008:'41'
	// ******
}

// STATE[99.68%|DONE]: Different stack size
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

	// FUNCTION BODY[0x59d7c0]: 11
	// <0x59d7c0>|0x000|+0x009:'45'	{
	// <0x59d7c9>|0x009|+0x00c:'46'
	// <0x59d7d5>|0x015|+0x012:'47'
	// <0x59d7e7>|0x027|+0x009:'48'
	// <0>
	// <0x59d7f0>|0x030|+0x00b:'50'
	// <0x59d7fb>|0x03b|+0x00a:'51'
	// <0>
	// <0x59d805>|0x045|+0x022|[1]:'53'
	// <0x59d827>|0x067|+0x046:'54'
	// <0>
	// <0x59d86d>|0x0ad|+0x00a:'56'
	// <0x59d877>|0x0b7|      :'57'	}
	// ******
}

// STATE[100%|DONE]
void generic_anomaly_core::inc_energy( float amount )
{
	ASSERT( UNKNOWN_EXPRESSION );
	if ( energy_enabled )
	{
		m_energy_current += amount;
		ASSERT( UNKNOWN_EXPRESSION );
	}
}

// STATE[100%|DONE]
void generic_anomaly_core::dec_energy( float amount )
{
	ASSERT( UNKNOWN_EXPRESSION );
	if ( energy_enabled )
	{
		m_energy_current -= math::min( m_energy_current, amount );
		ASSERT( UNKNOWN_EXPRESSION );
	}
}

// STATE[100%|DONE]
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

// STATE[100%|DONE]
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

// STATE[100%|DONE]
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

// STATE[100%|DONE]
void anomaly_state::initialize( )
{
	for ( u32 g = 0 ; g < groups.size( ) ; ++g )
		groups[g]->initialize( );

	if ( active_time_sec )
		m_finish_time_ms = owner->current_time( ) + 1000 * active_time_sec;
	else
		m_finish_time_ms = 0;
}

// STATE[100%|DONE]
void anomaly_state::execute( u32 const time_delta_ms, u32 const current_time_ms )
{
	for ( u32 g = 0 ; g < groups.size( ) ; ++g )
		groups[g]->execute( time_delta_ms, current_time_ms );

	if ( m_finish_time_ms && current_time_ms > m_finish_time_ms )
		owner->set_current_energy( (float)energy_on_exit );
}

// STATE[100%|DONE]
void anomaly_state::finalize( )
{
	for ( u32 g = 0 ; g < groups.size( ) ; ++g )
		groups[g]->finalize( );
}

// STATE[100%|DONE]
void zone_group::initialize( )
{
	charged_count = 0;
	recharge( );
}

// STATE[100%|DONE]
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

// STATE[100%|DONE]
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

// STATE[100%|DONE]
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

// STATE[100%|DONE]
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
