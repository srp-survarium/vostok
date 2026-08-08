////////////////////////////////////////////////////////////////////////////
//	Created		: 25.08.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/logging/format_specifier.h>

namespace vostok {
namespace logging {

format_specifier		format_thread_id	(format_specifier_thread_id);
format_specifier		format_initiator	(format_specifier_initiator);
format_specifier		format_time			(format_specifier_time);
format_specifier		format_time_brief	(format_specifier_time_brief);
format_specifier		format_verbosity	(format_specifier_verbosity);
format_specifier		format_message		(format_specifier_message);

format_specifier::format_specifier		(format_specifier_enum specifier)
	: m_specifier(specifier), m_left(NULL), m_right(NULL) {;}

format_specifier::format_specifier		(format_specifier const & left, format_specifier const & right)
	: m_left(& left), m_right(& right), m_specifier(format_specifier_unset) {;}

// Target keeps the shared strings::copy<512> wrapper out of line; this /Od
// caller inlines it. The matching append wrapper has a different inline budget.
void   format_specifier::fill_specifier_list	(format_specifier_list & list, format_string_type * out_format_string) const
{
	if ( m_left )
	{
		format_string_type					left_string;
		m_left->fill_specifier_list			(list, & left_string);
		format_string_type					right_string;
		m_right->fill_specifier_list		(list, & right_string);
		strings::copy						(*out_format_string, left_string);
		strings::append						(*out_format_string, right_string);
	}
	else
	{
		if ( m_specifier != format_specifier_separator )
		{
			list.push_back					(m_specifier);
			strings::copy					(*out_format_string, "%s");
		}
		else
		{
			format_separator const * const this_ptr	=	static_cast<format_separator const *>(this);
			strings::copy					(*out_format_string, this_ptr->separator.c_str());
		}
	}
}

} // namespace logging
} // namespace vostok
