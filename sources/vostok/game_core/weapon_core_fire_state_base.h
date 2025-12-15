////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_FIRE_STATE_BASE_H_INCLUDED
#define WEAPON_CORE_FIRE_STATE_BASE_H_INCLUDED

namespace survarium {

/* survarium::weapon_core_fire_state_base */

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

class survarium::weapon_core_animation_end_aware_state;

enum vostok::animation::mixing::playback_enum /* stored as s32 */ {
	play_cyclically = 0x0000,
	play_once_and_freeze_at_end = 0x0001,
	play_once_and_remove_at_end = 0x0002,
}

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

class weapon_core_fire_state_base : public weapon_core_animation_end_aware_state {
public:
			weapon_core_fire_state_base	( weapon_core& weapon, float animation_timescale );

	virtual	void								initialize					( ) override;

	virtual	void								execute						( ) override;

	virtual	void								finalize					( ) override;

	virtual	void								on_animation_end_impl		( bool& animation_player_tick_result ) override;

	virtual	animation::callback_return_type_enum	on_shot_event				( animation::animation_callback_params& params );

	virtual	~weapon_core_fire_state_base( ) { /* no source */ }


private:
	/* 0x0000 */	/* weapon_core_animation_end_aware_state */
	/* 0x0140 */	float								m_animation_timescale;
	/* 0x0144 */	animation::mixing::playback_enum	m_playback_type;
}; // class weapon_core_fire_state_base

STATIC_SIZE_ASSERT(weapon_core_fire_state_base, 0x148);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_FIRE_STATE_BASE_H_INCLUDED
