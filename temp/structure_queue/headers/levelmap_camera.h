////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef LEVELMAP_CAMERA_H_INCLUDED
#define LEVELMAP_CAMERA_H_INCLUDED

/* INCLUDES */
class vostok::math::aabb;
class survarium::camera_director;
class survarium::game_camera;
class survarium::game_world;
struct vostok::input::handler;
class vostok::input::enum_gamepad_action;
class vostok::input::enum_keyboard;
class vostok::input::enum_keyboard_action;
class vostok::input::enum_mouse_key_action;
class vostok::input::gamepad_button;
class vostok::input::mouse_button;

/* FORWARD REFS */
class vostok::input::world;

namespace survarium {

class levelmap_camera : public game_camera , public input::handler {
public:
	inline				levelmap_camera			( game_world& arg_0, camera_director& arg_1 ) { /* no source */ }

	virtual	float4x4	get_projection_matrix	( float2 const& arg_0 ) const override { /* no source */ }

	virtual	bool		on_keyboard_action		(
							input::world*					arg_0,
							input::enum_keyboard			arg_1,
							input::enum_keyboard_action		arg_2
						) override { /* no source */ }
	virtual	bool		on_gamepad_action		(
							input::world*					arg_0,
							input::gamepad_button			arg_1,
							input::enum_gamepad_action		arg_2
						) override { /* no source */ }
	virtual	bool		on_mouse_key_action		(
							input::world*					arg_0,
							input::mouse_button				arg_1,
							input::enum_mouse_key_action	arg_2
						) override { /* no source */ }
	virtual	bool		on_mouse_move			(
							input::world*		arg_0,
							s32					arg_1,
							s32					arg_2,
							s32					arg_3
						) override { /* no source */ }

	virtual	s32			input_priority			( ) override { /* no source */ }

	virtual	void		tick					( ) override { /* no source */ }

	virtual	void		on_activate				( camera_director* arg_0 ) override { /* no source */ }
	virtual	void		on_focus				( bool arg_0 ) override { /* no source */ }

	virtual				~levelmap_camera		( ) { /* no source */ }

private:
	/* 0x0000 */	/* game_camera */
	/* 0x0054 */	/* input::handler */
	/* 0x0058 */	math::aabb			levelmap_bbox;
	/* 0x0070 */	game_world&			m_game_world;
	/* 0x0074 */	camera_director&	m_camera_director;
}; // class levelmap_camera

STATIC_SIZE_ASSERT(levelmap_camera, 0x78);

} // namespace survarium

#endif // #ifndef LEVELMAP_CAMERA_H_INCLUDED
