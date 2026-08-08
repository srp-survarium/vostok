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

	for ( u32 i = 0; i < m_current_events_count; ++i )
	{
		enum_keyboard const		key = enum_keyboard( m_current_events[ i ].dwOfs );
		if ( m_current_events[ i ].dwData & 0x80 )
		{
			for ( handlers_type::iterator it = handlers.begin(), it_e = handlers.end(); it != it_e; ++it )
				if ( (*it)->on_keyboard_action	( &m_world, key, kb_key_down ) )
					break;
		}
		else
		{
			for ( handlers_type::iterator it = handlers.begin(), it_e = handlers.end(); it != it_e; ++it )
				if ( (*it)->on_keyboard_action	( &m_world, key, kb_key_up ) )
					break;
		}
	}


	for ( u32 i = 0; i < 256; ++i )
	{
		if ( m_current_key_state[ i ] )
		{
			for ( handlers_type::iterator it = handlers.begin(), it_e = handlers.end(); it != it_e; ++it )
				if ( (*it)->on_keyboard_action	( &m_world, enum_keyboard( i ), kb_key_hold ) )
					break;
		}
	}
	m_current_events_count	= 0;
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
