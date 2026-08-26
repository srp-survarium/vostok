////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_SHOTGUN_RELOAD_ONE_ROUND_SUBSTATE_H_INCLUDED
#define WEAPON_CORE_SHOTGUN_RELOAD_ONE_ROUND_SUBSTATE_H_INCLUDED

#include <vostok/game_core/weapon_core_shotgun_reload_base_substate.h>

namespace survarium {

class weapon_core_shotgun_reload_one_round_substate : public weapon_core_shotgun_reload_base_substate {
public:
	typedef weapon_core_shotgun_reload_base_substate super;
	enum { weapon_animations_count = 4, user_animations_count = 4, total_animations_count = 8 };

			explicit	weapon_core_shotgun_reload_one_round_substate(
							weapon_core&							weapon,
							const float									animation_time_scale,
							resources::managed_resource_ptr const*	animations,
							const u32										animations_count
						);

protected:
	static resources::class_id_enum const resource_class = resources::weapon_shotgun_reload_one_substate_class;

	virtual	void		initialize						( ) override;
	virtual	void		finalize						( ) override;

private:
	virtual	bool		is_ready_for_transition			( ) const override;

			animation::callback_return_type_enum
						on_animation_end				( animation::animation_callback_params& params );

}; // class weapon_core_shotgun_reload_one_round_substate

STATIC_SIZE_ASSERT(weapon_core_shotgun_reload_one_round_substate, 0x168);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_SHOTGUN_RELOAD_ONE_ROUND_SUBSTATE_H_INCLUDED
