////////////////////////////////////////////////////////////////////////////
//	Created		: 27.10.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "platform.h"
#include <xtl.h>
#include <xbdm.h>
#include <dxerr9.h>
#include <vostok/debug/debug.h>
#include <vostok/core/engine.h>
#include <vostok/debug/macros.h>
#include "utils.h"

#pragma comment( lib, "dxerr9.lib" )

pcstr vostok::debug::platform::get_graphics_api_error_description	( int const error_code )
{
	return						DXGetErrorDescription9( error_code );
}

bool vostok::debug::is_debugger_present( )
{
#ifdef DEBUG
	return						DmIsDebuggerPresent( ) == TRUE;
#else // #ifdef DEBUG
	return						false;
#endif // #ifdef DEBUG
}

#ifdef DEBUG
void vostok::debug::notify_xbox_debugger ( pcstr in_message )
{
	DmSendNotificationString	(in_message);
}
#else // #ifdef DEBUG
void vostok::debug::notify_xbox_debugger ( pcstr in_message )
{
	VOSTOK_UNREFERENCED_PARAMETER	(in_message);
}
#endif // #ifdef DEBUG

void vostok::debug::debug_message_box (pcstr message)
{
	VOSTOK_UNREFERENCED_PARAMETER	(message);
}
