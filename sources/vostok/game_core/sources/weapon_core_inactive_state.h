// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WEAPON_CORE_INACTIVE_STATE_H_INCLUDED
#define WEAPON_CORE_INACTIVE_STATE_H_INCLUDED

#include <vostok/game_core/weapon_core_base_state.h>

namespace survarium {

class weapon_core_inactive_state : public weapon_core_base_state {
public:
	inline	explicit							weapon_core_inactive_state	( weapon_core& weapon ) : weapon_core_base_state( weapon, false ) { m_is_ready_to_be_deactivated = true; }

private:
	virtual	bool								is_ready_for_transition		( ) const override { return true; }
	virtual	bool								has_animation_ended			( ) const { return true; }












	virtual	void								on_animation_end			( resources::managed_resource_ptr const& animation, const u32 callback_time_in_ms ) { VOSTOK_UNREFERENCED_PARAMETERS( animation, callback_time_in_ms ); }
	virtual	void								on_specific_event			( resources::managed_resource_ptr const& animation, const u32 callback_time_in_ms ) { VOSTOK_UNREFERENCED_PARAMETERS( animation, callback_time_in_ms ); }

	virtual	animation::mixing::expression		weapon_and_hands_expression	(
													mutable_buffer&							arg_0,
													const bool							arg_1,
													const weapon_user_state_enum			arg_2,
													animation::mixing::animation_lexeme&	arg_3
												) const override { VOSTOK_UNREFERENCED_PARAMETERS( arg_0, arg_1, arg_2, arg_3 ); VOSTOK_UNREACHABLE_CODE( ); }
}; // class weapon_core_inactive_state

STATIC_SIZE_ASSERT(weapon_core_inactive_state, 0x138);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_INACTIVE_STATE_H_INCLUDED
