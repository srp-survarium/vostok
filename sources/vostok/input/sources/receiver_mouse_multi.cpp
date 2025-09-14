////////////////////////////////////////////////////////////////////////////
//	Created 	: 21.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "receiver_mouse.h"
#include <vostok/input/handler.h>

using vostok::input::receiver::mouse;
using vostok::input::handler;
using vostok::input::world;

mouse::mouse			( IDirectInput8A& direct_input, HWND const window_handle, world& input_world ) 
:	m_window_handle		( window_handle ),
	m_device			( 0 ),
	m_world				( input_world )
{
	VOSTOK_UNREFERENCED_PARAMETER	( direct_input );
}

mouse::~mouse			( )
{
}

vostok::input::mouse::state const& mouse::get_state	( ) const
{
	return				( m_current_state );
}

void mouse::on_activate	( )
{
}

void mouse::on_deactivate ( )
{
}

void mouse::execute		( )
{
}