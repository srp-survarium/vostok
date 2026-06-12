////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef GLOBAL_INPUT_HANDLER_H_INCLUDED
#define GLOBAL_INPUT_HANDLER_H_INCLUDED

/* INCLUDES */
class survarium::game;
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

class global_input_handler : public input::handler , public boost::noncopyable {
public:
			explicit	global_input_handler	( game& game );
	virtual				~global_input_handler	( );

	virtual	bool		on_keyboard_action		(
							input::world*					input_world,
							input::enum_keyboard			key,
							input::enum_keyboard_action		action
						) override;
	virtual	bool		on_gamepad_action		(
							input::world*					input_world,
							input::gamepad_button			button,
							input::enum_gamepad_action		action
						) override;
	virtual	bool		on_mouse_key_action		(
							input::world*					input_world,
							input::mouse_button				button,
							input::enum_mouse_key_action	action
						) override;
	virtual	bool		on_mouse_move			(
							input::world*		input_world,
							s32					x,
							s32					y,
							s32					z
						) override;

	virtual	s32			input_priority			( ) override { /* no source */ }

private:
	/* 0x0000 */	/* input::handler */
	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	game&		m_game;
}; // class global_input_handler

STATIC_SIZE_ASSERT(global_input_handler, 0x8);

} // namespace survarium

#endif // #ifndef GLOBAL_INPUT_HANDLER_H_INCLUDED
