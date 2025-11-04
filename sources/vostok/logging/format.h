////////////////////////////////////////////////////////////////////////////
//	Created		: 31.08.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_LOGGING_FORMAT_H_INCLUDED
#define VOSTOK_LOGGING_FORMAT_H_INCLUDED

#include <vostok/logging/format_specifier.h>
#include <vostok/logging/api.h>

namespace vostok {
namespace logging {

struct log_format
{

	format_string_type						string;
	format_enabled_container				enabled;
	format_index_container					indexes;

	void	set							(format_specifier const & format_expression);
};

STATIC_SIZE_ASSERT(log_format, 0x228);

#if 0
// STATE[STUB]
// vostok::logging::log_format::log_format()
log_format::log_format( )
{
}
#endif


} // namespace logging
} // namespace vostok

#endif // #ifndef VOSTOK_LOGGING_FORMAT_H_INCLUDED