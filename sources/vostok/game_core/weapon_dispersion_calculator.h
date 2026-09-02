// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WEAPON_DISPERSION_CALCULATOR_H_INCLUDED
#define WEAPON_DISPERSION_CALCULATOR_H_INCLUDED

namespace survarium {

class weapon_dispersion_calculator {
public:
	explicit	weapon_dispersion_calculator	( );

	void		set_reload_dispersion_amount	( const float reload_dispersion_amount );
	void		set_one_shoot_dispersion_amount	( const float one_shoot_dispersion_amount );

	void		tick							( const u32 current_time_in_ms );

	float		get_value						( ) const;
	void		set_aiming_speed				( const float aiming_speed );

	void		fire							( );
	void		reload							( );


private:
	/* 0x0000 */	float		m_one_shoot_dispersion_amount;
	/* 0x0004 */	float		m_reload_dispersion_amount;
	/* 0x0008 */	float		m_growth_speed;
	/* 0x000c */	float		m_aiming_speed;
	/* 0x0010 */	float		m_max_value;
	/* 0x0014 */	float		m_target_coeff;
	/* 0x0018 */	float		m_current_coeff;
	/* 0x001c */	u32			m_current_time;
}; // class weapon_dispersion_calculator

STATIC_SIZE_ASSERT(weapon_dispersion_calculator, 0x20);

} // namespace survarium

#endif // #ifndef WEAPON_DISPERSION_CALCULATOR_H_INCLUDED
