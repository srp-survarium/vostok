////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "swf_input_translator.h"
#include <vostok/scaleform/sources/flash_movie.h>

namespace survarium {

 swf_input_translator::swf_input_translator( )
{
	initialize( );
}

// STATE[STUB]
// claude@NOTE: parked - LTCG-customized calling convention (ret 0Ch cleans only 3
// args, HandleMouseBtn called with no visible `mov ecx, movie`). Target maps the
// (button, action) pair to a flash_movie::mouse_btn_action via a switch on
// (action - 0x151) -> {.2:0, .1:1, default: code+2}, then movie->HandleMouseBtn(
// mouse_btn_action, button_as_u32, x, y); return true. NEXT: reconcile the switch
// constants + the register-passed args once the calling-convention shift is mapped.
bool swf_input_translator::process_mouse_btn(
	input::world*					__formal,
	input::mouse_button				button,
	input::enum_mouse_key_action	action,
	float							x,
	float							y,
	flash_movie*					movie
)
{
	return false;
}

bool swf_input_translator::process_mouse_move(
	input::world*		__formal,
	float				x,
	float				y,
	const float			scroll_delta,
	flash_movie*		movie
)
{
	movie->HandleMouseMove( x, y, scroll_delta );
	return true;
}

// STATE[STUB]
// claude@NOTE: parked - 61-statement keyboard dispatch. Reads input_world->keyboard()
// twice for the shift state (is_shift_now), looks up get_bind(key), and routes
// down/up/repeat actions to movie->HandleKeyboard / HandleChar using the bound
// dik_to_swf_bind (translate_key_action for character output). NEXT: recover the
// keyboard()/is_key_down() accessors + the action-branch structure (lines 53-115).
bool swf_input_translator::process_keyboard(
	input::world*					input_world,
	input::enum_keyboard			key,
	input::enum_keyboard_action		action,
	flash_movie*					movie,
	u32								time_current_ms
)
{
	return false;
}

dik_to_swf_bind* swf_input_translator::get_bind( input::enum_keyboard key )
{
	if ( char_map.find( key ) == char_map.end( ) )
		return NULL;

	return &char_map[ key ];
}

// STATE[STUB]
// claude@NOTE: parked - converts a bound key to its localized character. Uses a
// wchar_t[64] buff + the CRT current_locale, calls input_world->keyboard()->... to
// get the scancode-to-char mapping (the GetKeyNameText-style accessor at 0x5de589),
// honours is_shift_now to pick current.c vs current.c_shift. NEXT: recover the
// keyboard char-translation accessor signature + the buff/locale shape (lines 127-157).
wchar_t swf_input_translator::translate_key_action( input::world* input_world, bool is_shift_now, dik_to_swf_bind& current )
{
	return 0;
}

void swf_input_translator::initialize( )
{
	register_char_bind( input::key_0, L'0', L')', 0x30, true );
	register_char_bind( input::key_numpad0, L'0', L'0', 0x60, false );
	register_char_bind( input::key_1, L'1', L'!', 0x31, true );
	register_char_bind( input::key_numpad1, L'1', L'1', 0x61, false );
	register_char_bind( input::key_2, L'2', L'@', 0x32, true );
	register_char_bind( input::key_numpad2, L'2', L'2', 0x62, false );
	register_char_bind( input::key_3, L'3', L'#', 0x33, true );
	register_char_bind( input::key_numpad3, L'3', L'3', 0x63, false );
	register_char_bind( input::key_4, L'4', L'$', 0x34, true );
	register_char_bind( input::key_numpad4, L'4', L'4', 0x64, false );
	register_char_bind( input::key_5, L'5', L'%', 0x35, true );
	register_char_bind( input::key_numpad5, L'5', L'5', 0x65, false );
	register_char_bind( input::key_6, L'6', L'^', 0x36, true );
	register_char_bind( input::key_numpad6, L'6', L'6', 0x66, false );
	register_char_bind( input::key_7, L'7', L'&', 0x37, true );
	register_char_bind( input::key_numpad7, L'7', L'7', 0x67, false );
	register_char_bind( input::key_8, L'8', L'*', 0x38, true );
	register_char_bind( input::key_numpad8, L'8', L'8', 0x68, false );
	register_char_bind( input::key_9, L'9', L'(', 0x39, true );
	register_char_bind( input::key_numpad9, L'9', L'9', 0x69, false );
	register_char_bind( input::key_minus, L'-', L'_', 0xbd, false );
	register_char_bind( input::key_space, L' ', L' ', 0x20, false );
	register_char_bind( input::key_backslash, L'\\', L'|', 0xdc, true );
	register_char_bind( input::key_lbracket, L'[', L'{', 0xdb, true );
	register_char_bind( input::key_rbracket, L']', L'}', 0xdd, true );
	register_char_bind( input::key_apostrophe, L'\'', L'"', 0xdc, true );
	register_char_bind( input::key_comma, L',', L'<', 0xbc, true );
	register_char_bind( input::key_period, L'.', L'>', 0xbe, true );
	register_char_bind( input::key_equals, L'=', L'+', 0xbb, false );
	register_char_bind( input::key_semicolon, L';', L':', 0xba, true );
	register_char_bind( input::key_slash, L'/', L'?', 0xbf, true );
	register_char_bind( input::key_a, 0x41, true );
	register_char_bind( input::key_b, 0x42, true );
	register_char_bind( input::key_c, 0x43, true );
	register_char_bind( input::key_d, 0x44, true );
	register_char_bind( input::key_e, 0x45, true );
	register_char_bind( input::key_f, 0x46, true );
	register_char_bind( input::key_g, 0x47, true );
	register_char_bind( input::key_h, 0x48, true );
	register_char_bind( input::key_i, 0x49, true );
	register_char_bind( input::key_j, 0x4a, true );
	register_char_bind( input::key_k, 0x4b, true );
	register_char_bind( input::key_l, 0x4c, true );
	register_char_bind( input::key_m, 0x4d, true );
	register_char_bind( input::key_n, 0x4e, true );
	register_char_bind( input::key_o, 0x4f, true );
	register_char_bind( input::key_p, 0x50, true );
	register_char_bind( input::key_q, 0x51, true );
	register_char_bind( input::key_r, 0x52, true );
	register_char_bind( input::key_s, 0x53, true );
	register_char_bind( input::key_t, 0x54, true );
	register_char_bind( input::key_u, 0x55, true );
	register_char_bind( input::key_v, 0x56, true );
	register_char_bind( input::key_w, 0x57, true );
	register_char_bind( input::key_x, 0x58, true );
	register_char_bind( input::key_y, 0x59, true );
	register_char_bind( input::key_z, 0x5a, true );
	register_ctl_bind( input::key_delete, 0x2e );
	register_ctl_bind( input::key_escape, 0x1b );
	register_ctl_bind( input::key_back, 0x8 );
	register_ctl_bind( input::key_insert, 0x2d );
	register_ctl_bind( input::key_return, 0xd );
	register_ctl_bind( input::key_left, 0x25 );
	register_ctl_bind( input::key_right, 0x27 );
	register_ctl_bind( input::key_up, 0x26 );
	register_ctl_bind( input::key_down, 0x28 );
	register_ctl_bind( input::key_tab, 0x9 );
	register_ctl_bind( input::key_rcontrol, 0xa3 );
	register_ctl_bind( input::key_lcontrol, 0xa2 );
	register_ctl_bind( input::key_rmenu, 0xa5 );
	register_ctl_bind( input::key_lmenu, 0xa4 );
	register_ctl_bind( input::key_rshift, 0xa1 );
	register_ctl_bind( input::key_lshift, 0xa0 );
	register_ctl_bind( input::key_numpadenter, 0xd );
	register_ctl_bind( input::key_decimal, 0x6e );
	register_ctl_bind( input::key_add, 0x6b );
	register_ctl_bind( input::key_divide, 0x6f );
	register_ctl_bind( input::key_subtract, 0x6d );
	register_ctl_bind( input::key_multiply, 0x6a );
}
void swf_input_translator::register_ctl_bind( input::enum_keyboard key, s32 scan )
{
	dik_to_swf_bind& bind	= char_map[ key ];
	bind.key				= key;
	bind.c					= 0;
	bind.c_shift			= 0;
	bind.scan				= scan;
	bind.translate			= false;
	bind.is_character		= false;
}

void swf_input_translator::register_char_bind(
	input::enum_keyboard	key,
	wchar_t					c,
	wchar_t					c_shift,
	s32						scan,
	bool					translate
)
{
	dik_to_swf_bind& bind	= char_map[ key ];
	bind.key				= key;
	bind.c					= c;
	bind.c_shift			= c_shift;
	bind.scan				= scan;
	bind.translate			= translate;
	bind.is_character		= true;
}

void swf_input_translator::register_char_bind( input::enum_keyboard key, s32 scan, bool translate )
{
	dik_to_swf_bind& bind	= char_map[ key ];
	bind.key				= key;
	bind.c					= 0;
	bind.c_shift			= 0;
	bind.scan				= scan;
	bind.translate			= true;
	bind.is_character		= true;
}

} // namespace survarium
