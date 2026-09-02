// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef INTERACTIVE_OBJECT_H_INCLUDED
#define INTERACTIVE_OBJECT_H_INCLUDED

#include <vostok/animation/skeleton.h>
#include <vostok/animation/mixing_expression.h>

namespace vostok {
namespace animation {
	class animation_player;
}

namespace network_core {
	class udp_match_packet;
	class packet_reader;
}
}

namespace survarium {

struct base_player;
struct engine;
class weapon_core;
class game_world_ui;	// sushi@TODO

class interactive_object : public resources::unmanaged_resource {
public:
	virtual	void		activate					( base_player& user, engine& engine ) = 0;
	virtual	void		deactivate					( ) = 0;

	virtual	float4x4	transform					( ) const = 0;

	virtual	void		tick						( ) = 0;

	virtual	bool		is_ready_to_be_deactivated	( ) const = 0;

	virtual	animation::mixing::expression
						selected_animations			( mutable_buffer& buffer, const bool is_third_view ) const = 0;

	virtual	void		on_player_model_added		( ) = 0;
	virtual	void		on_player_model_removed		( ) = 0;

	virtual	void		update_bones_matrices		(
							animation::skeleton_ptr const&		user_skeleton,
							float4x4* const						user_matrices,
							const u32							user_matrices_count,
							const u32							current_time_in_ms,
							float4x4&							character_head_transform,
							float4x4&							character_transform,
							animation::animation_player const&	animation_player
						) = 0;

	virtual	bool		is_sprinting				( ) const = 0;
	virtual	void		assign_game_ui				( game_world_ui* arg_0 ) { }

	virtual	void		serialize					( network_core::udp_match_packet& packet, u32 client_offset ) const = 0;
	virtual	void		deserialize					( network_core::packet_reader& reader ) = 0;

	virtual	weapon_core*		cast_weapon_core			( ) { return NULL; }
	virtual	weapon_core const*	cast_weapon_core			( ) const { return NULL; }
}; // class interactive_object

STATIC_SIZE_ASSERT(interactive_object, 0x108);

typedef resources::resource_ptr< interactive_object, resources::unmanaged_intrusive_base > interactive_object_ptr;

} // namespace survarium

#endif // #ifndef INTERACTIVE_OBJECT_H_INCLUDED
