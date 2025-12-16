////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_AIMED_STATE_H_INCLUDED
#define WEAPON_CORE_AIMED_STATE_H_INCLUDED

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

class weapon_core_aimed_state : public weapon_core_aimed_state_base {
public:
			explicit							weapon_core_aimed_state		( weapon_core& weapon, resources::managed_resource_ptr const* animations, u32 animations_count );

	virtual	animation::mixing::expression		weapon_and_hands_expression	(
													mutable_buffer&							buffer,
													bool									is_third_view,
													weapon_user_state_enum					user_state_id,
													animation::mixing::animation_lexeme&	weight_driving_animation
												) const override;

			weapon_lexeme_pair					get_weapon_lexeme_pair		( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const;

private:
	/* 0x0000 */	/* weapon_core_aimed_state_base */
	/* 0x0138 */	resources::managed_resource_ptr	m_weapon_animations[2][2];
}; // class weapon_core_aimed_state

STATIC_SIZE_ASSERT(weapon_core_aimed_state, 0x148);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_AIMED_STATE_H_INCLUDED
