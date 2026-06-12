////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef LOGIN_MENU_H_INCLUDED
#define LOGIN_MENU_H_INCLUDED

/* INCLUDES */
class survarium::base_game_scene;
struct vostok::input::handler;
enum survarium::login_menu_status_enum;
class vostok::input::enum_gamepad_action;
class vostok::input::enum_keyboard;
class vostok::input::enum_keyboard_action;
class vostok::input::enum_mouse_key_action;
class vostok::input::gamepad_button;
class vostok::input::mouse_button;
typedef vostok::resources::resource_ptr<survarium::flash_movie_resource,vostok::resources::unmanaged_intrusive_base>
	survarium::flash_movie_resource_ptr;
class survarium::flash_movie_resource;

/* FORWARD REFS */
class vostok::input::world;
class vostok::resources::queries_result;
class survarium::bullet_manager;
class survarium::game;

namespace survarium {

enum survarium::login_menu_status_enum
{
	login_menu_status_error_connection		= 0x0,
	login_menu_status_invalid_user_or_password	= 0x1,
	login_menu_status_sign_in_attempt_interval_violated	= 0x2,
	login_menu_status_disconnected			= 0x3,
	login_menu_status_connected				= 0x4,
	login_menu_status_user_banned			= 0x5,
	login_menu_status_access_level_restriction	= 0x6,
	login_menu_status_sign_in_already_online	= 0x7,
	login_menu_status_invalid_version		= 0x8,
};

class login_menu : public base_game_scene , public input::handler {
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
	virtual	bool				on_gamepad_action	(
									input::world*					__formal,
									input::gamepad_button			__formal,
									input::enum_gamepad_action		__formal
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

	virtual	s32					input_priority		( ) override { /* no source */ }

			void				fill_labels			( );

	inline	bool				action_blocked		( ) const { /* no source */ }

	virtual	bullet_manager&		get_bullet_manager	( ) const override { /* no source */ }

			void				query_resources		( );

			void				on_resources_ready	( resources::queries_result& data );

			void				enable_button		( bool value );

private:
	/* 0x0000 */	/* base_game_scene */
	/* 0x00bc */	/* input::handler */
	/* 0x00c0 */	u32							m_block_btn_time;
	/* 0x00c4 */	login_menu_status_enum		m_status;
	/* 0x00c8 */	flash_movie_resource_ptr	m_login_menu_ui;
	/* 0x00cc */	flash_movie_resource_ptr	m_cursor_ui;
}; // class login_menu

STATIC_SIZE_ASSERT(login_menu, 0xD0);

} // namespace survarium

#endif // #ifndef LOGIN_MENU_H_INCLUDED
