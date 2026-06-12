////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef CLIENT_PLAYER_STATE_H_INCLUDED
#define CLIENT_PLAYER_STATE_H_INCLUDED

/* INCLUDES */
class vostok::animation::animation_player;
class vostok::physics::bt_character_controller;
typedef vostok::resources::resource_ptr<vostok::render::skeleton_model_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::skeleton_model_instance_ptr;
class vostok::render::skeleton_model_instance;

namespace survarium {

struct client_player_state {
			void	update_transform	( );

	inline			client_player_state	( ) { /* no source */ }
	inline			~client_player_state( ) { /* no source */ }

public:
	/* 0x0000 */	animation::animation_player				animation_player;
	/* 0x8548 */	float4x4								transform;
	/* 0x8588 */	float4x4								previous_transform;
	/* 0x85c8 */	render::skeleton_model_instance_ptr		model;
	/* 0x85cc */	physics::bt_character_controller*		physics_controller;
	/* 0x85d0 */	float									look_pitch;
}; // struct client_player_state

STATIC_SIZE_ASSERT(client_player_state, 0x85D4);

} // namespace survarium

#endif // #ifndef CLIENT_PLAYER_STATE_H_INCLUDED
