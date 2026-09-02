// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef SHOTGUN_WEAPON_RELOAD_STATE_H_INCLUDED
#define SHOTGUN_WEAPON_RELOAD_STATE_H_INCLUDED

#include <vostok/game_core/weapon_core_shotgun_reload_state.h>
#include <vostok/game_core/weapon_core_shotgun_reload_base_substate.h>

namespace survarium {

typedef resources::resource_ptr<
	weapon_core_shotgun_reload_base_substate,
	resources::unmanaged_intrusive_base
> weapon_core_shotgun_reload_base_substate_ptr;

class shotgun_weapon_reload_state : public weapon_core_shotgun_reload_state {
	typedef weapon_core_shotgun_reload_state super;

public:
	inline		shotgun_weapon_reload_state	(
					weapon_core&	weapon,
					weapon_core_shotgun_reload_base_substate_ptr const&	start_substate,
					weapon_core_shotgun_reload_base_substate_ptr const&	reload_one_substate,
					weapon_core_shotgun_reload_base_substate_ptr const&	finish_substate
				)
		:	weapon_core_shotgun_reload_state( weapon, start_substate.c_ptr( ), reload_one_substate.c_ptr( ), finish_substate.c_ptr( ) ),
			m_start_substate		( start_substate ),
			m_reload_one_substate	( reload_one_substate ),
			m_finish_substate		( finish_substate )
	{
		set_delete_substates_on_destruction	( false );
	}

private:
	/* 0x0000 */	/* weapon_core_shotgun_reload_state */
	/* 0x0140 */	weapon_core_shotgun_reload_base_substate_ptr	m_start_substate;
	/* 0x0144 */	weapon_core_shotgun_reload_base_substate_ptr	m_reload_one_substate;
	/* 0x0148 */	weapon_core_shotgun_reload_base_substate_ptr	m_finish_substate;
}; // class shotgun_weapon_reload_state

STATIC_SIZE_ASSERT(shotgun_weapon_reload_state, 0x150);

} // namespace survarium

#endif // #ifndef SHOTGUN_WEAPON_RELOAD_STATE_H_INCLUDED
