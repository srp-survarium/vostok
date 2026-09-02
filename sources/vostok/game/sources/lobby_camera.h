// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef LOBBY_CAMERA_H_INCLUDED
#define LOBBY_CAMERA_H_INCLUDED

#include <vostok/input/handler.h>

#include "game_camera.h"

namespace survarium {

class base_game_scene;
class camera_director;

class lobby_camera : public input::handler , public game_camera {
public:
				explicit	lobby_camera		( base_game_scene& w );

	virtual	bool			on_keyboard_action	(
								input::world*					input_world,
								input::enum_keyboard			key,
								input::enum_keyboard_action		actions_mask
							) override;
	virtual	bool			on_gamepad_action	(
								input::world*					input_world,
								input::gamepad_button			button,
								input::enum_gamepad_action		actions_mask
							) override;
	virtual	bool			on_mouse_key_action	(
								input::world*					input_world,
								input::mouse_button				button,
								input::enum_mouse_key_action	actions_mask
							) override;
	virtual	bool			on_mouse_move		(
								input::world*		input_world,
								s32					x,
								s32					y,
								s32					z
							) override;
	// target mangling: ?on_before_processing@...@@UAEXPAUworld@input@vostok@@I@Z -
	// the shipped input::handler interface passes (world*, u32); our legacy
	// handler.h still has the one-param shape, so no `override` here (same as
	// player_input_handler; revisit at TU enablement)
	virtual	void			on_before_processing( input::world* input_world, const u32 current_time_in_ms );
	virtual	void			on_after_processing	( input::world* input_world ) override;

	virtual	s32				input_priority		( ) override;

	virtual	void			on_activate			( camera_director* cd ) override;
	virtual	void			on_deactivate		( ) override;
	virtual	void			on_focus			( bool b_focus_enter ) override;

	virtual	void			tick				( ) override;

public:
	/* 0x0058 */	float3		m_target_point;

private:
			void			process_collision	(
								float3 const&		target_point,
								float3 const&		direction,
								float&				distance_to_focus_point
							);

private:
	/* 0x0000 */	/* input::handler */
	/* 0x0004 */	/* game_camera */
private:
	/* 0x0064 */	bool		m_capture_move;
	/* 0x0068 */	float		m_yaw;
	/* 0x006c */	float		m_pitch;
	/* 0x0070 */	float		m_target_distance_to_focus_point;
	/* 0x0074 */	float		m_current_distance_to_focus_point;
	/* 0x0078 */	float		m_z_mouse_axis;
	/* 0x007c */	float2		m_rotation_delta;
}; // class lobby_camera

STATIC_SIZE_ASSERT(lobby_camera, 0x84);

} // namespace survarium

#endif // #ifndef LOBBY_CAMERA_H_INCLUDED
