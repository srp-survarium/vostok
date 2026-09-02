// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DOUBLE_BARRELED_WEAPON_CORE_FIRE_STATE_H_INCLUDED
#define DOUBLE_BARRELED_WEAPON_CORE_FIRE_STATE_H_INCLUDED

#include <vostok/game_core/weapon_core_fire_state_base.h>
#include <vostok/game_core/weapon_user_state_enum.h>
#include <vostok/game_core/weapon_lexeme_pair.h>


namespace survarium {

class double_barreled_weapon_core_fire_state : public weapon_core_fire_state_base {
	typedef weapon_core_fire_state_base super;
	enum { views_count = 2, weapon_states_count = 2, user_states_count = 2, weapon_animations_count = 8, user_animations_count = 4, total_animations_count = 12 };

protected:
	static resources::class_id_enum const resource_class = resources::double_barreled_weapon_fire_state_class;

protected:
	// ctor mangles ??0...@@IAE@... -> protected, non-const
			explicit							double_barreled_weapon_core_fire_state(
													weapon_core&							weapon,
													const float									animation_time_scale,
													resources::managed_resource_ptr const*	animations,
													const u32										animations_count
												);

	// initialize mangles ?...@@MAE... -> protected virtual
	virtual	void								initialize						( ) override;

private:
	// weapon_and_hands_expression mangles ?...@@EBE... -> private virtual const
	virtual	animation::mixing::expression		weapon_and_hands_expression		(
													mutable_buffer&							buffer,
													const bool									is_third_view,
													const weapon_user_state_enum					user_state_id,
													animation::mixing::animation_lexeme&	weight_driving_animation
												) const override;

	// get_weapon_lexeme_pair / get_user_hands_expression mangle ?...@@ABE... -> private const
			weapon_lexeme_pair					get_weapon_lexeme_pair			( mutable_buffer& buffer, const bool is_third_view, const weapon_user_state_enum user_state_id ) const;

			animation::mixing::expression		get_user_hands_expression		(
													animation::mixing::animation_lexeme&	weapon_lexeme,
													mutable_buffer&							buffer,
													const bool									is_third_view,
													const weapon_user_state_enum					user_state_id,
													animation::mixing::animation_lexeme&	weight_driving_animation
												) const;

private:
	/* 0x0000 */	/* weapon_core_fire_state_base */
	/* 0x0148 */	resources::managed_resource_ptr		m_weapon_animations[2][2][2];
	/* 0x0168 */	resources::managed_resource_ptr		m_user_animations[2][2];
	/* 0x0178 */	u32									m_weapon_animation_index;

	template < typename T > friend class weapon_core_state_cook_template;
	template < typename T > friend class weapon_sound_events_handler_state;
}; // class double_barreled_weapon_core_fire_state

STATIC_SIZE_ASSERT(double_barreled_weapon_core_fire_state, 0x180);

} // namespace survarium

#endif // #ifndef DOUBLE_BARRELED_WEAPON_CORE_FIRE_STATE_H_INCLUDED
