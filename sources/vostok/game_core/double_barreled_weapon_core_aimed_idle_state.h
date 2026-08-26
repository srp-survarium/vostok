////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef DOUBLE_BARRELED_WEAPON_CORE_AIMED_IDLE_STATE_H_INCLUDED
#define DOUBLE_BARRELED_WEAPON_CORE_AIMED_IDLE_STATE_H_INCLUDED

#include <vostok/game_core/weapon_core_aimed_state_base.h>
#include <vostok/game_core/weapon_user_state_enum.h>
#include <vostok/game_core/weapon_lexeme_pair.h>

#include <vostok/animation/mixing_expression.h>

namespace vostok {
namespace animation {
namespace mixing {
	class animation_lexeme;
}
}
}


namespace survarium {

class weapon_core;

class double_barreled_weapon_core_aimed_idle_state : public weapon_core_aimed_state_base {
	typedef weapon_core_aimed_state_base super;
	enum { views_count = 2, weapon_states_count = 3, user_states_count = 2, weapon_animations_count = 12, user_animations_count = 0, total_animations_count = 12 };
	static resources::class_id_enum const resource_class = resources::double_barreled_weapon_aimed_state_class;

private:
	// ctor mangles ??0...@@AAE@... -> private, non-const (objdiff pairs by mangled name)
				explicit							double_barreled_weapon_core_aimed_idle_state	( weapon_core& weapon, resources::managed_resource_ptr const* animations, const u32 animations_count );

	// mangles ?...@@EBE... -> private virtual, const
	virtual	animation::mixing::expression		weapon_and_hands_expression		(
													mutable_buffer&							buffer,
													const bool									is_third_view,
													const weapon_user_state_enum					user_state_id,
													animation::mixing::animation_lexeme&	weight_driving_animation
												) const override;

	// mangles ?...@@ABE... -> private, const
				weapon_lexeme_pair					get_weapon_lexeme_pair			( mutable_buffer& buffer, const bool is_third_view, const weapon_user_state_enum user_state_id ) const;

private:
	/* 0x0000 */	/* weapon_core_aimed_state_base */
	/* 0x0138 */	resources::managed_resource_ptr		m_weapon_animations[2][2][3];

	template < typename T > friend class weapon_core_state_cook_template;
}; // class double_barreled_weapon_core_aimed_idle_state

STATIC_SIZE_ASSERT(double_barreled_weapon_core_aimed_idle_state, 0x168);

} // namespace survarium

#endif // #ifndef DOUBLE_BARRELED_WEAPON_CORE_AIMED_IDLE_STATE_H_INCLUDED
