////////////////////////////////////////////////////////////////////////////
//	Created		: 14.05.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <sys/dbg.h>
#include <cell/sysmodule.h>
#include <cell/atomic.h>
#include <vostok/debug/extensions.h>
#include <sys/timer.h>

#include "platform.h"
#include "utils.h"

#pragma comment( lib, "c")			// for _Sce_assert
#pragma comment( lib, "cs")			// for _Sce_assert
#pragma comment( lib, "c_stub")		// for _Sce_assert
#pragma comment( lib, "stdc++")		// for _Sce_assert

namespace vostok {
namespace debug {

atomic32 vostok::debug::interlocked_increment		( atomic32 & value )
{
	return				cellAtomicIncr32( &value ) + 1;
}

atomic32 vostok::debug::interlocked_decrement		( atomic32 & value )
{
	return				cellAtomicDecr32( &value ) - 1;
}

atomic32 vostok::debug::interlocked_exchange (atomic32 & target, atomic32 value)
{
	return									cellAtomicStore32( &target, value );
}

atomic32 vostok::debug::interlocked_compare_exchange	( atomic32 & target, atomic32 value, atomic32 comparand )
{
	return									cellAtomicCompareAndSwap32( & target, value, comparand);
}

} // namespace debug
} // namespace vostok


u32 vostok::debug::current_thread_id			( )
{
	sys_ppu_thread_t						result;
	sys_ppu_thread_get_id					( &result );
	return									result;
}

void vostok::debug::yield						( u32 milliseconds )
{
	if ( milliseconds ) {
		// receives time in microseconds
		sys_timer_usleep		( milliseconds*1000 );
		return;
	}

	sys_ppu_thread_yield		( );
}

bool vostok::debug::is_debugger_present( )
{
#ifdef DEBUG
	return						true;
#else // #ifdef DEBUG
	return						false;
#endif // #ifdef DEBUG
}

pcstr vostok::debug::platform::get_graphics_api_error_description	( int const error_code )
{
	NOT_IMPLEMENTED				( return "" );
}

void vostok::debug::notify_xbox_debugger ( pcstr message )
{
	VOSTOK_UNREFERENCED_PARAMETER	( message );
}

void vostok::debug::debug_message_box (pcstr message)
{
	VOSTOK_UNREFERENCED_PARAMETER	(message);
}

void vostok::debug::output								( pcstr message )
{
	VOSTOK_UNREFERENCED_PARAMETER	( message );
	OUTPUT_DEBUG_STRING			( message );
}