////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_HIDE_STATE_BASE_H_INCLUDED
#define WEAPON_CORE_HIDE_STATE_BASE_H_INCLUDED

namespace survarium {

/* survarium::weapon_core_hide_state_base */

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

class survarium::weapon_core_animation_end_aware_state;

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

class weapon_core_hide_state_base : public weapon_core_animation_end_aware_state {
public:
			weapon_core_hide_state_base	( weapon_core& weapon, bool& is_shown );

	virtual	void	initialize					( ) override;

	virtual	void	finalize					( ) override;

	virtual	bool	is_ready_for_transition		( ) const override { /* no source */ }

	virtual	void	on_animation_end_impl		( bool& animation_player_tick_result ) override;

	virtual	~weapon_core_hide_state_base( ) { /* no source */ }


private:
	/* 0x0000 */	/* weapon_core_animation_end_aware_state */
	/* 0x0140 */	bool&		m_is_shown;
}; // class weapon_core_hide_state_base

STATIC_SIZE_ASSERT(weapon_core_hide_state_base, 0x148);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_HIDE_STATE_BASE_H_INCLUDED
