// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef CHARACTER_DISPERSION_PARAMS_H_INCLUDED
#define CHARACTER_DISPERSION_PARAMS_H_INCLUDED

namespace survarium {

struct character_dispersion_params {
public:
	explicit	character_dispersion_params	( );
	void		load						( configs::binary_config_value const& cfg );

public:
	/* 0x0000 */	float		idle_multiplier;
	/* 0x0004 */	float		idle_aim_multiplier;
	/* 0x0008 */	float		walk_multiplier;
	/* 0x000c */	float		walk_aim_multiplier;
	/* 0x0010 */	float		run_multiplier;
	/* 0x0014 */	float		jump_multiplier;
	/* 0x0018 */	float		crouch_multiplier;
	/* 0x001c */	float		crouch_aim_multiplier;
	/* 0x0020 */	float		crouch_walk_multiplier;
	/* 0x0024 */	float		crouch_walk_aim_multiplier;
	/* 0x0028 */	float		prone_multiplier;
	/* 0x002c */	float		prone_aim_multiplier;
	/* 0x0030 */	float		injury_penalty_for_double_handed;
	/* 0x0034 */	float		injury_penalty_for_one_handed;
}; // struct character_dispersion_params

STATIC_SIZE_ASSERT(character_dispersion_params, 0x38);

} // namespace survarium

#endif // #ifndef CHARACTER_DISPERSION_PARAMS_H_INCLUDED
