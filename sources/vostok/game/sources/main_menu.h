////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef MAIN_MENU_H_INCLUDED
#define MAIN_MENU_H_INCLUDED

#include <vostok/input/handler.h>

#include "base_game_scene.h"

namespace vostok {
namespace resources {
	class queries_result;
} // namespace resources
} // namespace vostok

namespace survarium {

class bullet_manager;
class game;

// void* main_menu::`scalar deleting destructor'( u32 ) // FUNCTION BODY[0x91bf0]: <0x427c0>|0x000|      :'43'	{

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

	// STATE[STUB]
	virtual	input::handler&		input_handler		( )
	{
		return *this;

		// FUNCTION BODY[0x92e90]
		// <0x92e90>|0x000|      :'35'	{
		// ******
	}

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

	// STATE[STUB]
	virtual	s32					input_priority		( ) override
	{
		return 0;

		// FUNCTION BODY[0x92ea0]
		// <0x92ea0>|0x000|      :'41'	{
		// ******
	}

	// buildability return; the real body reaches the manager through m_game
	virtual	bullet_manager&		get_bullet_manager	( ) const override { /* no source */ return *( bullet_manager* )NULL; }

			void				query_resources		( );

			void				on_resources_ready	( resources::queries_result& data );
}; // class main_menu

STATIC_SIZE_ASSERT(main_menu, 0xC0);

} // namespace survarium

#endif // #ifndef MAIN_MENU_H_INCLUDED
