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

void fill_local_time( format_string_type& dest, bool brief )
{
	SYSTEMTIME date_time;
	GetLocalTime( &date_time );
	if ( brief )
	{
		sprintf(
			dest,
			"%02d:%02d:%02d",
			date_time.wHour,
			date_time.wMinute,
			date_time.wSecond
		);
	}
	else
	{
		sprintf(
			dest,
			"%02d:%02d:%02d:%03d",
			date_time.wHour,
			date_time.wMinute,
			date_time.wSecond,
			date_time.wMilliseconds
		);
	}
}

} // namespace logging
} // namespace vostok