////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef MAIN_MENU_H_INCLUDED
#define MAIN_MENU_H_INCLUDED

/* INCLUDES */
class survarium::base_game_scene;
struct vostok::input::handler;
class vostok::input::enum_gamepad_action;
class vostok::input::enum_keyboard;
class vostok::input::enum_keyboard_action;
class vostok::input::enum_mouse_key_action;
class vostok::input::gamepad_button;
class vostok::input::mouse_button;

/* FORWARD REFS */
class vostok::input::world;
class vostok::resources::queries_result;
class survarium::bullet_manager;
class survarium::game;

namespace survarium {

class main_menu : public base_game_scene , public input::handler {
public:
			explicit			main_menu			( game& g );
	virtual						~main_menu			( );

	virtual	void				on_activate			( ) override;
	virtual	void				on_deactivate		( ) override;

	virtual	void				tick				(
									const u32		frame_delta_in_ms,
									const u32		current_time_in_ms,
									const bool		is_game_paused
								) override;

	virtual	void				clear_resources		( ) override;

	virtual	input::handler&		input_handler		( ) { /* no source */ }

	virtual	bool				on_keyboard_action	(
									input::world*					input_world,
									input::enum_keyboard			key,
									input::enum_keyboard_action		action
								) override;
	virtual	bool				on_gamepad_action	(
									input::world*					input_world,
									input::gamepad_button			button,
									input::enum_gamepad_action		action
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

	virtual	bullet_manager&		get_bullet_manager	( ) const override { /* no source */ }

			void				query_resources		( );

			void				on_resources_ready	( resources::queries_result& data );
}; // class main_menu

STATIC_SIZE_ASSERT(main_menu, 0xC0);

} // namespace survarium

#endif // #ifndef MAIN_MENU_H_INCLUDED
