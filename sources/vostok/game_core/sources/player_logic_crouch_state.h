////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_LOGIC_CROUCH_STATE_H_INCLUDED
#define PLAYER_LOGIC_CROUCH_STATE_H_INCLUDED

#include <vostok/game_core/player_logic_base_state.h>

namespace survarium {

class player_logic_crouch_state : public player_logic_base_state {
public:
												player_logic_crouch_state	( weapon_user_animations_selector& owner );

	virtual	void								initialize					( ) override;
	virtual	void								finalize					( ) override;
	virtual	void								execute						( ) override { /* no source */ }

	virtual	std::pair< animation::mixing::expression, animation::mixing::animation_lexeme >
												selected_animations			( mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, bool is_third_view ) const override;

			animation::mixing::animation_lexeme	movement_lexeme				(
																				mutable_buffer&						buffer,
																				u32									animation_index,
																				animation::body_part_masks_enum		bones_mask,
																				bool								is_aimed,
																				bool								is_third_view,
																				bool								is_firing
																			) const;

			animation::mixing::expression		get_recoil_animation_lexeme	(
																				animation_type_enum					animation_index,
																				bool								aimed,
																				float								coeff,
																				animation::base_interpolator const&	interpolator,
																				mutable_buffer&						buffer,
																				bool								is_third_view,
																				u32									additivity_priority,
																				fastdelegate::FastDelegate<float(float,float,u32,u32,u32,float)> const&	time_calculator
																			) const;

			animation::mixing::expression		look_expression				(
																				mutable_buffer&							buffer,
																				u32										movement_animation_index,
																				bool									is_aimed,
																				bool									is_third_view,
																				weapon_animation_parameters const&		weapon_parameters,
																				animation::mixing::animation_lexeme&	weight_driving_animation
																			) const;
}; // class player_logic_crouch_state

STATIC_SIZE_ASSERT(player_logic_crouch_state, 0x28);


// STATE[STUB]
// void* survarium::player_logic_crouch_state::`scalar deleting destructor'(unsigned int)
void* player_logic_crouch_state::`scalar deleting destructor'( )
{
	return NULL;

	// FUNCTION BODY
	// <0x2a800>|0x000|      :'34'	{
	// ******
}

} // namespace survarium

#endif // #ifndef PLAYER_LOGIC_CROUCH_STATE_H_INCLUDED
