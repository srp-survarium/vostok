////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef JUMP_LOGIC_STATE_INACTIVE_H_INCLUDED
#define JUMP_LOGIC_STATE_INACTIVE_H_INCLUDED

#include "./jump_logic_base_state.h"

namespace survarium {

class jump_logic_state_inactive : public jump_logic_base_state {
public:
	inline	explicit	jump_logic_state_inactive	( jump_logic& owner ) : jump_logic_base_state( owner ) { }

	virtual	std::pair< animation::mixing::expression, animation::mixing::animation_lexeme >
						selected_animations			(
							mutable_buffer&						buffer,
							bool								is_third_view,		// sushi@NOTE: unused
							animation_delegate const&			look_calculator,	// sushi@NOTE: unused
							weapon_animation_parameters const&	weapon_parameters	// sushi@NOTE: unused
						) override;

	// STATE[100%|DONE]: empty body; bytes == target fold @0x1a800 (ICF-unscorable, see .md)
	virtual	void		initialize					( ) override { }
	virtual	void		finalize					( ) override { /* no source */ }
	virtual	void		execute						( ) override { /* no source */ }
	// STATE[100%|DONE]: return true; bytes == target fold @0xd23e0 (ICF-unscorable, see .md)
	virtual	bool		is_ready_for_transition		( ) const override { return true; }

private:
	/* 0x0000 */	/* jump_logic_base_state */
}; // class jump_logic_state_inactive

STATIC_SIZE_ASSERT(jump_logic_state_inactive, 0x28);

} // namespace survarium

#endif // #ifndef JUMP_LOGIC_STATE_INACTIVE_H_INCLUDED
