////////////////////////////////////////////////////////////////////////////
//	Created		: 06.05.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/fs_path.h>

namespace vostok {
namespace fs {

path_string   convert_to_native (pcstr path)
{
	path_string	res					=	path;
	for ( path_string::iterator	it	=	res.begin();
								it	!=	res.end();
								++it )
	{
		if ( *it == '/' )
			*it						=	'\\';
	}

	return								res;
}

} // namespace fs
} // namespace vostok