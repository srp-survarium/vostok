// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LEVELMAP_CAMERA_H_INCLUDED
#define LEVELMAP_CAMERA_H_INCLUDED

#include <vostok/math_aabb.h>
#include <vostok/input/handler.h>

#include "game_camera.h"

namespace survarium {

class base_game_scene;
class camera_director;
class game_world;

class levelmap_camera : public game_camera , public input::handler {
typedef game_camera super;

public:
	// game_world IS-A base_game_scene (batch 11) but is only forward-declared
	// here - the game_camera base init is a buildability placeholder until a
	// matcher fills the body (math::aabb's default ctor is private, hence the
	// create_invalid_aabb init)
	inline				levelmap_camera			( game_world& arg_0, camera_director& arg_1 ) :
							game_camera			( *( base_game_scene* )NULL ),
							levelmap_bbox		( math::create_invalid_aabb( ) ),
							m_game_world		( arg_0 ),
							m_camera_director	( arg_1 )
						{ /* no source */ }

	virtual	float4x4	get_projection_matrix	( float2 const& arg_0 ) const override { /* no source */ return float4x4(); }

	virtual	bool		on_keyboard_action		(
							input::world*					arg_0,
							input::enum_keyboard			arg_1,
							input::enum_keyboard_action		arg_2
						) override { /* no source */ return false; }
	virtual	bool		on_gamepad_action		(
							input::world*					arg_0,
							input::gamepad_button			arg_1,
							input::enum_gamepad_action		arg_2
						) override { /* no source */ return false; }
	virtual	bool		on_mouse_key_action		(
							input::world*					arg_0,
							input::mouse_button				arg_1,
							input::enum_mouse_key_action	arg_2
						) override { /* no source */ return false; }
	virtual	bool		on_mouse_move			(
							input::world*		arg_0,
							s32					arg_1,
							s32					arg_2,
							s32					arg_3
						) override { /* no source */ return false; }

	virtual	s32			input_priority			( ) override { /* no source */ return 0; }

	virtual	void		tick					( ) override { /* no source */ }

	virtual	void		on_activate				( camera_director* arg_0 ) override { /* no source */ }
	virtual	void		on_focus				( bool arg_0 ) override { /* no source */ }

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
