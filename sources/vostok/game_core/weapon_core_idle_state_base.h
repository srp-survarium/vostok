// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WEAPON_CORE_IDLE_STATE_BASE_H_INCLUDED
#define WEAPON_CORE_IDLE_STATE_BASE_H_INCLUDED

#include <vostok/game_core/weapon_core_base_state.h>


namespace survarium {

class weapon_core_idle_state_base : public weapon_core_base_state {
	typedef weapon_core_base_state super;

protected:
			explicit	weapon_core_idle_state_base	( weapon_core& weapon );

private:
	virtual	void		initialize					( ) override;
	virtual	void		finalize					( ) override;

}; // class weapon_core_idle_state_base

STATIC_SIZE_ASSERT(weapon_core_idle_state_base, 0x138);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_IDLE_STATE_BASE_H_INCLUDED
