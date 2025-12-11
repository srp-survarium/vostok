////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_LOGIC_SPRINT_STATE_H_INCLUDED
#define PLAYER_LOGIC_SPRINT_STATE_H_INCLUDED

namespace survarium {

class player_logic_sprint_state : public player_logic_base_state {
public:
												player_logic_sprint_state	( weapon_user_animations_selector& owner );

			void								set_callbacks				( boost::function<void()> const& initialize_callback, boost::function<void()> const& finalize_callback );

	virtual	void								initialize					( ) override;
	virtual	void								finalize					( ) override;
	virtual	void								execute						( ) override;

	virtual	std::pair< animation::mixing::expression, animation::mixing::animation_lexeme >
												selected_animations			( mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, bool is_third_view ) const override;

			void								on_stamina_depleted			( );

private:
	/* 0x0000 */	/* player_logic_base_state */
	/* 0x0028 */	boost::function<void()>			m_initialize_callback;
	/* 0x0048 */	boost::function<void()>			m_finalize_callback;
	/* 0x0068 */	player_stamina_subscriber		m_stamina_subscriber;
}; // class player_logic_sprint_state

STATIC_SIZE_ASSERT(player_logic_sprint_state, 0x90);


// STATE[STUB]
// void survarium::player_logic_sprint_state::execute()
void player_logic_sprint_state::execute( )
{
	// FUNCTION BODY
	// <0x2a800>|0x000|      :'36'	{
	// ******
}

} // namespace survarium

#endif // #ifndef PLAYER_LOGIC_SPRINT_STATE_H_INCLUDED
