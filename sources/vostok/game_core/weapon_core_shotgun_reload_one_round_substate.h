////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_SHOTGUN_RELOAD_ONE_ROUND_SUBSTATE_H_INCLUDED
#define WEAPON_CORE_SHOTGUN_RELOAD_ONE_ROUND_SUBSTATE_H_INCLUDED

namespace survarium {

/* survarium::weapon_core_shotgun_reload_one_round_substate */

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

class survarium::weapon_core_shotgun_reload_base_substate;

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

class weapon_core_shotgun_reload_one_round_substate : public weapon_core_shotgun_reload_base_substate {
public:
			weapon_core_shotgun_reload_one_round_substate(
														weapon_core&						weapon,
														float								animation_time_scale,
														resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const*	animations,
														u32									animations_count
													);

	virtual	void								initialize						( ) override;

	virtual	void								finalize						( ) override;

	virtual	bool								is_ready_for_transition			( ) const override;

			animation::callback_return_type_enum	on_animation_end				( animation::animation_callback_params& params );

	virtual	~weapon_core_shotgun_reload_one_round_substate( ) { /* no source */ }


private:
	/* 0x0000 */	/* weapon_core_shotgun_reload_base_substate */
}; // class weapon_core_shotgun_reload_one_round_substate

STATIC_SIZE_ASSERT(weapon_core_shotgun_reload_one_round_substate, 0x168);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_SHOTGUN_RELOAD_ONE_ROUND_SUBSTATE_H_INCLUDED
