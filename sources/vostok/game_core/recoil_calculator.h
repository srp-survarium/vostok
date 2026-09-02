// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef RECOIL_CALCULATOR_H_INCLUDED
#define RECOIL_CALCULATOR_H_INCLUDED

#include <vostok/game_core/character_recoil_calculator.h>
#include <vostok/game_core/weapon_recoil_calculator.h>
#include <vostok/game_core/weapon_user_state_enum.h>

namespace survarium {

struct character_recoil_params;
class weapon_core;

class recoil_calculator {
public:
			explicit	recoil_calculator			( );

	inline	void		set_character_recoil_params	( character_recoil_params const* params ) { m_character_calculator.set_character_recoil_params( params ); }

			float		get_horizontal_coeff		( ) const;
			float		get_vertical_coeff			( ) const;
			float		get_back_coeff				( ) const;

			void		tick						(
							const weapon_user_state_enum		character_state,
							const bool						is_aiming,
							const u32							current_time_in_ms,
							const float						time_scale
						);

			void		set_weapon					( weapon_core* weapon );

			void		reload						( );
			void		chamber_a_round				( );
			void		fire						( );

private:
	/* 0x0000 */	weapon_recoil_calculator		m_weapon_calculator;
	/* 0x0044 */	character_recoil_calculator		m_character_calculator;
	/* 0x005c */	weapon_core*					m_weapon;
}; // class recoil_calculator

STATIC_SIZE_ASSERT(recoil_calculator, 0x60);

} // namespace survarium

#endif // #ifndef RECOIL_CALCULATOR_H_INCLUDED
