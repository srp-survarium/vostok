////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef INTERACTIVE_OBJECT_H_INCLUDED
#define INTERACTIVE_OBJECT_H_INCLUDED

#include <vostok/animation/skeleton.h>

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

class base_player;		// sushi@TODO
class engine;			// sushi@TODO
class weapon_core;		// sushi@TODO
class game_world_ui;	// sushi@TODO

class interactive_object : public resources::unmanaged_resource {
public:
	virtual	void								activate					( base_player& arg_0, engine& arg_1 ) = 0;
	virtual	void								deactivate					( ) = 0;

	virtual	float4x4							transform					( ) const = 0;

	virtual	void								tick						( ) = 0;

	virtual	bool								is_ready_to_be_deactivated	( ) const = 0;

	// virtual	animation::mixing::expression		selected_animations			( mutable_buffer& arg_0, bool arg_1 ) const = 0; sushi@TODO

	virtual	void								on_player_model_added		( ) = 0;
	virtual	void								on_player_model_removed		( ) = 0;

	virtual	void								update_bones_matrices		(
													animation::skeleton_ptr const&		arg_0,
													float4x4*							arg_1,
													u32									arg_2,
													u32									arg_3,
													float4x4&							arg_4,
													float4x4&							arg_5,
													animation::animation_player const&	arg_6
												) = 0;

	virtual	bool								is_sprinting				( ) const = 0;

	virtual	void								assign_game_ui				( game_world_ui* arg_0 ) { /* no source */ }

	virtual	void								serialize					( network_core::udp_match_packet& packet, u32 client_offset ) const = 0;
	virtual	void								deserialize					( network_core::packet_reader& reader ) = 0;

	virtual	weapon_core const*					cast_weapon_core			( ) const { /* no source */ }
	virtual	weapon_core*						cast_weapon_core			( );
}; // class interactive_object

STATIC_SIZE_ASSERT(interactive_object, 0x108);

} // namespace survarium

#endif // #ifndef INTERACTIVE_OBJECT_H_INCLUDED
