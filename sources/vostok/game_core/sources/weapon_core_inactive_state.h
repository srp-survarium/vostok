// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WEAPON_CORE_INACTIVE_STATE_H_INCLUDED
#define WEAPON_CORE_INACTIVE_STATE_H_INCLUDED

#include <vostok/game_core/weapon_core_base_state.h>

namespace survarium {

class weapon_core_inactive_state : public weapon_core_base_state {
public:
	inline	explicit							weapon_core_inactive_state	( weapon_core& weapon ) : weapon_core_base_state( weapon, false ) { }

private:
	virtual	bool								is_ready_for_transition		( ) const override { return true; }
	virtual	bool								has_animation_ended			( ) const { return true; }

	// Retail pairs the parameter eater with UNREACHABLE_CODE: the __assume(0) lets the compiler
	// drop the identity(false) test, so the helper call is emitted unconditionally.
	virtual	void								on_animation_end			( resources::managed_resource_ptr const& animation, const u32 callback_time_in_ms )
	{
		VOSTOK_UNREFERENCED_PARAMETERS( animation, callback_time_in_ms );
		VOSTOK_UNREACHABLE_CODE( );
	}
	virtual	void								on_specific_event			( resources::managed_resource_ptr const& animation, const u32 callback_time_in_ms )
	{
		VOSTOK_UNREFERENCED_PARAMETERS( animation, callback_time_in_ms );
		VOSTOK_UNREACHABLE_CODE( );
	}

	// &weight_driving_animation: retail hands the lexeme to the eater as a pointer (the header only
	// forward-declares the class there); by value the eater would copy the whole 0x84-byte object.
	virtual	animation::mixing::expression		weapon_and_hands_expression	(
													mutable_buffer&							buffer,
													const bool							is_third_view,
													const weapon_user_state_enum			user_state_id,
													animation::mixing::animation_lexeme&	weight_driving_animation
												) const override
	{
		VOSTOK_UNREFERENCED_PARAMETERS( buffer, is_third_view, user_state_id, &weight_driving_animation );
		VOSTOK_UNREACHABLE_CODE( );
	}
}; // class weapon_core_inactive_state

STATIC_SIZE_ASSERT(weapon_core_inactive_state, 0x138);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_INACTIVE_STATE_H_INCLUDED
