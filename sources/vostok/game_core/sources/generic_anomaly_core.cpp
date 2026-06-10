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

// STATE[77.00%|PARTIAL]: vtable/base-init codegen of the multiply-inherited NO_VTABLE hierarchy, non-steerable.
generic_anomaly_core::generic_anomaly_core( ):
	m_artefact_grab_time_ms		( 0 ),
	m_current_state				( NULL ),
	m_was_zone_trigger_event	( false ),
	m_was_shoot_trigger_event	( false ),
	m_physics_world				( NULL ),
	m_scheduler					( NULL )
{
	// STRUCTURE DIFF: target 0 stmts / base 0 stmts (init-list only; 0xab vs 0xb2)
	// VERDICT: STRUCTURE MATCH - residual is the compiler-emitted base-subobject ctor calls + vtable stores of the multiply-inherited (NO_VTABLE + virtuals) hierarchy, decided by the base-class declarations, non-steerable.
}

// STATE[99.91%|DONE]: two adjacent member sub-object dtor calls emitted in swapped order, non-steerable here.
generic_anomaly_core::~generic_anomaly_core( )
{
	// STRUCTURE DIFF: target 0 stmts / base 0 stmts (0x4c both) - no diverging rows
	// VERDICT: STRUCTURE MATCH - empty body; sole residual: target destroys the members at +9 then +8, base at +8 then +9 (one swapped pair), a member-layout/destruction-order quirk needing a header member reorder (wide blast radius), out of scope at 99.91%.
}

// STATE[83.49%|PARTIAL]: boost::bind / register_on_frame inlined one layer deeper in target, non-steerable.
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

	// STRUCTURE DIFF: target 11 stmts / base 11 stmts
	// SIZE +0x3e | 58 | m_scheduler->register_on_frame( &m_scheduler_identifier, boost::bind( &generic_anomaly_core::tick, this, _1, _2 ), true );
	// SIZE +0x21 | 62 | m_artefact_containers[a]->activate( this, world, scheduler );
	// (7 more +0x3 rows = disp8->disp32 frame noise cascading from the larger base frame)
	// VERDICT: STRUCTURE MATCH (11/11) - the register_on_frame(boost::bind) machinery is inlined one layer deeper in target (0xce vs 0x10c); whole-program LTCG, non-steerable. The +0x3 rows are slot-displacement-width noise, not structure.
}

// STATE[99.68%|DONE]: frame size differs (slot allocation), non-steerable.
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

	// STRUCTURE DIFF: target 8 stmts / base 8 stmts (0xbb both) - no diverging rows
	// VERDICT: STRUCTURE MATCH - clean alignment; sole residual is a frame-size / [ebp-N] slot-allocation difference, non-steerable.
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
