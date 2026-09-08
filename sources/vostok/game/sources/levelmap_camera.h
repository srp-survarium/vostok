// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LEVELMAP_CAMERA_H_INCLUDED
#define LEVELMAP_CAMERA_H_INCLUDED

#include <vostok/math_aabb.h>
#include <vostok/input/handler.h>
#include <vostok/input/world.h>

#include "game_camera.h"
#include "game_world.h"
#include "game.h"

namespace survarium {

class base_game_scene;
class camera_director;
class game_world;

class levelmap_camera : public game_camera , public input::handler {
typedef game_camera super;

public:
	// sushi@TODO: World-binding model; recover original bounds initialization and construction consumer.
	inline				levelmap_camera			( game_world& world, camera_director& director ) :
							game_camera			( world ),
							levelmap_bbox		( math::create_invalid_aabb( ) ),
							m_game_world		( world ),
							m_camera_director	( director )
						{ }

	// sushi@TODO: Recover bounds producer, projected axes, aspect policy and clipping before choosing the projection.
	virtual	float4x4	get_projection_matrix	( float2 const& arg_0 ) const override { /* no source */ return float4x4(); }

	// sushi@TODO: Recover key/context and focus-switch bindings; this class has no free-fly event queues.
	virtual	bool		on_keyboard_action		(
							input::world*					arg_0,
							input::enum_keyboard			arg_1,
							input::enum_keyboard_action		arg_2
						) override { /* no source */ return false; }
	// sushi@TODO: Recover whether pad actions are consumed or ignored; sibling camera policies disagree.
	virtual	bool		on_gamepad_action		(
							input::world*					arg_0,
							input::gamepad_button			arg_1,
							input::enum_gamepad_action		arg_2
						) override { /* no source */ return false; }
	// sushi@TODO: Recover selection/capture policy; do not invent the lobby camera's missing capture member.
	virtual	bool		on_mouse_key_action		(
							input::world*					arg_0,
							input::mouse_button				arg_1,
							input::enum_mouse_key_action	arg_2
						) override { /* no source */ return false; }
	// sushi@TODO: Recover immediate pan/zoom versus ignored motion and its bounds/axis conventions.
	virtual	bool		on_mouse_move			(
							input::world*		arg_0,
							s32					arg_1,
							s32					arg_2,
							s32					arg_3
						) override { /* no source */ return false; }

	// sushi@TODO: Sibling camera priority model; level-map's own value and registration remain unverified.
	virtual	s32			input_priority			( ) override { return 10; }

	// sushi@TODO: Recover static bounds-to-view update versus per-frame input polling; no timer field is declared.
	virtual	void		tick					( ) override { /* no source */ }

	// sushi@TODO: Recover initial top-down pose and bounds ownership before copying or replacing director state.
	virtual	void		on_activate				( camera_director* arg_0 ) override { /* no source */ }
	// sushi@TODO: Sibling input-registration model; verify original focus consumer and base-hook delegation.
	virtual	void		on_focus				( bool focus_enter ) override
	{
		if ( focus_enter )
			m_game_world.get_game( ).input_world( ).add_handler( *this );
		else
			m_game_world.get_game( ).input_world( ).remove_handler( *this );
	}

	/* 0x0000 */	/* game_camera */
	/* 0x0054 */	/* input::handler */
	/* 0x0058 */	math::aabb			levelmap_bbox;

private:
	/* 0x0070 */	game_world&			m_game_world;
	/* 0x0074 */	camera_director&	m_camera_director;
}; // class levelmap_camera

STATIC_SIZE_ASSERT(levelmap_camera, 0x78);

} // namespace survarium

#endif // #ifndef LEVELMAP_CAMERA_H_INCLUDED
