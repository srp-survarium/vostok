////////////////////////////////////////////////////////////////////////////
//	Created		: 25.08.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/logging/format_specifier.h>

namespace vostok {
namespace logging {

// claude@NOTE: ALL six dynamic initializers + atexit dtors ARE emitted by the base, instruction-identical
// to the target (??__Eformat_* / ??__Fformat_* in the base PDB). The report counts them missing only
// because objdiff never demangles ??__E/??__F base names to pair with the target's pretty
// "dynamic initializer for '...'" names - tooling gap, see the logging README. The unit's 2/15 is
// almost entirely this artifact.
format_specifier		format_thread_id	(format_specifier_thread_id);
format_specifier		format_initiator	(format_specifier_initiator);
format_specifier		format_time			(format_specifier_time);
format_specifier		format_time_brief	(format_specifier_time_brief);
format_specifier		format_verbosity	(format_specifier_verbosity);
format_specifier		format_message		(format_specifier_message);

// STATE[100%|DONE]
format_specifier::format_specifier		(format_specifier_enum specifier)
	: m_specifier(specifier), m_left(NULL), m_right(NULL) {;}

// STATE[100%|DONE]
format_specifier::format_specifier		(format_specifier const & left, format_specifier const & right)
	: m_left(& left), m_right(& right), m_specifier(format_specifier_unset) {;}

// STATE[88%|DONE]: core strings::copy<512> called out-of-line in target (x3), inlined in base.
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

	// STRUCTURE DIFF: target 12 stmts / base 12 stmts
	// SIZE +0x5  | 37 | strings::copy						(*out_format_string, left_string);
	// SIZE +0x5  | 45 | strings::copy					(*out_format_string, "%s");
	// SIZE +0x11 | 50 | strings::copy					(*out_format_string, this_ptr->separator.c_str());
	// VERDICT: STRUCTURE MATCH (shape ok) - core strings::copy<512> (+ c_str at row 50) inline-vs-call; core-side, banked.
}

} // namespace logging
} // namespace vostok

