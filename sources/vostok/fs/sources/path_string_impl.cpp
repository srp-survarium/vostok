///////////////////////////////////////////////////////////////////////////
//	Created		: 18.02.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
///////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/fs/native_path_string.h>
#include <vostok/fs/portable_path_string.h>
#include <vostok/fs/virtual_path_string.h>

namespace vostok {
namespace fs_new {


native_path_string::native_path_string (portable_path_string const & other) : path_string_impl(separator)
{
	assign_with_conversion					(other.c_str());
}

portable_path_string::portable_path_string (native_path_string const & other) : path_string_impl(separator)
{
	assign_with_conversion					(other.c_str());
}

// out-of-line: the retail binary keeps verify_self as a real call (the
// ICF-folded ret-stub family); header-inline definition loses that call
void   path_string_impl::verify_self ()
{
	m_string.verify_self					();
	verify									(m_string.get_buffer());
}

void   path_string_impl::convert (iterator begin, iterator end)
{
	char const other_separator			=	(m_separator == '/') ? '\\' : '/';
	for ( iterator it = begin, it_end = end; it != it_end; ++it )
		if ( * it == other_separator )
			* it						=	m_separator;
}

} // namespace fs_new
} // namespace vostok