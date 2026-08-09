////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_SHOTGUN_RELOAD_BASE_SUBSTATE_H_INCLUDED
#define WEAPON_CORE_SHOTGUN_RELOAD_BASE_SUBSTATE_H_INCLUDED

#include <vostok/ai/fsm_state.h>

#include <vostok/animation/mixing.h>
#include <vostok/animation/mixing_animation_lexeme.h>
#include <vostok/animation/mixing_expression.h>
#include <vostok/animation/animation_callback.h>

#include <vostok/game_core/weapon_user_state_enum.h>
#include <vostok/game_core/weapon_lexeme_pair.h>

namespace vostok {
namespace animation {
	struct animation_playback_state;
	struct animation_callback_params;
}
}

namespace survarium {

class weapon_core;

class weapon_core_shotgun_reload_base_substate : public ai::fsm_state , public resources::unmanaged_resource {
protected:
	explicit			weapon_core_shotgun_reload_base_substate(
							weapon_core&							weapon,
							float const								animation_time_scale,
							resources::managed_resource_ptr const*	animations,
							u32 const								animations_count,
							animation::mixing::playback_enum const	playback_type,
							u32 const								time_synchronization_group,
							pcstr									animation_id,
							pcstr									hands_stand_animation_id,
							pcstr									hands_crouch_animation_id,
							pcstr									hands_jump_animation_id
						);

public:
	// STATE[STUB]
	virtual	void		initialize						( ) override { /* <0x2a800> */ }
	// STATE[STUB]
	virtual	void		execute							( ) override { /* <0x98860> */ }
	virtual	void		finalize						( ) override {
		ASSERT( UNKNOWN_EXPRESSION ); m_animation_playback_state->reset( );
	}
			animation::mixing::expression
					weapon_and_hands_expression			(
						mutable_buffer&							buffer,
						const bool									is_third_view,
						const weapon_user_state_enum					user_state_id,
						animation::mixing::animation_lexeme&	weight_driving_animation
					) const;

	inline	void	set_animation_playback_state_ptr	( animation::animation_playback_state* animation_playback_state ) { m_animation_playback_state = animation_playback_state; }

	inline	bool	deserializing						( ) const { /* no source */ }

private:
			weapon_lexeme_pair
					get_weapon_lexeme_pair				( mutable_buffer& buffer, bool const is_third_view, weapon_user_state_enum const user_state_id ) const;

protected:
	/* 0x0000 */	/* ai::fsm_state */
	/* 0x0018 */	/* resources::unmanaged_resource */
	/* 0x0120 */	weapon_core&							m_weapon;
	/* 0x0124 */	mutable resources::managed_resource_ptr	m_animation_to_wait_for;
	/* 0x0128 */	resources::managed_resource_ptr			m_weapon_animations[2][2];
	/* 0x0138 */	resources::managed_resource_ptr			m_user_animations[2][2];
	/* 0x0148 */	animation::animation_playback_state*	m_animation_playback_state;
	/* 0x014c */	float									m_animation_timescale;
	/* 0x0150 */	animation::mixing::playback_enum		m_playback_type;
	/* 0x0154 */	u32										m_time_synchronization_group;
	/* 0x0158 */	pcstr									m_animation_id;
	/* 0x015c */	pcstr									m_hand_animation_captions[3];
}; // class weapon_core_shotgun_reload_base_substate

STATIC_SIZE_ASSERT(weapon_core_shotgun_reload_base_substate, 0x168);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_SHOTGUN_RELOAD_BASE_SUBSTATE_H_INCLUDED
