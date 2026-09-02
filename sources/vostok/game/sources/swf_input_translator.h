// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SWF_INPUT_TRANSLATOR_H_INCLUDED
#define SWF_INPUT_TRANSLATOR_H_INCLUDED

#include <vostok/input/keyboard.h>	// enum_keyboard (map key + by-value params)
#include <vostok/input/mouse.h>		// mouse_button, enum_mouse_key_action

namespace vostok {
namespace input {
	struct world;	// world is a struct in <vostok/input/world.h>; the class
					// forward-decl mangled process_keyboard's param as PAV not the
					// target's PAU, breaking the link the moment a consumer pulls
					// in the real (struct) world.h
} // namespace input
} // namespace vostok


namespace survarium {

// must be complete here: the stlport map member instantiates
// pair< enum_keyboard, dik_to_swf_bind > when the class is completed (so the
// batch-2 "TU-local, owned by swf_input_translator.cpp" call was wrong - the
// original header had the definition; the dump's FORWARD REFS line is parser
// output, not source truth)
struct dik_to_swf_bind {

public:
	/* 0x0000 */	input::enum_keyboard	key;
	/* 0x0004 */	wchar_t					c;
	/* 0x0006 */	wchar_t					c_shift;
	/* 0x0008 */	s32						scan;
	/* 0x000c */	bool					translate;
	/* 0x000d */	bool					is_character;
}; // struct dik_to_swf_bind

STATIC_SIZE_ASSERT(dik_to_swf_bind, 0x10);

struct flash_movie;

class swf_input_translator {
public:
								swf_input_translator	( );

			bool				process_keyboard		(
									input::world*					input_world,
									input::enum_keyboard			key,
									input::enum_keyboard_action		action,
									flash_movie*					movie,
									u32								time_current_ms
								);
			bool				process_mouse_btn		(
									input::world*					__formal,
									input::mouse_button				button,
									input::enum_mouse_key_action	action,
									float							x,
									float							y,
									flash_movie*					movie
								);
			bool				process_mouse_move		(
									input::world*		__formal,
									float				x,
									float				y,
									const float			scroll_delta,
									flash_movie*		movie
								);

protected:
			void				initialize				( );

			dik_to_swf_bind*	get_bind				( input::enum_keyboard key );

			wchar_t				translate_key_action	( input::world* input_world, bool is_shift_now, dik_to_swf_bind& current );

			void				register_ctl_bind		( input::enum_keyboard key, s32 scan );
			void				register_char_bind		(
									input::enum_keyboard	key,
									wchar_t					c,
									wchar_t					c_shift,
									s32						scan,
									bool					translate
								);
			void				register_char_bind		( input::enum_keyboard key, s32 scan, bool translate );

public:

protected:
	/* 0x0000 */	map< input::enum_keyboard, dik_to_swf_bind >	char_map;
}; // class swf_input_translator

STATIC_SIZE_ASSERT(swf_input_translator, 0x18);

} // namespace survarium

#endif // #ifndef SWF_INPUT_TRANSLATOR_H_INCLUDED
