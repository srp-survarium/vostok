////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_CONFIG_PARAMS_H_INCLUDED
#define WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_CONFIG_PARAMS_H_INCLUDED

namespace survarium {

struct weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< pistol_weapon_core_hide_state > >::config_params {
	inline		config_params	( ) { /* no source */ }

public:
	/* 0x0000 */	bool	stop_sounds_on_state_finalize;
	/* 0x0001 */	u8		simultaneous_sounds_queue_size;
}; // struct weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< pistol_weapon_core_hide_state > >::config_params

STATIC_SIZE_ASSERT(weapon_sound_events_handler_state_cook< weapon_sound_events_handler_state< pistol_weapon_core_hide_state > >::config_params, 0x2);

} // namespace survarium

#endif // #ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_CONFIG_PARAMS_H_INCLUDED
