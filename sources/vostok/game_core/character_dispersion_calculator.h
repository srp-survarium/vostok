// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CHARACTER_DISPERSION_CALCULATOR_H_INCLUDED
#define CHARACTER_DISPERSION_CALCULATOR_H_INCLUDED

#include <vostok/game_core/weapon_user_state_enum.h>

namespace survarium {

struct character_dispersion_params;


class character_dispersion_calculator : private core::noncopyable {
public:
			explicit	character_dispersion_calculator	( );

			void		set_character_dispersion_params	( character_dispersion_params const* character_params );

			void		tick							(
							const weapon_user_state_enum		character_state,
							const bool						is_moving,
							const bool						is_aiming,
							const u8							broken_hands_count,
							const bool						using_double_handed_weapon,
							const u32							current_time_in_ms
						);

	inline	void		reset							( ) { /* no source */ }

	inline	float		get_value						( ) const { return m_value; }	// claude@MATCH: reads m_value (+0x0c); get_dispersion inlines it as [this+0x2C]
	inline	void		set_aiming_speed				( const float aiming_speed ) { m_aiming_speed = aiming_speed; }	// claude@MATCH: inlined as [this+0x34] store in apply_aim_speed

private:
			float		get_target_koef					( const weapon_user_state_enum character_state, const bool is_moving, const bool is_aiming ) const;
			float		get_broken_hands_penalty		( const u8 broken_hands_count, const bool using_double_handed_weapon ) const;

	/* 0x0000 */	/* core::noncopyable */
	/* 0x0000 */	character_dispersion_params const*	m_params;
	/* 0x0004 */	float								m_target_value;
	/* 0x0008 */	float								m_current_value;
	/* 0x000c */	float								m_value;
	/* 0x0010 */	float								m_value_smoothing_speed;
	/* 0x0014 */	float								m_aiming_speed;
	/* 0x0018 */	u32									m_current_time;
	/* 0x001c */	bool								m_jumped;
}; // class character_dispersion_calculator

STATIC_SIZE_ASSERT(character_dispersion_calculator, 0x20);

} // namespace survarium

#endif // #ifndef CHARACTER_DISPERSION_CALCULATOR_H_INCLUDED
