////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_FIRE_STATE_BASE_H_INCLUDED
#define WEAPON_CORE_FIRE_STATE_BASE_H_INCLUDED

#include <vostok/game_core/weapon_core_animation_end_aware_state.h>

namespace vostok { void use_game_core_weapon_core_fire_state_base( ); }

namespace survarium {

class weapon_core_fire_state_base : public weapon_core_animation_end_aware_state {
protected:
			explicit		weapon_core_fire_state_base	( weapon_core& weapon, float animation_timescale );

	// target mangles ctor @@IAE (protected), the four virtuals + on_shot_event
	// @@MAE (protected virtual); objdiff pairs by mangled name.
	virtual	void			initialize					( ) override;
	virtual	void			execute						( ) override;
	virtual	void			finalize					( ) override;

	virtual	void			on_animation_end_impl		( bool& animation_player_tick_result ) override;

	virtual	animation::callback_return_type_enum
							on_shot_event				( animation::animation_callback_params& params );

	// temp_include_all.cpp anchor; needs the protected ctor + virtuals.
	friend void ::vostok::use_game_core_weapon_core_fire_state_base( );

private:
	/* 0x0000 */	/* weapon_core_animation_end_aware_state */
	/* 0x0140 */	float								m_animation_timescale;
	/* 0x0144 */	animation::mixing::playback_enum	m_playback_type;
}; // class weapon_core_fire_state_base

STATIC_SIZE_ASSERT(weapon_core_fire_state_base, 0x148);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_FIRE_STATE_BASE_H_INCLUDED
