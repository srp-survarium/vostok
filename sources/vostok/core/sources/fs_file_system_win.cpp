////////////////////////////////////////////////////////////////////////////
//	Created		: 06.05.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "fs_file_system.h"
#include "fs_helper.h"
#include "fs_nodes.h"

namespace vostok {
namespace fs   {

VOSTOK_CORE_API bool	make_dir(pcstr portable_path);

bool initialize						( pcstr const replication_folder )
{
	if ( replication_folder )
 		make_dir					( replication_folder );

	VOSTOK_CONSTRUCT_REFERENCE		( g_fat, file_system );
	if ( replication_folder )
		g_fat->set_replication_dir	( replication_folder );
	
	return							( true );
}

void finalize						( )
{
	VOSTOK_DESTROY_REFERENCE			( g_fat );
}

void flush_replications	( )
{
}

} // namespace fs
} // namespace vostok