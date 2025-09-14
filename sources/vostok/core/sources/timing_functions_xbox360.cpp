////////////////////////////////////////////////////////////////////////////
//	Created 	: 23.01.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "timing.h"
#include <vostok/timing_functions.h>
#include <vostok/os_include.h>
#include <float.h>

u64	 vostok::timing::g_qpc_per_second;
bool vostok::timing::g_cpu_supports_time_stamp	= false;

void vostok::timing::initialize				( )
{
	QueryPerformanceFrequency	( ( PLARGE_INTEGER )&g_qpc_per_second );
	errno_t const error			= _controlfp_s( 0, _PC_24, MCW_PC );
	VOSTOK_UNREFERENCED_PARAMETER	( error );
	R_ASSERT					( !error );
}

u64 vostok::timing::query_performance_counter	( )
{
	u64							result;
	QueryPerformanceCounter		( ( PLARGE_INTEGER )&result );
	return						(result);
}