////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_RELOAD_STATE_H_INCLUDED
#define WEAPON_CORE_RELOAD_STATE_H_INCLUDED

#include <vostok/game_core/weapon_core_reload_state_base.h>


namespace survarium {

class weapon_core;

class weapon_core_reload_state : public weapon_core_reload_state_base {
	typedef weapon_core_reload_state_base super;
	enum { views_count = 2, user_states_count = 2, weapon_animations_count = 4, user_animations_count = 4, total_animations_count = 8 };

protected:
	static resources::class_id_enum const resource_class = resources::weapon_reload_state_class;

protected:
	// ctor mangles ??0...@@IAE@... -> protected, non-const (objdiff pairs by mangled name)
			explicit							weapon_core_reload_state	(
													weapon_core&							weapon,
													const float									animation_time_scale,
													resources::managed_resource_ptr const*	animations,
													const u32										animations_count
												);

private:
	// mangles ?...@@EBE... -> private virtual, const
	virtual	animation::mixing::expression		weapon_and_hands_expression	(
													mutable_buffer&							buffer,
													const bool									is_third_view,
													const weapon_user_state_enum					user_state_id,
													animation::mixing::animation_lexeme&	weight_driving_animation
												) const override;

	// mangles ?...@@ABE... -> private, const
			weapon_lexeme_pair					get_weapon_lexeme_pair		( mutable_buffer& buffer, const bool is_third_view, const weapon_user_state_enum user_state_id ) const;

	// mangles ?...@@ABE... -> private, const
			animation::mixing::expression		get_user_hands_expression	(
													animation::mixing::animation_lexeme&	weapon_lexeme,
													mutable_buffer&							buffer,
													const bool									is_third_view,
													const weapon_user_state_enum					user_state_id,
													animation::mixing::animation_lexeme&	weight_driving_animation
												) const;

private:
	/* 0x0000 */	/* weapon_core_reload_state_base */
	/* 0x0148 */	resources::managed_resource_ptr	m_weapon_animations[2][2];
	/* 0x0158 */	resources::managed_resource_ptr	m_user_animations[2][2];

	template < typename T > friend class weapon_core_state_cook_template;
	template < typename T > friend class weapon_sound_events_handler_state;
}; // class weapon_core_reload_state

STATIC_SIZE_ASSERT(weapon_core_reload_state, 0x168);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_RELOAD_STATE_H_INCLUDED
