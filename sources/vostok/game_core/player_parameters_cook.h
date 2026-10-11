// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PLAYER_PARAMETERS_COOK_H_INCLUDED
#define PLAYER_PARAMETERS_COOK_H_INCLUDED

#include <vostok/game_core/player_parameters_modifyer.h>

namespace survarium {

player_parameters_modifyer::player_parameters_modifyer( ) :
	speed_modifyer                 ( 0.0f ),
	total_items_weight             ( 0.0f ),
	dispersion_correction_perc     ( 0.0f ),
	aiming_speed_correction_perc   ( 0.0f ),
	movement_speed_correction_perc ( 0.0f ),
	additional_max_weight          ( 0.0f ),
	stamina_regen_correction_perc  ( 0.0f ),
	health_regen_correction_perc   ( 0.0f ),
	pain_healt_correction_perc     ( 0.0f ),
	artcontainer_time_corr_perc    ( 0.0f ),
	anomaly_damage_corr_perc       ( 0.0f ),
	engineer_use_time_corr_perc    ( 0.0f ),
	engineer_succ_chance_corr_perc ( 0.0f ),
	additional_artefact_slots      ( 0 ),
	additional_devices_slots       ( 0 )
{
}










} // namespace survarium

#endif // #ifndef PLAYER_PARAMETERS_COOK_H_INCLUDED
