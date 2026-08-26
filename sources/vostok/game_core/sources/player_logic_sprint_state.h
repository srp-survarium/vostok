////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_LOGIC_SPRINT_STATE_H_INCLUDED
#define PLAYER_LOGIC_SPRINT_STATE_H_INCLUDED

#include <vostok/game_core/player_logic_base_state.h>
#include <vostok/game_core/player_stamina_subscriber.h>

namespace survarium {

class player_logic_sprint_state : public player_logic_base_state {
public:
	typedef boost::function< void( ) > initialize_callback_type;
	typedef boost::function< void( ) > finalize_callback_type;

	explicit			player_logic_sprint_state	( weapon_user_animations_selector& owner );

			void		set_callbacks				( initialize_callback_type const& initialize_callback, finalize_callback_type const& finalize_callback );

	typedef player_logic_base_state super;
	typedef animation::mixing::expression expression;

private:
	// claude@MATCH: every virtual override mangles `E` (private); ctor/set_callbacks
	// stay public (`QAE`); on_stamina_depleted is private (`AAE`).
	virtual	void		initialize					( ) override;
	virtual	void		finalize					( ) override;
	// claude@MATCH: empty body; emitted bytes are 7/7 equal to target, but ICF folds
	// the empty stub so objdiff pairs it to a different fold-rep (reads None).
	virtual	void		execute						( ) override { /* no source */ }

	virtual	selected_animations_result_type
						selected_animations			(
							mutable_buffer&						buffer,
							weapon_animation_parameters const&	weapon_parameters,
							const bool								is_third_view
						) const override;

			void		on_stamina_depleted			( );

	/* 0x0000 */	/* player_logic_base_state */
	/* 0x0028 */	initialize_callback_type		m_initialize_callback;
	/* 0x0048 */	finalize_callback_type			m_finalize_callback;
	/* 0x0068 */	player_stamina_subscriber		m_stamina_subscriber;
}; // class player_logic_sprint_state

STATIC_SIZE_ASSERT(player_logic_sprint_state, 0x90);

} // namespace survarium

#endif // #ifndef PLAYER_LOGIC_SPRINT_STATE_H_INCLUDED
