////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_STATE_CREATION_PARAMS_H_INCLUDED
#define WEAPON_STATE_CREATION_PARAMS_H_INCLUDED

namespace survarium {

class weapon_core;

struct weapon_state_creation_params : public core::noncopyable {
public:
	inline	explicit	weapon_state_creation_params	(
							configs::binary_config_ptr const&	config,
							weapon_core&						weapon,
							const float						rounds_per_second,
							const float						reload_time,
							bool&								shown
						) : config				( config ),
							weapon				( weapon ),
							rounds_per_second	( rounds_per_second ),
							reload_time			( reload_time ),
							shown				( shown ) { }

public:
	/* 0x0000 */	/* core::noncopyable */
	/* 0x0000 */	configs::binary_config_ptr	config;
	/* 0x0004 */	weapon_core&				weapon;
	/* 0x0008 */	float						rounds_per_second;
	/* 0x000c */	float						reload_time;
	/* 0x0010 */	bool&						shown;
}; // struct weapon_state_creation_params

STATIC_SIZE_ASSERT(weapon_state_creation_params, 0x14);

} // namespace survarium

#endif // #ifndef WEAPON_STATE_CREATION_PARAMS_H_INCLUDED
