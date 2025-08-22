////////////////////////////////////////////////////////////////////////////
//	Created		: 27.10.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "debug_platform.h"
#include "debug.h"
#include <vostok/os_include.h>
#include <dxerr9.h>
#include <vostok/fixed_string.h>
#include <vostok/core/engine.h>

#pragma comment( lib, "dxerr9.lib" )

pcstr vostok::core::debug::platform::get_graphics_api_error_description	( int const error_code )
{
	return						DXGetErrorDescription9( error_code );
}

bool vostok::debug::is_debugger_present( )
{
#ifdef DEBUG
	return						( !!IS_DEBUGGER_PRESENT() );
#else // #ifdef DEBUG
	return						( false );
#endif // #ifdef DEBUG
}

#ifdef DEBUG
void vostok::core::debug::notify_xbox_debugger ( pcstr in_message )
{
	DmSendNotificationString	(in_message);
}
#else // #ifdef DEBUG
void vostok::core::debug::notify_xbox_debugger ( pcstr in_message )
{
	VOSTOK_UNREFERENCED_PARAMETER	(in_message);
}
#endif // #ifdef DEBUG