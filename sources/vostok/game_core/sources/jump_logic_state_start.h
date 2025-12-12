////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef JUMP_LOGIC_STATE_START_H_INCLUDED
#define JUMP_LOGIC_STATE_START_H_INCLUDED

#include "./jump_logic_base_state.h"

namespace survarium {

class jump_logic_state_start : public jump_logic_base_state {
public:
						jump_logic_state_start	( jump_logic& owner );

	virtual	std::pair< animation::mixing::expression, animation::mixing::animation_lexeme >
						selected_animations		(
													mutable_buffer&						buffer,
													bool								is_third_view,
													animation_delegate const&			look_calculator,
													weapon_animation_parameters const&	weapon_parameters
												) override;

	virtual	void		initialize				( ) override;
	virtual	void		finalize				( ) override;
	virtual	void		execute					( ) override;
	virtual	bool		is_ready_for_transition	( ) const override;

			animation::mixing::animation_lexeme
						get_main_lexeme			( mutable_buffer& buffer, bool is_third_view, animation::body_part_masks_enum bones_mask );

			animation::mixing::animation_lexeme
						get_look_lexeme			(
													mutable_buffer&							buffer,
													bool									is_third_view,
													animation_delegate const&				look_calculator,
													animation::mixing::animation_lexeme&	weight_driving_animation
												);

			animation::callback_return_type_enum
						on_interval_end			( animation::animation_callback_params& params );

			animation::callback_return_type_enum
						on_jump_event			( animation::animation_callback_params& params );

private:
	/* 0x0000 */	/* jump_logic_base_state */
	/* 0x0028 */	resources::managed_resource_ptr		m_preface_animation;
	/* 0x002c */	bool								m_physics_jumped;
	/* 0x002d */	bool								m_preface_interval_ended;
	/* 0x002e */	bool								m_jump_interval_ended;
}; // class jump_logic_state_start

STATIC_SIZE_ASSERT(jump_logic_state_start, 0x30);


// STATE[STUB]
// void survarium::jump_logic_state_start::execute()
void jump_logic_state_start::execute( )
{
	// FUNCTION BODY
	// <0x2a800>|0x000|      :'34'	{
	// ******
}

// STATE[STUB]
// bool survarium::jump_logic_state_start::is_ready_for_transition() const
bool jump_logic_state_start::is_ready_for_transition( ) const
{
	return false;

	// FUNCTION BODY
	// <0xcd480>|0x000|      :'35'	{
	// ******
}

} // namespace survarium

#endif // #ifndef JUMP_LOGIC_STATE_START_H_INCLUDED
