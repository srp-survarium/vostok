////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <sys/dbg.h>			// for floating point exceptions
#include <vostok/debug/debug.h>
#include <vostok/debug/extensions.h>
#include "platform.h"
#include "utils.h"

#pragma comment( lib, "lv2dbg_stub" )

void vostok::debug::platform::change_storage_access_handler	(
		error_mode error_mode, 
		void (*storage_access_handler) ( int )
	)
{
	NOT_IMPLEMENTED	( );
}

namespace vostok {
namespace debug {

static bool s_debug_preinitialized		=	false;

void preinitialize						( )
{
	if ( s_debug_preinitialized )
		return;

	s_debug_preinitialized				= true;
}

} // namespace debug
} // namespace vostok

#ifdef DEBUG
void vostok::debug::enable_fpe			( bool value )
{
	if ( value )
		sys_dbg_enable_floating_point_enabled_exception	( current_thread_id(), 0, 0, 0 );
	else
		sys_dbg_disable_floating_point_enabled_exception( current_thread_id(), 0, 0, 0 );
}
#endif // #ifdef DEBUG

