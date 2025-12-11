////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_LOGIC_BASE_STATE_H_INCLUDED
#define PLAYER_LOGIC_BASE_STATE_H_INCLUDED

#include <vostok/ai/fsm_state.h>

namespace survarium {

/* survarium::player_logic_base_state */

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

struct survarium::base_player;
class survarium::weapon_user_animations_selector;
struct vostok::ai::fsm_state;

enum survarium::weapon_user_state_enum /* stored as <NoType> */ {
}

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

class player_logic_base_state : public ai::fsm_state {
public:
												player_logic_base_state		( weapon_user_animations_selector& owner, weapon_user_state_enum weapon_user_state_id );

	virtual	void								set_user					( base_player& user );

	virtual	std::pair< animation::mixing::expression, animation::mixing::animation_lexeme >
												selected_animations			( mutable_buffer& arg_0, weapon_animation_parameters const& arg_1, bool arg_2 ) const = 0;

	virtual	float4x4							get_attachment_transform	( ) const { /* no source */ }

	virtual	void								serialize					( network_core::udp_match_packet& arg_0 ) const { /* no source */ }
	virtual	void								deserialize					( network_core::packet_reader& arg_0 ) { /* no source */ }

	inline	weapon_user_state_enum				id							( ) const { /* no source */ }

	inline	bool								is_ready_to_be_deactivated	( ) const { /* no source */ }

	virtual	bool								is_ready_for_transition		( ) const override;

	inline	bool								is_weapon_visible			( ) const { /* no source */ }
	inline	bool								is_smoothing_needed			( ) const { /* no source */ }
	inline	bool								is_physics_transform_allowed( ) const { /* no source */ }

	virtual										~player_logic_base_state	( );

	static	u32									movement_animation_index	( player_input const& input );


private:
	/* 0x0000 */	/* ai::fsm_state */
	/* 0x0018 */	weapon_user_animations_selector&	m_owner;
	/* 0x001c */	base_player*						m_user;
	/* 0x0020 */	weapon_user_state_enum				m_weapon_user_state_id;
	/* 0x0024 */	bool								m_is_weapon_weapon_visible;
	/* 0x0025 */	bool								m_is_smoothing_needed;
	/* 0x0026 */	bool								m_is_physics_transform_allowed;
	/* 0x0027 */	bool								m_is_ready_to_be_deactivated;
}; // class player_logic_base_state

STATIC_SIZE_ASSERT(player_logic_base_state, 0x28);

// STATE[STUB]
// void* survarium::weapon_user_dead_state::`scalar deleting destructor'(unsigned int)
void* weapon_user_dead_state::`scalar deleting destructor'( )
{
	return NULL;

	// FUNCTION BODY
	// <0x122e20>|0x000|      :'60'	{
	// ******
}

// STATE[STUB]
// void survarium::inventory_item::action(bool)
void inventory_item::action( bool key_down )
{
	// FUNCTION BODY
	// <0x22c50>|0x000|      :'61'	{
	// ******
}

// STATE[STUB]
// bool survarium::player_logic_base_state::is_ready_for_transition() const
bool player_logic_base_state::is_ready_for_transition( ) const
{
	return false;

	// FUNCTION BODY
	// <0x22700>|0x000|      :'67'	{
	// ******
}



} // namespace survarium

#endif // #ifndef PLAYER_LOGIC_BASE_STATE_H_INCLUDED
