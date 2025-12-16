////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_SHOTGUN_RELOAD_START_SUBSTATE_H_INCLUDED
#define WEAPON_CORE_SHOTGUN_RELOAD_START_SUBSTATE_H_INCLUDED

#include <vostok/game_core/weapon_core_shotgun_reload_base_substate.h>

namespace survarium {

class weapon_core_shotgun_reload_start_substate : public weapon_core_shotgun_reload_base_substate {
public:
			explicit	weapon_core_shotgun_reload_start_substate(
							weapon_core&							weapon,
							float									animation_time_scale,
							resources::managed_resource_ptr const*	animations,
							u32										animations_count
						);

	virtual	void		initialize						( ) override;
	virtual	void		finalize						( ) override;
	virtual	bool		is_ready_for_transition			( ) const override;

			animation::callback_return_type_enum
						on_animation_end				( animation::animation_callback_params& params );

private:
	/* 0x0000 */	/* weapon_core_shotgun_reload_base_substate */
	/* 0x0168 */	bool	m_animation_ended;
}; // class weapon_core_shotgun_reload_start_substate

STATIC_SIZE_ASSERT(weapon_core_shotgun_reload_start_substate, 0x170);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_SHOTGUN_RELOAD_START_SUBSTATE_H_INCLUDED
