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
	// Target emits this empty constructor out of line; this LTCG context expands it at callers.
	inline	log_format		( ) { }
	inline	log_format		( format_specifier const& format_expression ) { set( format_expression ); }

			void	set		( format_specifier const& format_expression );

	format_string_type				string;
	format_enabled_container		enabled;
	format_index_container			indexes;


};

STATIC_SIZE_ASSERT(log_format, 0x228);

} // namespace logging
} // namespace vostok

#endif // #ifndef VOSTOK_LOGGING_FORMAT_H_INCLUDED
