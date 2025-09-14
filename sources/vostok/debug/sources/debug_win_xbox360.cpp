////////////////////////////////////////////////////////////////////////////
//	Created		: 06.05.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/linkage_helper.h>
#include <vostok/debug/extensions.h>
#include <vostok/debug/call_stack.h>
#include "utils.h"
#include <stdio.h>
#include <vostok/memory_extensions.h>
#include <vostok/command_line_extensions.h>

static vostok::command_line::key s_disable_output_to_debugger( "disable_output_to_debugger", "disable_output_to_debugger", "debug", "disables output to debugger output window", "" );

#if VOSTOK_PLATFORM_XBOX_360
#	include <xtl.h>
#else 
#	include <windows.h>
#endif

#include <math.h>

namespace vostok {
namespace debug {

atomic32  interlocked_increment			( atomic32 & value )
{
	return									InterlockedIncrement( &value );
}

atomic32  interlocked_decrement			( atomic32 & value )
{
	return									InterlockedDecrement( &value );
}

atomic32  interlocked_exchange			( atomic32 & target, atomic32 value )
{
	return									InterlockedExchange( &target, value );
}

atomic32  interlocked_compare_exchange	( atomic32 & target, atomic32 value, atomic32 comparand )
{
	return									InterlockedCompareExchange( & target, value, comparand );
}

} // namespace debug
} // namespace vostok

u32 vostok::debug::current_thread_id			( )
{
	return									GetCurrentThreadId( );							
}

void vostok::debug::yield		( u32 milliseconds )
{
	if ( milliseconds ) {
		Sleep								( milliseconds );
		return;
	}

	if ( SwitchToThread( ) )
		return;

	Sleep									( 0 );
}

int vostok::debug::on_math_error		( struct ::_exception *exception )
{
	pcstr type				= "<no_description>";
	switch ( exception->type ) {
		case _DOMAIN : {
			type			= "Argument domain error";
			break;
		}
		case _SING : {
			type			= "Argument singularity";
			break;
		}
		case _OVERFLOW : {
			type			= "Overflow range error";
			break;
		}
		case _PLOSS : {
			type			= "Partial loss of significance";
			break;
		}
		case _TLOSS : {
			type			= "Total loss of significance";
			break;
		}
		case _UNDERFLOW : {
			return			1;
//			type			= "The result is too small to be represented";
//			break;
		}
		default : NODEFAULT();
	}
	
	log_callback const log		=	get_log_callback();
	if ( log )
	{
		string4096	message;
		sprintf_s			(message, "error[%s] in function %s( %f [, %f] )", 
								type, exception->name, exception->arg1, exception->arg2);

		log					( "debug", true, false, message);
	}

	dump_call_stack			( "debug", true, 3, 0, 0 );
	DEBUG_BREAK				();
    return					( 0 );    /* Else use the default actions */
}

void vostok::debug::output								( pcstr message )
{
	if ( s_disable_output_to_debugger.is_set() )
		return;

	VOSTOK_UNREFERENCED_PARAMETER	( message );
	OUTPUT_DEBUG_STRING			( message );
}