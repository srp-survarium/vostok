////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_SHOTGUN_RELOAD_BASE_SUBSTATE_H_INCLUDED
#define WEAPON_CORE_SHOTGUN_RELOAD_BASE_SUBSTATE_H_INCLUDED

#include <vostok/ai/fsm_state.h>

namespace survarium {

class survarium::weapon_core;
struct vostok::animation::animation_playback_state;
enum vostok::animation::mixing::playback_enum /* stored as s32 */ {
	play_cyclically = 0x0000,
	play_once_and_freeze_at_end = 0x0001,
	play_once_and_remove_at_end = 0x0002,
}


class weapon_core_shotgun_reload_base_substate : public ai::fsm_state , public resources::unmanaged_resource {
public:
	explicit									weapon_core_shotgun_reload_base_substate(
													weapon_core&							weapon,
													float									animation_time_scale,
													resources::managed_resource_ptr const*	animations,
													u32										animations_count,
													animation::mixing::playback_enum		playback_type,
													u32										time_synchronization_group,
													pcstr									animation_id,
													pcstr									hands_stand_animation_id,
													pcstr									hands_crouch_animation_id,
													pcstr									hands_jump_animation_id
												);

	virtual	void								initialize						( ) override;
	virtual	void								execute							( ) override;
	virtual	void								finalize						( ) override { /* no source */ }

			animation::mixing::expression		weapon_and_hands_expression		(
													mutable_buffer&						buffer,
													bool								is_third_view,
													weapon_user_state_enum				user_state_id,
													animation::mixing::animation_lexeme&	weight_driving_animation
												) const;

	inline	void								set_animation_playback_state_ptr( animation::animation_playback_state* arg_0 ) { /* no source */ }

	inline	bool								deserializing					( ) const { /* no source */ }

			weapon_lexeme_pair					get_weapon_lexeme_pair			( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const;

private:
	/* 0x0000 */	/* ai::fsm_state */
	/* 0x0018 */	/* resources::unmanaged_resource */
	/* 0x0120 */	weapon_core&							m_weapon;
	/* 0x0124 */	resources::managed_resource_ptr			m_animation_to_wait_for;
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
