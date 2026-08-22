////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_ANIMATION_END_AWARE_STATE_H_INCLUDED
#define WEAPON_CORE_ANIMATION_END_AWARE_STATE_H_INCLUDED

#include <vostok/game_core/weapon_core_base_state.h>
#include <vostok/game_core/weapon_lexeme_pair.h>
#include <vostok/game_core/weapon_user_state_enum.h>

#include <vostok/animation/api.h>
#include <vostok/animation/animation_callback.h>
#include <vostok/animation/mixing.h>


namespace survarium {

class weapon_core_animation_end_aware_state : public weapon_core_base_state {
public:
	inline explicit			weapon_core_animation_end_aware_state( weapon_core& weapon, bool serialize_animation_state ) : weapon_core_base_state( weapon, serialize_animation_state ) { }

protected:
	// target mangles initialize/finalize @@MAE (protected virtual),
	// set_animation_to_wait @@IBE (protected const), on_animation_end @@IAE
	// (protected non-virtual) - objdiff pairs by mangled name.
	virtual	void			initialize						( ) override;
	virtual	void			finalize						( ) override;

			void			set_animation_to_wait			( resources::managed_resource_ptr const& animation ) const;

protected:
			animation::callback_return_type_enum
							on_animation_end				( animation::animation_callback_params& params );
	virtual	void			on_animation_end_impl			( bool& animation_player_tick_result ) = 0;

private:
	/* 0x0000 */	/* weapon_core_base_state */
	// mutable: set_animation_to_wait() is a const method that assigns this
	// (target takes &m_animation_to_wait_for and calls intrusive_ptr::operator=).
	/* 0x0138 */	mutable resources::managed_resource_ptr	m_animation_to_wait_for;
}; // class weapon_core_animation_end_aware_state

STATIC_SIZE_ASSERT(weapon_core_animation_end_aware_state, 0x140);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_ANIMATION_END_AWARE_STATE_H_INCLUDED
