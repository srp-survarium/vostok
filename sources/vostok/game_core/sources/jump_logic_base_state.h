////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef JUMP_LOGIC_BASE_STATE_H_INCLUDED
#define JUMP_LOGIC_BASE_STATE_H_INCLUDED

#include <vostok/game_core/jump_logic.h>

#include <vostok/animation/api.h>
#include <vostok/animation/animation_player.h>
#include <vostok/ai/fsm_state.h>

namespace vostok {
namespace animation {
	struct animation_callback_params;
}
}

namespace survarium {

struct weapon_animation_parameters;

class jump_logic_base_state : public ai::fsm_state {
public:
	virtual				~jump_logic_base_state	( ) { /* no source */ }

	virtual	void		set_user				( base_player& user ) { m_user = &user; }


	typedef fastdelegate::FastDelegate< float( float, float, u32, u32, u32, float ) > animation_delegate;
	virtual	std::pair< animation::mixing::expression, animation::mixing::animation_lexeme >
						selected_animations		(
							mutable_buffer&						buffer,
							bool								is_third_view,
							animation_delegate const&			look_calculator,
							weapon_animation_parameters const&	weapon_parameters
						) = 0;

	inline	bool		is_jump_finished		( ) const { return m_is_jump_finished; }

protected:
	// claude@MATCH: target mangles the ctor ?...@@IAE (protected).
	explicit			jump_logic_base_state	( jump_logic& owner )
							:	m_jump_logic( owner ),
								m_user( 0 ),
								m_interval_id_to_wait_for( 0xFF ),
								m_is_jump_finished( false )
						{}

protected:
	// claude@MATCH: derived state overrides read m_jump_logic / m_animation /
	// m_interval_id_to_wait_for directly (no accessor in the target asm), so the data
	// members are protected. Access specifier changes no codegen, only visibility.
	/* 0x0000 */	/* ai::fsm_state */
	/* 0x0018 */	jump_logic&							m_jump_logic;
	/* 0x001c */	base_player*						m_user;
	/* 0x0020 */	resources::managed_resource_ptr		m_animation;
	/* 0x0024 */	u8									m_interval_id_to_wait_for;
	/* 0x0025 */	bool								m_is_jump_finished;
}; // class jump_logic_base_state

STATIC_SIZE_ASSERT(jump_logic_base_state, 0x28);

} // namespace survarium

#endif // #ifndef JUMP_LOGIC_BASE_STATE_H_INCLUDED
