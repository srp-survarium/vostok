////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef CHARACTER_RECOIL_PARAMS_H_INCLUDED
#define CHARACTER_RECOIL_PARAMS_H_INCLUDED

namespace survarium {

struct character_recoil_params {
public:
	// sushi@TODO: Might move it to where defintion is (where?)
			character_recoil_params	( );

	// sushi@TODO: Might move it to where defintion is
	void	load					( configs::binary_config_value const& cfg );

public:
	/* 0x0000 */	float		crouch_multiplier;
	/* 0x0004 */	float		stand_multiplier;
	/* 0x0008 */	float		aimed_crouch_multiplier;
	/* 0x000c */	float		aimed_stand_multiplier;
}; // struct character_recoil_params

STATIC_SIZE_ASSERT(character_recoil_params, 0x10);

} // namespace survarium

#endif // #ifndef CHARACTER_RECOIL_PARAMS_H_INCLUDED
