////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/core/core.h>		// for error_mode
#include "debug_platform.h"
#include <vostok/debug.h>
#include "debug.h"
#include <sys/dbg.h>			// for floating point exceptions

#pragma comment( lib, "lv2dbg_stub" )

namespace vostok {
namespace core {
namespace debug {
	extern vostok::command_line::key	g_fpe_disabled;
} // namespace debug 
} // namespace core
} // namespace vostok

void vostok::core::debug::platform::change_storage_access_handler	(
		error_mode error_mode, 
		void (*storage_access_handler) ( int )
	)
{
	NOT_IMPLEMENTED	( );
}

namespace vostok {
namespace core {
namespace debug {

static bool s_debug_preinitialized		=	false;

void preinitialize						( )
{
	if ( s_debug_preinitialized )
		return;

	s_debug_preinitialized				= true;
}

} // namespace debug
} // namespace core
} // namespace vostok

#ifdef DEBUG
void vostok::debug::enable_fpe			( bool value )
{
	if ( value )
		sys_dbg_enable_floating_point_enabled_exception	( threading::current_thread_id(), 0, 0, 0 );
	else
		sys_dbg_disable_floating_point_enabled_exception( threading::current_thread_id(), 0, 0, 0 );
}
#endif // #ifdef DEBUG

void vostok::core::debug::on_thread_spawn	( )
{
#if defined(DEBUG)
	if ( !g_fpe_disabled )
		vostok::debug::enable_fpe		( true );
#endif // #if defined(DEBUG)
}