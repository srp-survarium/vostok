////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef CLIENT_PLAYER_STATE_H_INCLUDED
#define CLIENT_PLAYER_STATE_H_INCLUDED

#include <vostok/animation/animation_player.h>
// PDB spells the model member vostok::render::skeleton_model_instance_ptr -
// the same resource_ptr type our render tree typedefs as skeleton_model_ptr
#include <vostok/render/facade/model.h>

namespace vostok {
namespace physics {
	class bt_character_controller;
} // namespace physics
} // namespace vostok

namespace survarium {

struct client_player_state {
public:
	/* 0x0000 */	animation::animation_player			animation_player;
	/* 0x8548 */	float4x4							transform;
	/* 0x8588 */	float4x4							previous_transform;
	/* 0x85c8 */	render::skeleton_model_ptr			model;
	/* 0x85cc */	physics::bt_character_controller*	physics_controller;
	/* 0x85d0 */	float								look_pitch;

			void	update_transform	( );
}; // struct client_player_state

STATIC_SIZE_ASSERT(client_player_state, 0x85D4);

} // namespace survarium

#endif // #ifndef CLIENT_PLAYER_STATE_H_INCLUDED
