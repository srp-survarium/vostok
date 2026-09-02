// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LOGIN_MENU_H_INCLUDED
#define LOGIN_MENU_H_INCLUDED

#include <vostok/input/handler.h>

#include "base_game_scene.h"
#include "login_menu_status_enum.h"

namespace vostok {
namespace resources {
	class queries_result;
} // namespace resources
} // namespace vostok

namespace survarium {

class bullet_manager;
class game;

class login_menu : public base_game_scene , public input::handler {
	typedef base_game_scene super;

public:
			explicit			login_menu			( game& g );
	virtual						~login_menu			( );

			void				set_status			( login_menu_status_enum status );

	virtual	void				on_activate			( ) override;
	virtual	void				on_deactivate		( ) override;

	virtual	void				tick				(
									const u32		frame_delta_in_ms,
									const u32		current_time_in_ms,
									const bool		is_game_paused
								) override;

	virtual	void				clear_resources		( ) override;

	virtual	bool				on_keyboard_action	(
									input::world*					input_world,
									input::enum_keyboard			key,
									input::enum_keyboard_action		action
								) override;
	// PDB: all three params are __formal (genuinely unused) - left unnamed,
	// three __formal would collide
	virtual	bool				on_gamepad_action	(
									input::world*,
									input::gamepad_button,
									input::enum_gamepad_action
								) override;
	virtual	bool				on_mouse_key_action	(
									input::world*					input_world,
									input::mouse_button				button,
									input::enum_mouse_key_action	action
								) override;
	virtual	bool				on_mouse_move		(
									input::world*		input_world,
									s32					x,
									s32					y,
									s32					z
								) override;

	virtual	s32					input_priority		( ) override { /* no source */ return 0; }

			void				fill_labels			( );

	inline	bool				action_blocked		( ) const { return m_block_btn_time != 0; }

	// the menu has no bullet manager; the override returns a null reference
	virtual	bullet_manager&		get_bullet_manager	( ) const override
	{
		return *( bullet_manager* )NULL;
	}

private:
			void				query_resources		( );

			void				on_resources_ready	( resources::queries_result& data );

			void				enable_button		( bool value );

private:
	/* 0x0000 */	/* base_game_scene */
	/* 0x00bc */	/* input::handler */
	// shadows base_game_scene::m_block_btn_time (both are in the PDB layout)
	/* 0x00c0 */	u32							m_block_btn_time;
	/* 0x00c4 */	login_menu_status_enum		m_status;
	/* 0x00c8 */	flash_movie_resource_ptr	m_login_menu_ui;
	/* 0x00cc */	flash_movie_resource_ptr	m_cursor_ui;
}; // class login_menu

STATIC_SIZE_ASSERT(login_menu, 0xD0);

} // namespace survarium

#endif // #ifndef LOGIN_MENU_H_INCLUDED
