////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef JUMP_LOGIC_STATE_INACTIVE_H_INCLUDED
#define JUMP_LOGIC_STATE_INACTIVE_H_INCLUDED

#include "./jump_logic_base_state.h"

namespace survarium {

class jump_logic_state_inactive : public jump_logic_base_state {
public:
	inline				jump_logic_state_inactive	( jump_logic& arg_0 ) { /* no source */ }

	virtual	std::pair< animation::mixing::expression, animation::mixing::animation_lexeme >
						selected_animations			(
														mutable_buffer&						buffer,
														bool								is_third_view,		// sushi@NOTE: unused
														animation_delegate const&			look_calculator,	// sushi@NOTE: unused
														weapon_animation_parameters const&	weapon_parameters	// sushi@NOTE: unused
													) override;

	virtual	void		initialize					( ) override;
	virtual	void		finalize					( ) override { /* no source */ }
	virtual	void		execute						( ) override { /* no source */ }
	virtual	bool		is_ready_for_transition		( ) const override;

private:
	/* 0x0000 */	/* jump_logic_base_state */
}; // class jump_logic_state_inactive

STATIC_SIZE_ASSERT(jump_logic_state_inactive, 0x28);


// STATE[STUB]
// void survarium::jump_logic_state_inactive::initialize()
void jump_logic_state_inactive::initialize( )
{
	// FUNCTION BODY
	// <0x2a800>|0x000|      :'24'	{
	// ******
}

// STATE[STUB]
// bool survarium::jump_logic_state_inactive::is_ready_for_transition() const
bool jump_logic_state_inactive::is_ready_for_transition( ) const
{
	return false;

	// FUNCTION BODY
	// <0xe23e0>|0x000|      :'27'	{
	// ******
}

} // namespace survarium

#endif // #ifndef JUMP_LOGIC_STATE_INACTIVE_H_INCLUDED
