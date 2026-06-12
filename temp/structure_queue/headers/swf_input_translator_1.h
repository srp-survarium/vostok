////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SWF_INPUT_TRANSLATOR_H_INCLUDED
#define SWF_INPUT_TRANSLATOR_H_INCLUDED

/* INCLUDES */
class survarium::map<enum vostok::input::enum_keyboard,survarium::dik_to_swf_bind,stlp_std::less<enum vostok::input::enum_keyboard> >;
class vostok::input::enum_keyboard;
class vostok::input::enum_keyboard_action;
class vostok::input::enum_mouse_key_action;
class vostok::input::mouse_button;

/* FORWARD REFS */
class vostok::input::world;
class survarium::dik_to_swf_bind;
class survarium::flash_movie;

namespace survarium {

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

	inline						~swf_input_translator	( ) { /* no source */ }

private:
	/* 0x0000 */	map< enum input::enum_keyboard, dik_to_swf_bind, std::less< enum input::enum_keyboard > >	char_map;
}; // class swf_input_translator

STATIC_SIZE_ASSERT(swf_input_translator, 0x18);

} // namespace survarium

#endif // #ifndef SWF_INPUT_TRANSLATOR_H_INCLUDED
