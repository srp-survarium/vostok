// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WEAPON_DISPERSION_PARAMS_H_INCLUDED
#define WEAPON_DISPERSION_PARAMS_H_INCLUDED

namespace survarium {

struct weapon_dispersion_params {
				weapon_dispersion_params( );
	explicit	weapon_dispersion_params( configs::binary_config_value const& cfg );

public:
	/* 0x0000 */	float		base_dispersion;
	/* 0x0004 */	float		from_the_hip_multiplier;
	/* 0x0008 */	float		aim_multiplier;
	/* 0x000c */	float		speed_of_aiming;
	/* 0x0010 */	float		one_shoot_dispersion_amount;
	/* 0x0014 */	float		reload_dispersion_amount;
	/* 0x0018 */	float		growth_speed;
	/* 0x001c */	float		max_dispersion;
}; // struct weapon_dispersion_params

STATIC_SIZE_ASSERT(weapon_dispersion_params, 0x20);

} // namespace survarium

#endif // #ifndef WEAPON_DISPERSION_PARAMS_H_INCLUDED
