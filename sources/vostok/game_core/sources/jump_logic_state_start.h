////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef JUMP_LOGIC_STATE_START_H_INCLUDED
#define JUMP_LOGIC_STATE_START_H_INCLUDED

#include "./jump_logic_base_state.h"

// claude@MATCH: anchor (temp_include_all.cpp) befriended so it can call the private
// is_ready_for_transition override (qualified); a friend decl emits no bytes.
// claude@TODO: remove later - anchor-only friend decl, not in the shipped header;
// drop once a real caller anchors is_ready_for_transition.
namespace vostok { void use_game_core_jump_logic_state_start( ); }

namespace survarium {

class jump_logic_state_start : public jump_logic_base_state {
	friend void ::vostok::use_game_core_jump_logic_state_start( );

public:
	explicit			jump_logic_state_start	( jump_logic& owner );

	virtual	std::pair< animation::mixing::expression, animation::mixing::animation_lexeme >
						selected_animations		(
							mutable_buffer&						buffer,
							bool								is_third_view,
							animation_delegate const&			look_calculator,
							weapon_animation_parameters const&	weapon_parameters
						) override;

	virtual	void		initialize				( ) override;
	virtual	void		finalize				( ) override;
	// STATE[100%|DONE]: empty body; bytes == target fold @0x1a800 (ICF-unscorable, see .md)
	virtual	void		execute					( ) override { }

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
	// claude@MATCH: target mangles this override private virtual (?...@@EBE_NXZ),
	// so it lives under private: (objdiff matches by symbol name -> access char).
	// STATE[100%|DONE]: return m_jump_interval_ended; reads this+0x2E (rva 0xbd480)
	virtual	bool		is_ready_for_transition	( ) const override { return m_jump_interval_ended; }

	/* 0x0000 */	/* jump_logic_base_state */
	/* 0x0028 */	resources::managed_resource_ptr		m_preface_animation;
	/* 0x002c */	bool								m_physics_jumped;
	/* 0x002d */	bool								m_preface_interval_ended;
	/* 0x002e */	bool								m_jump_interval_ended;
}; // class jump_logic_state_start

STATIC_SIZE_ASSERT(jump_logic_state_start, 0x30);

} // namespace survarium

#endif // #ifndef JUMP_LOGIC_STATE_START_H_INCLUDED
