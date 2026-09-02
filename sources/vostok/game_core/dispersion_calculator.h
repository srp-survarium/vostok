// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef DISPERSION_CALCULATOR_H_INCLUDED
#define DISPERSION_CALCULATOR_H_INCLUDED

#include <vostok/game_core/character_dispersion_calculator.h>
#include <vostok/game_core/weapon_dispersion_calculator.h>
#include <vostok/game_core/weapon_user_state_enum.h>

namespace survarium {

class weapon_core;


class dispersion_calculator {
public:
	explicit			dispersion_calculator			( );

	inline	void		set_character_dispersion_params	( character_dispersion_params const* arg_0 ) { m_character_calculator.set_character_dispersion_params( arg_0 ); }

			void		tick							(
							const weapon_user_state_enum		character_state,
							const bool						is_moving,
							const bool						is_aiming,
							const u8							broken_hands_count,
							const bool						using_double_handed_weapon,
							const u32							current_time_in_ms
						);

			float		get_dispersion					( ) const;

			void		set_weapon						( weapon_core* weapon );
			void		set_shooting_skill_coeff		( const float shooting_skill_coef );
			void		set_aiming_speed_coeff			( const float aiming_speed_coeff );

			void		reload							( );
			void		fire							( );

private:
			void		apply_aim_speed					( );	// ?apply_aim_speed@..@@AAEXXZ (private)

	/* 0x0000 */	weapon_dispersion_calculator		m_weapon_calculator;
	/* 0x0020 */	character_dispersion_calculator		m_character_calculator;
	/* 0x0040 */	weapon_core*						m_weapon;
	/* 0x0044 */	float								m_shooting_skill_coeff;
	/* 0x0048 */	float								m_aiming_speed_coeff;
}; // class dispersion_calculator

STATIC_SIZE_ASSERT(dispersion_calculator, 0x4C);

} // namespace survarium

#endif // #ifndef DISPERSION_CALCULATOR_H_INCLUDED
