// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef PLAYER_PARAMETERS_MODIFYER_H_INCLUDED
#define PLAYER_PARAMETERS_MODIFYER_H_INCLUDED

#include <vostok/game_core/body_part_parameters_modifyer.h>

namespace survarium {

struct base_player;

struct player_parameters_modifyer : public resources::unmanaged_resource , private boost::noncopyable {
public:
	explicit		player_parameters_modifyer	( );

public:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	/* boost::noncopyable */ // sushi@TODO: fixed_string is a body_part_name
	/* 0x0108 */	map< fixed_string<16>, body_part_parameters_modifyer >	body_part_parameters_modifyers;
	/* 0x0120 */	float			speed_modifyer;
	/* 0x0124 */	float			total_items_weight;
	/* 0x0128 */	float			dispersion_correction_perc;
	/* 0x012c */	float			aiming_speed_correction_perc;
	/* 0x0130 */	float			movement_speed_correction_perc;
	/* 0x0134 */	float			additional_max_weight;
	/* 0x0138 */	float			stamina_regen_correction_perc;
	/* 0x013c */	float			health_regen_correction_perc;
	/* 0x0140 */	float			pain_healt_correction_perc;
	/* 0x0144 */	float			artcontainer_time_corr_perc;
	/* 0x0148 */	float			anomaly_damage_corr_perc;
	/* 0x014c */	float			engineer_use_time_corr_perc;
	/* 0x0150 */	float			engineer_succ_chance_corr_perc;
	/* 0x0154 */	u8				additional_artefact_slots;
	/* 0x0155 */	u8				additional_devices_slots;

			void	apply						( base_player* player );
}; // struct player_parameters_modifyer

STATIC_SIZE_ASSERT(player_parameters_modifyer, 0x158);

typedef resources::resource_ptr< player_parameters_modifyer, resources::unmanaged_intrusive_base >	player_parameters_modifyer_ptr;


} // namespace survarium

#endif // #ifndef PLAYER_PARAMETERS_MODIFYER_H_INCLUDED
