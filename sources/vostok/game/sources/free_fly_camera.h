////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef FREE_FLY_CAMERA_H_INCLUDED
#define FREE_FLY_CAMERA_H_INCLUDED

#include <vostok/input/handler.h>

#include "game_camera.h"

namespace survarium {

class base_game_scene;
class camera_director;

class free_fly_camera : public game_camera , public input::handler {
public:
					free_fly_camera		( base_game_scene& w, camera_director& cd );

	virtual	bool	on_keyboard_action	(
						input::world*					input_world,
						input::enum_keyboard			key,
						input::enum_keyboard_action		action
					) override;
	virtual	bool	on_gamepad_action	(
						input::world*					input_world,
						input::gamepad_button			button,
						input::enum_gamepad_action		action
					) override;
	virtual	bool	on_mouse_key_action	(
						input::world*					input_world,
						input::mouse_button				button,
						input::enum_mouse_key_action	action
					) override;
	virtual	bool	on_mouse_move		(
						input::world*		input_world,
						s32					x,
						s32					y,
						s32					z
					) override;

	virtual	s32		input_priority		( ) override { /* no source */ return 0; }

	virtual	void	tick				( ) override;

	virtual	void	on_activate			( camera_director* cd ) override;
	virtual	void	on_focus			( bool b_focus_enter ) override;

private:
			void	build_view_matrix	(
						float2 const&		raw_angles,
						float				shift_forward,
						float				shift_right,
						float				shift_up
					);

			bool	keyb_event_present	( s32 e );
			bool	mouse_event_present	( s32 e );

public:
	virtual			~free_fly_camera	( ) { /* sushi@NOTE: Retail inlines member cleanup into its deleting destructor. */ }

private:
	/* 0x0000 */	/* game_camera */
	/* 0x0054 */	/* input::handler */
	/* 0x0058 */	camera_director&	m_camera_director;
	/* 0x005c */	u32					m_prev_time_ms;
	/* 0x0060 */	float				m_prev_delta_sec;
	/* 0x0064 */	vector< int >		m_keyb_events;
	/* 0x0070 */	vector< int >		m_mouse_events;
	/* 0x007c */	float3				m_mouse_move;
}; // class free_fly_camera

STATIC_SIZE_ASSERT(free_fly_camera, 0x88);

} // namespace survarium

#endif // #ifndef FREE_FLY_CAMERA_H_INCLUDED
