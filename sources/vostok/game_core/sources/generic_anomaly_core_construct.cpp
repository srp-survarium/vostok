////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/generic_anomaly_core.h>

#include <vostok/game_core/anomaly_state.h>
#include <vostok/game_core/zone_group.h>
#include <vostok/game_core/base_project.h>
#include <vostok/game_core/artefact_container_core.h>
#include <vostok/game_core/damage_zone_core.h>
#include <vostok/game_core/zone_group.h>


namespace survarium {

// STATE[96.07%|PARTIAL]: vectora::resize / math::min / VOSTOK_NEW inline-vs-call LTCG, non-steerable. trail: generic_anomaly_core_construct.md
// STRUCTURE DIFF[target 0x58c9c0 | base 0x455330]: target 54 / base 54 stmts
// 0x088 <0x22> | 0x088 <0x26> | m_artefact_containers.resize( artefact_containers_count );   SIZE
// 0x0aa <0x1a> | 0x0ae <0x32> | artefacts_max_count = math::min( artefacts_max_count, artefact_containers_count );   SIZE
// 0x2a1 <0x79> | 0x2bd <0x85> | anomaly_state* state = VOSTOK_NEW_IMPL( g_allocator, anomaly_state )( this );   SIZE
// ; aligned 51, size-diffs 3, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - 3 SIZE: vectora::resize, math::min (template), and the VOSTOK_NEW anomaly_state allocation are inlined to a different size; whole-program LTCG, non-steerable.
void generic_anomaly_core::load( configs::binary_config_value const& config )
{
	artefacts_enabled			= (bool)config["artefacts_enabled"];
	artefacts_max_count			= (u32)config["artefacts_max_count"];
	artefacts_respawn_time_sec	= (u32)config["artefacts_respawn_time_sec"];
	if ( artefacts_enabled )
	{
		u32 artefact_containers_count = config["artefact_containers"].size( );
		m_artefact_containers.resize( artefact_containers_count );
		artefacts_max_count = math::min( artefacts_max_count, artefact_containers_count );
	}

	energy_enabled				= (bool)config["energy_enabled"];
	m_energy_current			= (float)config["energy_initial"];
	energy_decrease_speed		= (u32)config["energy_decrease_speed"];
	energy_af_container_use		= (u32)config["energy_af_container_use"];
	energy_on_walk				= (u32)config["energy_on_walk"];
	energy_on_run				= (u32)config["energy_on_run"];
	energy_on_sprint			= (u32)config["energy_on_sprint"];
	energy_on_jump				= (u32)config["energy_on_jump"];
	energy_on_shoot				= (u32)config["energy_on_shoot"];
	energy_on_character_hit		= (u32)config["energy_on_character_hit"];
	energy_on_explosion			= (u32)config["energy_on_explosion"];
	energy_on_character_kill	= (u32)config["energy_on_character_kill"];

	u32 states_count			= config["states"].size( );
	m_states.resize( states_count );
	for ( u32 s = 0; s < states_count ; ++s )
	{
		configs::binary_config_value current_state = config["states"][s];
		anomaly_state* state = VOSTOK_NEW_IMPL( g_allocator, anomaly_state )( this );
		m_states[s] = state;
		state->debug_idx				= s;
		state->enabled					= (bool)current_state["enabled"];
		state->energy_threshold			= (u32)current_state["energy_threshold"];
		state->shoot_trigger			= (bool)current_state["shoot_trigger"];
		state->zone_activity_trigger	= (bool)current_state["zone_activity_trigger"];
		state->active_time_sec			= (u32)current_state["active_time_sec"];
		state->energy_on_exit			= (u32)current_state["energy_on_exit"];
		state->select_priority			= (u32)current_state["select_priority"];

		u32 groups_count				= (u32)current_state["groups"].size( );
		state->groups.resize( groups_count );
		for ( u32 g = 0 ; g < groups_count ; ++g )
		{
			configs::binary_config_value current_group = current_state["groups"][g];
			zone_group* group = VOSTOK_NEW_IMPL( g_allocator, zone_group )( state );
			state->groups[g] = group;
			group->enabled				= (bool)current_group["enabled"];
			group->max_charged_count	= (u32)current_group["max_charged_count"];
			group->recharge_time_sec	= (u32)current_group["recharge_time_sec"];

			u32 zones_count = current_group["zones"].size( );
			group->zones.resize( zones_count );
			math::clamp<u32>( group->max_charged_count, 0, zones_count );
		}
	}
}

// STATE[99.63%|DONE]: frame size differs (slot allocation), non-steerable. trail: generic_anomaly_core_construct.md
// STRUCTURE DIFF[target 0x58c630 | base 0x454fa0]: target 5 / base 5 stmts
// .. same ..
// ; aligned 5, size-diffs 0, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - all 5 statements align with 0 diffs; residual is a frame-size / slot-allocation difference, non-steerable. (Prior BLOCKED note about Ghidra generation is obsolete - the function is fully reconstructed and matches.)
bool state_prio( anomaly_state* s1, anomaly_state* s2 )
{	// true on less
	if ( s1->enabled == s2->enabled )
		return s1->energy_threshold == s2->energy_threshold
			? s1->energy_threshold < s2->energy_threshold	// sushi@NOTE: Bug they didn't notice
			: s2->select_priority < s1->select_priority;
	else
		return !s1->enabled < !s2->enabled; // s1->enabled && !s2->enabled
}

// STATE[100%|DONE]
void generic_anomaly_core::resolve_links( base_project* p, configs::binary_config_value config )
{
	u32 artefact_containers_count = m_artefact_containers.size( );
	for ( u32 a = 0 ; a < artefact_containers_count ; ++a )
	{
		pcstr full_path_name = (pcstr)config["artefact_containers"][a];
		m_artefact_containers[a] = static_cast_checked< artefact_container_core* >( p->get_object_by_name( full_path_name ) );
	}

	u32 states_count = m_states.size( );
	for ( u32 s = 0 ; s < states_count ; ++s )
	{
		anomaly_state* state = m_states[s];
		for ( u32 g = 0 ; g < state->groups.size( ) ; ++g )
		{
			zone_group* group = state->groups[g];
			for ( u32 z = 0 ; z < group->zones.size( ) ; ++z )
			{
				pcstr full_path_name = (pcstr)config["states"][s]["groups"][g]["zones"][z];
				group->zones[z].zone = static_cast_checked< damage_zone_core* >( p->get_object_by_name( full_path_name ) );
				group->zones[z].zone->set_standalone( false );
			}
		}
	}

	std::sort( m_states.begin( ), m_states.end( ), state_prio );
}

} // namespace survarium
