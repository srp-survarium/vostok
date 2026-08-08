////////////////////////////////////////////////////////////////////////////
//	Created		: 13.05.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "receiver_mouse.h"
#include <vostok/input/handler.h>

using vostok::input::receiver::mouse;
using vostok::input::mouse_button;

mouse_button mouse::convert_to_binder_mouse_button	( s32 receiver_button )
{
	switch ( receiver_button ) {
		case 1 << 0	: return mouse_button_left;
		case 1 << 1	: return mouse_button_right;
		case 1 << 2	: return mouse_button_middle;
		case 1 << 3	: return mouse_button_extended0;
		case 1 << 4	: return mouse_button_extended1;
		case 1 << 5	: return mouse_button_extended2;
		case 1 << 6	: return mouse_button_extended3;
		case 1 << 7	: return mouse_button_extended4;
	}

	NODEFAULT();
}

void mouse::process		( handlers_type& handlers )
{
	u16 const changed_buttons	= u16( m_current_state.buttons ^ m_previous_state.buttons );
	handlers_type::iterator const it_e = handlers.end();

	for ( u16 i = u16( m_current_state.buttons & changed_buttons ); i; i &= i - 1 ) {
		for ( handlers_type::iterator it = handlers.begin(); it != it_e; ++it )
		{
			mouse_button const button = convert_to_binder_mouse_button( i & ~( i - 1 ) );
			bool const down = (*it)->on_mouse_key_action( &m_world, button, ms_key_down ), hold = (*it)->on_mouse_key_action( &m_world, button, ms_key_hold );
			if ( down || hold )
				break;
		}
	}

	for ( u16 i = u16( ~m_current_state.buttons & changed_buttons ); i; i &= i - 1 )
		for ( handlers_type::iterator it = handlers.begin(); it != it_e; ++it )
			if ( (*it)->on_mouse_key_action( &m_world, convert_to_binder_mouse_button( i & ~( i - 1 ) ), ms_key_up ) )
				break;

	for ( u16 i = u16( m_previous_state.buttons & m_current_state.buttons ); i; i &= i - 1 )
		for ( handlers_type::iterator it = handlers.begin(); it != it_e; ++it )
			if ( (*it)->on_mouse_key_action( &m_world, convert_to_binder_mouse_button( i & ~( i - 1 ) ), ms_key_hold ) )
				break;


	if ( m_current_state.x || m_current_state.y || m_current_state.z )
		for ( handlers_type::iterator it = handlers.begin(); it != it_e; ++it )
			if ( (*it)->on_mouse_move( &m_world, m_current_state.x, m_current_state.y, m_current_state.z ) )
				break;
}
