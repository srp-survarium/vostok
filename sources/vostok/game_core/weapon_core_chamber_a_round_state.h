////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_CHAMBER_A_ROUND_STATE_H_INCLUDED
#define WEAPON_CORE_CHAMBER_A_ROUND_STATE_H_INCLUDED

#include <vostok/game_core/weapon_core_chamber_a_round_state_base.h>

namespace survarium {

class weapon_core_chamber_a_round_state : public weapon_core_chamber_a_round_state_base {
public:
			explicit							weapon_core_chamber_a_round_state(
													weapon_core&							weapon,
													float									animation_time_scale,
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
	/* 0x0000 */	/* weapon_core_chamber_a_round_state_base */
	/* 0x0148 */	resources::managed_resource_ptr		m_weapon_animations[2][2];
	/* 0x0158 */	resources::managed_resource_ptr		m_user_animations[2][2];
}; // class weapon_core_chamber_a_round_state

STATIC_SIZE_ASSERT(weapon_core_chamber_a_round_state, 0x168);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_CHAMBER_A_ROUND_STATE_H_INCLUDED
