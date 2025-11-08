////////////////////////////////////////////////////////////////////////////
//	Created		: 25.08.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_LOGGING_FORMAT_SPECIFIER_H_INCLUDED
#define VOSTOK_LOGGING_FORMAT_SPECIFIER_H_INCLUDED

#include <vostok/logging/noncopyable.h>
#include <vostok/fixed_string.h>
#include <vostok/containers_extensions.h>

namespace vostok {
namespace logging {

enum format_specifier_enum
{
	format_specifier_unset		=	 0,
	format_specifier_thread_id	=	 1,
	format_specifier_initiator	=	 2,
	format_specifier_time		=	 3,
	format_specifier_time_brief =	 4,
	format_specifier_verbosity	=	 5,
	format_specifier_message	=	 6,
	format_specifier_separator	=	 7,
	format_specifier_count		=	 8,
};

typedef	fixed_vector<format_specifier_enum, format_specifier_count>		format_specifier_list;
typedef	int		format_index_container[format_specifier_count];
typedef	bool	format_enabled_container[format_specifier_count];
typedef char	format_string_type[512];

// this is node of a tree that represents an ordered list of format specifiers enums
// that are used to format logging
class format_specifier : private noncopyable
{
public:
	format_specifier					(format_specifier_enum specifier);
	format_specifier					(format_specifier const & left, format_specifier const & right);

	void	fill_specifier_list			(format_specifier_list & list, format_string_type * out_format_string) const;

private:
	format_specifier const *			m_left;
	format_specifier const *			m_right;

	format_specifier_enum 				m_specifier;
};

struct format_separator : format_specifier
{
	format_separator					(pcstr separator)
		: format_specifier(format_specifier_separator), separator(separator) {}

	fixed_string<128>					separator;
};

inline
format_specifier	operator +			(format_specifier const & left, format_specifier const & right)
{
	return									format_specifier(left, right);
}

} // namespace logging
} // namespace vostok

#endif // #ifndef VOSTOK_LOGGING_FORMAT_SPECIFIER_H_INCLUDED