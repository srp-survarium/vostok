////////////////////////////////////////////////////////////////////////////
//	Created		: 24.02.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/fs/native_path_string.h>
#include <vostok/fs/path_string_utils.h>

#include <vostok/os_preinclude.h>
#	undef			NOUSER            			// All USER undefs and routines
#	undef			NOMSG             			// typedef MSG and associated routines
#include <vostok/os_include.h>

#include <shlwapi.h>
#pragma comment	(lib, "shlwapi.lib")

namespace vostok {
namespace fs_new {

static native_path_string 					s_current_directory;
static bool	s_initialized				=	false;

native_path_string const &	get_current_directory ()
{
	if ( s_initialized )
		return								s_current_directory;

	string_path								path;
	GetCurrentDirectory						(sizeof(path), path);
	s_current_directory.assign_with_conversion	(path);
	s_current_directory.rtrim				(s_current_directory.separator);
	s_current_directory.make_lowercase		();
	s_initialized						=	true;
	return									s_current_directory;
}

} // namespace fs_new
} // namespace vostok