////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PISTOL_WEAPON_CORE_RELOAD_STATE_H_INCLUDED
#define PISTOL_WEAPON_CORE_RELOAD_STATE_H_INCLUDED

#include <vostok/game_core/weapon_core_reload_state_base.h>

namespace survarium {

class pistol_weapon_core_reload_state : public weapon_core_reload_state_base {
public:
			explicit							pistol_weapon_core_reload_state	(
													weapon_core&							weapon,
													float									animation_timescale,
													resources::managed_resource_ptr const*	animations,
													u32										animations_count
												);

	virtual	animation::mixing::expression		weapon_and_hands_expression		(
													mutable_buffer&							buffer,
													bool									is_third_view,
													weapon_user_state_enum					user_state_id,
													animation::mixing::animation_lexeme&	weight_driving_animation
												) const override;

			weapon_lexeme_pair					get_weapon_lexeme_pair			( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const;

			animation::mixing::expression		get_user_hands_expression		(
													animation::mixing::animation_lexeme&	weapon_lexeme,
													mutable_buffer&							buffer,
													bool									is_third_view,
													weapon_user_state_enum					user_state_id,
													animation::mixing::animation_lexeme&	weight_driving_animation
												) const;

private:
	/* 0x0000 */	/* weapon_core_reload_state_base */
	/* 0x0148 */	resources::managed_resource_ptr		m_weapon_animations[2][2][2];
	/* 0x0168 */	resources::managed_resource_ptr		m_user_animations[2][2][2];
}; // class pistol_weapon_core_reload_state

STATIC_SIZE_ASSERT(pistol_weapon_core_reload_state, 0x188);

} // namespace survarium

#endif // #ifndef PISTOL_WEAPON_CORE_RELOAD_STATE_H_INCLUDED
