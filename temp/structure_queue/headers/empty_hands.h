////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef EMPTY_HANDS_H_INCLUDED
#define EMPTY_HANDS_H_INCLUDED

/* INCLUDES */
class vostok::math::random32;
class survarium::interactive_object;
struct survarium::base_player;
class vostok::animation::mixing::expression;
class vostok::animation::skeleton;
typedef vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>
	vostok::resources::managed_resource_ptr;
class vostok::resources::managed_resource;

/* FORWARD REFS */
class vostok::animation::animation_player;
class vostok::network_core::packet_reader;
class vostok::network_core::udp_match_packet;
class survarium::engine;

namespace survarium {

class empty_hands : public interactive_object {
public:
												empty_hands					( resources::managed_resource_ptr* animations, u32 animations_count );

	virtual	void								activate					( base_player& user, engine& engine ) override;

	virtual	void								deactivate					( ) override { /* no source */ }

	virtual	float4x4							transform					( ) const override { /* no source */ }

	virtual	void								tick						( ) override { /* no source */ }

	virtual	bool								is_ready_to_be_deactivated	( ) const override { /* no source */ }

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

	virtual	bool								is_sprinting				( ) const override { /* no source */ }

	virtual	void								serialize					( network_core::udp_match_packet& packet, u32 client_offset ) const override { /* no source */ }
	virtual	void								deserialize					( network_core::packet_reader& reader ) override { /* no source */ }

	virtual										~empty_hands				( ) { /* no source */ }

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
