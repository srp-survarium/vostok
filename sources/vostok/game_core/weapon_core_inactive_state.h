////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_INACTIVE_STATE_H_INCLUDED
#define WEAPON_CORE_INACTIVE_STATE_H_INCLUDED

namespace survarium {

/* survarium::weapon_core_inactive_state */

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

class survarium::weapon_core_base_state;

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

class weapon_core_inactive_state : public weapon_core_base_state {
public:
	inline	weapon_core_inactive_state	( weapon_core& arg_0 ) { /* no source */ }

	virtual	bool								is_ready_for_transition		( ) const override;

	virtual	bool								has_animation_ended			( ) const { /* no source */ }

	virtual	void								on_animation_end			( resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const& animation, u32 callback_time_in_ms );

	virtual	void								on_specific_event			( resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const& arg_0, u32 arg_1 ) { /* no source */ }

	virtual	animation::mixing::expression		weapon_and_hands_expression	(
													mutable_buffer&						arg_0,
													bool								arg_1,
													weapon_user_state_enum				arg_2,
													animation::mixing::animation_lexeme&	arg_3
												) const override { /* no source */ }

	virtual	~weapon_core_inactive_state	( ) { /* no source */ }


private:
	/* 0x0000 */	/* weapon_core_base_state */
}; // class weapon_core_inactive_state

STATIC_SIZE_ASSERT(weapon_core_inactive_state, 0x138);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_INACTIVE_STATE_H_INCLUDED
