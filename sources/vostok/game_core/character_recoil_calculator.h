// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CHARACTER_RECOIL_CALCULATOR_H_INCLUDED
#define CHARACTER_RECOIL_CALCULATOR_H_INCLUDED

#include <vostok/game_core/weapon_user_state_enum.h>

namespace survarium {

struct character_recoil_params;


class character_recoil_calculator {
public:
						character_recoil_calculator	( );

			void		set_character_recoil_params	( character_recoil_params const* params );
			void		tick						(
							const weapon_user_state_enum		character_state,
							const bool						is_aiming,
							const u32							current_time_in_ms,
							const float						time_scale
						);
	inline	float		get_value					( ) const { return m_current_value; }

private:
	/* 0x0000 */	character_recoil_params const*		m_params;
	/* 0x0004 */	float								m_target_value;
	/* 0x0008 */	float								m_current_value;
	/* 0x000c */	float								m_increase_speed;
	/* 0x0010 */	float								m_decrease_speed;
	/* 0x0014 */	u32									m_current_time;
}; // class character_recoil_calculator

STATIC_SIZE_ASSERT(character_recoil_calculator, 0x18);

} // namespace survarium

#endif // #ifndef CHARACTER_RECOIL_CALCULATOR_H_INCLUDED
