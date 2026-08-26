////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_RECOIL_CALCULATOR_H_INCLUDED
#define WEAPON_RECOIL_CALCULATOR_H_INCLUDED

#include <vostok/animation/api.h>
#include <vostok/animation/linear_interpolator.h>
#include <vostok/math_randoms_generator.h>

#include <vostok/game_core/pseudo_random.h>

namespace survarium {

class weapon_core;


class weapon_recoil_calculator {
public:
			explicit	weapon_recoil_calculator		( );

			void		set_weapon							( weapon_core* weapon );
	inline	void		set_interpolation_time				( const float arg_0 ) { /* no source */ }
	inline	void		set_character_multiplier			( const float character_multiplier ) { m_player_recoil_multiplier = character_multiplier; }
	inline	void		set_player_compensation_multiplier	( const float player_compensation_multiplier ) { m_player_compensation_multiplier = player_compensation_multiplier; }

			void		tick							( const u32 current_time_in_ms, const float time_scale );
			void		fire							( );
			void		reload							( );
			void		chamber_a_round					( );

	inline	float		get_vertical_koef				( ) const { return m_vertical_koef;		}
	inline	float		get_horizontal_koef				( ) const { return m_horizontal_koef;	}
	inline	float		get_back_koef					( ) const { return m_back_koef;			}

private:
			void		process_compensation			( const float dt_sec );

			float		get_random_angle				( const float range );
			float		get_random_amount				( const float range );

			void		reset							( );


	/* 0x0000 */	math::random32						m_random;
	/* 0x0004 */	pseudo_random						m_pseudo_random;
	/* 0x0008 */	animation::linear_interpolator		m_interpolator;
	/* 0x0010 */	weapon_core*						m_weapon;
	/* 0x0014 */	float								m_player_recoil_multiplier;
	/* 0x0018 */	float								m_player_compensation_multiplier;
	/* 0x001c */	float								m_time_since_shoot;
	/* 0x0020 */	float								m_additive_recoil_timer;
	/* 0x0024 */	float								m_time_since_last_dispersion_change;
	/* 0x0028 */	float								m_vertical_koef;
	/* 0x002c */	float								m_horizontal_koef;
	/* 0x0030 */	float								m_back_koef;
	/* 0x0034 */	float								m_target_vertical_koef;
	/* 0x0038 */	float								m_target_horizontal_koef;
	/* 0x003c */	float								m_target_recoil_koef;
	/* 0x0040 */	u32									m_last_time_in_ms;
}; // class weapon_recoil_calculator

STATIC_SIZE_ASSERT(weapon_recoil_calculator, 0x44);

} // namespace survarium

#endif // #ifndef WEAPON_RECOIL_CALCULATOR_H_INCLUDED
