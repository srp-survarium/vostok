////////////////////////////////////////////////////////////////////////////
//	Created		: 30.04.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include <vostok/os_include.h>
#include <vostok/logging/api.h>

namespace vostok {
namespace logging {

// sushi@TODO: To delete
void fill_local_time			( vostok::buffer_string& dest )
{
	SYSTEMTIME					date_time;
	GetLocalTime				( &date_time );
	dest.appendf				("%02d:%02d:%02d:%03d",
								 date_time.wHour,
								 date_time.wMinute,
								 date_time.wSecond,
								 date_time.wMilliseconds);
}


// STATE[STUB]
void fill_local_time( Buffer512& dest, bool brief )
{
	// LOCALS
	// _SYSTEMTIME 					date_time
	// ******

	// CALL SITE INFO
	// <0x70d8fa> -> void <unknown>(_SYSTEMTIME*)
	// ******

	// FUNCTION BODY
	// <1>
	// <0x70d8f6>|0x000|0x000:'17'
	// <0x70d900>|0x00a|0x00a:'18'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x70d908>|0x012|0x008:'26'
	// <0x70d927>|0x031|0x01f:'27'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x70d929>|0x033|0x002:'36'
	// <1>
	// ******
}

} // namespace logging
} // namespace vostok