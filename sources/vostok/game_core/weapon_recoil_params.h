////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_RECOIL_PARAMS_H_INCLUDED
#define WEAPON_RECOIL_PARAMS_H_INCLUDED

namespace survarium {

/* survarium::weapon_recoil_params */

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

struct weapon_recoil_params {
public:
	weapon_recoil_params( );
	explicit weapon_recoil_params( configs::binary_config_value const& cfg );

public:
	/* 0x0000 */	float		first_shoot_side_recoil;
	/* 0x0004 */	float		shoot_side_recoil;
	/* 0x0008 */	float		first_shoot_back_recoil;
	/* 0x000c */	float		shoot_back_recoil;
	/* 0x0010 */	float		shoot_recoil_min_angle;
	/* 0x0014 */	float		shoot_recoil_angle_range;
	/* 0x0018 */	float		additive_recoil_time;
	/* 0x001c */	float		additive_side_recoil;
	/* 0x0020 */	float		additive_back_recoil;
	/* 0x0024 */	float		additive_recoil_min_angle;
	/* 0x0028 */	float		additive_recoil_angle_range;
	/* 0x002c */	float		side_compensation_speed;
	/* 0x0030 */	float		back_compensation_speed;
}; // struct weapon_recoil_params

STATIC_SIZE_ASSERT(weapon_recoil_params, 0x34);

} // namespace survarium

#endif // #ifndef WEAPON_RECOIL_PARAMS_H_INCLUDED
