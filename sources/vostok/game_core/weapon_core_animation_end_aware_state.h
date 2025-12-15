////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_ANIMATION_END_AWARE_STATE_H_INCLUDED
#define WEAPON_CORE_ANIMATION_END_AWARE_STATE_H_INCLUDED

#include <vostok/game_core/weapon_core_base_state.h>
#include <vostok/game_core/weapon_lexeme_pair.h>

#include <vostok/animation/api.h>
#include <vostok/animation/animation_callback.h>

namespace survarium {

class weapon_core_animation_end_aware_state : public weapon_core_base_state {
public:
	inline explicit			weapon_core_animation_end_aware_state( weapon_core& weapon, bool serialize_animation_state ) : weapon_core_base_state( weapon, serialize_animation_state ) { }

	virtual	void			initialize						( ) override;
	virtual	void			finalize						( ) override;

			void			set_animation_to_wait			( resources::managed_resource_ptr const& animation ) const;

public:
			animation::callback_return_type_enum
							on_animation_end				( animation::animation_callback_params& params );
	virtual	void			on_animation_end_impl			( bool& animation_player_tick_result ) = 0;

private:
	/* 0x0000 */	/* weapon_core_base_state */
	/* 0x0138 */	resources::managed_resource_ptr	m_animation_to_wait_for;
}; // class weapon_core_animation_end_aware_state

STATIC_SIZE_ASSERT(weapon_core_animation_end_aware_state, 0x140);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_ANIMATION_END_AWARE_STATE_H_INCLUDED
