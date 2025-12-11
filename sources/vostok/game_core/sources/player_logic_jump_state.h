////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_LOGIC_JUMP_STATE_H_INCLUDED
#define PLAYER_LOGIC_JUMP_STATE_H_INCLUDED

namespace survarium {

class survarium::jump_logic;
class survarium::player_logic_base_state;

class player_logic_jump_state : public player_logic_base_state {
public:
			player_logic_jump_state	( weapon_user_animations_selector& owner );

	virtual	void								initialize				( ) override;
	virtual	void								finalize				( ) override;
	virtual	void								execute					( ) override;

	virtual	bool								is_ready_for_transition	( ) const override;

	virtual	std::pair< animation::mixing::expression, animation::mixing::animation_lexeme >
												selected_animations		( mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, bool is_third_view ) const override;

	virtual	void								set_user				( base_player& user ) override;

private:
	/* 0x0000 */	/* player_logic_base_state */
	/* 0x0028 */	jump_logic		m_logic;
}; // class player_logic_jump_state

STATIC_SIZE_ASSERT(player_logic_jump_state, 0x40);

} // namespace survarium

#endif // #ifndef PLAYER_LOGIC_JUMP_STATE_H_INCLUDED
