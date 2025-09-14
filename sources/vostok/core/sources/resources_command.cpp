////////////////////////////////////////////////////////////////////////////
//	Created		: 20.01.2011
//	Author		: 
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/resources_command.h>
#include <vostok/query_result.h>
#include "resources_fs_impl.h"

namespace vostok {
namespace resources {

query_result *   resource_command::cast_query_result ()
{
	if ( m_type == type_query )
		return					static_cast_checked<query_result *>(this);

	return						NULL;
}

fs_task *   resource_command::cast_fs_task ()
{
	if ( m_type == type_fs_task )
		return					static_cast_checked<fs_task *>(this);

	return						NULL;
}

} // namespace resources
} // namespace vostok
