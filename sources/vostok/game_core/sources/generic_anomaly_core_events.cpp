////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/generic_anomaly_core.h>

namespace survarium {

// STATE[BLOCKED]: Ghidra fails to parse functions with NODEFAULT, even with "Decompiler Switch Analysis" analyzer enabled! I compared manually in IDA and match seems to be correct.
void generic_anomaly_core::on_player_action( hit_receiver const* receiver, player_actions_subscriber::action action, float param )
{
	switch ( action )
	{
		case player_actions_subscriber::walk:
			inc_energy( energy_on_walk * param );
		break;
		case player_actions_subscriber::run:
			inc_energy( energy_on_run * param );
		break;
		case player_actions_subscriber::sprint:
			inc_energy( energy_on_sprint * param );
		break;
		case player_actions_subscriber::jump:
			inc_energy( energy_on_jump * param );
			m_was_zone_trigger_event = true;
		break;
		case player_actions_subscriber::shoot:
			inc_energy( energy_on_shoot * param );
		break;
		case player_actions_subscriber::character_hit:
			inc_energy( energy_on_character_hit * param );
		break;
		default:
			NODEFAULT( );
	}

	// FUNCTION BODY[0x6fcd30]: 25
	// <0>
	// <0x6fcd39>|0x009|+0x010:'16'	switch ( action )
	// <0>							{
	// <1>							case player_actions_subscriber::walk:
	// <0x6fcd49>|0x019|+0x025:'19'		inc_energy( energy_on_walk * param );
	// <0x6fcd6e>|0x03e|+0x005:'20'	break;
	// <0>							case player_actions_subscriber::run:
	// <0x6fcd73>|0x043|+0x025:'22'		inc_energy( energy_on_run * param );
	// <0x6fcd98>|0x068|+0x005:'23'	break;
	// <0>							case player_actions_subscriber::sprint:
	// <0x6fcd9d>|0x06d|+0x022:'25'
	// <0x6fcdbf>|0x08f|+0x002:'26'
	// <0>							case player_actions_subscriber::jump:
	// <0x6fcdc1>|0x091|+0x022:'28'
	// <0x6fcde3>|0x0b3|+0x007:'29'
	// <0x6fcdea>|0x0ba|+0x002:'30'
	// <0>							case player_actions_subscriber::shoot:
	// <0x6fcdec>|0x0bc|+0x022:'32'
	// <0x6fce0e>|0x0de|+0x002:'33'
	// <0>							case player_actions_subscriber::character_hit:
	// <0x6fce10>|0x0e0|+0x022:'35'
	// <0x6fce32>|0x102|+0x002:'36'	break;
	// <0>
	// <1>
	// <2>							}
	// ******
}

// STATE[100%|DONE]
void generic_anomaly_core::on_artefact_container_use( artefact_container_core* container )
{
	VOSTOK_UNREFERENCED_PARAMETER( container );
	inc_energy( (float)energy_af_container_use );
	if ( m_artefact_grab_time_ms == 0 )
		m_artefact_grab_time_ms = m_current_time;

	// FUNCTION BODY[0x6fcce0]: 4
	// <0>
	// <0x6fcce9>|0x009|+0x01f:'53'
	// <0x6fcd08>|0x028|+0x009:'54'
	// <0x6fcd11>|0x031|+0x00c:'55'
	// ******
}

// STATE[100%|DONE]
void generic_anomaly_core::on_zone_act( damage_zone_core* zone, hit_receiver* receiver )
{
	VOSTOK_UNREFERENCED_PARAMETERS( zone, receiver );
	m_was_zone_trigger_event = true;

	// FUNCTION BODY[0x6fcca0]: 2
	// <0x6fcca9>|0x009|+0x023:'60'
	// <0x6fcccc>|0x02c|+0x007:'61'
	// ******
}

// STATE[100%|DONE]
void generic_anomaly_core::on_hit_receiver_enter( hit_receiver* receiver, damage_zone_core* zone )
{
	VOSTOK_UNREFERENCED_PARAMETERS( receiver, zone );

	// FUNCTION BODY[0x6fcc60]: 1
	// <0x6fcc69>|0x009|+0x023:'66'
	// ******
}

// STATE[100%|DONE]
void generic_anomaly_core::on_hit_receiver_leave( hit_receiver* receiver, damage_zone_core* zone )
{
	VOSTOK_UNREFERENCED_PARAMETERS( receiver, zone );

	// FUNCTION BODY[0x6fcc20]: 1
	// <0x6fcc29>|0x009|+0x023:'71'
	// ******
}

} // namespace survarium
