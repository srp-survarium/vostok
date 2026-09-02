// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WEAPON_CORE_SHOTGUN_RELOAD_STATE_H_INCLUDED
#define WEAPON_CORE_SHOTGUN_RELOAD_STATE_H_INCLUDED

#include <vostok/game_core/weapon_core_base_state.h>

namespace vostok {
namespace ai {
	class fsm;
}
}

namespace survarium {

class weapon_core_shotgun_reload_base_substate;

class weapon_core_shotgun_reload_state : public weapon_core_base_state {
public:
	typedef weapon_core_base_state super;
	typedef weapon_core_shotgun_reload_base_substate substate_type;

			explicit	weapon_core_shotgun_reload_state(
							weapon_core&								weapon,
							weapon_core_shotgun_reload_base_substate*	reload_start,
							weapon_core_shotgun_reload_base_substate*	reload_one_round,
							weapon_core_shotgun_reload_base_substate*	reload_finish
						);
	virtual				~weapon_core_shotgun_reload_state( );

public:
	virtual	void		execute							( ) override;
	virtual	void		initialize						( ) override;
	virtual	void		finalize						( ) override;

	virtual	void		serialize						( network_core::udp_match_packet& packet ) const override;
	virtual	void		deserialize						( network_core::packet_reader& reader ) override;

	virtual	bool		is_ready_for_transition			( ) const override { return true; } /* <0x22700>|0x000:'39' */

private:
	// target mangling: weapon_and_hands_expression @@EBE (private virtual),
	// initialize_logic @@AAE / finish_reload_predicate @@ABE (private) - the public
	// spellings left all three unpaired in objdiff (U/Q letters).
	virtual	animation::mixing::expression
						weapon_and_hands_expression		(
							mutable_buffer&							buffer,
							const bool									is_third_view,
							const weapon_user_state_enum					user_state_id,
							animation::mixing::animation_lexeme&	weight_driving_animation
						) const override;

			void		initialize_logic				(
							weapon_core_shotgun_reload_base_substate* reload_start,
							weapon_core_shotgun_reload_base_substate* reload_one_round,
							weapon_core_shotgun_reload_base_substate* reload_finish
			);
			bool		finish_reload_predicate			( ) const;


protected:
	inline	void		set_delete_substates_on_destruction( const bool delete_substates_on_destruction ) { m_delete_substates_on_destruction = delete_substates_on_destruction; }

private:
	/* 0x0000 */	/* weapon_core_base_state */
	/* 0x0138 */	ai::fsm*	m_logic;
	/* 0x013c */	bool		m_delete_substates_on_destruction;
}; // class weapon_core_shotgun_reload_state

STATIC_SIZE_ASSERT(weapon_core_shotgun_reload_state, 0x140);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_SHOTGUN_RELOAD_STATE_H_INCLUDED
