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
	inline	log_format		( format_specifier const& format_expression ) { set( format_expression ); }
	// claude@NOTE: the target's vostok/logging/format.h unit carries exactly one 1-byte empty COMDAT
	// ("empty_stub") - this default ctor, emitted-but-not-expanded in their LTCG link; our link always
	// expands it, which costs the appends in logger.cpp one empty call each (see README).
	inline	log_format		( ) { }

			void	set		( format_specifier const& format_expression );

	format_string_type				string;
	format_enabled_container		enabled;
	format_index_container			indexes;


};

STATIC_SIZE_ASSERT(log_format, 0x228);

} // namespace logging
} // namespace vostok

#endif // #ifndef VOSTOK_LOGGING_FORMAT_H_INCLUDED