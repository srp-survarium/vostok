////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SHOTGUN_WEAPON_RELOAD_STATE_H_INCLUDED
#define SHOTGUN_WEAPON_RELOAD_STATE_H_INCLUDED

/* INCLUDES */
class survarium::weapon_core_shotgun_reload_state;
typedef vostok::resources::resource_ptr<survarium::weapon_core_shotgun_reload_base_substate,vostok::resources::unmanaged_intrusive_base>
	survarium::weapon_core_shotgun_reload_base_substate_ptr;
class survarium::weapon_core_shotgun_reload_base_substate;

/* FORWARD REFS */
class survarium::weapon_core;

namespace survarium {

class shotgun_weapon_reload_state : public weapon_core_shotgun_reload_state {
public:
	inline		shotgun_weapon_reload_state	(
					weapon_core&	weapon,
					weapon_core_shotgun_reload_base_substate_ptr const&	start_substate,
					weapon_core_shotgun_reload_base_substate_ptr const&	reload_one_substate,
					weapon_core_shotgun_reload_base_substate_ptr const&	finish_substate
				) { /* no source */ }
	virtual		~shotgun_weapon_reload_state( ) { /* no source */ }

private:
	/* 0x0000 */	/* weapon_core_shotgun_reload_state */
	/* 0x0140 */	weapon_core_shotgun_reload_base_substate_ptr	m_start_substate;
	/* 0x0144 */	weapon_core_shotgun_reload_base_substate_ptr	m_reload_one_substate;
	/* 0x0148 */	weapon_core_shotgun_reload_base_substate_ptr	m_finish_substate;
}; // class shotgun_weapon_reload_state

STATIC_SIZE_ASSERT(shotgun_weapon_reload_state, 0x150);

} // namespace survarium

#endif // #ifndef SHOTGUN_WEAPON_RELOAD_STATE_H_INCLUDED
