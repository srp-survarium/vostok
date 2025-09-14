////////////////////////////////////////////////////////////////////////////
//	Created 	: 22.01.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_TIMING_FUNCTIONS_H_INCLUDED
#define VOSTOK_TIMING_FUNCTIONS_H_INCLUDED

#if VOSTOK_PLATFORM_WINDOWS
#	include <intrin.h>
#	pragma intrinsic(__rdtsc)
#endif // #if VOSTOK_PLATFORM_WINDOWS

namespace vostok {
namespace timing {

VOSTOK_CORE_API	extern u64	g_qpc_per_second;
VOSTOK_CORE_API	u64			query_performance_counter	( );

VOSTOK_CORE_API	extern bool	g_cpu_supports_time_stamp;

inline			u64			get_clocks					( )
{
#if VOSTOK_PLATFORM_WINDOWS
	return		( __rdtsc( ) );
#else // #if VOSTOK_PLATFORM_WINDOWS
	return		( query_performance_counter( ) );
#endif // #if VOSTOK_PLATFORM_WINDOWS
}

inline u64					get_QPC						( )
{
	if ( g_cpu_supports_time_stamp )
	{
		return		get_clocks( );
	}
	else
	{
		return		( query_performance_counter( ) );
	}
}

} // namespace timing
} // namespace vostok

#endif // #ifndef VOSTOK_TIMING_FUNCTIONS_H_INCLUDED