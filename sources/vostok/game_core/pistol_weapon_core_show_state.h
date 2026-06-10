////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PISTOL_WEAPON_CORE_SHOW_STATE_H_INCLUDED
#define PISTOL_WEAPON_CORE_SHOW_STATE_H_INCLUDED

#include <vostok/game_core/weapon_core_show_state_base.h>
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

namespace vostok { void use_game_core_pistol_weapon_core_show_state( ); }

namespace survarium {

class weapon_core;

class pistol_weapon_core_show_state : public weapon_core_show_state_base {
protected:
	// ctor mangles ??0...@@IAE@... -> protected, non-const (objdiff pairs by mangled name)
			explicit							pistol_weapon_core_show_state	(
													weapon_core&							weapon,
													float									animation_timescale,
													resources::managed_resource_ptr const*	animations,
													u32										animations_count,
													bool&									is_shown
												);

private:
	// weapon_and_hands_expression mangles ?...@@EBE... -> private virtual, const
	virtual	animation::mixing::expression		weapon_and_hands_expression		(
													mutable_buffer&							buffer,
													bool									is_third_view,
													weapon_user_state_enum					user_state_id,
													animation::mixing::animation_lexeme&	weight_driving_animation
												) const override;

	// get_weapon_lexeme_pair / get_user_hands_expression mangle ?...@@ABE... -> private, const
			weapon_lexeme_pair					get_weapon_lexeme_pair			( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const;

			animation::mixing::expression		get_user_hands_expression		(
													animation::mixing::animation_lexeme&	weapon_lexeme,
													mutable_buffer&							buffer,
													bool									is_third_view,
													weapon_user_state_enum					user_state_id,
													animation::mixing::animation_lexeme&	weight_driving_animation
												) const;

public:
	virtual	~pistol_weapon_core_show_state	( ) { /* no source */ }


private:
	/* 0x0000 */	/* weapon_core_show_state_base */
	/* 0x0148 */	resources::managed_resource_ptr		m_weapon_animations[2][2][2];
	/* 0x0168 */	resources::managed_resource_ptr		m_user_animations[2][2];
	/* 0x0178 */	float								m_time_scale;

	// temp_include_all.cpp anchor; reaches the protected ctor / private virtuals.
	template < typename T > friend class weapon_core_state_cook_template;
	friend void ::vostok::use_game_core_pistol_weapon_core_show_state( );
}; // class pistol_weapon_core_show_state

STATIC_SIZE_ASSERT(pistol_weapon_core_show_state, 0x180);

} // namespace survarium

#endif // #ifndef PISTOL_WEAPON_CORE_SHOW_STATE_H_INCLUDED
