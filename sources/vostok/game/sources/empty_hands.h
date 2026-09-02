// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef EMPTY_HANDS_H_INCLUDED
#define EMPTY_HANDS_H_INCLUDED

#include <vostok/game_core/interactive_object.h>
#include <vostok/resources_managed_resource.h>
#include <vostok/math_randoms_generator.h>	// math::random32


namespace survarium {

class empty_hands_cook;

class empty_hands : public interactive_object {
	friend class empty_hands_cook;
private:
												empty_hands					( resources::managed_resource_ptr* animations, u32 animations_count );

	virtual	void								activate					( base_player& user, engine& engine ) override;

	virtual	void								deactivate					( ) override { /* no source */ }

	virtual	float4x4							transform					( ) const override
	{
		return m_transform;
	}

	virtual	void								tick						( ) override { /* no source */ }

	virtual	bool								is_ready_to_be_deactivated	( ) const override
	{
		return false;
	}

	virtual	animation::mixing::expression		selected_animations			( mutable_buffer& buffer, const bool is_third_view ) const override;

	virtual	void								on_player_model_added		( ) override { /* no source */ }
	virtual	void								on_player_model_removed		( ) override { /* no source */ }

	virtual	void								update_bones_matrices		(
													animation::skeleton_ptr const&			user_skeleton,
													float4x4* const							user_matrices,
													const u32								user_matrices_count,
													const u32								current_time_in_ms,
													float4x4&								character_head_transform,
													float4x4&								character_transform,
													animation::animation_player const&		animation_player
												) override;

	virtual	bool								is_sprinting				( ) const override
	{
		return false;
	}

	virtual	void								serialize					( network_core::udp_match_packet& packet, u32 client_offset ) const override
	{
		vostok::detail::unreferenced_parameter_helper( &packet, client_offset );
	}

	// claude@NOTE: faithful empty body. Target is a bare `int3` (0x1 byte, 0 stmts):
	// ICF maps the identical empty body elsewhere, so only the alignment trap survives
	// at this RVA; our genuine `ret 4` is correct and
	// the int3-vs-ret residual is a linker fold artifact, not source-steerable.
	virtual	void								deserialize					( network_core::packet_reader& reader ) override
	{
	}

public:

private:
	/* 0x0000 */	/* interactive_object */
	/* 0x0108 */	float4x4			m_transform;
	/* 0x0148 */	base_player*		m_user;
	/* 0x014c */	resources::managed_resource_ptr* const	m_animations;
	/* 0x0150 */	const u32			m_animations_count;
	/* 0x0154 */	math::random32		m_random;
}; // class empty_hands

STATIC_SIZE_ASSERT(empty_hands, 0x158);

} // namespace survarium

#endif // #ifndef EMPTY_HANDS_H_INCLUDED
