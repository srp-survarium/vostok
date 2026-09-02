// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUMP_LOGIC_STATE_START_H_INCLUDED
#define JUMP_LOGIC_STATE_START_H_INCLUDED

#include "./jump_logic_base_state.h"

namespace survarium {

class jump_logic_state_start : public jump_logic_base_state {

public:
	typedef jump_logic_base_state super;

	explicit			jump_logic_state_start	( jump_logic& owner );

private:
	// claude@MATCH: target mangles every override below private (EAE/AAE/EBE); only the
	// ctor and execute (which folds to a public empty rep) stay public.
	virtual	std::pair< animation::mixing::expression, animation::mixing::animation_lexeme >
						selected_animations		(
							mutable_buffer&						buffer,
							const bool								is_third_view,
							fastdelegate::FastDelegate< float( float, float, u32, u32, u32, float ) > const& look_calculator,
							weapon_animation_parameters const&	weapon_parameters
						) override;

	virtual	void		initialize				( ) override;
	virtual	void		finalize				( ) override;
	virtual	void		execute					( ) override { }
	virtual	bool		is_ready_for_transition	( ) const override { return m_jump_interval_ended; }

			animation::mixing::animation_lexeme
						get_main_lexeme			( mutable_buffer& buffer, const bool is_third_view, const animation::body_part_masks_enum bones_mask );

			animation::mixing::animation_lexeme
						get_look_lexeme			(
							mutable_buffer&							buffer,
							const bool									is_third_view,
							fastdelegate::FastDelegate< float( float, float, u32, u32, u32, float ) > const& look_calculator,
							animation::mixing::animation_lexeme&	weight_driving_animation
						);

			animation::callback_return_type_enum
						on_interval_end			( animation::animation_callback_params& params );

			animation::callback_return_type_enum
						on_jump_event			( animation::animation_callback_params& params );

	/* 0x0000 */	/* jump_logic_base_state */
	/* 0x0028 */	resources::managed_resource_ptr		m_preface_animation;
	/* 0x002c */	bool								m_physics_jumped;
	/* 0x002d */	bool								m_preface_interval_ended;
	/* 0x002e */	bool								m_jump_interval_ended;
}; // class jump_logic_state_start

STATIC_SIZE_ASSERT(jump_logic_state_start, 0x30);

} // namespace survarium

#endif // #ifndef JUMP_LOGIC_STATE_START_H_INCLUDED
