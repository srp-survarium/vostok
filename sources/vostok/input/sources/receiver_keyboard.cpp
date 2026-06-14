////////////////////////////////////////////////////////////////////////////
//	Created		: 13.05.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "direct_input_include.h"
#include "stdlib.h"
#include "receiver_keyboard.h"
#include <vostok/input/handler.h>

using vostok::input::receiver::keyboard;

void keyboard::process		( handlers_type& handlers )
{
	handlers_type::iterator it;
	handlers_type::iterator it_e = handlers.end();

	pcstr					i = m_current_key_buffer, b = i;
	pcstr					e = m_current_key_buffer + sizeof(m_current_key_buffer);
	pcstr					j = m_previous_key_buffer;
	for ( ; i != e; ++i, ++j ) {
		if ( is_key_down( *i ) && is_key_down( *j ) ) {
			for(it=handlers.begin(); it!=it_e; ++it)
				if( (*it)->on_keyboard_action	( &m_world, enum_keyboard( i - b ), kb_key_hold ) )
					break;

			continue;
		}

		it_e = handlers.end();
		if ( is_key_down( *i ) ) {
			for(it=handlers.begin(); it!=it_e; ++it)
				if( (*it)->on_keyboard_action	( &m_world, enum_keyboard( i - b ), kb_key_down ) )
					break;

			continue;
		}

		it_e = handlers.end();
		if ( is_key_down( *j ) ) {
			for(it=handlers.begin(); it!=it_e; ++it)
				if( (*it)->on_keyboard_action	( &m_world, enum_keyboard( i - b ), kb_key_up ) )
					break;

			continue;
		}
	}
}


bool keyboard::get_dik_name(  int dik, pstr dest_str, int dest_sz ) const
{
	DIPROPSTRING keyname;
	keyname.diph.dwSize			= sizeof(DIPROPSTRING);
	keyname.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
	keyname.diph.dwObj			= static_cast<DWORD>(dik);
	keyname.diph.dwHow			= DIPH_BYOFFSET;
	HRESULT hr					= m_device->GetProperty(DIPROP_KEYNAME, &keyname.diph);
	if(FAILED(hr))
		return false;

	const wchar_t* wct			= keyname.wsz;
	if(0==wcslen(wct))
		return					false;
	
	size_t converted_size = 0;
	
	errno_t res = wcstombs_s ( &converted_size, dest_str, dest_sz, (keyname.wsz),dest_sz )	;//				= WideCharToMultiByte(,0,keyname.wsz,-1,dest_str,dest_sz,NULL,NULL);
	
	if(res == EINVAL)
		return false;

	return						true;
}

// claude@NOTE: structure-match; sole byte residual is the m_device member offset
// (base 0x208 vs target 0x90c) - the target receiver::keyboard layout is
// m_current_key_state[256] + m_current_events_count + m_current_events[64]
// (DIDEVICEOBJECTDATA), not the string256 buffers this header still carries.
// Fixing the layout is a whole-keyboard-TU rewrite (process/is_key_down/execute).
bool keyboard::get_dik_unicode( int dik, wchar_t* buff, u32 buff_size ) const
{
	DIPROPSTRING keyname;
	keyname.diph.dwSize			= sizeof(DIPROPSTRING);
	keyname.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
	keyname.diph.dwObj			= static_cast<DWORD>(dik);
	keyname.diph.dwHow			= DIPH_BYOFFSET;
	HRESULT hr					= m_device->GetProperty(DIPROP_KEYNAME, &keyname.diph);
	if(FAILED(hr))
		return false;

	wcscpy_s( buff, buff_size, keyname.wsz );
	return						true;
}