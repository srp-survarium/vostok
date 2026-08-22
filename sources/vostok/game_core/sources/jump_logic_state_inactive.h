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

	virtual	void		initialize					( ) override { }
	virtual	void		finalize					( ) override { /* no source */ }
	virtual	void		execute						( ) override { /* no source */ }
	virtual	bool		is_ready_for_transition		( ) const override { return true; }

private:
	// claude@MATCH: target mangles this override private virtual (?...@@EAE...),
	// so it lives under private: (objdiff pairs by symbol name -> access char).
	virtual	std::pair< animation::mixing::expression, animation::mixing::animation_lexeme >
						selected_animations			(
							mutable_buffer&						buffer,
							const bool								is_third_view,		// sushi@NOTE: unused
							animation_delegate const&			look_calculator,	// sushi@NOTE: unused
							weapon_animation_parameters const&	weapon_parameters	// sushi@NOTE: unused
						) override;

	/* 0x0000 */	/* jump_logic_base_state */
}; // class jump_logic_state_inactive

STATIC_SIZE_ASSERT(jump_logic_state_inactive, 0x28);

} // namespace survarium

#endif // #ifndef JUMP_LOGIC_STATE_INACTIVE_H_INCLUDED
