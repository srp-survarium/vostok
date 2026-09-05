// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PLAYER_LOGIC_CROUCH_STATE_H_INCLUDED
#define PLAYER_LOGIC_CROUCH_STATE_H_INCLUDED

#include <vostok/game_core/player_logic_base_state.h>

namespace survarium {

class player_logic_crouch_state : public player_logic_base_state {
public:
											player_logic_crouch_state	( weapon_user_animations_selector& owner );

	typedef player_logic_base_state super;
	typedef animation::mixing::expression expression;
	typedef animation::mixing::animation_lexeme animation_lexeme;

private:
	virtual	void								initialize					( ) override;
	virtual	void								finalize					( ) override;
	virtual	void								execute						( ) override { }

	virtual	selected_animations_result_type
												selected_animations			(
													mutable_buffer&						buffer,
													weapon_animation_parameters const&	weapon_parameters,
													const bool								is_third_view
												) const override;

			animation_lexeme					movement_lexeme				(
													mutable_buffer&						buffer,
													const u32									animation_index,
													const animation::body_part_masks_enum		bones_mask,
													const bool								is_aimed,
													const bool								is_third_view,
													const bool								is_firing
												) const;

			expression							get_recoil_animation_lexeme	(
													animation_type_enum					animation_index,
													const bool								aimed,
													const float								coeff,
													animation::base_interpolator const&	interpolator,
													mutable_buffer&						buffer,
													const bool								is_third_view,
													const u32									additivity_priority,
													fastdelegate::FastDelegate<float(float,float,u32,u32,u32,float)> const&	time_calculator
												) const;

			expression							look_expression				(
													mutable_buffer&							buffer,
													const u32										movement_animation_index,
													const bool									is_aimed,
													const bool									is_third_view,
													weapon_animation_parameters const&		weapon_parameters,
													animation_lexeme&					weight_driving_animation
												) const;
}; // class player_logic_crouch_state

STATIC_SIZE_ASSERT(player_logic_crouch_state, 0x28);

} // namespace survarium

#endif // #ifndef PLAYER_LOGIC_CROUCH_STATE_H_INCLUDED
