// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
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

	// claude@NOTE: NOT a recoverable ASSERT. The leading mov byte[ebp-1],0; lea; call empty_stub
	// is the macro's own if(::vostok::identity(false)) lvalue-materialization (one call, both
	// sides) - there is no second empty_stub, so no compiled-out ASSERT to recover. The residual
	// is the dead-guard fold: target's COMDAT emits the eater UNCONDITIONAL (no movzx/test/je),
	// our /Od MASTER_GOLD build keeps the full if(identity(false)){...}else(void)0 branch. Verified
	// systemic: EVERY weapon_*_state::weapon_and_hands_expression override carries this same guard
	// and only fully pairs where its target COMDAT happened to be the /Od build (optimized
	// COMDAT in /Od unit - see optimized-comdat-in-od-unit.md). weapon_and_hands_expression below
	// carries a SECOND /Od artifact: the animation_lexeme& arg promoted through the variadic eater
	// is copied by value (sub esp,84h; rep movsd) in /Od vs passed as a pointer in the optimized
	// target. Both are non-steerable build-mode residuals; macro/structure are faithful.
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
