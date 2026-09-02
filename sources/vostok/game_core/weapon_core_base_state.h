// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef WEAPON_CORE_BASE_STATE_H_INCLUDED
#define WEAPON_CORE_BASE_STATE_H_INCLUDED

#include <vostok/ai/fsm_state.h>
#include <vostok/animation/api.h>
#include <vostok/animation/mixing_animation_lexeme.h>
#include <vostok/animation/mixing_expression.h>

#include <vostok/game_core/weapon_user_state_enum.h>
#include <vostok/game_core/weapon_lexeme_pair.h>

namespace vostok {
namespace network_core {
	class udp_match_packet;
	class packet_reader;
}
}

namespace survarium {

class weapon_core;

class weapon_core_base_state : public ai::fsm_state , public resources::unmanaged_resource , private boost::noncopyable {
protected:
	// ctor mangles ?? 0...@@IAE@... -> protected, non-const (objdiff pairs by mangled name)
	explicit									weapon_core_base_state		( weapon_core& weapon, bool serialize_animation_state );

public:
	inline	bool								is_ready_to_be_deactivated	( ) const { return m_is_ready_to_be_deactivated; }
	inline	animation::body_part_masks_enum		get_body_part_mask_for_user	( ) const { return m_body_part_mask_for_user; }
			bool								has_animation_ended			( ) const { return m_animation_has_been_ended; }

public:
	virtual	void		initialize					( ) override {}
	virtual	void		finalize					( ) override;
	virtual	void		execute						( ) override { m_animation_playback_state.reset( ); }
	virtual	bool		is_ready_for_transition		( ) const override { return true; }

	virtual	void		serialize					( network_core::udp_match_packet& packet ) const;
	virtual	void		deserialize					( network_core::packet_reader& reader );

	virtual	animation::mixing::expression
						weapon_and_hands_expression	(
							mutable_buffer&							arg_0,
							const bool							arg_1,
							const weapon_user_state_enum			arg_2,
							animation::mixing::animation_lexeme&	arg_3
						) const = 0;

	// claude@MATCH: real body proven by initialize_weapon_logic's target carcass -
	// the inlined call stores the pointer at [state+0x12C] (m_is_firing_ptr).
	inline	void		set_is_firing_ptr			( bool* is_firing ) { m_is_firing_ptr = is_firing; }
protected:
	inline	void		set_is_firing				( bool arg_0 ) { /* no source */ }

	// mangles ?deserializing@...@@IBE_NXZ -> protected, const (objdiff pairs by mangled name)
			bool		deserializing				( ) const;

protected:
	/* 0x0000 */	/* ai::fsm_state */
	/* 0x0018 */	/* resources::unmanaged_resource */
	/* 0x0120 */	animation::animation_playback_state	m_animation_playback_state;
	/* 0x0128 */	weapon_core&						m_weapon;
	/* 0x012c */	bool*								m_is_firing_ptr;
	/* 0x0130 */	animation::body_part_masks_enum		m_body_part_mask_for_user;
	/* 0x0134 */	bool								m_is_ready_to_be_deactivated;
	/* 0x0135 */	bool								m_animation_has_been_ended;
	/* 0x0136 */	const bool						m_serialize_animation_state;
}; // class weapon_core_base_state

STATIC_SIZE_ASSERT(weapon_core_base_state, 0x138);

typedef resources::resource_ptr< weapon_core_base_state, resources::unmanaged_intrusive_base > weapon_core_base_state_ptr;

inline void weapon_core_base_state::finalize( ) { m_animation_playback_state.reset( ); }

} // namespace survarium

#endif // #ifndef WEAPON_CORE_BASE_STATE_H_INCLUDED
