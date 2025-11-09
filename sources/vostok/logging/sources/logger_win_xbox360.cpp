////////////////////////////////////////////////////////////////////////////
//	Created		: 30.04.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include <vostok/os_include.h>
#include <vostok/logging/api.h>
#include <vostok/logging/format_specifier.h>

namespace vostok {
namespace logging {

// STATE[100%]
void fill_local_time( format_string_type& dest, bool brief )
{
	SYSTEMTIME date_time;
	GetLocalTime( &date_time );	// <0x70d8f6>|0x000|0x000:'17'
	if ( brief )				// <0x70d900>|0x00a|0x00a:'18'
	{
		sprintf(
			dest,
			"%02d:%02d:%02d",
			date_time.wHour,
			date_time.wMinute,
			date_time.wSecond
		);						// <0x70d908>|0x012|0x008:'26'
	}
	else						// <0x70d927>|0x031|0x01f:'27'
	{
		sprintf(
			dest,
			"%02d:%02d:%02d:%03d",
			date_time.wHour,
			date_time.wMinute,
			date_time.wSecond,
			date_time.wMilliseconds
		);						// <0x70d929>|0x033|0x002:'36'
	}
}

} // namespace logging
} // namespace vostok