////////////////////////////////////////////////////////////////////////////
//	Created 	: 20.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "input_world.h"
#include "receiver_gamepad.h"

using vostok::input::input_world;

void input_world::create_devices	( HWND const window_handle )
{
	VOSTOK_UNREFERENCED_PARAMETER	( window_handle );

	m_gamepad			= NEW ( receiver::gamepad ) ( 0, *this );
}

void input_world::destroy_devices	( )
{
	VOSTOK_DELETE_IMPL	( *g_allocator, m_gamepad );
}