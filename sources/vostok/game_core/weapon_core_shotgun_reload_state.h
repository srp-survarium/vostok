////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_SHOTGUN_RELOAD_STATE_H_INCLUDED
#define WEAPON_CORE_SHOTGUN_RELOAD_STATE_H_INCLUDED

namespace survarium {

/* survarium::weapon_core_shotgun_reload_state */

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

class survarium::weapon_core_base_state;
class vostok::ai::fsm;

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

class weapon_core_shotgun_reload_state : public weapon_core_base_state {
public:
			weapon_core_shotgun_reload_state(
													weapon_core&						weapon,
													weapon_core_shotgun_reload_base_substate*	reload_start,
													weapon_core_shotgun_reload_base_substate*	reload_one_round,
													weapon_core_shotgun_reload_base_substate*	reload_finish
												);

	virtual	~weapon_core_shotgun_reload_state( );

	virtual	void								execute							( ) override;

	virtual	void								initialize						( ) override;

	virtual	void								finalize						( ) override;

	virtual	void								serialize						( network_core::udp_match_packet& packet ) const override;

	virtual	void								deserialize						( network_core::packet_reader& reader ) override;

	virtual	bool								is_ready_for_transition			( ) const override { /* no source */ }

	virtual	animation::mixing::expression		weapon_and_hands_expression		(
													mutable_buffer&						buffer,
													bool								is_third_view,
													weapon_user_state_enum				user_state_id,
													animation::mixing::animation_lexeme&	weight_driving_animation
												) const override;

			void								initialize_logic				( weapon_core_shotgun_reload_base_substate* reload_start, weapon_core_shotgun_reload_base_substate* reload_one_round, weapon_core_shotgun_reload_base_substate* reload_finish );

			bool								finish_reload_predicate			( ) const;

	inline	void								set_delete_substates_on_destruction( bool arg_0 ) { /* no source */ }


private:
	/* 0x0000 */	/* weapon_core_base_state */
	/* 0x0138 */	ai::fsm*	m_logic;
	/* 0x013c */	bool		m_delete_substates_on_destruction;
}; // class weapon_core_shotgun_reload_state

STATIC_SIZE_ASSERT(weapon_core_shotgun_reload_state, 0x140);

// STATE[STUB]
// void* survarium::shotgun_weapon_reload_state_cook::`scalar deleting destructor'(unsigned int)
void* shotgun_weapon_reload_state_cook::`scalar deleting destructor'( )
{
	return NULL;

	// FUNCTION BODY
	// <0x22700>|0x000|      :'39'	{
	// ******
}


} // namespace survarium

#endif // #ifndef WEAPON_CORE_SHOTGUN_RELOAD_STATE_H_INCLUDED
