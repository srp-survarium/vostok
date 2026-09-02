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

// claude@NOTE: the target unit also holds ??4 survarium::map< fixed_string<16>,
// hit_type_parameters_modifyer >::operator= as a line-less COMDAT (fallback
// attribution). Our /GL base inlines survarium::map::operator= (declared
// `inline` in map.h) at its only call site (player_parameters_cook.cpp:226 via
// the implicit body_part_parameters_modifyer::operator=), so no standalone
// symbol is emitted; forcing it out-of-line means de-inlining shared map.h
// infra, and even then base line records would attribute it to map_inline.h,
// not this unit (same lottery as the 1191 unpaired ??_G dtors). Parked.

} // namespace survarium

#endif // #ifndef PLAYER_PARAMETERS_COOK_H_INCLUDED
