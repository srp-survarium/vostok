////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_INACTIVE_STATE_H_INCLUDED
#define WEAPON_CORE_INACTIVE_STATE_H_INCLUDED

#include <vostok/game_core/weapon_core_base_state.h>

namespace survarium {

// sushi@NOTE: This one is actually private

class weapon_core_inactive_state : public weapon_core_base_state {
public:
	inline	explicit							weapon_core_inactive_state	( weapon_core& weapon ) : weapon_core_base_state( weapon, false ) { }

	// STATE[STUB]
	virtual	bool								is_ready_for_transition		( ) const override { return true; }
	// STATE[STUB]: not in this unit's scope; placeholder `return true` so emitting the
	// vtable (now anchored via create_resource) does not trip C4716 -> LNK1257.
	virtual	bool								has_animation_ended			( ) const { return true; }

	// STATE[STUB]
	virtual	void								on_animation_end			( resources::managed_resource_ptr const& animation, u32 callback_time_in_ms ) { VOSTOK_UNREFERENCED_PARAMETERS( animation, callback_time_in_ms ); }
	virtual	void								on_specific_event			( resources::managed_resource_ptr const& animation, u32 callback_time_in_ms ) { /* no source */ }

	// STATE[STUB]
	virtual	animation::mixing::expression		weapon_and_hands_expression	(
													mutable_buffer&							arg_0,
													bool									arg_1,
													weapon_user_state_enum					arg_2,
													animation::mixing::animation_lexeme&	arg_3
												) const override { VOSTOK_UNREFERENCED_PARAMETERS( arg_0, arg_1, arg_2, arg_3 ); VOSTOK_UNREACHABLE_CODE( ); }
}; // class weapon_core_inactive_state

STATIC_SIZE_ASSERT(weapon_core_inactive_state, 0x138);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_INACTIVE_STATE_H_INCLUDED
