////////////////////////////////////////////////////////////////////////////
//	Created		: 06.09.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

namespace vostok {
namespace fs_new {

static native_path_string 					s_current_directory;
static bool	s_initialized				=	false;

native_path_string const &			get_current_directory ()
{
	if ( s_initialized )
		return								s_current_directory;

	s_current_directory					=	"game:";
	return									s_current_directory;
}

} // namespace fs_new
} // namespace vostok