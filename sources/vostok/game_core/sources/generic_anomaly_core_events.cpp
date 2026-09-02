// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/generic_anomaly_core.h>

namespace survarium {

void generic_anomaly_core::on_player_action( hit_receiver const* receiver, player_actions_subscriber::action action, float param )
{
	switch ( action )
	{
		case player_actions_subscriber::run:
			inc_energy( energy_on_run * param );
		break;
		case player_actions_subscriber::sprint:
			inc_energy( energy_on_sprint * param );
		break;
		case player_actions_subscriber::jump:
			inc_energy( (float)energy_on_jump );
		break;
		case player_actions_subscriber::shoot:
			inc_energy( (float)energy_on_shoot );
			m_was_shoot_trigger_event = true;
		break;
		case player_actions_subscriber::hit:
			inc_energy( (float)energy_on_character_hit );
		break;
		case player_actions_subscriber::kill:
			inc_energy( (float)energy_on_character_kill );
		break;
		default:
			NODEFAULT( );
	}
}

void generic_anomaly_core::on_artefact_container_use( artefact_container_core* container )
{
	VOSTOK_UNREFERENCED_PARAMETER( container );
	inc_energy( (float)energy_af_container_use );
	if ( m_artefact_grab_time_ms == 0 )
		m_artefact_grab_time_ms = m_current_time;
}

void generic_anomaly_core::on_zone_act( damage_zone_core* zone, hit_receiver* receiver )
{
	VOSTOK_UNREFERENCED_PARAMETERS( zone, receiver );
	m_was_zone_trigger_event = true;
}

void generic_anomaly_core::on_hit_receiver_enter( hit_receiver* receiver, damage_zone_core* zone )
{
	VOSTOK_UNREFERENCED_PARAMETERS( receiver, zone );
}

void generic_anomaly_core::on_hit_receiver_leave( hit_receiver* receiver, damage_zone_core* zone )
{
	VOSTOK_UNREFERENCED_PARAMETERS( receiver, zone );
}

} // namespace survarium
