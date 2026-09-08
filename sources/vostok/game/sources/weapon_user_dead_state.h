// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WEAPON_USER_DEAD_STATE_H_INCLUDED
#define WEAPON_USER_DEAD_STATE_H_INCLUDED

#include <vostok/game_core/player_logic_base_state.h>
#include <vostok/math_randoms_generator.h>

#include "weapon.h"

namespace survarium {

class weapon_user_dead_state : public player_logic_base_state {
	typedef	player_logic_base_state	super;

public:
	inline	explicit	weapon_user_dead_state	( weapon& arg_0 )
		:	player_logic_base_state	( arg_0.user_animations_selector( ), type_stand ),
			m_weapon				( arg_0 )
	{ }

private:
	virtual	std::pair< animation::mixing::expression, animation::mixing::animation_lexeme >	selected_animations		(
							mutable_buffer&							buffer,
							weapon_animation_parameters const&		weapon_parameters,
							const bool								is_third_view
						) const override;

	virtual	void		initialize				( ) override { }

	virtual	void		execute					( ) override { }

	virtual	void		finalize				( ) override { }

	virtual	bool		is_ready_for_transition	( ) const override;

private:
	/* 0x0000 */	/* player_logic_base_state */
	/* 0x0028 */	weapon&				m_weapon;
	// sushi@TODO: Const selection advances the seed; verify mutable versus an original const_cast.
	/* 0x002c */	mutable math::random32	m_random;

}; // class weapon_user_dead_state

STATIC_SIZE_ASSERT(weapon_user_dead_state, 0x30);

} // namespace survarium

#endif // #ifndef WEAPON_USER_DEAD_STATE_H_INCLUDED
