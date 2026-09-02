// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef BOOSTERS_ENUM_H_INCLUDED
#define BOOSTERS_ENUM_H_INCLUDED

namespace survarium {

enum boosters_enum {
	dispersion_correction_perc_id		= 0x1,
	aiming_speed_correction_perc_id		= 0x2,
	health_regen_correction_perc_id		= 0x3,
	stamina_regen_correction_perc_id	= 0x4,
	movement_speed_correction_perc_id	= 0x5,
	additional_max_weight_id			= 0x6,
	pain_healt_correction_perc_id		= 0x7,
	artcontainer_time_corr_perc_id		= 0x8,
	anomaly_damage_corr_perc_id			= 0x9,
	engineer_use_time_corr_perc_id		= 0xA,
	engineer_succ_chance_corr_perc_id	= 0xB,
};


} // namespace survarium

#endif // #ifndef BOOSTERS_ENUM_H_INCLUDED
