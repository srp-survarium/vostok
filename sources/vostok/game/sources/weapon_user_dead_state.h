// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef WEAPON_USER_DEAD_STATE_H_INCLUDED
#define WEAPON_USER_DEAD_STATE_H_INCLUDED

#include <vostok/game_core/player_logic_base_state.h>
#include <vostok/math_randoms_generator.h>

#include "weapon.h"

namespace survarium {

class weapon_user_dead_state : public player_logic_base_state {
	typedef	player_logic_base_state	super;

public:
	// buildability init: owner = the weapon's animations selector; the state id is a
	// placeholder (weapon_user_state_enum has no dead value) - a matcher confirms both.
	inline	explicit	weapon_user_dead_state	( weapon& arg_0 )
		:	player_logic_base_state	( arg_0.user_animations_selector( ), type_stand ),
			m_weapon				( arg_0 )
	{ /* no source */ }

private:
	// the target mangles this private (EBE)
	virtual	std::pair< animation::mixing::expression, animation::mixing::animation_lexeme >	selected_animations		(
							mutable_buffer&							buffer,
							weapon_animation_parameters const&		weapon_parameters,
							const bool								is_third_view
						) const override;

	virtual	void		initialize				( ) override { /* no source */ }

	virtual	void		execute					( ) override { /* no source */ }

	virtual	void		finalize				( ) override { /* no source */ }

	virtual	bool		is_ready_for_transition	( ) const override;

private:
	/* 0x0000 */	/* player_logic_base_state */
	/* 0x0028 */	weapon&				m_weapon;
	/* 0x002c */	mutable math::random32	m_random;	// selected_animations (const) advances the seed

}; // class weapon_user_dead_state

STATIC_SIZE_ASSERT(weapon_user_dead_state, 0x30);

} // namespace survarium

#endif // #ifndef WEAPON_USER_DEAD_STATE_H_INCLUDED
