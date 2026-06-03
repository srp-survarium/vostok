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

// STATE[96.31%|DONE]
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

	// FUNCTION BODY[0x59c9c0]: 59
	// <0>
	// <0x59c9d0>|0x010|+0x01d:'18'
	// <0x59c9ed>|0x02d|+0x01b:'19'
	// <0x59ca08>|0x048|+0x01b:'20'
	// <0x59ca23>|0x063|+0x00e:'21'
	// <0>
	// <0x59ca31>|0x071|+0x017:'23'
	// <0x59ca48>|0x088|+0x022:'24'
	// <0x59ca6a>|0x0aa|+0x01a:'25'
	// <0>
	// <1>
	// <2>
	// <0x59ca84>|0x0c4|+0x01d:'29'
	// <0x59caa1>|0x0e1|+0x01d:'30'
	// <0x59cabe>|0x0fe|+0x01b:'31'
	// <0x59cad9>|0x119|+0x01b:'32'
	// <0x59caf4>|0x134|+0x01b:'33'
	// <0x59cb0f>|0x14f|+0x01b:'34'
	// <0x59cb2a>|0x16a|+0x01b:'35'
	// <0x59cb45>|0x185|+0x01b:'36'
	// <0x59cb60>|0x1a0|+0x01b:'37'
	// <0x59cb7b>|0x1bb|+0x01b:'38'
	// <0x59cb96>|0x1d6|+0x01b:'39'
	// <0x59cbb1>|0x1f1|+0x01b:'40'		energy_on_character_kill	= (u32)config["energy_on_character_kill"];
	// <0>
	// <1>
	// <0x59cbcc>|0x20c|+0x017:'43'
	// <0x59cbe3>|0x223|+0x026:'44'
	// <0x59cc09>|0x249|+0x01e|[1]:'45'	for ( u32 s = 0; s < states_count ; ++s )
	// <0>
	// <0x59cc27>|0x267|+0x03a:'47'			configs::binary_config_value current_state = config["states"][s];
	// <0x59cc61>|0x2a1|+0x079:'48'			anomaly_state* state = VOSTOK_NEW_IMPL( g_allocator, anomaly_state )( this );
	// <0x59ccda>|0x31a|+0x040:'49'			m_states[s] = state;
	// <0x59cd1a>|0x35a|+0x009:'50'
	// <0x59cd23>|0x363|+0x019:'51'
	// <0x59cd3c>|0x37c|+0x018:'52'
	// <0x59cd54>|0x394|+0x01a:'53'
	// <0x59cd6e>|0x3ae|+0x01a:'54'
	// <0x59cd88>|0x3c8|+0x018:'55'
	// <0x59cda0>|0x3e0|+0x018:'56'
	// <0x59cdb8>|0x3f8|+0x018:'57'
	// <0>
	// <0x59cdd0>|0x410|+0x017:'59'
	// <0x59cde7>|0x427|+0x023:'60'			u32 groups_count				= (u32)current_state["groups"];
	// <0x59ce0a>|0x44a|+0x01e|[3]:'61'		for ( u32 g = 0 ; g < groups_count ; ++g )
	// <0>
	// <0x59ce28>|0x468|+0x03a:'63'				configs::binary_config_value current_group = current_state["groups"];
	// <0x59ce62>|0x4a2|+0x082:'64'				zone_group* group = VOSTOK_NEW_IMPL( g_allocator, zone_group )( this );
	// <0x59cee4>|0x524|+0x03d:'65'				state->groups[g] = group;
	// <0x59cf21>|0x561|+0x019:'66'
	// <0x59cf3a>|0x57a|+0x018:'67'
	// <0x59cf52>|0x592|+0x018:'68'
	// <0>
	// <0x59cf6a>|0x5aa|+0x017:'70'
	// <0x59cf81>|0x5c1|+0x01b:'71'
	// <0x59cf9c>|0x5dc|+0x010:'72'
	// <0x59cfac>|0x5ec|+0x005:'73'
	// <0x59cfb1>|0x5f1|+0x005:'74'
	// <0>
	// ******
}

// STATE[BLOCKED]: Blocked until I update Ghidra script to properly generate functions like these.
bool state_prio( anomaly_state* s1, anomaly_state* s2 )
{	// true on less
	if ( s1->enabled == s2->enabled )
		return s1->energy_threshold == s2->energy_threshold
			? s1->energy_threshold < s2->energy_threshold	// sushi@NOTE: Bug they didn't notice
			: s2->select_priority < s1->select_priority;
	else
		return !s1->enabled < !s2->enabled; // s1->enabled && !s2->enabled

	// FUNCTION BODY[0x59c630]: 8
	// <0x59c634>|0x004|+0x010:'80'
	// <0>
	// <1>
	// <2>
	// <0x59c644>|0x014|+0x03b:'84'
	// <0x59c67f>|0x04f|+0x002:'85'
	// <0x59c681>|0x051|+0x023:'86'
	// <0>
	// ******
}

// STATE[99.00%|DONE]
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

	// FUNCTION BODY[0x59c6b0]: 24
	// <0x59c6bf>|0x00f|+0x011:'92'
	// <0x59c6d0>|0x020|+0x01e|[1]:'93'		for ( u32 a = 0 ; a < artefact_containers_count ; ++a )
	// <0>									{
	// <0x59c6ee>|0x03e|+0x021|[2]:'95'			pcstr full_path_name = config["artefact_containers"][a];
	// <0x59c70f>|0x05f|+0x069:'96'				m_artefact_containers[a] = static_cast_checked< artefact_container_core* >( p->get_object_by_name( full_path_name ) );
	// <0x59c778>|0x0c8|+0x005:'97'			}
	// <0>
	// <0x59c77d>|0x0cd|+0x011:'99'
	// <0x59c78e>|0x0de|+0x01e|[2]:'100'	for ( u32 s = 0 ; a < states_count ; ++s )
	// <0>									{
	// <0x59c7ac>|0x0fc|+0x034|[3]:'102'		anomaly_state* state = m_states[s];
	// <0x59c7e0>|0x130|+0x026|[4]:'103'		for ( u32 g = 0 ; g < state->groups.size( ) ; ++g )
	// <0>										{
	// <0x59c806>|0x156|+0x031|[5]:'105'			zone_group* group = state->groups[g];
	// <0x59c837>|0x187|+0x032|[6]:'106'			for ( u32 z = 0 ; z < group->zones.size( ) ; ++z )
	// <0>
	// <0x59c869>|0x1b9|+0x04f|[7]:'108'				pcstr full_path_name = (pcstr)config["states"][s]["groups"][g]["zones"][z];
	// <0x59c8b8>|0x208|+0x056:'109'
	// <0x59c90e>|0x25e|+0x037:'110'					group->zones[z].zone->set_standalone( false );
	// <0x59c945>|0x295|+0x005:'111'				}
	// <0x59c94a>|0x29a|+0x005:'112'			}
	// <0x59c94f>|0x29f|+0x005:'113'		}
	// <0>
	// <0x59c954>|0x2a4|+0x061:'115'
	// ******
}

} // namespace survarium
