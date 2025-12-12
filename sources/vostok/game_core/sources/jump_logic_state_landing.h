////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef JUMP_LOGIC_STATE_LANDING_H_INCLUDED
#define JUMP_LOGIC_STATE_LANDING_H_INCLUDED

#include "./jump_logic_base_state.h"

namespace survarium {

class jump_logic_state_landing : public jump_logic_base_state {
public:
						jump_logic_state_landing	( jump_logic& owner );

	virtual	std::pair< animation::mixing::expression, animation::mixing::animation_lexeme >
						selected_animations			(
														mutable_buffer&						buffer,
														bool								is_third_view,
														animation_delegate const&			look_calculator,
														weapon_animation_parameters const&	weapon_parameters
													) override;

	virtual	void		initialize					( ) override;
	virtual	void		finalize					( ) override;
	virtual	void		execute						( ) override;
	virtual	bool		is_ready_for_transition		( ) const override;

			animation::mixing::animation_lexeme
						get_main_lexeme				( mutable_buffer& buffer, bool is_third_view, animation::body_part_masks_enum bones_mask );

			animation::mixing::animation_lexeme
						get_look_lexeme				(
														mutable_buffer&							buffer,
														bool									is_third_view,
														weapon_animation_parameters const&		look_calculator,
														animation::mixing::animation_lexeme&	weight_driving_animation
													);

			animation::callback_return_type_enum
						on_interval_end				( animation::animation_callback_params& params );

private:
	/* 0x0000 */	/* jump_logic_base_state */
	/* 0x0028 */	jump_animation_parts	m_landing_type;
}; // class jump_logic_state_landing

STATIC_SIZE_ASSERT(jump_logic_state_landing, 0x2C);



// STATE[STUB]
// void survarium::jump_logic_state_landing::execute()
void jump_logic_state_landing::execute( )
{
	// FUNCTION BODY
	// <0x2a800>|0x000|      :'35'	{
	// ******
}

// STATE[STUB]
// bool survarium::jump_logic_state_landing::is_ready_for_transition() const
bool jump_logic_state_landing::is_ready_for_transition( ) const
{
	return false;

	// FUNCTION BODY
	// <0xe2040>|0x000|      :'36'	{
	// ******
}

} // namespace survarium

#endif // #ifndef JUMP_LOGIC_STATE_LANDING_H_INCLUDED
