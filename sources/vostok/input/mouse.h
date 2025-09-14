////////////////////////////////////////////////////////////////////////////
//	Created 	: 21.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_INPUT_MOUSE_H_INCLUDED
#define VOSTOK_INPUT_MOUSE_H_INCLUDED

namespace vostok {
namespace input {

enum enum_mouse_key_action{
	ms_key_down,
	ms_key_up,
	ms_key_hold,
	ms_key_click,
	ms_key_dbl_click
};//enum_mouse_button_action

enum mouse_button {
	mouse_button_left		= u8( 1 << 0 ),
	mouse_button_right		= u8( 1 << 1 ),
	mouse_button_middle		= u8( 1 << 2 ),
	mouse_button_extended0	= u8( 1 << 3 ),
	mouse_button_extended1	= u8( 1 << 4 ),
	mouse_button_extended2	= u8( 1 << 5 ),
	mouse_button_extended3	= u8( 1 << 6 ),
	mouse_button_extended4	= u8( 1 << 7 ),
	mouse_button_count		= 8,
}; // enum mouse_buttons

struct VOSTOK_NOVTABLE mouse {
public:
	struct state {
		long    x;
		long    y;
		long    z;
		u8		buttons;
	}; // struct state

public:
	virtual	state const&	get_state	( ) const = 0;

protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( mouse )
}; // class handler

} // namespace input
} // namespace vostok

#endif // #ifndef VOSTOK_INPUT_MOUSE_H_INCLUDED